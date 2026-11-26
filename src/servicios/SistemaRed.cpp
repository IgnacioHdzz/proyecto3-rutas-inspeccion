#include "servicios/SistemaRed.h"

ReporteValidacion SistemaRed::cargarDatos(const std::string& pRutaVertices,
                                          const std::string& pRutaAristas) {
    Grafo nuevo;
    ReporteValidacion reporte = cargador.cargar(pRutaVertices, pRutaAristas, nuevo);
    if (!reporte.esValido()) {
        return reporte;
    }

    grafo = nuevo;
    datosCargados = true;
    return reporte;
}
