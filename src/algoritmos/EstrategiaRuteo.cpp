#include "algoritmos/EstrategiaRuteo.h"

#include <algorithm>

ResultadoRuta EstrategiaRuteo::reconstruirRuta(const Grafo& pGrafo, int pOrigen, int pDestino,
                                               const std::vector<int>& pAristaPrevia,
                                               CriterioPeso pCriterio) const {
    ResultadoRuta resultado;
    resultado.algoritmo = getNombre();

    if (pOrigen != pDestino && pAristaPrevia[pDestino] < 0) {
        return resultado;
    }

    // Del destino hacia atras con la arista previa de cada vertice
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

    std::reverse(resultado.vertices.begin(), resultado.vertices.end());
    std::reverse(resultado.aristas.begin(), resultado.aristas.end());
    resultado.encontrada = true;
    return resultado;
}
