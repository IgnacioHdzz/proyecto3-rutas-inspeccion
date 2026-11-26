#include "algoritmos/EstrategiaRuteo.h"

#include <algorithm>

ResultadoRuta EstrategiaRuteo::reconstruirRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                               const std::vector<int>& pAristaPrevia,
                                               CriterioPeso pCriterio) const {
    ResultadoRuta resultado;
    resultado.algoritmo = getNombre();

    // Si al destino no se llego por ninguna arista, no hay ruta
    // (salvo que origen y destino sean el mismo vertice)
    if (pOrigen != pDestino && pAristaPrevia[pDestino] < 0) {
        return resultado;
    }

    // Se camina del destino hacia atras usando la arista con la que se llego a cada vertice
    int actual = pDestino;
    resultado.vertices.push_back(actual);
    while (actual != pOrigen) {
        int indiceArista = pAristaPrevia[actual];
        const Arista& arista = pGrafo.getArista(indiceArista);
        resultado.aristas.push_back(indiceArista);
        resultado.costo += arista.getPeso(pCriterio);
        resultado.longitudTotal += arista.getLongitud();
        resultado.tiempoTotal += arista.getTiempo();
        actual = arista.getOtroExtremo(actual);
        resultado.vertices.push_back(actual);
    }

    // Quedo de destino a origen, se invierte para que vaya de origen a destino
    std::reverse(resultado.vertices.begin(), resultado.vertices.end());
    std::reverse(resultado.aristas.begin(), resultado.aristas.end());
    resultado.encontrada = true;
    return resultado;
}
