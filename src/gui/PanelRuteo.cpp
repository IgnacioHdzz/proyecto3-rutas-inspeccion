#include "gui/PanelRuteo.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

namespace {
const QColor COLOR_RUTA(40, 110, 220);
const QColor COLOR_ALTERNATIVA(230, 130, 30);
const QColor COLOR_EVITADA(210, 50, 50);
}

PanelRuteo::PanelRuteo(SistemaRed& pSistema, QWidget* pPadre) : PanelBase(pSistema, pPadre) {
    construirControles();
}

void PanelRuteo::construirControles() {
    comboOrigen = new QComboBox();
    comboDestino = new QComboBox();
    comboAlgoritmo = new QComboBox();
    comboAlgoritmo->addItem("Dijkstra");
    comboAlgoritmo->addItem("A*");
    comboCriterio = new QComboBox();
    comboCriterio->addItem("Tiempo");
    comboCriterio->addItem("Distancia");
    comboCriterioCritico = new QComboBox();
    comboCriterioCritico->addItem("Por peso");
    comboCriterioCritico->addItem("Por puente");
    botonRuta = new QPushButton("Calcular ruta optima");
    botonAlternativa = new QPushButton("Calcular ruta alternativa");

    QGroupBox* grupoRuta = new QGroupBox("Ruta optima");
    QFormLayout* formulario = new QFormLayout(grupoRuta);
    formulario->addRow("Origen", comboOrigen);
    formulario->addRow("Destino", comboDestino);
    formulario->addRow("Algoritmo", comboAlgoritmo);
    formulario->addRow("Criterio", comboCriterio);
    formulario->addRow(botonRuta);

    QGroupBox* grupoAlternativa = new QGroupBox("Ruta alternativa");
    QFormLayout* formularioAlternativa = new QFormLayout(grupoAlternativa);
    formularioAlternativa->addRow("Arista critica", comboCriterioCritico);
    formularioAlternativa->addRow(botonAlternativa);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(grupoRuta);
    layout->addWidget(grupoAlternativa);
    layout->addStretch();

    connect(botonRuta, &QPushButton::clicked, this, &PanelRuteo::alCalcularRuta);
    connect(botonAlternativa, &QPushButton::clicked, this, &PanelRuteo::alCalcularAlternativa);
}

void PanelRuteo::actualizar() {
    llenarComboVertices(comboOrigen);
    llenarComboVertices(comboDestino);
    if (comboDestino->count() > 1) {
        comboDestino->setCurrentIndex(comboDestino->count() - 1);
    }
}

void PanelRuteo::alCalcularRuta() {
    if (!sistema.hayDatos()) {
        emit textoGenerado("Primero cargue los datos desde Archivo > Cargar CSV.");
        return;
    }
    sistema.setAlgoritmo(comboAlgoritmo->currentIndex() == 0 ? AlgoritmoRuteo::DIJKSTRA
                                                             : AlgoritmoRuteo::A_ESTRELLA);
    CriterioPeso criterio =
        comboCriterio->currentIndex() == 0 ? CriterioPeso::TIEMPO : CriterioPeso::DISTANCIA;
    ResultadoRuta ruta =
        sistema.calcularRuta(idSeleccionado(comboOrigen), idSeleccionado(comboDestino), criterio);

    emit resaltadoLimpiado();
    emit aristasResaltadas(ruta.aristas, COLOR_RUTA);
    emit verticesResaltados(ruta.vertices, COLOR_RUTA);
    QString titulo = "Ruta optima con " + comboAlgoritmo->currentText() + " por " +
                     comboCriterio->currentText().toLower() + "\n\n";
    emit textoGenerado(titulo + QString::fromStdString(
                                    sistema.getReportesRuteo().textoRuta(sistema.getGrafo(), ruta)));
}

void PanelRuteo::alCalcularAlternativa() {
    if (!sistema.hayDatos()) {
        emit textoGenerado("Primero cargue los datos desde Archivo > Cargar CSV.");
        return;
    }
    sistema.setAlgoritmo(comboAlgoritmo->currentIndex() == 0 ? AlgoritmoRuteo::DIJKSTRA
                                                             : AlgoritmoRuteo::A_ESTRELLA);
    CriterioPeso criterio =
        comboCriterio->currentIndex() == 0 ? CriterioPeso::TIEMPO : CriterioPeso::DISTANCIA;
    CriterioCritico criterioCritico = comboCriterioCritico->currentIndex() == 0
                                          ? CriterioCritico::POR_PESO
                                          : CriterioCritico::POR_PUENTE;
    ResultadoAlternativa resultado = sistema.calcularAlternativa(
        idSeleccionado(comboOrigen), idSeleccionado(comboDestino), criterio, criterioCritico);

    // Principal en azul, alternativa en naranja y la arista quitada en rojo
    emit resaltadoLimpiado();
    emit aristasResaltadas(resultado.principal.aristas, COLOR_RUTA);
    emit verticesResaltados(resultado.principal.vertices, COLOR_RUTA);
    emit aristasResaltadas(resultado.alternativa.aristas, COLOR_ALTERNATIVA);
    emit verticesResaltados(resultado.alternativa.vertices, COLOR_ALTERNATIVA);
    if (resultado.aristaEvitada >= 0) {
        emit aristasResaltadas({resultado.aristaEvitada}, COLOR_EVITADA);
    }
    emit textoGenerado(QString::fromStdString(
        sistema.getReportesRuteo().textoAlternativa(sistema.getGrafo(), resultado)));
}
