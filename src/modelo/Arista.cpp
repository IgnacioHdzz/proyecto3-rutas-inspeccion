#include "modelo/Arista.h"

double Arista::getTiempo() const {
    return longitud / velocidad;
}

double Arista::getPeso(CriterioPeso pCriterio) const {
    if (pCriterio == CriterioPeso::TIEMPO) {
        return getTiempo();
    }
    return longitud;
}

int Arista::getOtroExtremo(int pVertice) const {
    if (pVertice == origen) {
        return destino;
    }
    if (pVertice == destino) {
        return origen;
    }
    return -1;
}
