#ifndef RECORRIDOS_H
#define RECORRIDOS_H

#include <vector>

#include "algoritmos/Resultados.h"
#include "modelo/Grafo.h"

// Recorridos en anchura (BFS) y profundidad (DFS) y componentes conexas.
// Trabaja sobre el grafo SIN direccion (usa getAristasIncidentes).
class Recorridos {
public:
    // Orden en que se visitan los vertices partiendo de pOrigen
    std::vector<int> bfs(const Grafo& pGrafo, int pOrigen) const;
    std::vector<int> dfs(const Grafo& pGrafo, int pOrigen) const;

    ResultadoComponentes componentesBFS(const Grafo& pGrafo) const;
    ResultadoComponentes componentesDFS(const Grafo& pGrafo) const;

    bool esConexo(const Grafo& pGrafo) const;

private:
    // Visita todo lo alcanzable desde pVertice y lo agrega a pOrden
    void bfsDesde(const Grafo& pGrafo, int pVertice, std::vector<bool>& pVisitados,
                  std::vector<int>& pOrden) const;
    void dfsDesde(const Grafo& pGrafo, int pVertice, std::vector<bool>& pVisitados,
                  std::vector<int>& pOrden) const;
};

#endif
