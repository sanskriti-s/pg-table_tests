#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i=0;
	char *str;
	//FILE *f;
	//f = fopen("output.txt", "w");
	/*if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}*/

	for (;;)
	{
		str = (char*) malloc(100000000);
		if (str == NULL)
		{
			perror("malloc");
			break;
		}
		i+=100;
		
		//fprintf(f,"%d \n",i);
			
	}

	
	printf ("\n %d megabytes \n",i);
	return 0;
}
