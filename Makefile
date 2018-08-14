# Vars
CC=gcc
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -std=c11 -Wall -g
else
    CFLAGS= -std=c11 -Wall -O3
endif
LIBS= -lm

# Files
_DEPS = ntw_math.h ntw_crs.h ntwpr_wg.h ntwpr.h
_OBJ = ntw_math.o ntw_crs.o ntwpr_wg.o ntwpr.o test.o

# Folders, Paths
IDIR=include
SDIR=src
ODIR=src/obj
BPATH=bin/pr

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Rules
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $(BPATH) $^ $(CFLAGS) $(LIBS)

debug: $(OBJ)
	$(CC) -o $(BPATH) $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ $(SDIR)/*~
