
CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity

SOURCES := $(shell find src -type f -name "*.cpp")
OBJECTS := $(patsubst src/%, build/%, $(SOURCES:.cpp=.o))

CFLAGS := -g -Wall -std=c++11

LIB := -L /usr/local/lib -lglfw3 -lglew -lglboy -lglfw -framework OpenGL -framework IOKit -framework Cocoa
INC := -I include -I /usr/local/include

all: sample01 wave_plane01 ellipse testing

sample01: build/sample01.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/sample01 $(LIB)"; $(CC) $^ -o bin/sample01 $(LIB)
#	@cp -f src/glboy/shaders/*.vert bin/sample01
#	@cp -f src/glboy/shaders/*.frag bin/sample01
#	@cp -f src/sample01/resource/* bin/sample01

wave_plane01: build/wave_plane01.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/wave_plane01 $(LIB)"; $(CC) $^ -o bin/wave_plane01 $(LIB)

ellipse: build/ellipse.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/ellipse $(LIB)"; $(CC) $^ -o bin/ellipse $(LIB)

quadratic_bezier: build/quadratic_bezier.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/quadratic_bezier $(LIB)"; $(CC) $^ -o bin/quadratic_bezier $(LIB)

bezier_processing_method: build/bezier_processing_method.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/bezier_processing_method $(LIB)"; $(CC) $^ -o bin/bezier_processing_method $(LIB)

testing: build/testing.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/testing $(LIB)"; $(CC) $^ -o bin/testing $(LIB)

dynamic_vertex: build/dynamic_vertex.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/dynamic_vertex $(LIB)"; $(CC) $^ -o bin/dynamic_vertex $(LIB)

line: build/line.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/line $(LIB)"; $(CC) $^ -o bin/line $(LIB)

color_cut: build/color_cut.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/color_cut $(LIB)"; $(CC) $^ -o bin/color_cut $(LIB)

glow: build/glow.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/glow $(LIB)"; $(CC) $^ -o bin/glow $(LIB)

bezier: build/bezier.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/bezier $(LIB)"; $(CC) $^ -o bin/bezier $(LIB)

bezier_triangle: build/bezier_triangle.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/bezier_triangle $(LIB)"; $(CC) $^ -o bin/bezier_triangle $(LIB)

rotate: build/rotate.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/rotate $(LIB)"; $(CC) $^ -o bin/rotate $(LIB)

cull_face: build/cull_face.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/cull_face $(LIB)"; $(CC) $^ -o bin/cull_face $(LIB)

%: build/%.o
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/$@ $(LIB)"; $(CC) $^ -o bin/$@ $(LIB)


# bin/sample01: sample01.cpp fwplayer.cpp
#	@echo " Linking..."
#	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

build/%.o: src/%.cpp
	@mkdir -p build
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< 

clean:
	@echo " Cleaning...";
	$(RM) -r build bin/*

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean