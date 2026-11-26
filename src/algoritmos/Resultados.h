#ifndef RESULTADOS_H
#define RESULTADOS_H

#include <string>
#include <vector>

// Objetos que devuelven los algoritmos. Solo transportan datos entre
// la logica y la interfaz, y todo se guarda por indice.

struct ResultadoRuta {
    bool encontrada = false;
    std::vector<int> vertices;   // secuencia de origen a destino
    std::vector<int> aristas;    // aristas usadas, en el mismo orden
    double costo = 0.0;          // segun el criterio con que se calculo
    double longitudTotal = 0.0;
    double tiempoTotal = 0.0;
    int nodosExplorados = 0;     // para comparar Dijkstra contra A*
    std::string algoritmo;
};

struct ResultadoComponentes {
    std::vector<std::vector<int>> componentes;  // cada una es una lista de vertices
};

struct InfoPuente {
    int arista = -1;
    int tamanoMenor = 0;   // vertices del lado mas pequeno al quitar el puente
    int tamanoMayor = 0;
};

struct ResultadoCriticos {
    std::vector<int> articulaciones;
    std::vector<InfoPuente> puentes;
};

struct ResultadoMST {
    std::vector<int> aristas;
    double costoTotal = 0.0;
    double costoRed = 0.0;          // costo de toda la red habilitada
    double porcentajeAhorro = 0.0;
    int cantidadComponentes = 1;    // mayor que 1 significa que es un bosque
};

struct ResultadoAlternativa {
    ResultadoRuta principal;
    ResultadoRuta alternativa;
    int aristaEvitada = -1;
    bool aristaEraPuente = false;   // si es true no existe alternativa posible
};

struct ResultadoDesvio {
    ResultadoRuta original;
    ResultadoRuta nueva;
    std::vector<int> aristasCerradas;
    double deltaCosto = 0.0;
    double deltaLongitud = 0.0;
    double deltaTiempo = 0.0;
};

#endif
