#ifndef REPORTEVALIDACION_H
#define REPORTEVALIDACION_H

#include <string>
#include <vector>

// Junta los errores y advertencias encontrados al cargar los CSV.
// Con un solo error la carga se rechaza completa (D8), pero se
// reportan TODOS los errores con su linea, no solo el primero.
// Las advertencias no bloquean la carga (por ejemplo, grafo no conexo).
// Responsable Persona 1
class ReporteValidacion {
public:
    // Ejemplo de mensaje generado "aristas.csv linea 7, speed_limit debe ser mayor que 0"
    void agregarError(const std::string& pArchivo, int pLinea, const std::string& pMensaje);
    void agregarAdvertencia(const std::string& pMensaje);

    // true si no hay errores
    bool esValido() const;

    const std::vector<std::string>& getErrores() const { return errores; }
    const std::vector<std::string>& getAdvertencias() const { return advertencias; }

    // Todo el reporte listo para mostrarlo en pantalla
    std::string toTexto() const;

private:
    std::vector<std::string> errores;
    std::vector<std::string> advertencias;
};

#endif
