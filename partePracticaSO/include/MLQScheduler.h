#ifndef MLQSCHEDULER_H
#define MLQSCHEDULER_H

#include "Process.h"
#include <deque>
#include <vector>
#include <string>

class MLQScheduler {
private:
    // Colas de procesos con diferentes niveles de prioridad y pol�ticas de planificaci�n
    std::deque<Process*> queue1; // Cola de alta prioridad: Round Robin con quantum de 3 unidades
    std::deque<Process*> queue2; // Cola de media prioridad: Round Robin con quantum de 5 unidades
    std::deque<Process*> queue3; // Cola de baja prioridad: FCFS (First-Come, First-Served)

    // Lista de procesos que han completado su ejecuci�n en el sistema
    std::vector<Process*> finished;

    // Tiempo actual de la simulaci�n, avanza seg�n se ejecutan los procesos
    int currentTime;

    // M�todos privados para la l�gica interna del planificador
    void checkArrivals(std::vector<Process*>& pending, int time);
    Process* getFirstJob(std::deque<Process*>& q);
    void removeFromQueue(Process* p, std::deque<Process*>& q);

public:
    // Constructor y destructor para inicializar y limpiar recursos
    MLQScheduler();
    ~MLQScheduler();

    // M�todo est�tico para comparar procesos por nombre (orden alfab�tico)
    static bool compareByLabel(Process* a, Process* b);

    // M�todo para cargar procesos desde un archivo de texto con formato espec�fico
    std::vector<Process*> readFile(std::string filename);

    // M�todo principal que ejecuta la simulaci�n completa del planificador MLQ
    void run(std::vector<Process*> allProcesses);

    // M�todo para mostrar los resultados de la simulaci�n en la consola
    void showResults();

    // M�todo para guardar los resultados en un archivo de texto de salida
    void saveToFile(std::string inputFile);
};

#endif
