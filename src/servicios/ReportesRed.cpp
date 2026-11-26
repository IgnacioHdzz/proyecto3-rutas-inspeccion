#include "servicios/ReportesRed.h"

#include <sstream>

namespace {

// "A, B, C" a partir de una lista de indices de vertices
std::string listaIds(const Grafo& pGrafo, const std::vector<int>& pVertices) {
    std::ostringstream texto;
    for (size_t i = 0; i < pVertices.size(); i++) {
        texto << (i > 0 ? ", " : "") << pGrafo.getVertice(pVertices[i]).getId();
    }
    return texto.str();
}

}  // namespace

std::vector<std::pair<int, int>> ReportesRed::topVerticesCriticos(
    const Grafo& pGrafo, const ResultadoCriticos& pCriticos, int pK) const {
    // TODO
    return {};
}

std::vector<InfoPuente> ReportesRed::topPuentesCriticos(const ResultadoCriticos& pCriticos,
                                                        int pK) const {
    // TODO
    return {};
}

std::string ReportesRed::textoRecorrido(const Grafo& pGrafo,
                                        const std::vector<int>& pOrden) const {
    if (pOrden.empty()) {
        return "No se pudo hacer el recorrido.";
    }
    std::ostringstream texto;
    texto << "Orden de visita (" << pOrden.size() << " de " << pGrafo.getCantidadVertices()
          << " vertices):\n" << listaIds(pGrafo, pOrden) << "\n";
    if (static_cast<int>(pOrden.size()) < pGrafo.getCantidadVertices()) {
        texto << "\nNo se alcanzaron todos los vertices, la red no es conexa.\n";
    }
    return texto.str();
}

std::string ReportesRed::textoComponentes(const Grafo& pGrafo,
                                          const ResultadoComponentes& pComponentes) const {
    std::ostringstream texto;
    size_t cantidad = pComponentes.componentes.size();
    texto << "Componentes conexas: " << cantidad
          << (cantidad == 1 ? " (la red es conexa)\n\n" : " (la red NO es conexa)\n\n");
    for (size_t i = 0; i < cantidad; i++) {
        const std::vector<int>& componente = pComponentes.componentes[i];
        texto << "Componente " << (i + 1) << " (" << componente.size() << " vertices): "
              << listaIds(pGrafo, componente) << "\n";
    }
    return texto.str();
}

std::string ReportesRed::textoCriticos(const Grafo& pGrafo,
                                       const ResultadoCriticos& pCriticos) const {
    // TODO
    return "";
}

std::string ReportesRed::textoTopVertices(const Grafo& pGrafo,
                                          const std::vector<std::pair<int, int>>& pTop) const {
    // TODO
    return "";
}

std::string ReportesRed::textoTopPuentes(const Grafo& pGrafo,
                                         const std::vector<InfoPuente>& pTop) const {
    // TODO
    return "";
}

std::string ReportesRed::textoMST(const Grafo& pGrafo, const ResultadoMST& pMST) const {
    // TODO
    return "";
}

std::string ReportesRed::textoDesvio(const Grafo& pGrafo, const ResultadoDesvio& pDesvio) const {
    // TODO
    return "";
}
