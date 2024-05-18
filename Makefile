CC = gcc
LIBS = -lncurses

SRCDIR = src
INCDIR = include
BUILDDIR = .build

SOURCES := $(wildcard $(SRCDIR)/*.c)
HEADERS := $(wildcard $(INCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

EXECUTABLE = fo-hacking-minigame-clone

all: release

release: CFLAGS = 
release: $(EXECUTABLE)

debug: CFLAGS = -g -DDEBUG=1
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(EXECUTABLE)

clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)
