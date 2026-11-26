#ifndef PANELCONECTIVIDAD_H
#define PANELCONECTIVIDAD_H

#include <QComboBox>
#include <QPushButton>

#include "gui/PanelBase.h"

// Panel de conectividad. Recorridos BFS y DFS y componentes conexas.
// Mas adelante se agregan puentes y puntos de articulacion (Tarjan).
class PanelConectividad : public PanelBase {
    Q_OBJECT

public:
    explicit PanelConectividad(SistemaRed& pSistema, QWidget* pPadre = nullptr);
    void actualizar() override;

private slots:
    void alRecorrer();
    void alCalcularComponentes();

private:
    void construirControles();
    bool usarBFS() const;

    QComboBox* comboOrigen = nullptr;
    QComboBox* comboTipoRecorrido = nullptr;  // BFS o DFS
    QPushButton* botonRecorrer = nullptr;
    QPushButton* botonComponentes = nullptr;
};

#endif
