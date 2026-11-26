#include "algoritmos/RuteoDijkstra.h"

#include <functional>
#include <limits>
#include <queue>
#include <utility>

ResultadoRuta RuteoDijkstra::calcularRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                          CriterioPeso pCriterio) const {
    const int cantidad = pGrafo.getCantidadVertices();
    if (pOrigen < 0 || pOrigen >= cantidad || pDestino < 0 || pDestino >= cantidad) {
        ResultadoRuta vacio;
        vacio.algoritmo = getNombre();
        return vacio;
    }

    const double INFINITO = std::numeric_limits<double>::infinity();
    std::vector<double> distancia(cantidad, INFINITO);
    std::vector<int> aristaPrevia(cantidad, -1);
    std::vector<bool> visitado(cantidad, false);

    // Cola de prioridad de minimos: (distancia acumulada, vertice)
    using Par = std::pair<double, int>;
    std::priority_queue<Par, std::vector<Par>, std::greater<Par>> cola;

    distancia[pOrigen] = 0.0;
    cola.push({0.0, pOrigen});
    int nodosExplorados = 0;

    while (!cola.empty()) {
        int actual = cola.top().second;
        cola.pop();

        // Un vertice puede quedar varias veces en la cola con distancias viejas.
        // Solo cuenta la primera vez que sale, que es la de menor distancia
        if (visitado[actual]) {
            continue;
        }
        visitado[actual] = true;
        nodosExplorados++;

        if (actual == pDestino) {
            break;  // su distancia ya es definitiva, no hace falta seguir
        }

        // getAristasSalientes ya filtra las cerradas y respeta oneway
        for (int indiceArista : pGrafo.getAristasSalientes(actual)) {
            const Arista& arista = pGrafo.getArista(indiceArista);
            int vecino = arista.getOtroExtremo(actual);
            double nuevaDistancia = distancia[actual] + arista.getPeso(pCriterio);
            if (nuevaDistancia < distancia[vecino]) {
                distancia[vecino] = nuevaDistancia;
                aristaPrevia[vecino] = indiceArista;
                cola.push({nuevaDistancia, vecino});
            }
        }
    }

    ResultadoRuta resultado = reconstruirRuta(pGrafo, pOrigen, pDestino, aristaPrevia, pCriterio);
    resultado.nodosExplorados = nodosExplorados;
    return resultado;
}
