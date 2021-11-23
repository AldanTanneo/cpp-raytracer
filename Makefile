SRCS = $(wildcard src/*.cpp)
INCLUDES = $(wildcard src/include/*.hpp)
CC = clang++
CFLAGS = -Wall -Werror -Wfatal-errors
ifeq ($(OS),Windows_NT)
TARGET = main.exe
RELEASE_TARGET = raytracer.exe
else
TARGET = main
RELEASE_TARGET = raytracer
endif

$(TARGET): $(SRCS) $(INCLUDES)
	@echo Building $(TARGET) [debug]
	@$(CC) -o $(TARGET) $(SRCS) -Isrc/include -O0 $(CFLAGS)
	@echo Done!

release: $(SRCS) $(INCLUDES)
	@echo Building $(TARGET) [release]
	@$(CC) -o $(TARGET) $(SRCS) -Isrc/include -O3 $(CFLAGS)
	@echo Done!

clean:
	@echo == Cleaning up $(TARGET)... ==
	@$(RM) $(TARGET)

run: $(TARGET)
	@echo == Running $(TARGET)... ==
	@./$(TARGET)

build: | $(TARGET)
