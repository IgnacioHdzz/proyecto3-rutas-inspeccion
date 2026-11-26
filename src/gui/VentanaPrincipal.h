#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <vector>

#include "gui/PanelBase.h"
#include "gui/VistaGrafo.h"
#include "servicios/SistemaRed.h"

// Ventana principal. A la izquierda las pestanas con los paneles, a la derecha
// el mapa del grafo y abajo el area de texto donde salen los reportes.
// Solo arma la ventana y conecta las senales de los paneles con el mapa y el texto.
// Compartido, ya viene completo.
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget* pPadre = nullptr);

private slots:
    void alCargarDatos();
    void alMostrarGrafoTexto();
    void alMostrarTexto(const QString& pTexto);
    void alGrafoModificado();

private:
    void construirMenu();
    void construirLayout();
    void agregarPanel(PanelBase* pPanel, const QString& pTitulo);

    SistemaRed sistema;
    VistaGrafo* vista = nullptr;
    QPlainTextEdit* salida = nullptr;
    QTabWidget* pestanas = nullptr;
    std::vector<PanelBase*> paneles;
};

#endif
