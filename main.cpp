
#include <iostream>
#include <process.h>
#include <random>
#include <time.h>
#include <queue>
    
// Variáveis Globais
double t_meditacao_filosofos[5];
double T_BASE = 0.05;
//std::queue<int> fila;
int fila[5] = {-1, -1, -1, -1, -1};
int NFILS = 5;
int acordados = 0;
    
// Declaração de funções
void tempo_meditacao();
void dormir();
void acordar();
void pegar_dir();
void pegar_esq();
void comer();

int main() {

    // Pre-processamento
    tempo_meditacao();
    

    return 0;
}

// Funções

// Origem da expressão: https://stackoverflow.com/questions/686353/random-float-number-generation
void tempo_meditacao() {
    for (int i = 0; i < NFILS; i++) {
        t_meditacao_filosofos[i] = T_BASE/2 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / T_BASE);
        //std::cout << "t" << i << " = " << t_meditacao_filosofos[i] << std::endl;
    }
}

void acordar() {

}

void dormir() {

}

void pegar_dir() {

}

void pegar_esq() {

}

void comer() {

}

void atualiza_fila() {

}
