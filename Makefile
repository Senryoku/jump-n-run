C = gcc
OPT = -Wall -pedantic
OBJ = obj/
SRC = src/
BIN = bin/
POINTC = $(wildcard $(SRC)*.c)
POINTO = $(POINTC: .c=.o)
ifeq ($(SHELL), sh.exe) 
OS := Win
else
OS := $(shell uname)
endif

ifeq ($(OS), Linux)
RM = rm
endif
ifeq ($(OS), Darwin)
RM = rm
endif
ifeq ($(OS), Win)
RM = del
endif

all : test

$(OBJ)%.o : $(SRC)%.c
	$(C) $(OPT) $^ -c -o $@

test : $(POINTO)
	$(C) $(OPT) $^ -o $(BIN)$@
	
debug : debug_option all

debug_option :
OPT = -Wall -pedantic -g

run :
	./$(EXEC)
.PHONY : run

clean:
	$(RM) $(OBJ)*.o $(BIN)*
.PHONY : clean

doc:
	cd doc/ ; \
	doxygen Doxyfile ; \
	cd ..
.PHONY : doc
 