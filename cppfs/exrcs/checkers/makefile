
inc_dir = include/
obj_dir = objs/
src_dir = srcs/
graph_dir = graphics_utils/

SRCS = $(wildcard $(src_dir)*.cpp)
OBJS = $(patsubst $(src_dir)%.cpp, $(obj_dir)%.o, $(SRCS))
LIBS = $(graph_dir)libglut_utils.so $(graph_dir)libpoint.a
CPP = g++
FLAGS = -ansi -pedantic-errors -Wall -Wextra -g -std=c++98

checkers.out: $(OBJS)
	$(CPP) $(FLAGS) -o $@ $^ $(LIBS) -Wl,-rpath=$(graph_dir)
	
$(obj_dir)%.o: $(src_dir)%.cpp
	$(CPP) $(FLAGS) -c -o $@ $^ -Iinclude

.PHONY: clean

clean:
	rm $(obj_dir)*.o
	rm *.out
