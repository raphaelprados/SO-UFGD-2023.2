
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
int T_BASE = 500;
const int NFILS = 5;

// Mutex
HANDLE mutex_hashi[5];
HANDLE mutex_io;

// Funções
void comer(int);
void meditar(int);
void acoes(int);

int main() {

	std::thread filosofos[NFILS];
	for (int i = 0; i < NFILS; i++)
		filosofos[i] = std::thread(acoes, i);
	for (int i = 0; i < NFILS; i++)
		filosofos[i].join();

	return 0;
}

// Fonte: https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/

void comer(int id) {
	bool sucesso = false;
	int espera = 0;
	int dir = (id + 1) % 5,
		esq = id;
	while (1) {
		WaitForSingleObject(mutex_hashi[dir], INFINITE);
		if (hashis[dir] == -1) {
			hashis[dir] = id;
			WaitForSingleObject(mutex_io, INFINITE);
			std::cout << id << " filosofo pegou o hashi direito!" << std::endl;
			ReleaseMutex(mutex_io);
		}
		WaitForSingleObject(mutex_hashi[esq], INFINITE);
		if (hashis[esq] == -1) {
			hashis[esq] = id;
			WaitForSingleObject(mutex_io, INFINITE);
			std::cout << id << " filosofo pegou o hashi esquerdo!" << std::endl;
			ReleaseMutex(mutex_io);
		}
		sucesso = (hashis[dir] == id) && (hashis[esq] = id);
		if (!sucesso) {
			espera++;
			WaitForSingleObject(mutex_io, INFINITE);
			std::cout << id << " filosofo nao consegue comer pela " << espera << "a vez!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(T_BASE));
			ReleaseMutex(mutex_io);
		}
		else {
			WaitForSingleObject(mutex_io, INFINITE);
			std::cout << id << " filosofo esta comendo!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(3 * T_BASE));
			ReleaseMutex(mutex_io);
			hashis[dir] = -1;
			hashis[esq] = -1;
			ReleaseMutex(mutex_hashi[dir]);
			ReleaseMutex(mutex_hashi[esq]);
		}
		if (sucesso)
			break;
	}
}

void meditar(int id) {
	std::cout << "entrou" << std::endl;
	int t_meditacao = T_BASE + (rand()) % 2 * T_BASE;
	WaitForSingleObject(mutex_io, INFINITE);
	std::cout << id << " filosofo ira meditar por " << t_meditacao << "ms" << std::endl;
	ReleaseMutex(mutex_io);
	std::this_thread::sleep_for(std::chrono::milliseconds(t_meditacao)); // Chamada para thread/filosofo dormir

}

void acoes(int id) {
	while (1) {
		meditar(id);
		comer(id);
	}
}

