#include "algoritmos/RuteoAEstrella.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <utility>

ResultadoRuta RuteoAEstrella::calcularRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                           CriterioPeso pCriterio) const {
    const int cantidad = pGrafo.getCantidadVertices();
    if (pOrigen < 0 || pOrigen >= cantidad || pDestino < 0 || pDestino >= cantidad) {
        ResultadoRuta vacio;
        vacio.algoritmo = getNombre();
        return vacio;
    }

    const double factorEscala = calcularFactorEscala(pGrafo);
    const double velocidadMaxima = calcularVelocidadMaxima(pGrafo);

    const double INFINITO = std::numeric_limits<double>::infinity();
    std::vector<double> costoReal(cantidad, INFINITO);
    std::vector<int> aristaPrevia(cantidad, -1);
    std::vector<bool> visitado(cantidad, false);

    // La cola ordena por costo real + heuristica
    using Par = std::pair<double, int>;
    std::priority_queue<Par, std::vector<Par>, std::greater<Par>> cola;

    costoReal[pOrigen] = 0.0;
    cola.push({heuristica(pGrafo, pOrigen, pDestino, pCriterio, factorEscala, velocidadMaxima),
               pOrigen});
    int nodosExplorados = 0;

    while (!cola.empty()) {
        int actual = cola.top().second;
        cola.pop();

        if (visitado[actual]) {
            continue;
        }
        visitado[actual] = true;
        nodosExplorados++;

        if (actual == pDestino) {
            break;
        }

        for (int indiceArista : pGrafo.getAristasSalientes(actual)) {
            const Arista& arista = pGrafo.getArista(indiceArista);
            int vecino = arista.getOtroExtremo(actual);
            double nuevoCosto = costoReal[actual] + arista.getPeso(pCriterio);
            if (nuevoCosto < costoReal[vecino]) {
                costoReal[vecino] = nuevoCosto;
                aristaPrevia[vecino] = indiceArista;
                double estimado = nuevoCosto + heuristica(pGrafo, vecino, pDestino, pCriterio,
                                                          factorEscala, velocidadMaxima);
                cola.push({estimado, vecino});
            }
        }
    }

    ResultadoRuta resultado = reconstruirRuta(pGrafo, pOrigen, pDestino, aristaPrevia, pCriterio);
    resultado.nodosExplorados = nodosExplorados;
    return resultado;
}

double RuteoAEstrella::heuristica(const Grafo& pGrafo, int pDesde, int pHasta,
                                  CriterioPeso pCriterio, double pFactorEscala,
                                  double pVelocidadMaxima) const {
    double distancia = distanciaEuclidiana(pGrafo.getVertice(pDesde), pGrafo.getVertice(pHasta))
                       * pFactorEscala;
    if (pCriterio == CriterioPeso::TIEMPO) {
        return distancia / pVelocidadMaxima;
    }
    return distancia;
}

double RuteoAEstrella::distanciaEuclidiana(const Vertice& pA, const Vertice& pB) const {
    double dx = pA.getX() - pB.getX();
    double dy = pA.getY() - pB.getY();
    return std::sqrt(dx * dx + dy * dy);
}

double RuteoAEstrella::calcularFactorEscala(const Grafo& pGrafo) const {
    // Menor length / distancia euclidiana, asi la heuristica nunca sobreestima
    double factor = std::numeric_limits<double>::infinity();
    for (int i = 0; i < pGrafo.getCantidadAristas(); i++) {
        const Arista& arista = pGrafo.getArista(i);
        double recta = distanciaEuclidiana(pGrafo.getVertice(arista.getOrigen()),
                                           pGrafo.getVertice(arista.getDestino()));
        if (recta > 0) {
            factor = std::min(factor, arista.getLongitud() / recta);
        }
    }
    return std::isinf(factor) ? 0.0 : factor;
}

double RuteoAEstrella::calcularVelocidadMaxima(const Grafo& pGrafo) const {
    double maxima = 0.0;
    for (int i = 0; i < pGrafo.getCantidadAristas(); i++) {
        maxima = std::max(maxima, pGrafo.getArista(i).getVelocidad());
    }
    return maxima > 0 ? maxima : 1.0;
}
