#include "gui/VistaGrafo.h"

#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QWheelEvent>
#include <algorithm>

namespace {
const double ANCHO_ESCENA = 800.0;
const double ALTO_ESCENA = 600.0;
const double RADIO_VERTICE = 10.0;
const QColor COLOR_ARISTA(120, 120, 120);
const QColor COLOR_CERRADA(215, 215, 215);
const QColor COLOR_VERTICE(70, 130, 180);
}

VistaGrafo::VistaGrafo(QWidget* pPadre) : QGraphicsView(pPadre), escena(new QGraphicsScene(this)) {
    setScene(escena);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void VistaGrafo::dibujar(const Grafo& pGrafo) {
    grafoActual = &pGrafo;
    escena->clear();
    lineas.clear();
    circulos.clear();

    int cantidad = pGrafo.getCantidadVertices();
    if (cantidad == 0) {
        return;
    }

    // Rango de coordenadas para escalar el grafo al tamano de la escena
    double minX = pGrafo.getVertice(0).getX();
    double maxX = minX;
    double minY = pGrafo.getVertice(0).getY();
    double maxY = minY;
    for (int i = 1; i < cantidad; i++) {
        const Vertice& vertice = pGrafo.getVertice(i);
        minX = std::min(minX, vertice.getX());
        maxX = std::max(maxX, vertice.getX());
        minY = std::min(minY, vertice.getY());
        maxY = std::max(maxY, vertice.getY());
    }
    double ancho = std::max(maxX - minX, 1e-9);
    double alto = std::max(maxY - minY, 1e-9);
    double escala = std::min(ANCHO_ESCENA / ancho, ALTO_ESCENA / alto);

    // La y se invierte porque en pantalla crece hacia abajo
    auto mapear = [&](const Vertice& pVertice) {
        return QPointF((pVertice.getX() - minX) * escala, (maxY - pVertice.getY()) * escala);
    };

    for (int i = 0; i < pGrafo.getCantidadAristas(); i++) {
        const Arista& arista = pGrafo.getArista(i);
        const Vertice& origen = pGrafo.getVertice(arista.getOrigen());
        const Vertice& destino = pGrafo.getVertice(arista.getDestino());

        QPen lapiz(arista.estaHabilitada() ? COLOR_ARISTA : COLOR_CERRADA, 2);
        if (arista.esUnSentido()) {
            lapiz.setStyle(Qt::DashLine);
        }
        QGraphicsLineItem* linea = escena->addLine(QLineF(mapear(origen), mapear(destino)), lapiz);
        linea->setToolTip(QString("%1 - %2\nlongitud %3, velocidad %4%5")
                              .arg(QString::fromStdString(origen.getId()))
                              .arg(QString::fromStdString(destino.getId()))
                              .arg(arista.getLongitud())
                              .arg(arista.getVelocidad())
                              .arg(arista.estaHabilitada() ? "" : "\nCERRADA"));
        lineas.push_back(linea);
    }

    for (int i = 0; i < cantidad; i++) {
        const Vertice& vertice = pGrafo.getVertice(i);
        QPointF centro = mapear(vertice);
        QGraphicsEllipseItem* circulo = escena->addEllipse(
            centro.x() - RADIO_VERTICE, centro.y() - RADIO_VERTICE, 2 * RADIO_VERTICE,
            2 * RADIO_VERTICE, QPen(Qt::white, 1.5), QBrush(COLOR_VERTICE));
        circulo->setZValue(1);
        circulo->setToolTip(QString("%1\n%2\ntipo %3, prioridad %4")
                                .arg(QString::fromStdString(vertice.getId()))
                                .arg(QString::fromStdString(vertice.getNombre()))
                                .arg(QString::fromStdString(vertice.getTipo()))
                                .arg(QString::fromStdString(vertice.getPrioridad())));
        circulos.push_back(circulo);

        QGraphicsSimpleTextItem* etiqueta =
            escena->addSimpleText(QString::fromStdString(vertice.getId()));
        QFont fuente = etiqueta->font();
        fuente.setPointSize(12);
        fuente.setBold(true);
        etiqueta->setFont(fuente);
        etiqueta->setPos(centro.x() + RADIO_VERTICE, centro.y() - 3 * RADIO_VERTICE);
        etiqueta->setZValue(2);
    }

    ajustarVista();
}

void VistaGrafo::resaltarAristas(const std::vector<int>& pAristas, const QColor& pColor) {
    for (int indice : pAristas) {
        if (indice >= 0 && indice < static_cast<int>(lineas.size())) {
            QPen lapiz = lineas[indice]->pen();
            lapiz.setColor(pColor);
            lapiz.setWidth(5);
            lineas[indice]->setPen(lapiz);
        }
    }
}

void VistaGrafo::resaltarVertices(const std::vector<int>& pVertices, const QColor& pColor) {
    for (int indice : pVertices) {
        if (indice >= 0 && indice < static_cast<int>(circulos.size())) {
            circulos[indice]->setBrush(QBrush(pColor));
        }
    }
}

void VistaGrafo::limpiarResaltado() {
    if (grafoActual != nullptr) {
        dibujar(*grafoActual);
    }
}

void VistaGrafo::resizeEvent(QResizeEvent* pEvento) {
    QGraphicsView::resizeEvent(pEvento);
    ajustarVista();
}

void VistaGrafo::wheelEvent(QWheelEvent* pEvento) {
    double factor = pEvento->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
    scale(factor, factor);
}

void VistaGrafo::ajustarVista() {
    if (!escena->items().isEmpty()) {
        fitInView(escena->itemsBoundingRect().adjusted(-30, -30, 30, 30), Qt::KeepAspectRatio);
    }
}
