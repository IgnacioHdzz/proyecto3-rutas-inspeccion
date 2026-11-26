#include "gui/PanelConectividad.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

namespace {
const QColor COLOR_VISITADO(30, 160, 120);

// Un color por componente. Si hay mas componentes que colores se repiten
const QColor COLORES_COMPONENTES[] = {QColor(30, 160, 120), QColor(220, 120, 30),
                                      QColor(130, 90, 200), QColor(210, 70, 110),
                                      QColor(60, 140, 210), QColor(150, 150, 40)};
}

PanelConectividad::PanelConectividad(SistemaRed& pSistema, QWidget* pPadre)
    : PanelBase(pSistema, pPadre) {
    construirControles();
}

void PanelConectividad::construirControles() {
    comboOrigen = new QComboBox();
    comboTipoRecorrido = new QComboBox();
    comboTipoRecorrido->addItem("BFS (anchura)");
    comboTipoRecorrido->addItem("DFS (profundidad)");
    botonRecorrer = new QPushButton("Recorrer desde el origen");
    botonComponentes = new QPushButton("Calcular componentes conexas");

    QGroupBox* grupoRecorrido = new QGroupBox("Recorrido");
    QFormLayout* formulario = new QFormLayout(grupoRecorrido);
    formulario->addRow("Tipo", comboTipoRecorrido);
    formulario->addRow("Origen", comboOrigen);
    formulario->addRow(botonRecorrer);

    QGroupBox* grupoComponentes = new QGroupBox("Componentes conexas");
    QVBoxLayout* columnaComponentes = new QVBoxLayout(grupoComponentes);
    columnaComponentes->addWidget(botonComponentes);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(grupoRecorrido);
    layout->addWidget(grupoComponentes);
    layout->addStretch();

    connect(botonRecorrer, &QPushButton::clicked, this, &PanelConectividad::alRecorrer);
    connect(botonComponentes, &QPushButton::clicked, this,
            &PanelConectividad::alCalcularComponentes);
}

void PanelConectividad::actualizar() {
    llenarComboVertices(comboOrigen);
}

bool PanelConectividad::usarBFS() const {
    return comboTipoRecorrido->currentIndex() == 0;
}

void PanelConectividad::alRecorrer() {
    if (!sistema.hayDatos()) {
        emit textoGenerado("Primero cargue los datos desde Archivo > Cargar CSV.");
        return;
    }
    std::vector<int> orden = sistema.recorrer(idSeleccionado(comboOrigen), usarBFS());

    emit resaltadoLimpiado();
    emit verticesResaltados(orden, COLOR_VISITADO);
    QString titulo = usarBFS() ? "Recorrido BFS\n" : "Recorrido DFS\n";
    emit textoGenerado(titulo + QString::fromStdString(
                                    sistema.getReportesRed().textoRecorrido(sistema.getGrafo(), orden)));
}

void PanelConectividad::alCalcularComponentes() {
    if (!sistema.hayDatos()) {
        emit textoGenerado("Primero cargue los datos desde Archivo > Cargar CSV.");
        return;
    }
    ResultadoComponentes resultado = sistema.calcularComponentes(usarBFS());

    emit resaltadoLimpiado();
    const int cantidadColores = sizeof(COLORES_COMPONENTES) / sizeof(COLORES_COMPONENTES[0]);
    for (size_t i = 0; i < resultado.componentes.size(); i++) {
        emit verticesResaltados(resultado.componentes[i], COLORES_COMPONENTES[i % cantidadColores]);
    }
    emit textoGenerado(QString::fromStdString(
        sistema.getReportesRed().textoComponentes(sistema.getGrafo(), resultado)));
}
