
#include <iostream>
#include <process.h>
#include <random>
#include <time.h>
#include <vector>    
#include <algorithm>
#include <process.h>
#include <windows.h>
#include <thread>
#include <chrono>

// Estruturas de dados
struct filosofo {
    int id;
    double t_meditacao;
    char estado;            // c = comendo, d = dormindo, e = esperando
};

// Variáveis Globais
std::vector<struct filosofo> filosofos = { {0, 0.0, 'd'}, {1, 0.0, 'd'}, {2, 0.0, 'd'}, {3, 0.0, 'd'}, {4, 0.0, 'd'} };
double T_BASE = 0.05;
std::vector<int> fila;
int hashis[5] = { -1, -1, -1, -1, -1 };
int NFILS = 5;

// Mutex
HANDLE mutex_hashi;
HANDLE mutex_io;

// Declaração de funções
void tempo_meditacao();
void dormir(int);
void acordar(int);
void comer(int);
void func_thread(void*);

int main() {
    std::vector<HANDLE> threads;
    tempo_meditacao();                              // Define tempo de descanso para filosofos
    for (int i = 0; i < NFILS; i++)                // Inicia as threads
        threads.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&func_thread, &filosofos[i], 0, NULL));
    for (int i = 0; i < threads.size(); i++) {     // Suspende e remove todas as threads
        SuspendThread(threads[i]);
    }
    for (int i = 0; i < threads.size(); i++) {     // Suspende e remove todas as threads
        CloseHandle(threads[i]);
    }

    return 0;
}

// Funções
void tempo_meditacao() {
    for (int i = 0; i < NFILS; i++) {
        filosofos[i].t_meditacao = T_BASE / 2 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / T_BASE);
        std::cout << "t" << i << " = " << filosofos[i].t_meditacao << std::endl;
    }
}

void acordar(int id) {
    fila.push_back(id);
    filosofos[id].estado = 'e';
}

void dormir(int id) {
    // remove o filosofo que acabou de comer e irá dormir
    WaitForSingleObject(mutex_io, INFINITE);
    std::cout << "O " << id << "o filosofo esta dormindo!" << std::endl;
    ReleaseMutex(mutex_io);
    std::cout << "Aqui" << std::endl;
    std::this_thread::sleep_for(std::chrono::duration<double>(filosofos[id].t_meditacao)); // Chamada para thread/filosofo dormir
}

void comer(int id) {
    while (1) {
        WaitForSingleObject(mutex_hashi, INFINITE);
        if (hashis[(id + 1) % NFILS] != -1) {
            hashis[(id + 1) % NFILS] = id;
            WaitForSingleObject(mutex_io, INFINITE);
            std::cout << "O " << id << "o filosofo pegou o hashi direito" << std::endl;
            ReleaseMutex(mutex_io);
        }    
        if (hashis[id] == -1) {
            hashis[(id + 1) % NFILS] = id;
            WaitForSingleObject(mutex_io, INFINITE);
            std::cout << "O " << id << "o filosofo pegou o hashi direito" << std::endl;
            ReleaseMutex(mutex_io);
        }
        if(hashis[id] == id && hashis[(id + 1)%5] == id) {
            WaitForSingleObject(mutex_io, INFINITE);
            std::cout << "O " << id << "o filosofo esta comendo" << std::endl;
            ReleaseMutex(mutex_io);
        }
        ReleaseMutex(mutex_hashi);
    }
    std::cout << "O " << id << "o filosofo esta comendo!" << std::endl;
    hashis[id] = -1; 
    hashis[(id + 1) % NFILS] = -1;
}

void func_thread(void* parametros) {
    struct filosofo* fil = (struct filosofo*)parametros;
    while (1) {
        dormir(fil->id);
        comer(fil->id);
    }
}

