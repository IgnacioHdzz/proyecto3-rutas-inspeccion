#include "servicios/SistemaRed.h"

#include "algoritmos/RuteoAEstrella.h"
#include "algoritmos/RuteoDijkstra.h"

SistemaRed::SistemaRed() : estrategia(std::make_unique<RuteoDijkstra>()) {}

ReporteValidacion SistemaRed::cargarDatos(const std::string& pRutaVertices,
                                          const std::string& pRutaAristas) {
    Grafo nuevo;
    ReporteValidacion reporte = cargador.cargar(pRutaVertices, pRutaAristas, nuevo);
    if (!reporte.esValido()) {
        return reporte;
    }

    grafo = nuevo;
    datosCargados = true;

    // Un grafo no conexo es valido pero se avisa
    if (!recorridos.esConexo(grafo)) {
        reporte.agregarAdvertencia("La red no es conexa, hay partes aisladas entre si.");
    }
    return reporte;
}

std::vector<int> SistemaRed::recorrer(const std::string& pOrigen, bool pUsarBFS) const {
    int origen = grafo.buscarIndice(pOrigen);
    if (origen < 0) {
        return {};
    }
    return pUsarBFS ? recorridos.bfs(grafo, origen) : recorridos.dfs(grafo, origen);
}

ResultadoComponentes SistemaRed::calcularComponentes(bool pUsarBFS) const {
    return pUsarBFS ? recorridos.componentesBFS(grafo) : recorridos.componentesDFS(grafo);
}

void SistemaRed::setAlgoritmo(AlgoritmoRuteo pAlgoritmo) {
    if (pAlgoritmo == AlgoritmoRuteo::DIJKSTRA) {
        estrategia = std::make_unique<RuteoDijkstra>();
    } else {
        estrategia = std::make_unique<RuteoAEstrella>();
    }
}

ResultadoRuta SistemaRed::calcularRuta(const std::string& pOrigen, const std::string& pDestino,
                                       CriterioPeso pCriterio) const {
    int origen = grafo.buscarIndice(pOrigen);
    int destino = grafo.buscarIndice(pDestino);
    if (origen < 0 || destino < 0) {
        return ResultadoRuta();
    }
    return estrategia->calcularRuta(grafo, origen, destino, pCriterio);
}

ResultadoAlternativa SistemaRed::calcularAlternativa(const std::string& pOrigen,
                                                     const std::string& pDestino,
                                                     CriterioPeso pCriterio,
                                                     CriterioCritico pCriterioCritico) {
    int origen = grafo.buscarIndice(pOrigen);
    int destino = grafo.buscarIndice(pDestino);
    if (origen < 0 || destino < 0) {
        return ResultadoAlternativa();
    }
    // TODO cambiar por analizarCriticos() cuando este Tarjan
    ResultadoCriticos criticos;
    return buscadorAlternativa.buscar(grafo, *estrategia, origen, destino, pCriterio,
                                      pCriterioCritico, criticos);
}
