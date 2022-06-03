BIN = calc
LDLIBS = -lfl

%.tab.c %.tab.h: %.y
	bison -d $<

.PHONY: all clean
all: $(BIN)
lex.c: calc.tab.h
calc: lex.c
clean:
	rm -f $(BIN) calc.tab.[hc]
