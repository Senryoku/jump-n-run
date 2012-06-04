CXX = g++
OBJ = obj/
SRC = src/
BIN = bin/
TESTS = tests/
POINTC = $(wildcard $(SRC)*/*.c) $(wildcard $(SRC)*.c) 
POINTCPP = $(wildcard $(SRC)*/*.cpp) #$(wildcard $(SRC)*.cpp) 
POINTOP := $(POINTC:.c=.o) $(POINTCPP:.cpp=.o)
POINTO = $(patsubst src/%,$(OBJ)%,$(POINTOP)) #$(POINTOP:src=obj)

OPT := -Wall -pedantic -Wno-long-long -O2 -g -I "$(SRC)"


ifeq ($(SHELL), sh.exe) 
OS := Win
else
OS := $(shell uname)
endif

ifeq ($(OS), Linux)
RM = rm
LIBS := -lsfml-network -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -lGL
endif
ifeq ($(OS), Darwin)
RM = rm
LIBS := -framework sfml-system -framework sfml-window -framework sfml-graphics -framework sfml-audio -framework sfml-network -framework OpenGL
endif
ifeq ($(OS), Win)
RM = del
LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lopengl32
endif

all : dirs test 
	@echo "\n\n\t== Faire 'make run' pour lancer l'application ==\n\n"


dirs : 
ifeq ($(OS), Darwin)
	@./configure.sh bin obj src
endif
ifeq ($(OS), Linux)
	@./configure.sh bin obj src
endif
	
#A refaire mais là dessuite j'ai la flemme :D
ifeq ($(OS), Win)
	@echo "Faire make windirs pour les dossiers la PREMIÈRE fois"
endif
.PHONY : dirs

windirs :
	mkdir bin\
	mkdir obj\Core
	mkdir obj\Audio
	mkdir obj\Physics
	mkdir obj\Game
	mkdir obj\Score
	mkdir obj\Rendering
	mkdir obj\Level
.PHONY : windirs

$(OBJ)%.o : $(SRC)%.cpp
	@echo "Compilation du fichier $^" ; \
	$(CXX) $(OPT) $^ -c -o $@
	
#$(OBJ)%.o : $(SRC)%.cpp
#$(C) $(OPT) $^ -c -o $@

test : $(POINTO) $(OBJ)Test.o
	@echo "Édition des liens pour $@" ; \
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	
$(OBJ)Test.o : $(SRC)Test.cpp
	@echo "Compilation du fichier $^" ; \
	$(CXX) $(OPT) $^ -c -o $@
	
debug : debug_option all

debug_option :
#OPT = -g

run : dirs all
	@echo "Éxécution de $(BIN)test" ; \
	./$(BIN)test
.PHONY : run

valgrind : all
	valgrind --leak-check=full --tool=memcheck ./$(BIN)test
:PHONY valgrind

clean:
	@$(RM) -vf $(OBJ)*.o $(OBJ)*/*.o $(BIN)test $(BIN)testvec2
.PHONY : clean

cleanTest:
	@$(RM) -vf $(OBJ)*.o $(BIN)Test*
.PHONY : clean

doc:
	doxygen doc/Doxyfile ; \
	chmod u+x doc/html/index.html ; \
	open doc/html/index.html
.PHONY : doc

info:
	@echo "Lignes\tMots\tOctets"
	@cd src ; \
	find -E . -regex "(.+)\.(h|cpp|c|hpp)" -print0 | xargs -0 cat | wc ; \
	cd ..
.PHONY : info
#find -E src/ -regex "(.+)\.(h|cpp|c|hpp)" -exec cat {} \; | wc -l | (read A ; echo "Lignes dans le projet: $A")
#find -E src/ -regex "(.+)\.(h|cpp|c|hpp)" -exec cat {} \; | wc | (read L W B ; echo "Le projet a $L lignes, $W mots et pèse $B octets")


#Tests de regression

MODULES = "Core/Vec2 Physics/Vertex Physics/Polygon"
#À compléter encore avec une règle génerale selon cette liste de modules

testVec2 : $(OBJ)Core/Vec2.o $(OBJ)testVec2.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind --leak-check=full --tool=memcheck ./$(BIN)testVec2
	
$(OBJ)testVec2.o :
	$(CXX) $(OPT) $(TESTS)TestVec2.cpp -c -o $@
	
TestCore : $(OBJ)Core/Vec2.o $(OBJ)Core/DynArr.o $(OBJ)Core/md5.o $(OBJ)Core/List.o $(OBJ)Core/Tools.o $(OBJ)Core/Node.o $(OBJ)TestCore.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestCore.o :
	$(CXX) $(OPT) $(TESTS)TestCore.cpp -c -o $@
	
TestPhysics : $(POINTO) $(OBJ)TestPhysics.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestPhysics.o :
	$(CXX) $(OPT) $(TESTS)TestPhysics.cpp -c -o $@
	
testGrid : $(OBJ)Physics/Grid.o $(OBJ)testGrid.o $(OBJ)Core/List.o $(OBJ)Core/Node.o $(OBJ)Core/DynArr.o $(OBJ)Physics/Polygon.o $(OBJ)Physics/Rigid.o $(OBJ)Physics/Vertex.o $(OBJ)Core/Vec2.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	#valgrind --leak-check=full --tool=memcheck ./$(BIN)testGrid
	./$(BIN)testGrid
	
$(OBJ)testGrid.o :
	$(CXX) $(OPT) $(TESTS)TestGrid.cpp -c -o $@
	
testMenuItem : $(OBJ)Menu/MenuItem.o $(OBJ)Core/Tools.o $(OBJ)Core/Vec2.o $(OBJ)Core/DynArr.o $(OBJ)testMenuItem.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind --leak-check=full --tool=memcheck ./$(BIN)testMenuItem
	
$(OBJ)testMenuItem.o :
	$(CXX) $(OPT) $(TESTS)TestMenuItem.cpp -c -o $@


TestLevel : $(POINTO) $(OBJ)TestLevel.o
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	#./$(BIN)$@
	
$(OBJ)TestLevel.o :
	$(CXX) $(OPT) $(TESTS)TestLevel.cpp -c -o $@

 