#include <stdio.h>
#include <stdlib.h>

#define SIZE_GiB		(1ULL << 30)
#define SIZE_TiB		(1ULL << 40)

/* 2 ^ 47 = 256 TiB, split in half for user/kernel address space. */
#define MAX_USER_VM_4LVL	(128ULL * SIZE_TiB)

int main()
{
	int ret;
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

	if ((i * SIZE_GiB) > MAX_USER_VM_4LVL)
	{
		fprintf(stderr,
			"Unexpected malloc total (%llu bytes > %llu bytes)\n",
			i * SIZE_GiB, MAX_USER_VM_4LVL);
		ret = EXIT_FAILURE;
	}
	else
	{
		ret = EXIT_SUCCESS;
	}

	return ret;
}
