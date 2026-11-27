#ifndef REPORTESRUTEO_H
#define REPORTESRUTEO_H

#include <string>

#include "algoritmos/Resultados.h"
#include "modelo/Grafo.h"

// Texto de los reportes de ruteo que muestra la interfaz.
// Solo da formato a resultados ya calculados, no ejecuta algoritmos.
class ReportesRuteo {
public:
    // Secuencia de nodos con el costo detallado tramo por tramo y los totales
    std::string textoRuta(const Grafo& pGrafo, const ResultadoRuta& pRuta) const;

    // Ruta principal, arista evitada y ruta alternativa (o por que no existe)
    std::string textoAlternativa(const Grafo& pGrafo,
                                 const ResultadoAlternativa& pAlternativa) const;
};

#endif
