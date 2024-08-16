# Makefile

# Detect the platform
UNAME := $(shell uname)

# Source files and directories
SRCS_MAIN = main/main.c
SRCS_SOURCE = source/init.c source/game.c source/render.c source/cleanup.c
SRCS = $(SRCS_MAIN) $(SRCS_SOURCE)

# Object files
OBJS = $(SRCS:.c=.o)

# Include path for header files
INCLUDES = -Iheaders

# Compiler flags based on platform
ifeq ($(UNAME), Linux)
	CFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 $(INCLUDES)
	TARGET = RectangleGame
else ifeq ($(UNAME), Darwin)  # macOS
	CFLAGS = -lraylib -lm -lpthread $(INCLUDES)
	TARGET = RectangleGame
else  # Windows (using MinGW)
	CFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm $(INCLUDES)
	TARGET = RectangleGame.exe
endif

all: $(TARGET)

# Compile the target
$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) $(CFLAGS)

# Compile object files from main folder
main/%.o: main/%.c
	gcc -c $< -o $@ $(CFLAGS)

# Compile object files from source folder
source/%.o: source/%.c
	gcc -c $< -o $@ $(CFLAGS)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
