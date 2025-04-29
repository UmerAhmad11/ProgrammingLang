# --- Configuration ---
exec = hello.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

# --- Set Default Goal ---
.DEFAULT_GOAL := install # Tell make to run 'install' by default

# --- Build Rules ---

# How to build the final program (hello.out)
$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

# How to build an object file (.o) from a source file (.c)
%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

# --- Other Commands ---

# How to install the program:
# Depends on the executable being built first.
install: $(exec)
# Copy the program to a standard location.
# NOTE: This still uses sudo and will likely ask for a password.
	sudo cp ./$(exec) /usr/local/bin/hello


# How to clean up generated files:
clean:
	-rm -f $(exec) *.o src/*.o

# Explicitly define a target just for building, if needed separately
build: $(exec)
