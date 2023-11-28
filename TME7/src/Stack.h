#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t mutex;
	sem_t popsem;
	sem_t pushsem;
public :
	Stack () : sz(0) { 
		memset(tab,0,sizeof tab);
		sem_init(&mutex, 1, 1);
		sem_init(&popsem, 1, 0);
		sem_init(&pushsem, 1, STACKSIZE);
	}

	~Stack() {
		sem_destroy(&mutex);
		sem_destroy(&popsem);
		sem_destroy(&pushsem);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&popsem);
		sem_wait(&mutex);
		T toret = tab[--sz];
		sem_post(&mutex);
		sem_post(&pushsem);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&pushsem);
		sem_wait(&mutex);
		tab[sz++] = elt;
		sem_post(&mutex);
		sem_post(&popsem);
	}
};

}
