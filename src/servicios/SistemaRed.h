#ifndef SISTEMARED_H
#define SISTEMARED_H

#include <string>

#include "io/CargadorCSV.h"
#include "modelo/Grafo.h"

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

private:
    Grafo grafo;
    bool datosCargados = false;

    CargadorCSV cargador;
};

#endif
