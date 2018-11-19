#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include "mmf.h"
#include "mmf-shm.h"


int shm_init(){return 0;}

int shm_create(map_t *map, int order, test_t *test){
	int shmid;
	void *shm_ptr;
	void *addr = (void*)((unsigned long long) 1<<order);
	shmid = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
	if (shmid == -1) {
		perror ("shmget: ");
		return -1;
	}
	map->shmem_id = shmid;

	shm_ptr = shmat(shmid, addr, 0);
	if (shm_ptr == (void *)-1){
		perror("shmat: ");
		return -1;
	}
	map->addr = shm_ptr;
	return 0;
}

void shm_destroy(map_t *map){
	shmdt(map->addr);
	shmctl(map->shmem_id, IPC_RMID, NULL);
}

void shm_cleanup(){}


