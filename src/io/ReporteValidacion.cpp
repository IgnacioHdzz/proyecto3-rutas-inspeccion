#include "io/ReporteValidacion.h"

#include <sstream>

void ReporteValidacion::agregarError(const std::string& pArchivo, int pLinea,
                                     const std::string& pMensaje) {
    // La linea 0 se usa para errores del archivo completo (por ejemplo, no se pudo abrir)
    if (pLinea > 0) {
        errores.push_back(pArchivo + " linea " + std::to_string(pLinea) + ", " + pMensaje);
    } else {
        errores.push_back(pArchivo + ", " + pMensaje);
    }
}

void ReporteValidacion::agregarAdvertencia(const std::string& pMensaje) {
    advertencias.push_back(pMensaje);
}

bool ReporteValidacion::esValido() const {
    return errores.empty();
}

std::string ReporteValidacion::toTexto() const {
    std::ostringstream texto;
    if (esValido()) {
        texto << "Carga exitosa.\n";
    } else {
        texto << "Carga rechazada, se encontraron " << errores.size()
              << (errores.size() == 1 ? " error:\n" : " errores:\n");
        for (const std::string& error : errores) {
            texto << "  - " << error << "\n";
        }
    }
    if (!advertencias.empty()) {
        texto << "Advertencias:\n";
        for (const std::string& advertencia : advertencias) {
            texto << "  - " << advertencia << "\n";
        }
    }
    return texto.str();
}
