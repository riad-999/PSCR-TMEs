#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>


// using namespace std;

int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	int count = 0;

	// system("echo line > lines.txt");

	for (int i=1, j=N; i<=N && j==N; i++ ) {
		pid_t pidf = fork(); 
		if (pidf == 0) {
			// fils
			// system("echo line >> lines.txt");
			std::cout << " i:j " << i << ":" << j << " | pid = " << getpid() << std::endl;
			count = 0;
			for (int k=1; k<=i && j==N ; k++) {
				pidf = fork();
				if (pidf == 0) {
					// system("echo line >> lines.txt");
					j=0;
					std::cout << " k:j " << k << ":" << j << " | pid = " << getpid() << std::endl;
				} else {
					count++;
				}
			}
		} else {
			//pere
			count++;
			break;
		}
	}

	while (wait(NULL) != -1) {}
	return 0;
}
