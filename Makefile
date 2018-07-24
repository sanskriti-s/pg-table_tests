PROGS:=heap mmap mmap+memset+fork
SCRIPTS:=run-tests

all: $(PROGS)

check:
	shellcheck $(SCRIPTS)

clean:
	rm -f *.o $(PROGS)
