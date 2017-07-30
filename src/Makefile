IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

APP=uno

_DEPS = card.h deck.h utility.h game.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o card.o deck.o game.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

$(APP): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

run:
	./$(APP)
    
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
