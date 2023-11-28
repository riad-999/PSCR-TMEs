#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>
#include <semaphore.h>
#include <cstring>


using namespace std;
using namespace pr;

const int N = 2;
std::vector<pid_t> processes;

void killall(int) {
	cout << "killing processes" << std::endl;
	for (auto pid : processes) {
		kill(pid, SIGINT);
	}
}

void producteur (Stack<char> * stack) {
	char c;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int main () {
	int fd; 
	void* addr;
	int size = sizeof(Stack<char>);

	shm_unlink("/myshm");

	fd = shm_open("/myshm", O_CREAT | O_EXCL | O_RDWR, 0666);
	ftruncate(fd, size);
	addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	Stack<char> * s = new (addr) Stack<char>();

	for(int i = 0; i < N; i++) {
		pid_t pp = fork();
		if (pp==0) {
			producteur(s);
			return 0;
		} else {
			processes.push_back(pp);
		}
	}

	for(int i = 0; i < N; i++) {
		pid_t pc = fork();
		if (pc==0) {
			consomateur(s);
			return 0;
		} else {
			processes.push_back(pc);
		}
	}

	// wait
	signal(SIGINT, killall);

	for (int i = 0; i < 2*N; i++) {
		wait(nullptr);
	}

	s->~Stack();
	munmap(addr, size);
	shm_unlink("/myshm");

	return 0;
}

