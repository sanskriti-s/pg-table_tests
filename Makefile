PROGS:=heap mmf shmat
SRCS:=heap.c mmf.c shmat.c mmf-shm.c mmf-mmap.c
SCRIPTS:=run-tests
GENERATED:=mmap-datafile

all: $(PROGS)

mmf: mmf-mmap.o mmf-shm.o

check:
	shellcheck $(SCRIPTS)
	cppcheck --error-exitcode=1 $(SRCS)

clean:
	rm -f $(PROGS) $(GENERATED) $(SRCS:.c=.o)
