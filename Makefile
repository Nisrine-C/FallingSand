appname  := falling_sand
bindir   := bin
objdir   := obj
target   := $(bindir)/$(appname).exe

CXX      := g++
# Look for headers in your local include folder
CXXFLAGS := -std=c++11 -Wall -Iinclude -O3

# Link libraries found in your local lib folder
# Raylib on Windows needs gdi32, winmm, and opengl32
LDFLAGS  := -Llib -lraylib -lgdi32 -lwinmm -lopengl32

SRCS     := main.c++
OBJS     := $(patsubst %.c++, $(objdir)/%.o, $(SRCS))

.PHONY: all clean run

all: $(target)

$(objdir)/%.o: %.c++
	@if not exist $(objdir) mkdir $(objdir)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(target): $(OBJS)
	@if not exist $(bindir) mkdir $(bindir)
	$(CXX) $(OBJS) -o $(target) $(LDFLAGS)

run: all
	./$(target)

clean:
	@if exist $(bindir) rmdir /s /q $(bindir)
	@if exist $(objdir) rmdir /s /q $(objdir)
