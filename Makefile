# Vars
CC=gcc
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -g
else
    CFLAGS= -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -O3
endif
LIBS= -lm

# Files
_DEPS = ntw_math.h ntw_crs.h ntwpr_wg.h ntwpr.h ntw_debug.h
_OBJ = ntw_math.o ntw_crs.o ntwpr_wg.o ntwpr.o ntw_debug.o
_CONVERTER = WGFcreator.o
_CALCULATOR = NTWPRcalc.o

# Folders
IDIR=include
SDIR=src
ODIR=src/obj
BDIR =bin

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
CONVERTER = $(patsubst %,$(ODIR)/%,$(_CONVERTER))
CALCULATOR = $(patsubst %,$(ODIR)/%,$(_CALCULATOR))

# Rules
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: PRcalculator WFGcreator

PRcalculator: $(OBJ) $(CALCULATOR)
	$(CC) -o $(BDIR)/PRcalculator $^ $(CFLAGS) $(LIBS)

WFGcreator: $(OBJ) $(CONVERTER)
	$(CC) -o $(BDIR)/WGFcreator $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ $(SDIR)/*~
