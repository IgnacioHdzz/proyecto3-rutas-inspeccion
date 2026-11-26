#include "servicios/SistemaRed.h"

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
