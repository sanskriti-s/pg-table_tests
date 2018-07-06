#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

int i;
int i_from;
int i_till;
int map_size;
int set_size;
void *ptr;
void * addr;
int c = 1;

        if (argc==1){
                i_from = 40;
                i_till = 64;
                map_size = 4096;
		set_size = 4096;
        }

        else if (argc!=5){
                printf ("Enter mmap <i_from> <i_till> <allocation size> <set size>");
		return 0;
        }

        else {
                i_from = atoi (argv[1]);
                i_till = atoi (argv[2]);
                map_size = atoi (argv[3]);
		set_size = atoi (argv[4]);
        }

        if ( i_from >=i_till){
                printf("Error: i_from must be less than i_till");
		return 0;
        }

        if ( i_from > 64 || i_till > 64){
                printf("Error: i_from and i_till must be less than 64");
		return 0;
        }


        for (i=i_from; i<i_till; i++){
                void * addr = (void*)((unsigned long long) 1<<i);
                ptr= mmap(addr, map_size, PROT_READ|PROT_WRITE,
                        MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED,-1, 0);
                printf ("i=%d,addr=%p,ptr=%p \n",i,addr,ptr);
                getchar();
		if (ptr != MAP_FAILED){
	 		memset (addr,c, set_size);
        	        printf ("touched %p",addr);
                	getchar();
                }
		

        }


return 0;
}
