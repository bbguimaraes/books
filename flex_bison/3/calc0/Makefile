BIN = calc
LDLIBS = -lfl

%.tab.c %.tab.h: %.y
	bison -d $<

.PHONY: all clean
all: $(BIN)
lex.c: calc.h calc.tab.h
calc_c.o: calc.h calc.tab.h
calc: lex.o calc_c.o
clean:
	rm -f $(BIN) calc.c calc.tab.[hc] lex.[co] calc_c.o
