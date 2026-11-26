#ifndef BUSCADORRUTAALTERNATIVA_H
#define BUSCADORRUTAALTERNATIVA_H

#include "algoritmos/EstrategiaRuteo.h"
#include "algoritmos/Resultados.h"
#include "modelo/Grafo.h"

// Forma de elegir la arista "mas critica" de la ruta principal (D6)
enum class CriterioCritico { POR_PESO, POR_PUENTE };

// Calcula la ruta principal, quita su arista mas critica y recalcula.
// La arista se deshabilita solo durante el calculo y luego se restaura,
// aunque no sea cerrable, porque aqui es una simulacion y no un cierre real.
// Si el criterio es POR_PUENTE y la ruta no tiene puentes se usa POR_PESO.
class BuscadorRutaAlternativa {
public:
    ResultadoAlternativa buscar(Grafo& pGrafo, const EstrategiaRuteo& pEstrategia,
                                int pOrigen, int pDestino, CriterioPeso pCriterio,
                                CriterioCritico pCriterioCritico,
                                const ResultadoCriticos& pCriticos) const;

private:
    // Arista de la ruta con mayor peso
    int elegirPorPeso(const Grafo& pGrafo, const ResultadoRuta& pRuta,
                      CriterioPeso pCriterio) const;

    // Primer puente que aparezca en la ruta, -1 si no tiene
    int elegirPorPuente(const ResultadoRuta& pRuta, const ResultadoCriticos& pCriticos) const;

    bool esPuente(int pArista, const ResultadoCriticos& pCriticos) const;
};

#endif
