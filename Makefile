CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm  # Link the math library
SRC = polyFitExtrapolator.c
OBJ = $(SRC:.c=.o)
HEADER = polyFitExtrapolator.h
LIBRARY = libpolyfit.so

all: $(LIBRARY)

$(LIBRARY): $(OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f $(OBJ) $(LIBRARY)
