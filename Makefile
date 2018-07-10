PROGS:=malloc mmap

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)
