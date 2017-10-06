CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -g -lm
OBJECTS = main.o listf.o calculator.o
EXES = main listf calculator

all: main.o
	$(MAKE) $(EXES)

calculator: calculator.o
	$(CC) -o calculator $(LDFLAGS) calculator.o

listf: listf.o
	$(CC) -o listf $(LDFLAGS) listf.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXES)
