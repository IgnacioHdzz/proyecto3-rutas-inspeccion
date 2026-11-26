#ifndef ESTRATEGIARUTEO_H
#define ESTRATEGIARUTEO_H

#include <string>
#include <vector>

#include "algoritmos/Resultados.h"
#include "modelo/Grafo.h"

// Interfaz del patron Strategy para el ruteo (D2).
// Dijkstra y A* la implementan y el resto del programa solo conoce esta clase,
// por eso la ruta alternativa y los cierres funcionan con cualquiera de los dos.
// Responsable Persona 1
class EstrategiaRuteo {
public:
    virtual ~EstrategiaRuteo() = default;

    // Ruta de pOrigen a pDestino respetando oneway y aristas cerradas.
    // Si no hay ruta devuelve un resultado con encontrada = false
    virtual ResultadoRuta calcularRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                       CriterioPeso pCriterio) const = 0;

    virtual std::string getNombre() const = 0;

protected:
    // Arma el ResultadoRuta (vertices, aristas y totales) a partir de la arista
    // con la que se llego a cada vertice. La comparten Dijkstra y A*
    ResultadoRuta reconstruirRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                  const std::vector<int>& pAristaPrevia,
                                  CriterioPeso pCriterio) const;
};

#endif
