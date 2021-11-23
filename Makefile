SRCS = $(wildcard src/*.cpp)
INCLUDES = $(wildcard src/include/*.hpp) $(wildcard src/include/**/*.hpp)
CC = clang++
CFLAGS = -Wall -Werror -Wfatal-errors

ESC = 
GREEN = $(ESC)[32m
RED = $(ESC)[31m
NC = $(ESC)[0m
NEG = $(ESC)[7m

ifeq ($(OS),Windows_NT)
TARGET = main.exe
RELEASE_TARGET = raytracer.exe
DEL = del
else
TARGET = main
RELEASE_TARGET = raytracer
DEL = rm -f
endif

$(TARGET): $(SRCS) $(INCLUDES)
	@echo $(GREEN)   Building $(NC)$(TARGET)$(GREEN) [debug]$(NC)
	@$(CC) -o $(TARGET) $(SRCS) -Isrc/include -O0 $(CFLAGS)

run: $(TARGET)
	@echo $(GREEN)    Running $(NC)$(TARGET)$(GREEN) [debug]$(NC)
	@./$(TARGET)

clean:
ifneq ("$(wildcard $(TARGET))","")
	@echo $(RED)Cleaning up $(NC)$(TARGET)$(RED)...$(NC)
	@$(DEL) $(TARGET)
endif

release: $(SRCS) $(INCLUDES)
	@echo $(GREEN)   Building $(NC)$(TARGET)$(GREEN) [release]$(NC)
	@$(CC) -o $(TARGET) $(SRCS) -Isrc/include -Ofast $(CFLAGS)

benchmark: release
	@echo $(GREEN)    Running $(NC)$(TARGET)$(GREEN) [release]$(NC)
	@./$(TARGET)

build: | $(TARGET)

all: run
