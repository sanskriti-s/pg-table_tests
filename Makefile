PROGS:=malloc mmap sbrk

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
