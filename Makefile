##
# >To build everything just run:
#	$ make
# >To build the API for debug run:
# 	$ make DEBUG=1 
# >To build only one of the 3 available programs run:
# 	For the serial pagerank calculator: 	$ make PRcalculatorSerial
#	For the parallel pagerank calculator:	$ make PRcalculator
#	For the web graph file creator:			$ make WFGcreator
# >To clean the extra files (.o and temp files) run:
#	$ make clean
##

# Vars
CC=gcc
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS= -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -g
else
    CFLAGS= -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -O3
endif
LIBS= -lm -fopenmp

# Files
_DEPS = ntw_math.h ntw_crs.h ntwpr_wg.h ntwpr.h ntw_debug.h ntw_collections.h
_OBJ = ntw_math.o ntw_crs.o ntwpr_wg.o ntwpr.o ntw_debug.o ntw_collections.o
_CONVERTER = WGFcreator.o
_CALCULATOR = NTWpagerank_serial.o
_PCALCULATOR = NTWpagerank.o

# Folders
IDIR=include
SDIR=src
ODIR=src/obj
BDIR =bin

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
CONVERTER = $(patsubst %,$(ODIR)/%,$(_CONVERTER))
CALCULATOR = $(patsubst %,$(ODIR)/%,$(_CALCULATOR))
PCALCULATOR = $(patsubst %,$(ODIR)/%,$(_PCALCULATOR))

# Rules
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

all: PRcalculator PRcalculatorSerial WFGcreator

PRcalculator: $(OBJ) $(PCALCULATOR)
	$(CC) -o $(BDIR)/PRcalculator $^ $(CFLAGS) $(LIBS)

PRcalculatorSerial: $(OBJ) $(CALCULATOR)
	$(CC) -o $(BDIR)/PRserialcalculator $^ $(CFLAGS) $(LIBS)

WFGcreator: $(OBJ) $(CONVERTER)
	$(CC) -o $(BDIR)/WGFcreator $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ $(SDIR)/*~
