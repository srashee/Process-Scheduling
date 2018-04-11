#
# PROGRAM:    scheduler
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
scheduler: scheduler.o 
	g++ $(CCFLAGS) -o scheduler scheduler.o 

# Rule to compile source code file to object code
scheduler.o: scheduler.cpp assign4.h process.h
	g++ $(CCFLAGS) -c scheduler.cpp


# Pseudo-target to remove object code and executable files
clean:
	-rm *.o scheduler



