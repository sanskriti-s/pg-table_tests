#include <stdio.h>
#include <stdlib.h>

#define SIZE_GiB		(1ULL << 30)

int main()
{
	int i=0;
	void *ptr;

	for (;;)
	{
		ptr = malloc(1 * SIZE_GiB);
		if (ptr == NULL)
			break;
		i++;
	}
	
	printf("%d GiB allocated.\n",i);

	return 0;
}
