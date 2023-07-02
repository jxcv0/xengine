#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "assetmgr.h"

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	int fd = shm_open(ASSETMGR_SHMPATH, O_CREAT | O_TRUNC | O_RDWR,
										S_IRWXU | S_IRWXG);

	if (fd == -1) {
		perror("shm_open");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(fd, sizeof(struct assetmgr_shm)) == -1) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	struct assetmgr_shm *shm = mmap(NULL, sizeof(*shm),
												 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (shm == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	if (sem_init(&shm->sem, 1, 0) == -1) {
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	while (1) {
		if (sem_wait(&shm->sem) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		printf("Request recieved for filepath: %s\n", shm->filepath);
		printf("Destination address: %p\n", shm->dest);
	}

	shm_unlink(ASSETMGR_SHMPATH);
	return 0;
}
