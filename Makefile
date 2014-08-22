TARGET := Game.run
OBJDIR := obj
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system
CC_FLAGS := -std=c++11 -Wall -O0 -g
CXX := clang++-mp-3.4

.PHONY: objects

all: $(TARGET)
	

$(TARGET): $(OBJDIR) objects
	

objects: $(OBJ_FILES)
	$(CXX) $(CC_FLAGS) -o $(TARGET) $^ $(LD_FLAGS)

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CC_FLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(TARGET)
	@rmdir $(OBJDIR)
