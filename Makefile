PROGS:=heap mmap+memset+fork
SRCS:=heap.c mmap+memset+fork.c
SCRIPTS:=run-tests
GENERATED:=mmap-datafile

all: $(PROGS)

check:
	shellcheck $(SCRIPTS)
	cppcheck --error-exitcode=1 $(SRCS)

clean:
	rm -f $(PROGS) $(GENERATED)
