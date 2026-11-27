#include "gui/VentanaPrincipal.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>

#include "gui/PanelConectividad.h"
#include "gui/PanelRuteo.h"


VentanaPrincipal::VentanaPrincipal(QWidget* pPadre) : QMainWindow(pPadre) {
    setWindowTitle("Rutas de inspeccion y puntos criticos");
    resize(1200, 750);
    construirMenu();
    construirLayout();

    // Cada panel se agrega aqui con agregarPanel cuando su parte este lista
    agregarPanel(new PanelRuteo(sistema), "Ruteo");
    agregarPanel(new PanelConectividad(sistema), "Conectividad");
}

void VentanaPrincipal::construirMenu() {
    QMenu* menuArchivo = menuBar()->addMenu("Archivo");
    menuArchivo->addAction("Cargar CSV...", this, &VentanaPrincipal::alCargarDatos);
    menuArchivo->addAction("Mostrar grafo en texto", this, &VentanaPrincipal::alMostrarGrafoTexto);
    menuArchivo->addSeparator();
    menuArchivo->addAction("Salir", this, &QWidget::close);
}

void VentanaPrincipal::construirLayout() {
    pestanas = new QTabWidget();
    pestanas->setMinimumWidth(320);

    vista = new VistaGrafo();
    salida = new QPlainTextEdit();
    salida->setReadOnly(true);
    salida->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    QSplitter* derecha = new QSplitter(Qt::Vertical);
    derecha->addWidget(vista);
    derecha->addWidget(salida);
    derecha->setStretchFactor(0, 3);
    derecha->setStretchFactor(1, 1);

    QSplitter* principal = new QSplitter(Qt::Horizontal);
    principal->addWidget(pestanas);
    principal->addWidget(derecha);
    principal->setStretchFactor(1, 1);
    setCentralWidget(principal);
}

void VentanaPrincipal::agregarPanel(PanelBase* pPanel, const QString& pTitulo) {
    pestanas->addTab(pPanel, pTitulo);
    paneles.push_back(pPanel);

    connect(pPanel, &PanelBase::textoGenerado, this, &VentanaPrincipal::alMostrarTexto);
    connect(pPanel, &PanelBase::aristasResaltadas, vista, &VistaGrafo::resaltarAristas);
    connect(pPanel, &PanelBase::verticesResaltados, vista, &VistaGrafo::resaltarVertices);
    connect(pPanel, &PanelBase::resaltadoLimpiado, vista, &VistaGrafo::limpiarResaltado);
    connect(pPanel, &PanelBase::grafoModificado, this, &VentanaPrincipal::alGrafoModificado);
}

void VentanaPrincipal::alCargarDatos() {
    QString rutaVertices = QFileDialog::getOpenFileName(this, "Seleccione vertices.csv", "datos",
                                                        "CSV (*.csv)");
    if (rutaVertices.isEmpty()) {
        return;
    }
    QString rutaAristas = QFileDialog::getOpenFileName(this, "Seleccione aristas.csv", "datos",
                                                       "CSV (*.csv)");
    if (rutaAristas.isEmpty()) {
        return;
    }

    ReporteValidacion reporte =
        sistema.cargarDatos(rutaVertices.toStdString(), rutaAristas.toStdString());
    QString texto = QString::fromStdString(reporte.toTexto());

    if (!reporte.esValido()) {
        QMessageBox::warning(this, "Archivos rechazados", texto);
        return;
    }

    vista->dibujar(sistema.getGrafo());
    for (PanelBase* panel : paneles) {
        panel->actualizar();
    }
    alMostrarTexto(texto.isEmpty() ? "Datos cargados correctamente." : texto);
}

void VentanaPrincipal::alMostrarGrafoTexto() {
    alMostrarTexto(QString::fromStdString(sistema.getGrafo().toTexto()));
}

void VentanaPrincipal::alMostrarTexto(const QString& pTexto) {
    salida->setPlainText(pTexto);
}

void VentanaPrincipal::alGrafoModificado() {
    vista->dibujar(sistema.getGrafo());
}
