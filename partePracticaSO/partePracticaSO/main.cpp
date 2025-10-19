#include <iostream>
#include <vector>
#include <string>
#include "MLQScheduler.h"

using namespace std;

int main() {
    // Lista de archivos de entrada que contienen los datos de los procesos
    // Cada archivo debe seguir el formato: nombre;burst_time;arrival_time;cola;prioridad
    vector<string> files = {"mlq001.txt", "mlq002.txt", "mlq003.txt"};

    // Procesar cada archivo de la lista ejecutando la simulaci�n MLQ
    for (const string& file : files) {
        // Crear una nueva instancia del planificador para cada archivo
        // Esto garantiza que cada simulaci�n comienza con estado limpio
        MLQScheduler scheduler;

        // Leer y cargar los procesos desde el archivo actual
        // El m�todo readFile parsea el archivo y crea los objetos Process correspondientes
        vector<Process*> processes = scheduler.readFile(file);

        // Verificar que el archivo se ley� correctamente y contiene procesos
        if (!processes.empty()) {
            // Ejecutar la simulaci�n completa del planificador MLQ
            // Esto incluye: asignar procesos a colas, ejecutar seg�n pol�ticas de cada cola,
            // y calcular todas las m�tricas de desempe�o
            scheduler.run(processes);

            // Guardar los resultados de la simulaci�n en un archivo de salida
            // El archivo de salida tendr� el mismo nombre base con "_output" agregado
            scheduler.saveToFile(file);
        }
    }

    // Mensaje informativo final indicando d�nde encontrar los resultados
    cout << "Revise los archivos _output.txt para los resultados de la simulacion." << endl;

    return 0;
}
