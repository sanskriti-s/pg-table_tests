#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int debug_print = 0;
int i_from = 30;
int i_till = 39;

typedef struct node {
	int shmid;
	void* shm_ptr;
	struct node *next;
} LLnode;

LLnode *addNode(LLnode *list, int v1, void* v2) {
	LLnode *newNode;
	newNode = malloc(sizeof(LLnode));
	newNode->shmid = v1;
	newNode->shm_ptr = v2;
	newNode->next = list;
	
	return newNode;
}

void freeList(LLnode *list) {
	LLnode *prev = NULL;
	LLnode *curr = list;

	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
}

void parse_options (int argc, char *argv[]){
	int long_index = 0;
	int option;
	struct option long_options [] =
	{
		{"debug", no_argument, 0, 'd'},
		{"begin", required_argument, 0, 'b'},
		{"end", required_argument, 0, 'e'},
		{0,0,0,0}
	};

	while ((option = getopt_long(argc,argv,"db:e:",long_options, &long_index)) != -1){
		switch (option){
			case 'd' :
				debug_print = 1;
				break;
			case 'b':
				i_from = atoi (optarg);
				break;
			case 'e':
				i_till = atoi (optarg);
				break;
		}
	}
}

int main(int argc, char *argv[]){

	int shmid;
	int i;
	int last_i;
	void* addr;
	void* shm_ptr;
	LLnode *first = NULL;
	LLnode *n;

	parse_options (argc,argv);

	for (i = i_from; i <= i_till; i++){

		addr = (void*)((unsigned long long) 1<<i);
		shmid = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
		if (shmid == -1) {
			if (debug_print == 1)
				perror ("shmget: ");
			goto cleanup;
		}
		shm_ptr = shmat(shmid, addr, 0);
 		if (debug_print == 1)
			printf ("%p, %d \n", addr, i);
		first = addNode(first,shmid,shm_ptr);
		if (shm_ptr == (void*) -1) {
			printf("Allocates memory till %d \n",i);
			goto cleanup;
		}
		last_i =i;
	}
	printf ("Allocates memory till %d \n",last_i);

cleanup:
	
	for (n = first; n != NULL; n = n->next){
		shmdt(n->shm_ptr);
		shmctl(n->shmid, IPC_RMID, NULL);
	}

	freeList(first);

	return 0;
}
