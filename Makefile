CC=cc -Wall
CFLAGS=
LDFLAGS= -lglfw -framework OpenGL
TUTORIALS = tutorial_1 tutorial_2 tutorial_3 
TARGETS = $(join $(addsuffix /,$(TUTORIALS)), $(TUTORIALS))

VPATH=./src/
ODIR=obj

_OBJ=tutorial.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

APP=tutorial

all: $(ODIR) $(APP) $(TARGETS)

$(TUTORIALS)/%:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(APP): $(OBJ) 
	$(CC) $(LDFLAGS) $^ -o $@

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $(ODIR)

.PHONY: clean
clean:
	rm -rf $(ODIR)/*.o $(APP) $(TARGETS)
