#ifndef RUTEOAESTRELLA_H
#define RUTEOAESTRELLA_H

#include "algoritmos/EstrategiaRuteo.h"

// A* con heuristica euclidiana basada en las coordenadas x, y.
// Para que la heuristica nunca sobreestime (D4) se multiplica la distancia
// euclidiana por un factor de escala, el menor length / distanciaEuclidiana
// entre todas las aristas. Si el criterio es tiempo se divide ademas entre
// la velocidad maxima de la red.
// Responsable Persona 1
class RuteoAEstrella : public EstrategiaRuteo {
public:
    ResultadoRuta calcularRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                               CriterioPeso pCriterio) const override;
    std::string getNombre() const override { return "A*"; }

private:
    double heuristica(const Grafo& pGrafo, int pDesde, int pHasta, CriterioPeso pCriterio,
                      double pFactorEscala, double pVelocidadMaxima) const;
    double distanciaEuclidiana(const Vertice& pA, const Vertice& pB) const;
    double calcularFactorEscala(const Grafo& pGrafo) const;
    double calcularVelocidadMaxima(const Grafo& pGrafo) const;
};

#endif
