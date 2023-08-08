CC= gcc #compiler
TARGET = MeshMaking #Final EXE file name
INC_DIR = ./include #header directory
SRC_DIR = src #source code directory

#Compiler flags for compiling *.c to *.o
CFLAGS = -gdwarf-3 -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -I$(INC_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt 

#Gathers an array of all *.c files within the source directory
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
$(info $$SOURCES is [${SOURCES}]) #Prints out which files it found
$(info $$HEADERS is [${HEADERS}]) #Prints out which files it found

OBJECTS = $(SOURCES:.c=.o) #computes and converts all possible *.c to *.o files

.PHONY: all clean #idk

all: $(TARGET) #Main make intro

#Compute final executable
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ 

#remove all object files and the binary
clean:
	rm $(TARGET) $(OBJECTS)
