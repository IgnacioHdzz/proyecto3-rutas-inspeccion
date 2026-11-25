#include <QApplication>
#include <QLabel>

// Si esta ventana abre, Qt, CMake y el compilador estan bien configurados
int main(int argc, char* argv[]) {
    QApplication aplicacion(argc, argv);
    QLabel etiqueta("Entorno listo. Qt " + QString(qVersion()) + " funcionando.");
    etiqueta.setAlignment(Qt::AlignCenter);
    etiqueta.resize(420, 120);
    etiqueta.setWindowTitle("Prueba de entorno");
    etiqueta.show();
    return aplicacion.exec();
}