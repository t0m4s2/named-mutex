SRC = src/main.cc src/named_mutex.cc src/named_mutex_impl.cc
OBJ = $(SRC:.cpp=.o)
LIBS := -pthread -lrt

named_mutex : $(OBJ)
	$(CXX) -g -o $@ -I include $(OBJ) $(LIBS)

clean :
	rm -f named_mutex *.o
