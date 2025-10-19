#ifndef MLQSCHEDULER_H
#define MLQSCHEDULER_H

#include "Process.h"
#include <deque>
#include <vector>
#include <string>

class MLQScheduler {
private:
    // Colas de procesos con diferentes niveles de prioridad y políticas de planificación
    std::deque<Process*> queue1; // Cola de alta prioridad: Round Robin con quantum de 3 unidades
    std::deque<Process*> queue2; // Cola de media prioridad: Round Robin con quantum de 5 unidades
    std::deque<Process*> queue3; // Cola de baja prioridad: FCFS (First-Come, First-Served)

    // Lista de procesos que han completado su ejecución en el sistema
    std::vector<Process*> finished;

    // Tiempo actual de la simulación, avanza según se ejecutan los procesos
    int currentTime;

    // Métodos privados para la lógica interna del planificador
    void checkArrivals(std::vector<Process*>& pending, int time);
    Process* getFirstJob(std::deque<Process*>& q);
    void removeFromQueue(Process* p, std::deque<Process*>& q);

public:
    // Constructor y destructor para inicializar y limpiar recursos
    MLQScheduler();
    ~MLQScheduler();

    // Método estático para comparar procesos por nombre (orden alfabético)
    static bool compareByLabel(Process* a, Process* b);

    // Método para cargar procesos desde un archivo de texto con formato específico
    std::vector<Process*> readFile(std::string filename);

    // Método principal que ejecuta la simulación completa del planificador MLQ
    void run(std::vector<Process*> allProcesses);

    // Método para mostrar los resultados de la simulación en la consola
    void showResults();

    // Método para guardar los resultados en un archivo de texto de salida
    void saveToFile(std::string inputFile);
};

#endif
