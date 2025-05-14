# Compiler and flags
CPP = g++
CXXFLAGS = -O5 -Wall -W -pg
LDFLAGS = -lncurses
RM = rm -f

# Executable name
EXEC2 = samvaad

# Source files and object files
SRCS =   main.cpp functions.cpp 
OBJS = $(SRCS:.cpp=.o)

# Header dependencies
DEPS = functions.h global.h user.h

# Default target to build the executable
all: $(EXEC2)

# Link the object files to create the executable
$(EXEC2): $(OBJS)
	$(CPP) $(OBJS) -o $(EXEC2) $(LDFLAGS)

# Compile source files into object files with header dependencies
%.o: %.cpp $(DEPS)
	$(CPP) $(CXXFLAGS) -c $< -o $@

# Clean up the build
clean:
	$(RM) $(OBJS) $(EXEC2)

# Message after successful compilation
touch:
	@echo " "
	@echo "Compilation done successfully..."
	@echo " "

# To remove all object files and the executable
fullclean: clean
	$(RM) $(EXEC2)

# Rebuild everything from scratch
rebuild: fullclean all

.PHONY: all clean touch fullclean rebuild

