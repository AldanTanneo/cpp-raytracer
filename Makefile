PROJECT_NAME := xtrem-raytracer

# Platform specific variables
ifeq ($(OS),Windows_NT)
TARGET_DEBUG   := main.exe
TARGET_RELEASE := $(PROJECT_NAME).exe
DEL            := del
RMDIR          := rd /S /Q
MKDIR          := md
else
TARGET_DEBUG   := main
TARGET_RELEASE := $(PROJECT_NAME)
DEL            := rm -f
RMDIR          := rm -rf
MKDIR          := mkdir -p
endif

# Sources
SRC      := src
SRCS     := $(wildcard $(SRC)/*.cpp)
INCLUDES := $(wildcard $(SRC)/include/*.hpp) $(wildcard $(SRC)/include/**/*.hpp)

# Object files
OBJ          := obj
OBJ_DEBUG    := $(OBJ)/debug
OBJ_RELEASE  := $(OBJ)/release
OBJS_DEBUG   := $(patsubst $(SRC)/%.cpp,$(OBJ_DEBUG)/%.o,$(SRCS))
OBJS_RELEASE := $(patsubst $(SRC)/%.cpp,$(OBJ_RELEASE)/%.o,$(SRCS))

# Compiler and compile flags
CC          := clang++
CFLAGS      := -I$(SRC)/include -Wall -Werror -Wfatal-errors
OPT_DEBUG   := -O0
OPT_RELEASE := -Ofast -mavx2

MODE_DEBUG   := [debug]
MODE_RELEASE := [release]

# Cool colours
ESC   := 
NC    := $(ESC)[0m
BOLD  := $(ESC)[1m
NEG   := $(ESC)[7m
RED   := $(ESC)[31m
GREEN := $(ESC)[32m

build: $(TARGET_DEBUG)

# Create build directories

$(OBJ_DEBUG) $(OBJ_RELEASE):
ifeq ($(OS),Windows_NT)
	@if not exist $(subst /,\,$@) $(MKDIR) $(subst /,\,$@)
else
	@$(MKDIR) $@
endif

# Debug build

$(OBJ_DEBUG)/%.o: $(SRC)/%.cpp $(INCLUDES) | $(OBJ_DEBUG)
	@echo $(BOLD)$(GREEN)  Compiling $(NC)$(notdir $@)$(GREEN) $(MODE_DEBUG)$(NC)
	@$(CC) -c $< -o $@ $(OPT_DEBUG) $(CFLAGS) 

$(TARGET_DEBUG): $(OBJS_DEBUG) | $(OBJ_DEBUG)
	@echo $(BOLD)$(GREEN)    Linking $(NC)$@$(GREEN) $(MODE_DEBUG)$(NC)
	@$(CC) -o $@ $(OBJS_DEBUG) $(OPT_DEBUG) $(CFLAGS)

# Release build

$(OBJ_RELEASE)/%.o: $(SRC)/%.cpp $(INCLUDES) | $(OBJ_RELEASE)
	@echo $(BOLD)$(GREEN)  Compiling $(NC)$(notdir $@)$(GREEN) $(MODE_RELEASE)$(NC)
	@$(CC) -c $< -o $@ $(OPT_RELEASE) $(CFLAGS)

$(TARGET_RELEASE): $(OBJS_RELEASE) | $(OBJ_RELEASE)
	@echo $(BOLD)$(GREEN)    Linking $(NC)$@$(GREEN) $(MODE_RELEASE)$(NC)
	@$(CC) $(OBJS_RELEASE) -o $@ $(OPT_RELEASE) $(CFLAGS)

# Phony targets

debug: $(TARGET_DEBUG)

run: $(TARGET_DEBUG)
	@echo $(BOLD)$(GREEN)    Running $(NC)$(TARGET_DEBUG)$(GREEN) $(MODE_DEBUG)$(NC)
	@./$(TARGET_DEBUG)

release: $(TARGET_RELEASE)

bench benchmark: $(TARGET_RELEASE)
	@echo $(BOLD)$(GREEN)    Running $(NC)$(TARGET_RELEASE)$(GREEN) $(MODE_RELEASE)$(NC)
	@./$(TARGET_RELEASE)

clean:
ifneq ("$(wildcard $(TARGET_DEBUG))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(TARGET_DEBUG)$(RED)...$(NC)
	@$(DEL) $(TARGET_DEBUG)
endif
ifneq ("$(wildcard $(TARGET_RELEASE))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(TARGET_RELEASE)$(RED)...$(NC)
	@$(DEL) $(TARGET_RELEASE)
endif
ifneq ("$(wildcard $(OBJ))","")
	@echo $(BOLD)$(RED)Cleaning up $(NC)$(OBJ)$(RED)...$(NC)
	@$(RMDIR) $(OBJ)
endif

.PHONY: build debug run release bench benchmark clean
