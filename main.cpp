
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

// Variáveis Globais
int hashis[5] = { -1, -1, -1, -1, -1 };
double T_BASE = 0.05;
const int NFILS = 5;

// Mutex
HANDLE mutex_hashi;
HANDLE mutex_io;

// Funções
void comer(int);
void dormir(int);
void acoes(int);

int main() {

	std::thread filosofos[NFILS];
	for (int i = 0; i < NFILS; i++)
		filosofos[i] = std::thread(acoes, i);
	for (int i = 0; i < NFILS; i++)
		filosofos[i].join();

	return 0;
}

void comer(int id) {

}

void dormir(int id) {
	double t_meditacao = T_BASE / 2 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / T_BASE);
	WaitForSingleObject(mutex_io, INFINITE);
	std::cout << "O " << id << "o filosofo ira dormir por " << t_meditacao << "s" << std::endl;
	ReleaseMutex(mutex_io);
	std::this_thread::sleep_for(std::chrono::duration<double>(t_meditacao)); // Chamada para thread/filosofo dormir

}

void acoes(int id) {
	while (1) {
		dormir(id);
		comer(id);
	}
}

