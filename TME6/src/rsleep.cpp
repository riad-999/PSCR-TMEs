#include <ctime>
#include <cstdlib>
#include <iostream> 
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int points = 10;

void randsleep() {
  int r = rand();
  double ratio = (double)r / (double) RAND_MAX;
  struct timespec tosleep;
  tosleep.tv_sec =0;
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio*700000000;
  struct timespec remain;
  while ( nanosleep(&tosleep, &remain) != 0) {
    tosleep = remain;
  }
}

void handler(int signum) {
  points--;
  std::cout << "process " << getpid() << " points left = " << points << std::endl;
  if (points == 0) {
    std::cout << "process: " << getpid() << " have died" << std::endl;
    exit(1);
  }
}

void attack(pid_t pid) {
  signal(SIGUSR1, handler);
  std::cout << "" ;
  if(kill(pid, SIGUSR1) != 0) {
    std::cout << "process " << getpid() << " won the fight " << std::endl;
    exit(0);
  };
  randsleep();
}

void defense() {
  signal(SIGUSR1, SIG_IGN);
  randsleep();
}

void combat(pid_t pid) {
  while (true) {
    defense();
    attack(pid);
  }
}

int main() {
  pid_t main_pid = getpid();

  pid_t pid = fork();

  if (pid == 0) {
    combat(main_pid);
  } else {
    combat(pid);
  }
}