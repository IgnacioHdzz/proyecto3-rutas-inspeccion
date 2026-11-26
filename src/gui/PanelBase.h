#ifndef PANELBASE_H
#define PANELBASE_H

#include <QColor>
#include <QComboBox>
#include <QString>
#include <QWidget>
#include <vector>

#include "servicios/SistemaRed.h"

// Clase base de todos los paneles de la izquierda.
// Cada panel pide cosas a SistemaRed y avisa a la ventana con senales
// (mostrar texto, resaltar en el mapa...). Los paneles no conocen la VistaGrafo.
// Compartido, ya viene completo.
class PanelBase : public QWidget {
    Q_OBJECT

public:
    explicit PanelBase(SistemaRed& pSistema, QWidget* pPadre = nullptr);

    // La ventana lo llama cada vez que se cargan datos nuevos.
    // Sirve por ejemplo para volver a llenar los combos de vertices
    virtual void actualizar() {}

signals:
    void textoGenerado(const QString& pTexto);
    void aristasResaltadas(const std::vector<int>& pAristas, const QColor& pColor);
    void verticesResaltados(const std::vector<int>& pVertices, const QColor& pColor);
    void resaltadoLimpiado();
    void grafoModificado();  // por ejemplo despues de cerrar o abrir aristas

protected:
    // Llena el combo con "A - Edificio A" y guarda el id como dato interno
    void llenarComboVertices(QComboBox* pCombo) const;

    // Id del vertice elegido en un combo llenado con llenarComboVertices
    std::string idSeleccionado(const QComboBox* pCombo) const;

    SistemaRed& sistema;
};

#endif
