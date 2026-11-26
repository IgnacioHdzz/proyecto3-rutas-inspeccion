#include "gui/PanelBase.h"

PanelBase::PanelBase(SistemaRed& pSistema, QWidget* pPadre)
    : QWidget(pPadre), sistema(pSistema) {}

void PanelBase::llenarComboVertices(QComboBox* pCombo) const {
    pCombo->clear();
    const Grafo& grafo = sistema.getGrafo();
    for (int i = 0; i < grafo.getCantidadVertices(); i++) {
        const Vertice& vertice = grafo.getVertice(i);
        QString id = QString::fromStdString(vertice.getId());
        QString texto = id + " - " + QString::fromStdString(vertice.getNombre());
        pCombo->addItem(texto, id);
    }
}

std::string PanelBase::idSeleccionado(const QComboBox* pCombo) const {
    return pCombo->currentData().toString().toStdString();
}
