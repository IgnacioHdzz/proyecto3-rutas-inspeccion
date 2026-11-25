#include "modelo/Arista.h"

double Arista::getTiempo() const {
    // El CargadorCSV garantiza velocidad > 0, asi que no hay division entre cero
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
    return -1;  // pVertice no es extremo de esta arista
}
