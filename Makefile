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
	
ifeq ($(OS), Win)
	@echo "Faire make windirs pour les dossiers la PREMIÈRE fois"
endif
.PHONY : dirs

windirs :
	mkdir bin
	mkdir obj\Core
	mkdir obj\Audio
	mkdir obj\Physics
	mkdir obj\Game
	mkdir obj\Score
	mkdir obj\Rendering
	mkdir obj\Level
	mkdir obj\Menu
	mkdir obj\Objects
.PHONY : windirs

$(OBJ)%.o : $(SRC)%.cpp
	@echo "Compilation du fichier $^"
	@$(CXX) $(OPT) $^ -c -o $@
	
#$(OBJ)%.o : $(SRC)%.cpp
#$(C) $(OPT) $^ -c -o $@

test : $(POINTO) $(OBJ)Test.o
	@echo "Édition des liens pour $@"
	@$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	
Editor : $(POINTO) $(OBJ)Editor.o
	@echo "Édition des liens pour $@"
	@$(CXX) $(OPT) $^ -o $(BIN)"JumpnRun Level Editor" $(LIBS)
	
runEditor : dirs Editor
ifeq ($(OS), Win)
	$(BIN)JumpnRun\ Level\ Editor.exe
else
	./$(BIN)JumpnRun\ Level\ Editor
endif
	
runGame : dirs JumpnRun
ifeq ($(OS), Win)
	$(BIN)JumpnRun.exe
else
	./$(BIN)JumpnRun
endif
	
JumpnRun : $(POINTO) $(OBJ)Main.o
	@echo "Édition des liens pour $@"
	@$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	
$(OBJ)Test.o : $(SRC)Test.cpp
	@echo "Compilation du fichier $^"
	@$(CXX) $(OPT) $^ -c -o $@
	
debug : debug_option all

debug_option :
#OPT = -g

run : dirs all
	@echo "Éxécution de $(BIN)test" 
ifeq ($(OS), Win)
	$(BIN)test.exe
else
	./$(BIN)test
endif
.PHONY : run

valgrind : all
	valgrind --leak-check=full --tool=memcheck ./$(BIN)test
.PHONY : valgrind

help:
	@echo "runEditor\t\tLance l'éditeur (compile si nécessaire)\nrunGame\t\tLance le jeu (compile si nécessaire)\nclean\t\tÉlimine les fichier .o et exécutable\ncleanTest\t\tÉlimine les exécutables des tests de regression\nTest(nom d'un module)\t\tCompile et lance sous valgrind un test de regression"
.PHONY : help


clean:
	@$(RM) -vf $(OBJ)*.o $(OBJ)*/*.o $(BIN)*
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
	
TestCore : $(OBJ)Core/Vec2.o $(OBJ)Core/DynArr.o $(OBJ)Core/md5.o $(OBJ)Core/List.o $(OBJ)Core/Tools.o $(OBJ)Core/Node.o $(OBJ)TestCore.o
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestCore.o :
	$(CXX) $(OPT) $(TESTS)TestCore.cpp -c -o $@
	
TestPhysics : $(POINTO) $(OBJ)TestPhysics.o
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestPhysics.o :
	$(CXX) $(OPT) $(TESTS)TestPhysics.cpp -c -o $@

	
TestMenu :$(OBJ)TestMenu.o $(POINTO)
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestMenu.o :
	$(CXX) $(OPT) $(TESTS)TestMenu.cpp -c -o $@
	

TestLevel : $(POINTO) $(OBJ)TestLevel.o
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	#./$(BIN)$@
	
$(OBJ)TestLevel.o :
	$(CXX) $(OPT) $(TESTS)TestLevel.cpp -c -o $@
	
	
TestSoundManager :$(OBJ)TestSoundManager.o $(POINTO)
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestSoundManager.o :
	$(CXX) $(OPT) $(TESTS)TestSoundManager.cpp -c -o $@
	
TestScore : $(POINTO) $(OBJ)TestScore.o
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestScore.o :
	$(CXX) $(OPT) $(TESTS)TestScore.cpp -c -o $@
	
TestSharedResources : $(POINTO) $(OBJ)TestSharedResources.o
	@$(RM) -f $(BIN)$@
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)$@
	
$(OBJ)TestSharedResources.o :
	$(CXX) $(OPT) $(TESTS)TestSharedResources.cpp -c -o $@

 