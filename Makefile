PROGS:=malloc mmap sbrk
SCRIPTS:=run-tests

all: $(PROGS)

check:
	shellcheck $(SCRIPTS)

clean:
	rm -f *.o $(PROGS)
