#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


// using namespace std;

int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	int count = 0;

	for (int i=1, j=N; i<=N && j==N; i++ ) {
		pid_t pidf = fork(); 
		if (pidf == 0) {
			// fils
			std::cout << " i:j " << i << ":" << j << " | pid = " << getpid() << std::endl;
			count = 0;
			for (int k=1; k<=i && j==N ; k++) {
				pidf = fork();
				if (pidf == 0) {
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
	std::cout << "count = " << count << " | pid = " << getpid() << std::endl;
	return 0;
}
