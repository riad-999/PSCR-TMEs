#include "Banque.h"
#include <thread>
#include <random>
#include <iostream>

using namespace std;

const int NB_THREAD = 20;
size_t nb_comptes = 1000;

size_t generateRandomSizeT(size_t min, size_t max) {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<size_t> dis(min, max);

    return dis(gen);
}

void qst1_transaction(pr::Banque& banque) {
	for (int i = 0; i <= 1000; ++i){
		size_t deb = generateRandomSizeT(0, nb_comptes - 1);
		size_t cred = generateRandomSizeT(0, nb_comptes - 1);
		while (deb == cred) 
			cred = generateRandomSizeT(0, nb_comptes - 1);
		unsigned int val = generateRandomSizeT(0, 100);
		banque.transfert(deb, cred, val);
	}
	// int ms = generateRandomSizeT(0, 20);
	// this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void qst4_transaction(pr::Banque& banque) {
	for (int i = 0; i <= 1000; ++i){
		size_t deb = generateRandomSizeT(0, nb_comptes - 1);
		size_t cred = generateRandomSizeT(0, nb_comptes - 1);
		while (deb == cred) 
			cred = generateRandomSizeT(0, nb_comptes - 1);
		unsigned int val = generateRandomSizeT(0, 100);
		banque.transfert_atomic(deb, cred, val);
	}
	// int ms = generateRandomSizeT(0, 20);
	// this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void qst_1() {
	vector<thread> threads;
	int init_solde = 10000;
	pr::Banque banque(nb_comptes, init_solde);

	threads.reserve(NB_THREAD);
	for (size_t i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(qst1_transaction, ref(banque));
	}
	for (int i = 0; i < NB_THREAD; ++i) {
		threads[i].join();
	}
	for (size_t i = 0; i < nb_comptes; ++i) {
		cout << "compte num " << i << ", solde: " << banque.get_solde(i) << endl;
	}
}

void qst_4() {
	vector<thread> threads;
	int init_solde = 10000;
	pr::Banque banque(nb_comptes, init_solde);

	threads.reserve(NB_THREAD);
	for (size_t i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(qst4_transaction, ref(banque));
	}
	for (int i = 0; i < NB_THREAD; ++i) {
		threads[i].join();
	}
	for (size_t i = 0; i < nb_comptes; ++i) {
		cout << "compte num " << i << ", solde: " << banque.get_solde(i) << endl;
	}
}

void comptabliser(pr::Banque& banque, int att) {
	banque.comptabiliser(att);
}

void qst_8() {
	vector<thread> threads;
	int init_solde = 10000;
	pr::Banque banque(nb_comptes, init_solde);

	threads.reserve(NB_THREAD);
	for (size_t i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(qst4_transaction, ref(banque));
	}
	thread t(comptabliser, ref(banque), nb_comptes * init_solde);
	for (int i = 0; i < NB_THREAD; ++i) {
		threads[i].join();
	}
	t.join();
	// for (size_t i = 0; i < nb_comptes; ++i) {
	// 	cout << "compte num " << i << ", solde: " << banque.get_solde(i) << endl;
	// }
}

int main () {
	qst_8();
	// vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé


	// for (auto & t : threads) {
	// 	t.join();
	// }

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}


// g++ src/*.cpp -o outexe -std=c++20 -Wall