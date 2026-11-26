#ifndef VERTICE_H
#define VERTICE_H

#include <string>

// Punto de la red (edificio, subestacion, cruce...). Solo guarda datos.
class Vertice {
public:
    Vertice(const std::string& pId, const std::string& pNombre, double pX, double pY,
            const std::string& pTipo, const std::string& pPrioridad)
        : id(pId), nombre(pNombre), x(pX), y(pY), tipo(pTipo), prioridad(pPrioridad) {}

    const std::string& getId() const { return id; }
    const std::string& getNombre() const { return nombre; }
    double getX() const { return x; }
    double getY() const { return y; }
    const std::string& getTipo() const { return tipo; }
    const std::string& getPrioridad() const { return prioridad; }

private:
    std::string id;
    std::string nombre;
    double x;
    double y;
    std::string tipo;
    std::string prioridad;
};

#endif
