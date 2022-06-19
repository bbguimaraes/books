SUBDIRS = wc0 wc1 include concordance cross ex1 ex2 ex3

.PHONY: all clean
all:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x"; done
clean:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x" clean; done
