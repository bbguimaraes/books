SUBDIRS = calc0 calc1 calc2 calc3 calc4 ex1 ex2 ex3 ex4 ex6 wc

.PHONY: all clean
all:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x"; done
clean:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x" clean; done
