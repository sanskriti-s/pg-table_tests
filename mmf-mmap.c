#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "mmf.h"
#include "mmf-mmap.h"
#include "mmf-shm.h"

int mmap_init(test_t *test){

	int fd = -1;
	/* Open file, create and truncate it */
	fd = open("mmap-datafile", O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR |
			S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH);
	if (fd == -1) {
		perror("open");
		return EXIT_FAILURE;
	}

	/* Set the file size */
	if (ftruncate(fd, map_size) < 0) {
		perror("ftruncate");
		return EXIT_FAILURE;
	}
	test->fd = fd;
	return 0;
}

int mmap_create(map_t *map, int order, test_t *test){

	void *addr = (void*)((unsigned long long) 1<<order);
	void *ptr;
	ptr= mmap(addr, map_size, PROT_READ | PROT_WRITE,
		test->flag, test->fd, 0);
	printf ("order=%d,addr=%p,ptr=%p \n",order,addr,ptr);
	if (ptr == MAP_FAILED){
		perror ("mmap");
		return -1;
	}
	map->addr = ptr;
	return 0;
}

void mmap_destroy(map_t *map){
	munmap(map->addr, map_size);
}

void mmap_cleanup(test_t *test){
	if (test->fd != -1)
		close(test->fd);
}


