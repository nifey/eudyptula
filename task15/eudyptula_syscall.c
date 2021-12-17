#include<stdio.h>
#include<unistd.h>
#define __NR_eudyptula 449

int main() {
	int retval;

	retval = syscall(__NR_eudyptula, 2, 2);
	if (retval < 0) {
		printf("Passed\n");
	} else {
		printf("Failed\n");
	}

	retval = syscall(__NR_eudyptula, 0, 1234567);
	if (retval == 1234567) {
		printf("Passed\n");
	} else {
		printf("Failed\n");
	}
}
