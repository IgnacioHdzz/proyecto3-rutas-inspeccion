#ifndef SISTEMARED_H
#define SISTEMARED_H

#include <string>
#include <vector>

#include "algoritmos/Recorridos.h"
#include "algoritmos/Resultados.h"
#include "io/CargadorCSV.h"
#include "modelo/Grafo.h"
#include "servicios/ReportesRed.h"

// Fachada del sistema. La interfaz solo habla con esta clase.
// Recibe ids de texto, los traduce a indices y delega en la clase que corresponde.
// No contiene logica de algoritmos.
class SistemaRed {
public:
    SistemaRed() = default;

    // Si los CSV tienen errores el grafo actual se conserva sin cambios
    ReporteValidacion cargarDatos(const std::string& pRutaVertices,
                                  const std::string& pRutaAristas);
    bool hayDatos() const { return datosCargados; }
    const Grafo& getGrafo() const { return grafo; }

    // Conectividad
    std::vector<int> recorrer(const std::string& pOrigen, bool pUsarBFS) const;
    ResultadoComponentes calcularComponentes(bool pUsarBFS) const;

    // Reportes
    const ReportesRed& getReportesRed() const { return reportesRed; }

private:
    Grafo grafo;
    bool datosCargados = false;

    CargadorCSV cargador;
    Recorridos recorridos;
    ReportesRed reportesRed;
};

#endif
