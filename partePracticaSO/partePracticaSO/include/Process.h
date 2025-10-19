#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    // Atributos b�sicos del proceso definidos en el archivo de entrada
    std::string name;        // Identificador �nico o etiqueta del proceso
    int burstTime;           // Tiempo total de CPU requerido para completar el proceso
    int remainingTime;       // Tiempo de CPU que a�n necesita ejecutar el proceso
    int arrivalTime;         // Momento en que el proceso llega al sistema listo para ejecutar
    int queueNum;            // N�mero de cola asignada al proceso (1, 2 o 3)
    int priority;            // Nivel de prioridad dentro de la cola asignada

    // M�tricas de desempe�o que se calculan durante la simulaci�n
    int responseTime;        // Tiempo desde la llegada hasta la primera ejecuci�n (RT)
    int completionTime;      // Momento en que el proceso termina completamente su ejecuci�n (CT)
    int turnaroundTime;      // Tiempo total que el proceso permanece en el sistema (TAT = CT - AT)
    int waitingTime;         // Tiempo que el proceso espera en colas sin ejecutar (WT = TAT - BT)

    // Constructor que inicializa el proceso con los par�metros b�sicos
    // y establece los valores iniciales para las m�tricas de desempe�o
    Process(std::string n, int bt, int at, int q, int pr);
};

#endif
