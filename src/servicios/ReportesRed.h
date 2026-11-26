#ifndef REPORTESRED_H
#define REPORTESRED_H

#include <string>
#include <utility>
#include <vector>

#include "algoritmos/Resultados.h"
#include "modelo/Grafo.h"

// Top-k de elementos criticos y texto de los reportes de la red
// (recorridos, componentes, criticos, MST y desvios).
// Solo trabaja con resultados ya calculados, no ejecuta algoritmos.
// Responsable Persona 2
class ReportesRed {
public:
    // Puntos de articulacion ordenados por grado de mayor a menor. Pares (vertice, grado)
    std::vector<std::pair<int, int>> topVerticesCriticos(const Grafo& pGrafo,
                                                         const ResultadoCriticos& pCriticos,
                                                         int pK) const;

    // Puentes ordenados por tamanoMenor de mayor a menor
    std::vector<InfoPuente> topPuentesCriticos(const ResultadoCriticos& pCriticos, int pK) const;

    std::string textoRecorrido(const Grafo& pGrafo, const std::vector<int>& pOrden) const;
    std::string textoComponentes(const Grafo& pGrafo,
                                 const ResultadoComponentes& pComponentes) const;
    std::string textoCriticos(const Grafo& pGrafo, const ResultadoCriticos& pCriticos) const;
    std::string textoTopVertices(const Grafo& pGrafo,
                                 const std::vector<std::pair<int, int>>& pTop) const;
    std::string textoTopPuentes(const Grafo& pGrafo, const std::vector<InfoPuente>& pTop) const;

    // Costo, lista de aristas y porcentaje de ahorro contra la red completa
    std::string textoMST(const Grafo& pGrafo, const ResultadoMST& pMST) const;

    // Ruta antes y despues del cierre con los deltas
    std::string textoDesvio(const Grafo& pGrafo, const ResultadoDesvio& pDesvio) const;
};

#endif
