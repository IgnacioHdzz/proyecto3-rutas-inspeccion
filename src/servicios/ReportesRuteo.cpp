#include "servicios/ReportesRuteo.h"

#include <iomanip>
#include <sstream>

namespace {

std::string numero(double pValor) {
    std::ostringstream texto;
    texto << std::fixed << std::setprecision(2) << pValor;
    return texto.str();
}

std::string nombreArista(const Grafo& pGrafo, int pArista) {
    const Arista& arista = pGrafo.getArista(pArista);
    return pGrafo.getVertice(arista.getOrigen()).getId() + " - " +
           pGrafo.getVertice(arista.getDestino()).getId();
}

}

std::string ReportesRuteo::textoRuta(const Grafo& pGrafo, const ResultadoRuta& pRuta) const {
    if (!pRuta.encontrada) {
        return "No existe una ruta entre el origen y el destino elegidos.\n";
    }
    std::ostringstream texto;
    texto << "Ruta: ";
    for (size_t i = 0; i < pRuta.vertices.size(); i++) {
        texto << (i > 0 ? " -> " : "") << pGrafo.getVertice(pRuta.vertices[i]).getId();
    }
    texto << "\n\n";

    texto << std::left << std::setw(16) << "Tramo" << std::right << std::setw(12) << "Longitud"
          << std::setw(12) << "Tiempo" << "\n";
    for (size_t i = 0; i < pRuta.aristas.size(); i++) {
        const Arista& arista = pGrafo.getArista(pRuta.aristas[i]);
        std::string tramo = pGrafo.getVertice(pRuta.vertices[i]).getId() + " -> " +
                            pGrafo.getVertice(pRuta.vertices[i + 1]).getId();
        texto << std::left << std::setw(16) << tramo << std::right << std::setw(12)
              << numero(arista.getLongitud()) << std::setw(12) << numero(arista.getTiempo())
              << "\n";
    }
    texto << std::left << std::setw(16) << "Total" << std::right << std::setw(12)
          << numero(pRuta.longitudTotal) << std::setw(12) << numero(pRuta.tiempoTotal) << "\n\n";

    texto << "Costo segun el criterio: " << numero(pRuta.costo) << "\n";
    texto << "Nodos explorados por " << pRuta.algoritmo << ": " << pRuta.nodosExplorados << "\n";
    return texto.str();
}

std::string ReportesRuteo::textoAlternativa(const Grafo& pGrafo,
                                            const ResultadoAlternativa& pAlternativa) const {
    std::ostringstream texto;
    texto << "RUTA PRINCIPAL\n" << textoRuta(pGrafo, pAlternativa.principal);
    if (!pAlternativa.principal.encontrada) {
        return texto.str();
    }
    if (pAlternativa.aristaEvitada < 0) {
        texto << "\nEl origen y el destino son el mismo, no hay arista que evitar.\n";
        return texto.str();
    }

    std::string evitada = nombreArista(pGrafo, pAlternativa.aristaEvitada);
    texto << "\nArista mas critica: " << evitada << "\n";
    if (pAlternativa.aristaEraPuente) {
        texto << "Es un puente: sin ella la red queda dividida y no existe ruta alternativa.\n";
        return texto.str();
    }

    texto << "\nRUTA ALTERNATIVA (sin " << evitada << ")\n";
    if (!pAlternativa.alternativa.encontrada) {
        texto << "Sin esa arista no existe otra ruta entre el origen y el destino.\n";
        return texto.str();
    }
    texto << textoRuta(pGrafo, pAlternativa.alternativa);
    texto << "\nDiferencia: +" << numero(pAlternativa.alternativa.longitudTotal -
                                         pAlternativa.principal.longitudTotal)
          << " de longitud, +" << numero(pAlternativa.alternativa.tiempoTotal -
                                         pAlternativa.principal.tiempoTotal)
          << " de tiempo\n";
    return texto.str();
}
