#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    // Atributos básicos del proceso definidos en el archivo de entrada
    std::string name;        // Identificador único o etiqueta del proceso
    int burstTime;           // Tiempo total de CPU requerido para completar el proceso
    int remainingTime;       // Tiempo de CPU que aún necesita ejecutar el proceso
    int arrivalTime;         // Momento en que el proceso llega al sistema listo para ejecutar
    int queueNum;            // Número de cola asignada al proceso (1, 2 o 3)
    int priority;            // Nivel de prioridad dentro de la cola asignada

    // Métricas de desempeño que se calculan durante la simulación
    int responseTime;        // Tiempo desde la llegada hasta la primera ejecución (RT)
    int completionTime;      // Momento en que el proceso termina completamente su ejecución (CT)
    int turnaroundTime;      // Tiempo total que el proceso permanece en el sistema (TAT = CT - AT)
    int waitingTime;         // Tiempo que el proceso espera en colas sin ejecutar (WT = TAT - BT)

    // Constructor que inicializa el proceso con los parámetros básicos
    // y establece los valores iniciales para las métricas de desempeño
    Process(std::string n, int bt, int at, int q, int pr);
};

#endif
