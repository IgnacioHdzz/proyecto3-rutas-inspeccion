#ifndef SISTEMARED_H
#define SISTEMARED_H

#include <memory>
#include <string>
#include <vector>

#include "algoritmos/BuscadorRutaAlternativa.h"
#include "algoritmos/EstrategiaRuteo.h"
#include "algoritmos/Recorridos.h"
#include "algoritmos/Resultados.h"
#include "io/CargadorCSV.h"
#include "modelo/Grafo.h"
#include "servicios/ReportesRed.h"
#include "servicios/ReportesRuteo.h"

enum class AlgoritmoRuteo { DIJKSTRA, A_ESTRELLA };

// Fachada del sistema. La interfaz solo habla con esta clase.
// Recibe ids de texto, los traduce a indices y delega en la clase que corresponde.
// No contiene logica de algoritmos.
class SistemaRed {
public:
    SistemaRed();

    // Si los CSV tienen errores el grafo actual se conserva sin cambios
    ReporteValidacion cargarDatos(const std::string& pRutaVertices,
                                  const std::string& pRutaAristas);
    bool hayDatos() const { return datosCargados; }
    const Grafo& getGrafo() const { return grafo; }

    // Conectividad
    std::vector<int> recorrer(const std::string& pOrigen, bool pUsarBFS) const;
    ResultadoComponentes calcularComponentes(bool pUsarBFS) const;

    // Ruteo. Cambiar el algoritmo cambia la estrategia que usan ruta, alternativa y cierres
    void setAlgoritmo(AlgoritmoRuteo pAlgoritmo);
    ResultadoRuta calcularRuta(const std::string& pOrigen, const std::string& pDestino,
                               CriterioPeso pCriterio) const;
    ResultadoAlternativa calcularAlternativa(const std::string& pOrigen,
                                             const std::string& pDestino,
                                             CriterioPeso pCriterio,
                                             CriterioCritico pCriterioCritico);

    // Reportes
    const ReportesRed& getReportesRed() const { return reportesRed; }
    const ReportesRuteo& getReportesRuteo() const { return reportesRuteo; }

private:
    Grafo grafo;
    bool datosCargados = false;
    std::unique_ptr<EstrategiaRuteo> estrategia;

    CargadorCSV cargador;
    Recorridos recorridos;
    BuscadorRutaAlternativa buscadorAlternativa;
    ReportesRed reportesRed;
    ReportesRuteo reportesRuteo;
};

#endif
