#include "io/CargadorCSV.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <unordered_set>

namespace {

// Columnas obligatorias de cada archivo, en el orden en que se usan abajo
const std::vector<std::string> COLUMNAS_VERTICES = {"id", "name", "x", "y", "type", "priority"};
const std::vector<std::string> COLUMNAS_ARISTAS = {"u", "v", "length", "speed_limit",
                                                    "oneway", "can_close"};

// "datos/pequeno/aristas.csv" -> "aristas.csv", para que los mensajes sean cortos
std::string nombreArchivo(const std::string& pRuta) {
    size_t barra = pRuta.find_last_of("/\\");
    return barra == std::string::npos ? pRuta : pRuta.substr(barra + 1);
}

std::string enMinusculas(std::string pTexto) {
    std::transform(pTexto.begin(), pTexto.end(), pTexto.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return pTexto;
}

}  // namespace

ReporteValidacion CargadorCSV::cargar(const std::string& pRutaVertices,
                                      const std::string& pRutaAristas, Grafo& pGrafo) const {
    ReporteValidacion reporte;
    Grafo temporal;

    // Las aristas se revisan aunque los vertices tengan errores,
    // asi el usuario ve todos los problemas en una sola pasada (D8)
    cargarVertices(pRutaVertices, temporal, reporte);
    cargarAristas(pRutaAristas, temporal, reporte);

    if (reporte.esValido()) {
        pGrafo = temporal;
    }
    return reporte;
}

void CargadorCSV::cargarVertices(const std::string& pRuta, Grafo& pGrafo,
                                 ReporteValidacion& pReporte) const {
    const std::string archivo = nombreArchivo(pRuta);
    std::ifstream entrada(pRuta);
    if (!entrada.is_open()) {
        pReporte.agregarError(archivo, 0, "no se pudo abrir el archivo");
        return;
    }

    std::string linea;
    if (!std::getline(entrada, linea)) {
        pReporte.agregarError(archivo, 1, "el archivo esta vacio");
        return;
    }
    std::vector<int> posiciones;
    if (!leerEncabezado(linea, COLUMNAS_VERTICES, posiciones)) {
        for (size_t i = 0; i < COLUMNAS_VERTICES.size(); i++) {
            if (posiciones[i] < 0) {
                pReporte.agregarError(archivo, 1, "falta la columna " + COLUMNAS_VERTICES[i]);
            }
        }
        return;
    }

    int numeroLinea = 1;
    while (std::getline(entrada, linea)) {
        numeroLinea++;
        if (recortar(linea).empty()) {
            continue;  // las lineas en blanco se ignoran
        }
        std::vector<std::string> campos = separar(linea, ',');

        // Saca el campo de la columna i, o reporta si viene vacio o no existe
        auto campo = [&](size_t pColumna) -> std::string {
            size_t posicion = static_cast<size_t>(posiciones[pColumna]);
            std::string valor = posicion < campos.size() ? recortar(campos[posicion]) : "";
            if (valor.empty()) {
                pReporte.agregarError(archivo, numeroLinea,
                                      COLUMNAS_VERTICES[pColumna] + " esta vacio");
            }
            return valor;
        };

        std::string id = campo(0);
        std::string nombre = campo(1);
        std::string textoX = campo(2);
        std::string textoY = campo(3);
        std::string tipo = campo(4);
        std::string prioridad = campo(5);

        double x = 0.0;
        double y = 0.0;
        if (!textoX.empty() && !convertirDouble(textoX, x)) {
            pReporte.agregarError(archivo, numeroLinea, "x no es un numero (\"" + textoX + "\")");
        }
        if (!textoY.empty() && !convertirDouble(textoY, y)) {
            pReporte.agregarError(archivo, numeroLinea, "y no es un numero (\"" + textoY + "\")");
        }

        if (id.empty()) {
            continue;
        }
        if (pGrafo.buscarIndice(id) >= 0) {
            pReporte.agregarError(archivo, numeroLinea, "el id " + id + " esta repetido");
            continue;
        }
        // Aunque la linea tenga otros errores, el vertice se agrega igual.
        // Asi las aristas que lo usan no reportan un falso "no existe".
        // No hay riesgo porque con cualquier error el grafo temporal se descarta
        pGrafo.agregarVertice(Vertice(id, nombre, x, y, tipo, prioridad));
    }
}

void CargadorCSV::cargarAristas(const std::string& pRuta, Grafo& pGrafo,
                                ReporteValidacion& pReporte) const {
    const std::string archivo = nombreArchivo(pRuta);
    std::ifstream entrada(pRuta);
    if (!entrada.is_open()) {
        pReporte.agregarError(archivo, 0, "no se pudo abrir el archivo");
        return;
    }

    std::string linea;
    if (!std::getline(entrada, linea)) {
        pReporte.agregarError(archivo, 1, "el archivo esta vacio");
        return;
    }
    std::vector<int> posiciones;
    if (!leerEncabezado(linea, COLUMNAS_ARISTAS, posiciones)) {
        for (size_t i = 0; i < COLUMNAS_ARISTAS.size(); i++) {
            if (posiciones[i] < 0) {
                pReporte.agregarError(archivo, 1, "falta la columna " + COLUMNAS_ARISTAS[i]);
            }
        }
        return;
    }

    int numeroLinea = 1;
    while (std::getline(entrada, linea)) {
        numeroLinea++;
        if (recortar(linea).empty()) {
            continue;
        }
        std::vector<std::string> campos = separar(linea, ',');
        bool lineaValida = true;

        auto campo = [&](size_t pColumna) -> std::string {
            size_t posicion = static_cast<size_t>(posiciones[pColumna]);
            std::string valor = posicion < campos.size() ? recortar(campos[posicion]) : "";
            if (valor.empty()) {
                pReporte.agregarError(archivo, numeroLinea,
                                      COLUMNAS_ARISTAS[pColumna] + " esta vacio");
                lineaValida = false;
            }
            return valor;
        };

        std::string idOrigen = campo(0);
        std::string idDestino = campo(1);
        std::string textoLongitud = campo(2);
        std::string textoVelocidad = campo(3);
        std::string textoUnSentido = campo(4);
        std::string textoCerrable = campo(5);

        int origen = -1;
        int destino = -1;
        if (!idOrigen.empty()) {
            origen = pGrafo.buscarIndice(idOrigen);
            if (origen < 0) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "el vertice " + idOrigen + " no existe");
                lineaValida = false;
            }
        }
        if (!idDestino.empty()) {
            destino = pGrafo.buscarIndice(idDestino);
            if (destino < 0) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "el vertice " + idDestino + " no existe");
                lineaValida = false;
            }
        }

        double longitud = 0.0;
        if (!textoLongitud.empty()) {
            if (!convertirDouble(textoLongitud, longitud)) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "length no es un numero (\"" + textoLongitud + "\")");
                lineaValida = false;
            } else if (longitud <= 0) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "length debe ser mayor que 0 (es " + textoLongitud + ")");
                lineaValida = false;
            }
        }

        double velocidad = 0.0;
        if (!textoVelocidad.empty()) {
            if (!convertirDouble(textoVelocidad, velocidad)) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "speed_limit no es un numero (\"" + textoVelocidad + "\")");
                lineaValida = false;
            } else if (velocidad <= 0) {
                pReporte.agregarError(archivo, numeroLinea,
                                      "speed_limit debe ser mayor que 0 (es " + textoVelocidad + ")");
                lineaValida = false;
            }
        }

        bool unSentido = false;
        if (!textoUnSentido.empty() && !convertirBool(textoUnSentido, unSentido)) {
            pReporte.agregarError(archivo, numeroLinea,
                                  "oneway debe ser true o false (\"" + textoUnSentido + "\")");
            lineaValida = false;
        }
        bool cerrable = false;
        if (!textoCerrable.empty() && !convertirBool(textoCerrable, cerrable)) {
            pReporte.agregarError(archivo, numeroLinea,
                                  "can_close debe ser true o false (\"" + textoCerrable + "\")");
            lineaValida = false;
        }

        if (lineaValida) {
            pGrafo.agregarArista(Arista(origen, destino, longitud, velocidad, unSentido, cerrable));
        }
    }
}

bool CargadorCSV::leerEncabezado(const std::string& pLinea,
                                 const std::vector<std::string>& pColumnasRequeridas,
                                 std::vector<int>& pPosiciones) const {
    std::vector<std::string> columnas = separar(pLinea, ',');
    // Excel a veces guarda el CSV con una marca invisible (BOM) al inicio
    if (!columnas.empty() && columnas[0].rfind("\xEF\xBB\xBF", 0) == 0) {
        columnas[0] = columnas[0].substr(3);
    }

    pPosiciones.assign(pColumnasRequeridas.size(), -1);
    for (size_t i = 0; i < columnas.size(); i++) {
        std::string nombre = enMinusculas(recortar(columnas[i]));
        for (size_t j = 0; j < pColumnasRequeridas.size(); j++) {
            if (nombre == pColumnasRequeridas[j]) {
                pPosiciones[j] = static_cast<int>(i);
            }
        }
    }
    return std::find(pPosiciones.begin(), pPosiciones.end(), -1) == pPosiciones.end();
}

std::vector<std::string> CargadorCSV::separar(const std::string& pLinea, char pSeparador) const {
    // Respeta comillas, para que un nombre como "Lab, piso 2" no se parta en dos
    std::vector<std::string> campos;
    std::string actual;
    bool entreComillas = false;
    for (char c : pLinea) {
        if (c == '"') {
            entreComillas = !entreComillas;
        } else if (c == pSeparador && !entreComillas) {
            campos.push_back(actual);
            actual.clear();
        } else {
            actual += c;
        }
    }
    campos.push_back(actual);
    return campos;
}

std::string CargadorCSV::recortar(const std::string& pTexto) const {
    // Quita espacios y el \r que dejan los archivos guardados en Windows
    const char* espacios = " \t\r\n";
    size_t inicio = pTexto.find_first_not_of(espacios);
    if (inicio == std::string::npos) {
        return "";
    }
    size_t fin = pTexto.find_last_not_of(espacios);
    return pTexto.substr(inicio, fin - inicio + 1);
}

bool CargadorCSV::convertirDouble(const std::string& pTexto, double& pResultado) const {
    try {
        size_t usados = 0;
        double valor = std::stod(pTexto, &usados);
        // "12abc" no es valido: stod tiene que haber usado todo el texto
        if (usados != pTexto.size() || !std::isfinite(valor)) {
            return false;
        }
        pResultado = valor;
        return true;
    } catch (...) {
        return false;  // "cuatro", texto vacio o numero fuera de rango
    }
}

bool CargadorCSV::convertirBool(const std::string& pTexto, bool& pResultado) const {
    std::string texto = enMinusculas(pTexto);
    if (texto == "true") {
        pResultado = true;
        return true;
    }
    if (texto == "false") {
        pResultado = false;
        return true;
    }
    return false;
}
