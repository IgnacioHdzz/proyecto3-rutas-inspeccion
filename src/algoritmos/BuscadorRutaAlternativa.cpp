#include "algoritmos/BuscadorRutaAlternativa.h"

ResultadoAlternativa BuscadorRutaAlternativa::buscar(Grafo& pGrafo,
                                                     const EstrategiaRuteo& pEstrategia,
                                                     int pOrigen, int pDestino,
                                                     CriterioPeso pCriterio,
                                                     CriterioCritico pCriterioCritico,
                                                     const ResultadoCriticos& pCriticos) const {
    ResultadoAlternativa resultado;
    resultado.principal = pEstrategia.calcularRuta(pGrafo, pOrigen, pDestino, pCriterio);
    resultado.alternativa.algoritmo = pEstrategia.getNombre();

    if (!resultado.principal.encontrada || resultado.principal.aristas.empty()) {
        return resultado;
    }

    // Si se pidio por puente y la ruta no tiene, se usa por peso
    int critica = -1;
    if (pCriterioCritico == CriterioCritico::POR_PUENTE) {
        critica = elegirPorPuente(resultado.principal, pCriticos);
    }
    if (critica < 0) {
        critica = elegirPorPeso(pGrafo, resultado.principal, pCriterio);
    }
    resultado.aristaEvitada = critica;

    // Si es puente no existe alternativa
    if (esPuente(critica, pCriticos)) {
        resultado.aristaEraPuente = true;
        return resultado;
    }

    // setHabilitada y no cerrarArista, porque tiene que funcionar aunque no sea cerrable
    Arista& arista = pGrafo.getArista(critica);
    bool estabaHabilitada = arista.estaHabilitada();
    arista.setHabilitada(false);
    resultado.alternativa = pEstrategia.calcularRuta(pGrafo, pOrigen, pDestino, pCriterio);
    arista.setHabilitada(estabaHabilitada);

    return resultado;
}

int BuscadorRutaAlternativa::elegirPorPeso(const Grafo& pGrafo, const ResultadoRuta& pRuta,
                                           CriterioPeso pCriterio) const {
    int mejor = -1;
    double mayorPeso = -1.0;
    for (int indiceArista : pRuta.aristas) {
        double peso = pGrafo.getArista(indiceArista).getPeso(pCriterio);
        if (peso > mayorPeso) {
            mayorPeso = peso;
            mejor = indiceArista;
        }
    }
    return mejor;
}

int BuscadorRutaAlternativa::elegirPorPuente(const ResultadoRuta& pRuta,
                                             const ResultadoCriticos& pCriticos) const {
    for (int indiceArista : pRuta.aristas) {
        if (esPuente(indiceArista, pCriticos)) {
            return indiceArista;
        }
    }
    return -1;
}

bool BuscadorRutaAlternativa::esPuente(int pArista, const ResultadoCriticos& pCriticos) const {
    for (const InfoPuente& puente : pCriticos.puentes) {
        if (puente.arista == pArista) {
            return true;
        }
    }
    return false;
}
