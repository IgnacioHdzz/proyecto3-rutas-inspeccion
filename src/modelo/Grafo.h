#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <unordered_map>
#include <vector>

#include "modelo/Arista.h"
#include "modelo/Vertice.h"

// Red de infraestructura como lista de adyacencia con indices (D1).
// Cada arista se guarda una sola vez y cada vertice conoce los indices
// de las aristas que lo tocan, sin importar la direccion.
// Aqui no van algoritmos, solo datos y consultas basicas.
// Responsable Persona 1
class Grafo {
public:
    Grafo() = default;

    // Devuelve el indice asignado al vertice nuevo
    int agregarVertice(const Vertice& pVertice);

    // Devuelve el indice asignado a la arista nueva.
    // Tambien la registra en la adyacencia de sus dos extremos
    int agregarArista(const Arista& pArista);

    int getCantidadVertices() const { return static_cast<int>(vertices.size()); }
    int getCantidadAristas() const { return static_cast<int>(aristas.size()); }
    const Vertice& getVertice(int pIndice) const { return vertices.at(pIndice); }
    const Arista& getArista(int pIndice) const { return aristas.at(pIndice); }
    Arista& getArista(int pIndice) { return aristas.at(pIndice); }

    // Traduce un id de texto ("A") a su indice. Devuelve -1 si no existe
    int buscarIndice(const std::string& pId) const;

    // Aristas habilitadas por las que se puede SALIR de pVertice respetando oneway.
    // Las usan el ruteo y los cierres
    std::vector<int> getAristasSalientes(int pVertice) const;

    // Aristas habilitadas que tocan pVertice ignorando la direccion.
    // Las usan los recorridos, Tarjan y el MST
    std::vector<int> getAristasIncidentes(int pVertice) const;

    // Grado sin direccion contando solo aristas habilitadas
    int getGrado(int pVertice) const;

    // Cierra una arista solo si es cerrable. Devuelve false si no se pudo
    bool cerrarArista(int pIndice);
    void abrirArista(int pIndice);
    void abrirTodas();
    std::vector<int> getAristasCerradas() const;

    // Suma del peso de todas las aristas habilitadas. La usa el % de ahorro del MST
    double getCostoTotal(CriterioPeso pCriterio) const;

    // Representacion textual (lista de adyacencia) para "imprimir el grafo"
    std::string toTexto() const;

private:
    std::vector<Vertice> vertices;
    std::vector<Arista> aristas;
    std::vector<std::vector<int>> adyacencia;          // indices de aristas por vertice
    std::unordered_map<std::string, int> indicePorId;  // id de texto a indice
};

#endif
