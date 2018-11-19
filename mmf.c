#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "mmf.h"
#include "mmf-mmap.h"
#include "mmf-shm.h"


/* for_each_map - iterate over a test_t's map[] array
 *   t: test_t to span
 *   m: this iteration's map_t *
 *   o: allocation order, used to calculate array index
 */
#define for_each_map(t, m, o) \
        for ((o) = (t).min_order, (m) = &(t).mappings[(o) - (t).min_order]; \
             (o) <= (t).max_order; \
             (o)++, (m) = &(t).mappings[(o) - (t).min_order])

int do_mmap = 0;
int do_shmem = 0;
int do_fork = 0;
int demo = 0;
int debug_print = 0;

unsigned long long map_size = 1UL << 12;

void usage(){
	printf("Usage: ./mmap+memset+fork [OPTION]\n");
	printf("Tests virtual adress space updates with fork and mmap options\n\n");
	printf("-m, --mmap		allocates memory using mmap\n");
	printf("-c, --shmem		allocates memory using shared memory\n");
	printf("-d, --debug		prints out the PID and address touched by memset\n");
	printf("-k, --keystroke		same as debug, but requires a char to be entered to show each line\n");
	printf("-h, --help              prints out help message\n");
	printf("-f, --fork_child        causes program to fork a child process\n");
	printf("-b, --begin             maps and memsets from 2^(arg)\n");
	printf("-e, --end               maps and memsets till 2^(arg)\n");
	printf("-o, --mmap_order        maps given 2^(arg) size\n");
	printf("-u, --map_populate      sets MAP_POPULATE flag in mmap\n");
	printf("-a, --map_anonymous     sets MAP_ANONYMOUS flag in mmap\n");
	printf("-s, --map_shared        sets MAP_SHARED flag in mmap\n");
	printf("-p, --map_private       sets MAP_PRIVATE flag in mmap\n");
	exit(EXIT_FAILURE);
}


int parse_options (test_t *test, int argc, char *argv[]){
	int long_index = 0;
	int option;
	struct option long_options [] =
	{
		{"mmap", no_argument, 0, 'm'},
		{"shmem", no_argument, 0, 'c'},
		{"debug", no_argument, 0, 'd'},
		{"keystroke", no_argument, 0, 'k'},
		{"help", no_argument, 0, 'h'},
		{"map_shared", no_argument, 0, 's'},
		{"map_private", no_argument, 0, 'p'},
		{"fork_child", no_argument, 0, 'f'},
		{"begin", required_argument, 0, 'b'},
		{"end", required_argument, 0, 'e'},
		{"mmap_order", required_argument, 0, 'o'},
		{"map_populate", no_argument, 0, 'u'},
		{"map_anonymous", no_argument, 0, 'a'},
		{0,0,0,0}
	};

	while ((option = getopt_long(argc,argv,"mcdkhspfb:e:o:ua",long_options, &long_index)) != -1){
		switch (option){
			case 'm' :
				do_mmap = 1;
				test->mappings_init = &mmap_init;
				test->map_create = &mmap_create;
				test->map_destroy = &mmap_destroy;
				test->mappings_cleanup = &mmap_cleanup;
				break;
			case 'c' :
				do_shmem = 1;
				test->mappings_init = &shm_init;
				test->map_create = &shm_create;
				test->map_destroy = &shm_destroy;
				test->mappings_cleanup = &shm_cleanup;
				break;
			case 'd' :
				debug_print = 1;
				break;
			case 'f' :
				do_fork = 1;
				break;

			case 'k' :
				demo = 1;
				debug_print = 1;
				break;
			case 'h':
				usage();
			case 's':
				test->flag |= MAP_SHARED;
				break;
			case 'p':
				test->flag |= MAP_PRIVATE;
				break;
			case 'b':
				test->min_order = atoi (optarg);
				break;
			case 'e':
				test->max_order = atoi (optarg);
				break;
			case 'o':
				map_size = 1ULL << atoi (optarg);
				break;
			case 'u':
				test->flag |= MAP_POPULATE;
				break;
			case 'a':
				test->flag |= MAP_ANONYMOUS;
				break;
			default:
				usage();
		}
	}

	if (do_mmap ^ do_shmem == 0){
		printf ("Choose either option mmap or option shmem\n");
		return -1;
	}

 	if (do_mmap == 1) {
                int do_shared = !!(test->flag & MAP_SHARED);
                int do_private = !!(test->flag & MAP_PRIVATE);
                if (do_shared ^ do_private == 0){
                        printf("In case of mmap, either map shared or map private must be chosen\n");
                	return -1;
		}
	}

	if (test->min_order >= test->max_order){
		printf ("Max order must be greater than min order\n");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[]){

	int order;
	int pid;
	unsigned char write_pattern;
	size_t mappings_sz;
	test_t test = { .flag=MAP_FIXED, .min_order=30, .max_order=39 };
	map_t *map;
	int ret;

	ret = parse_options (&test, argc,argv);
	if (ret == -1)
		return -1;

	mappings_sz = sizeof(*test.mappings)*((test.max_order-test.min_order)+1);
	test.mappings = malloc (mappings_sz);
	if (!test.mappings) {
		perror("malloc");
		goto done;
	}
	memset (test.mappings, 0, mappings_sz);

	if (test.mappings_init(&test))
		goto cleanup;

	for_each_map(test, map, order) {
		if (test.map_create(map,order, &test)<0)
			goto cleanup;
	}

	if (do_fork == 1){
		fork();
	}
	pid = getpid();
	printf ("PID = %d \n",pid);
	write_pattern = pid;
	for_each_map(test, map, order) {
		if (demo == 1){
			getchar ();
		}
		memset (map->addr, write_pattern, map_size);
		if (debug_print == 1){
			printf ("PID = %d, touched %p \n", pid, map->addr);
		}
	}
	for_each_map(test, map, order) {
		unsigned char val = *(char*) map->addr;
		if (val!=write_pattern){
			printf("PID = %d, %p read(0x%02x) != write_pattern(0x%02x)\n",
					pid, map, val, write_pattern);
		}
	}
cleanup:
	if (test.mappings){
		for_each_map(test, map, order) {
			if (map){ 
				test.map_destroy(map);
			}
		}
		free (test.mappings);
	}
	test.mappings_cleanup(&test);

done:

	return 0;
}
