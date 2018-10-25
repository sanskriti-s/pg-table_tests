PROGS:=heap mmap+memset+fork shmat
SRCS:=heap.c mmap+memset+fork.c shmat.c
SCRIPTS:=run-tests
GENERATED:=mmap-datafile

all: $(PROGS)

check:
	shellcheck $(SCRIPTS)
	cppcheck --error-exitcode=1 $(SRCS)

clean:
	rm -f $(PROGS) $(GENERATED)
