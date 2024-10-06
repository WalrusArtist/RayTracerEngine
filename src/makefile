linkTarget = waRay
LIBS = -lSDL2
CFLAGS = -std=c++17
objects = main.o \
		  CApp.o \
		  $(patsubst %.cpp,%.o,$(wildcard ./waRayTrace/*.cpp))

rebuilables = $(objects) $(linkTarget)

# Rule
$(linkTarget): $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)

# Rule 2
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

.PHONEY:
clean:
	rm $(rebuilables)