#include "modelo/Grafo.h"

#include <sstream>

int Grafo::agregarVertice(const Vertice& pVertice) {
    if (indicePorId.count(pVertice.getId()) > 0) {
        return -1;
    }
    int indice = static_cast<int>(vertices.size());
    vertices.push_back(pVertice);
    adyacencia.emplace_back();
    indicePorId[pVertice.getId()] = indice;
    return indice;
}

int Grafo::agregarArista(const Arista& pArista) {
    int origen = pArista.getOrigen();
    int destino = pArista.getDestino();
    int cantidad = getCantidadVertices();
    if (origen < 0 || origen >= cantidad || destino < 0 || destino >= cantidad) {
        return -1;
    }
    int indice = static_cast<int>(aristas.size());
    aristas.push_back(pArista);

    // Se registra en ambos extremos aunque sea de un sentido
    adyacencia[origen].push_back(indice);
    if (destino != origen) {
        adyacencia[destino].push_back(indice);
    }
    return indice;
}

int Grafo::buscarIndice(const std::string& pId) const {
    auto encontrado = indicePorId.find(pId);
    if (encontrado == indicePorId.end()) {
        return -1;
    }
    return encontrado->second;
}

std::vector<int> Grafo::getAristasSalientes(int pVertice) const {
    std::vector<int> salientes;
    for (int indice : adyacencia.at(pVertice)) {
        const Arista& arista = aristas[indice];
        if (!arista.estaHabilitada()) {
            continue;
        }
        // Si es de un sentido solo sale desde el origen
        if (arista.esUnSentido() && arista.getOrigen() != pVertice) {
            continue;
        }
        salientes.push_back(indice);
    }
    return salientes;
}

std::vector<int> Grafo::getAristasIncidentes(int pVertice) const {
    std::vector<int> incidentes;
    for (int indice : adyacencia.at(pVertice)) {
        if (aristas[indice].estaHabilitada()) {
            incidentes.push_back(indice);
        }
    }
    return incidentes;
}

int Grafo::getGrado(int pVertice) const {
    return static_cast<int>(getAristasIncidentes(pVertice).size());
}

bool Grafo::cerrarArista(int pIndice) {
    if (pIndice < 0 || pIndice >= getCantidadAristas()) {
        return false;
    }
    Arista& arista = aristas[pIndice];
    if (!arista.esCerrable()) {
        return false;
    }
    arista.setHabilitada(false);
    return true;
}

void Grafo::abrirArista(int pIndice) {
    aristas.at(pIndice).setHabilitada(true);
}

void Grafo::abrirTodas() {
    for (Arista& arista : aristas) {
        arista.setHabilitada(true);
    }
}

std::vector<int> Grafo::getAristasCerradas() const {
    std::vector<int> cerradas;
    for (int i = 0; i < getCantidadAristas(); i++) {
        if (!aristas[i].estaHabilitada()) {
            cerradas.push_back(i);
        }
    }
    return cerradas;
}

double Grafo::getCostoTotal(CriterioPeso pCriterio) const {
    double total = 0.0;
    for (const Arista& arista : aristas) {
        if (arista.estaHabilitada()) {
            total += arista.getPeso(pCriterio);
        }
    }
    return total;
}

std::string Grafo::toTexto() const {
    std::ostringstream texto;
    texto << "Grafo con " << getCantidadVertices() << " vertices y "
          << getCantidadAristas() << " aristas\n";
    for (int v = 0; v < getCantidadVertices(); v++) {
        texto << vertices[v].getId() << " (" << vertices[v].getNombre() << "):";
        bool primera = true;
        for (int indice : adyacencia[v]) {
            const Arista& arista = aristas[indice];
            int otro = arista.getOtroExtremo(v);
            texto << (primera ? " " : ", ") << vertices[otro].getId()
                  << " [long " << arista.getLongitud() << ", vel " << arista.getVelocidad();
            if (arista.esUnSentido()) {
                texto << (arista.getOrigen() == v ? ", un sentido, sale de aqui" : ", un sentido, solo llega");
            }
            if (!arista.estaHabilitada()) {
                texto << ", cerrada";
            }
            texto << "]";
            primera = false;
        }
        if (primera) {
            texto << " (sin conexiones)";
        }
        texto << "\n";
    }
    return texto.str();
}
