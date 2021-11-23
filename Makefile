PROJECT_NAME := xtrem-raytracer

SRC      := src
OBJ      := obj
SRCS     := $(wildcard $(SRC)/*.cpp)
OBJS     := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
# Headers
INCLUDES := $(wildcard $(SRC)/include/*.hpp) $(wildcard $(SRC)/include/**/*.hpp)
CC       := clang++ # Compiler
CFLAGS   := -Wall -Werror -Wfatal-errors # Compiler flags
OPT      := -O0     # Default optimisation setting
MODE     := [debug] # Default mode

# Cool colours
ESC   := 
NC    := $(ESC)[0m
BOLD  := $(ESC)[1m
NEG   := $(ESC)[7m
RED   := $(ESC)[31m
GREEN := $(ESC)[32m

# Platform specific variables
ifeq ($(OS),Windows_NT)
TARGET         := main.exe
RELEASE_TARGET := $(PROJECT_NAME).exe
DEL            := del
RMDIR          := rd /S /Q
else
TARGET         := main
RELEASE_TARGET := $(PROJECT_NAME)
DEL            := rm -f
RMDIR          := rm -rf
endif

all: $(TARGET)

$(OBJ):
ifeq ("$(wildcard $(OBJ))","")
	@mkdir $@
endif

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDES) | $(OBJ)
	@echo $(BOLD)$(GREEN)  Compiling $(NC)$(notdir $@)$(GREEN) $(MODE)$(NC)
	@$(CC) -c $< -o $@ -Isrc/include $(OPT) $(CFLAGS) 

$(TARGET): $(OBJ) $(OBJS)
	@echo $(BOLD)$(GREEN)    Linking $(NC)$(TARGET)$(GREEN) $(MODE)$(NC)
	@$(CC) -o $(TARGET) $(OBJS) $(OPT) $(CFLAGS)

.PHONY: all release run benchmark clean build

release: OPT    := -Ofast
release: MODE   := [release]
release: TARGET := $(RELEASE_TARGET)
release: $(OBJ) $(OBJS) $(TARGET)

debug: $(OBJ) $(OBJS) $(TARGET)

run: $(TARGET)
	@echo $(BOLD)$(GREEN)    Running $(NC)$(TARGET)$(GREEN) [debug]$(NC)
	@./$(TARGET)

benchmark: release # run in release mode
	@echo $(BOLD)$(GREEN)    Running $(NC)$(TARGET)$(GREEN) [release]$(NC)
	@./$(TARGET)

clean:
ifneq ("$(wildcard $(TARGET))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(TARGET)$(RED)...$(NC)
	@$(DEL) $(TARGET)
endif
ifneq ("$(wildcard $(RELEASE_TARGET))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(RELEASE_TARGET)$(RED)...$(NC)
	@$(DEL) $(RELEASE_TARGET)
endif
ifneq ("$(wildcard $(OBJ))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(OBJ)$(RED)...$(NC)
	@$(RMDIR) $(OBJ)
endif

