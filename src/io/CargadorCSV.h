#ifndef CARGADORCSV_H
#define CARGADORCSV_H

#include <string>
#include <vector>

#include "io/ReporteValidacion.h"
#include "modelo/Grafo.h"

// Lee vertices.csv y aristas.csv, valida cada linea y construye el Grafo.
// Valida columnas obligatorias, campos vacios, numeros y booleanos mal escritos,
// ids repetidos, aristas hacia vertices inexistentes y longitud o velocidad <= 0.
// Si hay errores pGrafo no se toca y el reporte explica por que.
class CargadorCSV {
public:
    ReporteValidacion cargar(const std::string& pRutaVertices, const std::string& pRutaAristas,
                             Grafo& pGrafo) const;

private:
    void cargarVertices(const std::string& pRuta, Grafo& pGrafo,
                        ReporteValidacion& pReporte) const;
    void cargarAristas(const std::string& pRuta, Grafo& pGrafo,
                       ReporteValidacion& pReporte) const;

    // Revisa que esten las columnas requeridas y guarda en que posicion esta cada una.
    // Asi el archivo funciona aunque las columnas vengan en otro orden
    bool leerEncabezado(const std::string& pLinea,
                        const std::vector<std::string>& pColumnasRequeridas,
                        std::vector<int>& pPosiciones) const;

    std::vector<std::string> separar(const std::string& pLinea, char pSeparador) const;
    std::string recortar(const std::string& pTexto) const;
    bool convertirDouble(const std::string& pTexto, double& pResultado) const;
    bool convertirBool(const std::string& pTexto, bool& pResultado) const;
};

#endif
