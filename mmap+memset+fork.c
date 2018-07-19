#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int flag = MAP_ANONYMOUS | MAP_POPULATE|MAP_FIXED;
int do_fork = 0;
int demo = 0;
int debug_print = 0;
int i_from = 30;
int i_till = 39;
int map_size = 4096;
int set_size = 4096;

void usage(){
	printf("Usage: ./mmap+memset+fork [OPTION]\n");	
	printf("Tests virtual adress space updates with fork and mmap options\n\n");
	printf("-d, --debug,		prints out the PID and address touched by memset\n");
	printf("-k, --keystroke,	same as debug, but requires a char to b entered to show each line\n");
	printf("-h, --help,		prints out help message\n");
	printf("-s, --map_shared,	sets MAP_SHARED flag in mmap\n");
	printf("-p, --map_private,	sets MAP_PRIVATE flag in mmap\n");
	printf("-f, --fork_child,	causes program to fork a child process\n");
	printf("-g, --from,		mmaps and memsets from 2^(arg)\n");
	printf("-t, --till,		mmaps and memsets till 2^(arg)\n");
	printf("-a, --map_size,		mmaps given size\n");
	printf("-b, --set_size,		memsets given size\n");
	exit(EXIT_FAILURE);
}


void parse_options (int argc, char *argv[]){
	int long_index = 0;
	int option;
	struct option long_options [] =
	{
		{"debug", no_argument, 0, 'd'},
		{"keystroke", no_argument, 0, 'k'},
		{"help", no_argument, 0, 'h'},
		{"map_shared", no_argument, 0, 's'},
		{"map_private", no_argument, 0, 'p'},
		{"fork_child", no_argument, 0, 'f'},
		{"from", required_argument, 0, 'g'},
		{"till", required_argument, 0, 't'},
		{"map_size", required_argument, 0, 'a'},
		{"set_size", required_argument, 0, 'b'},
		{0,0,0,0}
	};

	while ((option = getopt_long(argc,argv,"dkhspfg:t:a:b:",long_options, &long_index)) != -1){
		switch (option){
		
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
			flag |= MAP_SHARED;
			break;
		case 'p':
			flag |= MAP_PRIVATE;
			break;
		case 'g':
			i_from = atoi (optarg);
			break;
		case 't':
			i_till = atoi (optarg);
			break;
		case 'a':
			map_size = atoi (optarg);
			break;
		case 'b':
			set_size = atoi (optarg);
			break;

		default:
			usage();
		}
	}

}

int main(int argc, char *argv[]){

	int i;
	void * ptr;
	void * addr;
	void ** array;
	int pid = 1;

	
	parse_options (argc,argv);

	array = malloc (sizeof(*array)*(i_till-i_from));

	for (i = i_from; i < i_till; i++){
		addr = (void*)((unsigned long long) 1<<i);
		ptr= mmap(addr, map_size, PROT_READ | PROT_WRITE,
				flag, -1, 0);
		array[i - i_from] = ptr;
		printf ("i=%d,addr=%p,ptr=%p \n",i,addr,ptr);
		if (ptr == MAP_FAILED){
			perror ("mmap");
			return EXIT_FAILURE;
		}
	}
	if (do_fork == 1){
		fork();
	}
	pid = getpid();
	printf ("PID = %d \n",pid);
	for (i = 0; i < (i_till - i_from); i++){

		if (demo == 1){
			getchar ();
		}
		memset (array[i], pid, set_size);
		if (debug_print == 1){
			printf ("PID = %d, touched %p \n", pid, array[i]);
		}
	}
	for (i = i_from; i < i_till; i++){
	char val = *(char*)array [i - i_from];
	if (val!=(char)pid){
		printf("PID = %d\n",pid);
		}
	}
	free(array);	
	return 0;
}




