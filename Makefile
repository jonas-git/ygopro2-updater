CC      = gcc
CFLAGS  = -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter \
	-Wno-unused-function -Wmissing-prototypes -Wstrict-prototypes \
	-Wold-style-definition \
	`pkg-config --cflags jansson` \
	`pkg-config --cflags libcurl`
LDFLAGS = `pkg-config --libs jansson` \
	`pkg-config --libs libcurl`
SRC = main.c path.c
OBJ = $(SRC:.c=.o)
EXE = updater

all: $(EXE)

$(EXE): $(OBJ) 
	$(CC) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(EXE)
