linkTarget = waRay
LIBS = -lSDL2
CFLAGS = -std=c++17 -Ofast
objects =	main.o \
					CApp.o \
					$(patsubst %.cpp,%.o,$(wildcard ./waRayTrace/*.cpp)) \
					$(patsubst %.cpp,%.o,$(wildcard ./waRayTrace/primitives/*.cpp)) \
					$(patsubst %.cpp,%.o,$(wildcard ./waRayTrace/lights/*.cpp)) \
					$(patsubst %.cpp,%.o,$(wildcard ./waRayTrace/materials/*.cpp))
rebuildables = $(objects) $(linkTarget)
$(linkTarget): $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)
.PHONEY:
clean:
	rm $(rebuildables)