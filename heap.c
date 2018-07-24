#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#define SIZE_GiB		(1ULL << 30)

int set_malloc;
int set_sbrk;

void usage(){
	printf("Usage: ./heap [OPTION]\n");
	printf("Allocates requested amount of memory\n\n");
	printf("-m, --malloc		uses malloc to allocate memory\n");
	printf("-s, --sbrk		uses sbrk to allocate memory\n");
	printf("-h, --help              prints out help message\n");
	exit(EXIT_FAILURE);
}

void parse_options (int argc, char *argv[]){
	int long_index = 0;
	int option;
	struct option long_options [] =
	{
		{"malloc", no_argument, 0, 'm'},
		{"sbrk", no_argument, 0, 's'},
		{"help", no_argument, 0, 'h'},
		{0,0,0,0}
	};

	while ((option = getopt_long(argc,argv,"smh",long_options, &long_index)) != -1){
		switch (option){

			case 'm' :
				set_malloc = 1;
				break;
			case 's' :
				set_sbrk = 1;
				break;
			case 'h':
				usage();
			default:
				usage();

		}
	}
}
int main(int argc, char *argv [])
{
	int i=0;
	void *ptr;

	parse_options (argc,argv);
	
	if (set_malloc == 1 && set_sbrk == 1)
		usage();
	
	else if (set_malloc == 1){
		for(;;){
			ptr = malloc(1 * SIZE_GiB);
			if (ptr == NULL)
				break;
			i++;
		}
	free (ptr);
	}
	else if (set_sbrk == 1){
		for(;;){
			ptr = sbrk(1 * SIZE_GiB);
			if (ptr == (void *)-1)
				break;
			i++;
		}
	}

	printf("%d GiB allocated.\n",i);

	return 0;
}
