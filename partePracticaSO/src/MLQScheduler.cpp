#include "MLQScheduler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Constructor que inicializa el tiempo actual de simulación a cero
MLQScheduler::MLQScheduler() {
    currentTime = 0;
}

// Destructor que libera la memoria de todos los procesos completados
MLQScheduler::~MLQScheduler() {
    for (auto p : finished) delete p;
}

// Función de comparación para ordenar procesos alfabéticamente por nombre
// Se utiliza cuando múltiples procesos llegan al mismo tiempo
bool MLQScheduler::compareByLabel(Process* a, Process* b) {
    return a->name < b->name;
}

// Método para leer y cargar procesos desde un archivo de texto
// El archivo debe contener líneas con formato: nombre;burst_time;arrival_time;cola;prioridad
vector<Process*> MLQScheduler::readFile(string filename) {
    vector<Process*> processes;
    ifstream file(filename);
    string line;

    // Leer el archivo línea por línea
    while (getline(file, line)) {
        // Ignorar líneas vacías y comentarios (líneas que empiezan con #)
        if (line.empty() || line[0] == '#') continue;

        // Utilizar stringstream para dividir la línea por punto y coma
        stringstream ss(line);
        string name, bt, at, q, pr;

        // Extraer cada campo separado por punto y coma
        getline(ss, name, ';');
        getline(ss, bt, ';');
        getline(ss, at, ';');
        getline(ss, q, ';');
        getline(ss, pr, ';');

        // Eliminar espacios en blanco de cada campo para evitar errores de parsing
        name.erase(remove(name.begin(), name.end(), ' '), name.end());
        bt.erase(remove(bt.begin(), bt.end(), ' '), bt.end());
        at.erase(remove(at.begin(), at.end(), ' '), at.end());
        q.erase(remove(q.begin(), q.end(), ' '), q.end());
        pr.erase(remove(pr.begin(), pr.end(), ' '), pr.end());

        // Crear nuevo objeto Process y agregarlo al vector de procesos
        Process* p = new Process(name, stoi(bt), stoi(at), stoi(q), stoi(pr));
        processes.push_back(p);
    }

    file.close();
    return processes;
}

// Método para verificar y procesar los procesos que han llegado al sistema
// Transfiere los procesos de la lista de pendientes a sus colas correspondientes
void MLQScheduler::checkArrivals(vector<Process*>& pending, int time) {
    vector<Process*> arrivedNow;

    // Recorrer la lista de procesos pendientes y recolectar los que han llegado
    auto it = pending.begin();
    while (it != pending.end()) {
        if ((*it)->arrivalTime <= time) {
            arrivedNow.push_back(*it);
            it = pending.erase(it); // Eliminar de pendientes una vez transferido
        } else {
            it++;
        }
    }

    // Ordenar los procesos llegados alfabéticamente para mantener consistencia
    sort(arrivedNow.begin(), arrivedNow.end(), compareByLabel);

    // Distribuir cada proceso a su cola correspondiente según su número de cola
    for (auto p : arrivedNow) {
        if (p->queueNum == 1) queue1.push_back(p);      // Cola 1: Round Robin con quantum 3
        else if (p->queueNum == 2) queue2.push_back(p); // Cola 2: Round Robin con quantum 5
        else if (p->queueNum == 3) queue3.push_back(p); // Cola 3: FCFS (First-Come, First-Served)
    }
}

// Método para obtener el primer proceso de la cola de baja prioridad (FCFS)
// En política FCFS simplemente se retorna el proceso al frente de la cola
Process* MLQScheduler::getFirstJob(deque<Process*>& q) {
    if (q.empty()) return nullptr;
    return q.front();
}

// Método para remover un proceso específico de una cola determinada
// Se utiliza cuando un proceso completa su ejecución y debe ser eliminado del sistema
void MLQScheduler::removeFromQueue(Process* p, deque<Process*>& q) {
    auto it = find(q.begin(), q.end(), p);
    if (it != q.end()) {
        q.erase(it);
    }
}

// Método principal que ejecuta la simulación completa del planificador MLQ
// Coordina la ejecución de procesos según las políticas de cada cola de prioridad
void MLQScheduler::run(vector<Process*> allProcesses) {
    // Ordenar todos los procesos por tiempo de llegada para procesamiento secuencial
    sort(allProcesses.begin(), allProcesses.end(),
         [](Process* a, Process* b) { return a->arrivalTime < b->arrivalTime; });

    vector<Process*> pending = allProcesses;
    currentTime = 0;

    // Bucle principal de simulación que continúa hasta que no queden procesos
    while (!pending.empty() || !queue1.empty() || !queue2.empty() || !queue3.empty()) {

        // Verificar si hay nuevos procesos que hayan llegado al tiempo actual
        checkArrivals(pending, currentTime);

        Process* current = nullptr;
        int quantum = 0;
        int queueId = 0;

        // Selección del próximo proceso a ejecutar basado en prioridad de colas
        // Prioridad estricta: Cola 1 > Cola 2 > Cola 3
        if (!queue1.empty()) {
            current = queue1.front();
            quantum = 3;  // Quantum de 3 unidades para Round Robin en cola 1
            queueId = 1;
        } else if (!queue2.empty()) {
            current = queue2.front();
            quantum = 5;  // Quantum de 5 unidades para Round Robin en cola 2
            queueId = 2;
        } else if (!queue3.empty()) {
            current = getFirstJob(queue3);
            quantum = current->remainingTime; // FCFS: ejecutar hasta completar
            queueId = 3;
        }

        // Si no hay procesos listos, avanzar al próximo tiempo de llegada
        if (current == nullptr) {
            if (!pending.empty()) {
                currentTime = pending[0]->arrivalTime;
            }
            continue;
        }

        // Registrar el tiempo de respuesta si es la primera ejecución del proceso
        if (current->responseTime == -1) {
            current->responseTime = currentTime;
        }

        // Ejecutar el proceso por el tiempo determinado (quantum o tiempo restante)
        int timeToRun = min(quantum, current->remainingTime);
        current->remainingTime -= timeToRun;
        currentTime += timeToRun;

        // Verificar llegadas de nuevos procesos durante el período de ejecución
        checkArrivals(pending, currentTime);

        // Procesar el resultado de la ejecución (terminó o continúa)
        if (current->remainingTime == 0) {
            // Calcular métricas finales del proceso completado
            current->completionTime = currentTime;
            current->turnaroundTime = current->completionTime - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;

            // Agregar a la lista de procesos terminados
            finished.push_back(current);

            // Remover el proceso de su cola original
            if (queueId == 1) removeFromQueue(current, queue1);
            else if (queueId == 2) removeFromQueue(current, queue2);
            else if (queueId == 3) removeFromQueue(current, queue3);

        } else {
            // Si el proceso no terminó, aplicar política de la cola
            if (queueId == 1) {
                // Round Robin: mover al final de la cola 1
                queue1.pop_front();
                queue1.push_back(current);
            } else if (queueId == 2) {
                // Round Robin: mover al final de la cola 2
                queue2.pop_front();
                queue2.push_back(current);
            }
            // Para FCFS (cola 3) el proceso permanece al frente hasta completar
        }
    }
}

// Método para guardar los resultados de la simulación en un archivo de texto
// Genera un archivo con todas las métricas de cada proceso y promedios generales
void MLQScheduler::saveToFile(string inputFile) {
    // Generar nombre del archivo de salida reemplazando la extensión
    string outputFile = inputFile;
    size_t pos = outputFile.find(".txt");
    if (pos != string::npos) {
        outputFile.replace(pos, 4, "_output.txt");
    }

    ofstream out(outputFile);
    // Escribir encabezados descriptivos en el archivo
    out << "# archivo: " << inputFile << endl;
    out << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT" << endl;

    double totalWT = 0, totalCT = 0, totalRT = 0, totalTAT = 0;

    // Escribir los datos de cada proceso completado
    for (auto p : finished) {
        out << p->name << ";" << p->burstTime << ";" << p->arrivalTime << ";"
            << p->queueNum << ";" << p->priority << ";" << p->waitingTime << ";"
            << p->completionTime << ";" << p->responseTime << ";"
            << p->turnaroundTime << endl;

        // Acumular valores para calcular promedios
        totalWT += p->waitingTime;
        totalCT += p->completionTime;
        totalRT += p->responseTime;
        totalTAT += p->turnaroundTime;
    }

    // Calcular y escribir los promedios de todas las métricas
    int n = finished.size();
    out << fixed << setprecision(1);
    out << "WT=" << totalWT/n << "; ";
    out << "CT=" << totalCT/n << "; ";
    out << "RT=" << totalRT/n << "; ";
    out << "TAT=" << totalTAT/n << ";" << endl;

    out.close();
}
