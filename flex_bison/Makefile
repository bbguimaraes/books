SUBDIRS = 1 2

.PHONY: all clean
all:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x"; done
clean:
	for x in $(SUBDIRS); do $(MAKE) -C "$$x" clean; done
