CC=cc -Wall
CFLAGS=
LDFLAGS= -lglfw -framework OpenGL -framework CoreFoundation -lSOIL
TUTORIALS = 1 2 3 4 5 6 7 8 9 10 11 
TARGETS = $(patsubst %,%/run,$(TUTORIALS))

all: $(TARGETS)

%/run : %/*.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGETS)
