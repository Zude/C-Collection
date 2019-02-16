# ---------------------------------------------	#
# Fachhochschule Wedel                          #
# SP-Uebung 05                                  #
# kar, mhe                                      #
#                                               #
# Makefile                                      #
# ---------------------------------------------	#

# epsilon for regula falsi
POLYNOM_EPSILON = 1.0e-10

# Flag that states that warnings will not lead to compilation errors
FORCE = false

# Compiler
CC		= gcc
# Doc generator
DOC		= doxygen

# Include directories
INCLUDES	 =
# Compiler flags
CFLAGS_FORCE = -c -g -mfpmath=sse -msse2 -ansi -pedantic -Wall -Wextra -DPOLYNOM_EPSILON=$(POLYNOM_EPSILON)
CFLAGS		 = $(CFLAGS_FORCE) -Werror
# Linker flags
LDFLAGS		 = -lm

# Source codes 
SOURCE		= polynom.c errors.c state.c output.c printer.c ueb05.c
OBJECTS		= $(SOURCE:.c=.o)

# Target name
BINARY		= ueb05

.PHONY: all help clean doc

default: all

force: 
	@make FORCE=true

all: $(BINARY)

# Compile printer
printer.o : printer.c
	@echo "  - Building $@"
	$(CC) $(CFLAGS) -Wno-unused-parameter $(INCLUDES) -o $@ $<
	@echo "  ... done"

# Compile other sources
%.o : %.c
	@echo "  - Building $@"
ifeq ($(FORCE),true)
	$(CC) $(CFLAGS_FORCE) $(INCLUDES) -o $@ $<
else
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
endif
	@echo "  ... done"

# Link files to an executable
$(BINARY): $(OBJECTS)
	@echo "  - linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)
	@echo "  ... done"

# Clean the project
clean:
	@echo "  - delete object all files"
	rm -f $(OBJECTS) $(BINARY) *~ doxygen.log
	rm -rf doc/
	@echo "  ... done"

doc:
	@echo "  - creating documentation"
	$(DOC)
	@echo "  ... done"

# Show help
help:
	@echo "Options:"
	@echo "make all      - create program"
	@echo "make clean    - clean up"
	@echo "make doc      - create documentation"
	@echo "make force    - compile without -Werror (just for 'comfier' development)"
