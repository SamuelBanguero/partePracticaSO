#include <iostream>
#include <vector>
#include <string>
#include "MLQScheduler.h"

using namespace std;

int main() {
    // Lista de archivos de entrada que contienen los datos de los procesos
    // Cada archivo debe seguir el formato: nombre;burst_time;arrival_time;cola;prioridad
    vector<string> files = {"mlq001.txt", "mlq002.txt", "mlq003.txt"};

    // Procesar cada archivo de la lista ejecutando la simulación MLQ
    for (const string& file : files) {
        // Crear una nueva instancia del planificador para cada archivo
        // Esto garantiza que cada simulación comienza con estado limpio
        MLQScheduler scheduler;

        // Leer y cargar los procesos desde el archivo actual
        // El método readFile parsea el archivo y crea los objetos Process correspondientes
        vector<Process*> processes = scheduler.readFile(file);

        // Verificar que el archivo se leyó correctamente y contiene procesos
        if (!processes.empty()) {
            // Ejecutar la simulación completa del planificador MLQ
            // Esto incluye: asignar procesos a colas, ejecutar según políticas de cada cola,
            // y calcular todas las métricas de desempeño
            scheduler.run(processes);

            // Guardar los resultados de la simulación en un archivo de salida
            // El archivo de salida tendrá el mismo nombre base con "_output" agregado
            scheduler.saveToFile(file);
        }
    }

    // Mensaje informativo final indicando dónde encontrar los resultados
    cout << "Revise los archivos _output.txt para los resultados de la simulacion." << endl;

    return 0;
}
