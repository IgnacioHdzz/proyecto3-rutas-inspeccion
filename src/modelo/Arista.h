#ifndef ARISTA_H
#define ARISTA_H

// Criterio con el que se mide el costo de recorrer una arista (D3)
enum class CriterioPeso { TIEMPO, DISTANCIA };

// Conexion entre dos vertices (calle, tuberia, cableado...).
// Guarda los indices de sus extremos dentro del Grafo, no los ids de texto.
class Arista {
public:
    Arista(int pOrigen, int pDestino, double pLongitud, double pVelocidad,
           bool pUnSentido, bool pCerrable)
        : origen(pOrigen), destino(pDestino), longitud(pLongitud), velocidad(pVelocidad),
          unSentido(pUnSentido), cerrable(pCerrable), habilitada(true) {}

    int getOrigen() const { return origen; }
    int getDestino() const { return destino; }
    double getLongitud() const { return longitud; }
    double getVelocidad() const { return velocidad; }
    bool esUnSentido() const { return unSentido; }
    bool esCerrable() const { return cerrable; }
    bool estaHabilitada() const { return habilitada; }
    void setHabilitada(bool pHabilitada) { habilitada = pHabilitada; }

    // Tiempo de recorrido, longitud / velocidad
    double getTiempo() const;

    // Costo segun el criterio elegido
    double getPeso(CriterioPeso pCriterio) const;

    // Dado un extremo devuelve el otro. Util al recorrer el grafo sin direccion
    int getOtroExtremo(int pVertice) const;

private:
    int origen;
    int destino;
    double longitud;
    double velocidad;
    bool unSentido;   // oneway del CSV, solo se puede ir de origen a destino
    bool cerrable;    // can_close del CSV
    bool habilitada;  // false cuando la arista esta cerrada (D5)
};

#endif
