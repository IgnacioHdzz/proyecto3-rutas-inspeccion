#ifndef VISTAGRAFO_H
#define VISTAGRAFO_H

#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>

#include "modelo/Grafo.h"

// Dibuja el grafo usando las coordenadas x, y de cada vertice (D10).
// Aristas de un sentido punteadas, cerradas en gris claro.
// Permite resaltar rutas, puentes, articulaciones o el MST con colores.
// Rueda del mouse para zoom.
// Compartido, ya viene completo.
class VistaGrafo : public QGraphicsView {
    Q_OBJECT

public:
    explicit VistaGrafo(QWidget* pPadre = nullptr);

    void dibujar(const Grafo& pGrafo);
    void resaltarAristas(const std::vector<int>& pAristas, const QColor& pColor);
    void resaltarVertices(const std::vector<int>& pVertices, const QColor& pColor);

    // Vuelve a dibujar el ultimo grafo sin resaltados
    void limpiarResaltado();

protected:
    void resizeEvent(QResizeEvent* pEvento) override;
    void wheelEvent(QWheelEvent* pEvento) override;

private:
    void ajustarVista();

    QGraphicsScene* escena;
    const Grafo* grafoActual = nullptr;
    std::vector<QGraphicsLineItem*> lineas;      // una por arista, mismo indice
    std::vector<QGraphicsEllipseItem*> circulos; // uno por vertice, mismo indice
};

#endif
