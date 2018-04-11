#
# PROGRAM:    assign4
# PROGRAMMER: Saad Rasheed
# LOGON ID:   z1783832
# DATE DUE:   10/13/2017
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
assign4: assign4.o 
	g++ $(CCFLAGS) -o assign4 assign4.o 

# Rule to compile source code file to object code
assign4.o: assign4.cpp assign4.h process.h
	g++ $(CCFLAGS) -c assign4.cpp


# Pseudo-target to remove object code and executable files
clean:
	-rm *.o assign4



