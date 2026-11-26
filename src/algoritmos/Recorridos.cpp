#include "algoritmos/Recorridos.h"

#include <queue>

std::vector<int> Recorridos::bfs(const Grafo& pGrafo, int pOrigen) const {
    std::vector<int> orden;
    if (pOrigen < 0 || pOrigen >= pGrafo.getCantidadVertices()) {
        return orden;
    }
    std::vector<bool> visitados(pGrafo.getCantidadVertices(), false);
    bfsDesde(pGrafo, pOrigen, visitados, orden);
    return orden;
}

std::vector<int> Recorridos::dfs(const Grafo& pGrafo, int pOrigen) const {
    std::vector<int> orden;
    if (pOrigen < 0 || pOrigen >= pGrafo.getCantidadVertices()) {
        return orden;
    }
    std::vector<bool> visitados(pGrafo.getCantidadVertices(), false);
    dfsDesde(pGrafo, pOrigen, visitados, orden);
    return orden;
}

ResultadoComponentes Recorridos::componentesBFS(const Grafo& pGrafo) const {
    ResultadoComponentes resultado;
    std::vector<bool> visitados(pGrafo.getCantidadVertices(), false);

    // Cada vertice que siga sin visitar es el inicio de una componente nueva.
    // Todo lo que alcance el recorrido desde ahi pertenece a esa componente
    for (int v = 0; v < pGrafo.getCantidadVertices(); v++) {
        if (!visitados[v]) {
            std::vector<int> componente;
            bfsDesde(pGrafo, v, visitados, componente);
            resultado.componentes.push_back(componente);
        }
    }
    return resultado;
}

ResultadoComponentes Recorridos::componentesDFS(const Grafo& pGrafo) const {
    ResultadoComponentes resultado;
    std::vector<bool> visitados(pGrafo.getCantidadVertices(), false);

    // Misma idea que componentesBFS, cambiando solo el recorrido
    for (int v = 0; v < pGrafo.getCantidadVertices(); v++) {
        if (!visitados[v]) {
            std::vector<int> componente;
            dfsDesde(pGrafo, v, visitados, componente);
            resultado.componentes.push_back(componente);
        }
    }
    return resultado;
}

bool Recorridos::esConexo(const Grafo& pGrafo) const {
    // Un grafo vacio se considera conexo, no hay nada desconectado
    return componentesBFS(pGrafo).componentes.size() <= 1;
}

void Recorridos::bfsDesde(const Grafo& pGrafo, int pVertice, std::vector<bool>& pVisitados,
                          std::vector<int>& pOrden) const {
    std::queue<int> cola;

    // Se marca al ENTRAR a la cola y no al salir.
    // Asi un vertice nunca entra dos veces aunque varios vecinos lo descubran
    pVisitados[pVertice] = true;
    cola.push(pVertice);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        pOrden.push_back(actual);

        for (int indiceArista : pGrafo.getAristasIncidentes(actual)) {
            int vecino = pGrafo.getArista(indiceArista).getOtroExtremo(actual);
            if (!pVisitados[vecino]) {
                pVisitados[vecino] = true;
                cola.push(vecino);
            }
        }
    }
}

void Recorridos::dfsDesde(const Grafo& pGrafo, int pVertice, std::vector<bool>& pVisitados,
                          std::vector<int>& pOrden) const {
    pVisitados[pVertice] = true;
    pOrden.push_back(pVertice);

    // La recursion hace de pila. Cada llamada queda en pausa mientras
    // se explora a fondo el vecino, y al volver sigue con el siguiente
    for (int indiceArista : pGrafo.getAristasIncidentes(pVertice)) {
        int vecino = pGrafo.getArista(indiceArista).getOtroExtremo(pVertice);
        if (!pVisitados[vecino]) {
            dfsDesde(pGrafo, vecino, pVisitados, pOrden);
        }
    }
}
