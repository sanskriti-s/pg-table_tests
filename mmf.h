typedef struct {
	int shmem_id;
	void *addr;
}map_t;


typedef struct test_info test_t;
struct test_info {
	int fd;
	map_t *mappings;
	int min_order;
	int max_order;
	int flag;

	int (*mappings_init)(test_t *); 
	int (*map_create)(map_t *, int, test_t *);
	void (*map_destroy)(map_t *);
	void (*mappings_cleanup)(test_t *);
};

extern unsigned long long map_size;

