#include "Process.h"

// Constructor de la clase Process que inicializa todos los atributos del proceso
// Recibe los parámetros básicos necesarios para definir un proceso en el sistema
Process::Process(std::string n, int bt, int at, int q, int pr) {
    name = n;                    // Nombre o identificador del proceso
    burstTime = bt;              // Tiempo total de CPU que requiere el proceso
    remainingTime = bt;          // Tiempo que le falta ejecutar (inicialmente igual al burst time)
    arrivalTime = at;            // Tiempo en que el proceso llega al sistema
    queueNum = q;                // Número de cola a la que pertenece (1, 2 o 3)
    priority = pr;               // Prioridad dentro de su cola asignada
    responseTime = -1;           // Tiempo de respuesta (inicializado a -1 indica que no ha empezado)
    completionTime = 0;          // Tiempo en que el proceso termina su ejecución
    turnaroundTime = 0;          // Tiempo total que el proceso permanece en el sistema
    waitingTime = 0;             // Tiempo que el proceso espera en colas sin ejecutar
}
