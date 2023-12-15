#include <ctime>
#include <cstdlib>
#include <iostream> 
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int points = 10;
sigset_t sig;
sigset_t sigpos;

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
  if (signum != SIGUSR1)
    return;
  points--;
  std::cout << "process " << getpid() << " points left = " << points << std::endl;
  if (points == 0) {
    std::cout << "process: " << getpid() << " have died" << std::endl;
    exit(1);
  }
}

void handler2(int signum) {
  std::cout << "process " << getpid() << " hit paried" << std::endl;
}

void attack(pid_t pid) {
  signal(SIGUSR1, handler);
  // sigprocmask(SIG_UNBLOCK, &sig, nullptr);
  std::cout << "sig sent" << std::endl;
  if(kill(pid, SIGUSR1) < 0) {
    std::cout << "process " << getpid() << " won the fight " << std::endl;
    exit(0);
  };
  randsleep();
}

void defense() {
  // sigprocmask(SIG_BLOCK, &sig, nullptr);
  signal(SIGUSR1, handler2);
  randsleep();
  sigsuspend(&sigpos);
}

void combat(pid_t pid) {
  while (true) {
    attack(pid);
    defense();
  }
}

int main() {
  // sigemptyset(&sig);
  // sigaddset(&sig, SIGUSR1);

  sigfillset(&sigpos);
  sigdelset(&sigpos, SIGUSR1);

  pid_t main_pid = getpid();

  pid_t pid = fork();

  if (pid == 0) {
    combat(main_pid);
  } else {
    combat(pid);
  }
}