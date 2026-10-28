COMPILER = g++
CXXFLAGS = -std=c++11 -g
SOURCE_LIBS = -Ilib/ -Iinclude
CFILES = src/*.cpp src/screens/*.cpp src/structs/*.cpp

# macOS
OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a -fsanitize=address
OUTPUT = bin/build_osx

build_osx:
	$(COMPILER) $(CXXFLAGS) -fsanitize=address $(CFILES) $(SOURCE_LIBS) -o $(OUTPUT) $(OSX_OPT)

run: build_osx
	./$(OUTPUT) $(PEDIDOS) $(HIST)

# Linux
LINUX_FLAGS = -fsanitize=address -fno-omit-frame-pointer
LINUX_LIBS = -lraylib -lGL -lm -lX11 -lpthread -ldl -lrt
LINUX_OUTPUT = bin/build_linux

build_linux:
	$(COMPILER) $(CXXFLAGS) $(LINUX_FLAGS) $(CFILES) $(SOURCE_LIBS) -o $(LINUX_OUTPUT) $(LINUX_LIBS)

run_linux: build_linux
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=0 ./$(LINUX_OUTPUT) $(PEDIDOS) $(HIST)
