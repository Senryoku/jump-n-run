C = gcc
OBJ = obj/
SRC = src/
BIN = bin/
POINTC = $(wildcard $(SRC)*/*.c) $(wildcard $(SRC)*.c) 
POINTCPP = $(wildcard $(SRC)*/*.cpp) $(wildcard $(SRC)*.cpp) 
POINTOP := $(POINTC:.c=.o) $(POINTCPP:.cpp=.o)
POINTO = $(patsubst src/%,$(OBJ)%,$(POINTOP)) #$(POINTOP:src=obj)

OPT := -Wall -pedantic -I$(SRC) 


ifeq ($(SHELL), sh.exe) 
OS := Win
else
OS := $(shell uname)
endif

ifeq ($(OS), Linux)
RM = rm
LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
endif
ifeq ($(OS), Darwin)
RM = rm
LIBS := -framework sfml-system -framework sfml-window -framework
endif
ifeq ($(OS), Win)
RM = del
LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
endif

default :
	@echo "C Files:" $(POINTC) ; \
	echo "C++ Files:" $(POINTCPP) ; \
	echo "O Files:" $(POINTO) ; \
	echo $(OPT)

.PHONY : default


all : test dirs

dirs : 
	@if [ -e  $(BIN) ] ; \
	then mkdir $(BIN) ; \
	fi ; \
	if [ -e  $(OBJ)/Core ] ; \
	then mkdir $(OBJ)/Core ; \
	fi ; \
	if [ -e  $(OBJ)/Audio ] ; \
	then mkdir $(OBJ)/Audio ; \
	fi ; \
	if [ -e  $(OBJ)/Physics ] ; \
	then mkdir $(OBJ)/Physics ; \
	fi ; \
.PHONY : dirs

$(OBJ)%.o : $(SRC)%.c
	$(C) $(OPT) $^ -c -o $@
	
$(OBJ)%.o : $(SRC)%.cpp
	$(C) $(OPT) $^ -c -o $@

test : $(POINTO)
	$(C) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	
debug : debug_option all

debug_option :
#OPT = -g

run :
	./$(EXEC)
.PHONY : run

clean:
	$(RM) -v $(OBJ)*.o $(BIN)*
.PHONY : clean

doc:
	cd doc/ ; \
	doxygen Doxyfile ; \
	cd ..
.PHONY : doc
 