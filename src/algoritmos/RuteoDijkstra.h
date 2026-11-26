#ifndef RUTEODIJKSTRA_H
#define RUTEODIJKSTRA_H

#include "algoritmos/EstrategiaRuteo.h"

// Dijkstra de un origen a un destino con cola de prioridad.
class RuteoDijkstra : public EstrategiaRuteo {
public:
    ResultadoRuta calcularRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                               CriterioPeso pCriterio) const override;
    std::string getNombre() const override { return "Dijkstra"; }
};

#endif
