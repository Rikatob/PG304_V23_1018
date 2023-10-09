# Makefile template

# //////////Rule//////////////
# // target : prerequisites //
# //	 recipe....		    	 //
# ////////////////////////////


TARGET = source

# Used in clean
TARGETS =

# So preprosessor check this dir when #include "something.h" is used.
INCLDIR = ./include

# Used when making pathString to .a files.
LIBDIR = ./lib


# Use gcc compiler
CC = gcc

# What optimalisation to use when compiling
CFLAGS = -O0

# Used to tell the compiler where include directories are
CFLAGS += -I$(INCLDIR)

CFLAGS += 

# remove command 
RM = rm -rf ./$(OBJDIR)/*.o *~ core

# Store where object files are.
OBJDIR = obj

# List of libraries is needed in this project.
# -> Make a path string to those .a files.
_LIBS = pgdbglog.a 
LIBS = $(patsubst %,$(LIBDIR)/%,$(_LIBS))

# List of Header files needed in this project.
# -> Make a path string to those .h files.
_DEPS = $(TARGET).h pgdbglog.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

# List of Object files made from .c files in the "MAKE_OBJECT_FILES" rule.
# -> Make a path string to those .o files.
_OBJS = $(TARGET).o

OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

# MAKE_OBJECT_FILES rule.
# Compile .o files from all .c files and put them in "OBJDIR".
$(OBJDIR)/%.o: %.c $(DEPS) 	
	$(CC) -c -o $@ $< $(CFLAGS)

# COMPILE_TO_RUNABLE_FILE rule.
# Compile the runnable file with linking all object files(.o) and libraries (.a).
# -> use CFLAGS to tell what optimalization to use and what directories to use as include directory. 
$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

.PHONY: clean
clean:
	$(RM) $(TARGETS) $(TARGET)
	make clearDebugFiles

clearDebugFiles:
	rm -rf ./debugLogs/debug*.txt
#Remember including the uncompiled pgDbgLogger in exam!!!!!
setup:
	mkdir obj include lib debugLogs
	touch $(TARGET).c include/$(TARGET).h
	cp /home/rikatob/Documents/labexc/pgDbgLoggerSoft/pgdbglog.a ./lib/pgdbglog.a
	cp /home/rikatob/Documents/labexc/pgDbgLoggerSoft/pgdbglog.h ./include/pgdbglog.h
