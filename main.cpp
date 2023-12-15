
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
//std::queue<int> fila;
std::vector<int> fila;
bool garfos[5] = {false, false, false, false, false};
int NFILS = 5;
int acordados = 0;
int NITS = 5;

// Mutex
HANDLE mutex_garfo;
HANDLE mutex_fila;

// Declaração de funções
int verificar_prioridade(int, int);
void tempo_meditacao();
void dormir(int);
void acordar(int);
void comer(int);
void func_thread(void *);

int main() {

    // Vetor de Threads
    std::vector<HANDLE> hThreads;
    // Pre-processamento
    tempo_meditacao();

    // Cria as threads
    for(int i = 0; i < NFILS; i++)
        hThreads.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&func_thread, &filosofos[i], CREATE_SUSPENDED, NULL));
    for (int l = 0; l < hThreads.size(); l++)
        ResumeThread(hThreads[l]);
    // Executa o código e para aqui
    WaitForMultipleObjects(NFILS, hThreads.data(), TRUE, INFINITE);
    
    // Suspende todas as threads
    for (int i = 0; i < hThreads.size(); i++)
        SuspendThread(hThreads[i]);

    // Remove as threads utilizadas
    for (int i = 0; i < NFILS; i++)
        CloseHandle(hThreads[i]);

    return 0;
}

// Funções

// Origem da expressão: https://stackoverflow.com/questions/686353/random-float-number-generation
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
    filosofos[id].estado = 'd';
    for (int i = 0; i < fila.size(); i++)
        if (fila[i] == id)
            fila.erase(std::remove(fila.begin(), fila.end(), id), fila.end());
    // Fonte: https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
    
    // Chamada para thread/filosofo dormir
    std::this_thread::sleep_for(std::chrono::duration<double>(filosofos[id].t_meditacao));
}

void comer(int id) {
    filosofos[id].estado = 'e';
    while (!(!garfos[(id + 1) % 5] && !garfos[id] && (verificar_prioridade(id, id - 1) == id) && (verificar_prioridade(id, id + 1)))) {
        std::this_thread::sleep_for(std::chrono::duration<double>(T_BASE/5));
        std::cout << id << " esperando..." << std::endl;
        for (struct filosofo fil : filosofos)
            std::cout << fil.id;
        std::cout << std::endl;
    }
    filosofos[id].estado = 'c';
    
    WaitForSingleObject(mutex_garfo, INFINITE);
        garfos[(id + 1) % 5] = true;
        garfos[id] = true;
    ReleaseMutex(mutex_garfo);
    std::cout << "O filosofo " << id << " esta comendo!" << std::endl;
    WaitForSingleObject(mutex_garfo, INFINITE);
        garfos[(id + 1) % 5] = false;
        garfos[id] = false;
    ReleaseMutex(mutex_garfo);
}

int verificar_prioridade(int id_a, int id_b) {
    // retorna o item que aparece primeiro no vetor
    for(int i = 0; i < fila.size(); i++) {
        if (fila[i] == id_a)
            return id_a;
        else if (fila[i] == id_b)
            return id_b;
    }
    return -1;
}

void func_thread(void *parametros) {
    struct filosofo* fil = (struct filosofo*)parametros;
    // Roda por NITS iterações
    for (int i = 0; i < NITS; i++) {
        dormir(fil->id);
        comer(fil->id);
    }
}

