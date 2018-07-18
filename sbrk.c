#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_GiB	(1ULL << 30)

int main()
{
	int i=0;
	void *ptr;

	for (;;)
	{
		ptr = sbrk(1 * SIZE_GiB);
		if (ptr == (void *)-1)
			break;
		i++;
	}
	
	printf("%d GiB allocated.\n",i);
	return 0;
}
