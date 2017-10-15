CC      = gcc
CFLAGS  = -IC:/msys64/mingw64/include -std=c99 -pedantic -Wall -Wextra \
	-Wno-unused-parameter -Wno-unused-function -Wmissing-prototypes \
	-Wstrict-prototypes -Wold-style-definition
LDFLAGS = -LC:/msys64/mingw64/lib -ljansson -lcurl
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
