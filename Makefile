CC=cc -Wall
CFLAGS=
LDFLAGS= -lglfw -framework OpenGL -framework CoreFoundation -lSOIL
TUTORIALS = tutorial_1 tutorial_2 tutorial_3 tutorial_4 tutorial_5 tutorial_6 tutorial_7 tutorial_8 tutorial_9 tutorial_10 
TARGETS = $(patsubst %,%/run,$(TUTORIALS))

all: $(TARGETS)

%/run : %/*.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGETS)
