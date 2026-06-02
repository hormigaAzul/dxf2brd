COMPILER = g++
FLAGS = -Wall -O3 -ldxflib
EXECUTABLE = dxf2brd
SRC = dxf2brd.cpp polyline.cpp

$(EXECUTABLE): $(SRC)
	$(COMPILER) $(SRC) $(FLAGS) -o $(EXECUTABLE)
	
.PHONY : clean
clean :
	-rm -f $(EXECUTABLE) 
