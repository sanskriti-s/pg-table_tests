PROGS:=heap mmap mmap+memset+fork
SRCS:=heap.c mmap.c mmap+memset+fork.c
SCRIPTS:=run-tests

all: $(PROGS)

check:
	shellcheck $(SCRIPTS)
	cppcheck --error-exitcode=1 $(SRCS)

clean:
	rm -f *.o $(PROGS)
