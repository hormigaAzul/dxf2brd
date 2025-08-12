COMPILER = g++
FLAGS = -O3 -ldxflib
EXECUTABLE = dxf2brd

$(EXECUTABLE): dxf2brd.cpp
	$(COMPILER) dxf2brd.cpp $(FLAGS) -o $(EXECUTABLE)
	
.PHONY : clean
clean :
	-rm -f $(EXECUTABLE) 
