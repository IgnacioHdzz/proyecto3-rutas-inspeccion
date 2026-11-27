#ifndef PANELRUTEO_H
#define PANELRUTEO_H

#include <QComboBox>
#include <QPushButton>

#include "gui/PanelBase.h"

// Panel de ruteo. Origen, destino, algoritmo (Dijkstra o A*), criterio
// (tiempo o distancia), ruta optima y ruta alternativa.
class PanelRuteo : public PanelBase {
    Q_OBJECT

public:
    explicit PanelRuteo(SistemaRed& pSistema, QWidget* pPadre = nullptr);
    void actualizar() override;

private slots:
    void alCalcularRuta();
    void alCalcularAlternativa();

private:
    void construirControles();

    QComboBox* comboOrigen = nullptr;
    QComboBox* comboDestino = nullptr;
    QComboBox* comboAlgoritmo = nullptr;
    QComboBox* comboCriterio = nullptr;
    QComboBox* comboCriterioCritico = nullptr;  // por peso o por puente
    QPushButton* botonRuta = nullptr;
    QPushButton* botonAlternativa = nullptr;
};

#endif
