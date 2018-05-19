CXX = g++
CFLAGS = -Wall -Werror -std=c++11 -fPIC


INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib 
LDLIBS = -lcurl


all: vk

vk: main.o api.o attachment.o
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) main.o api.o attachment.o -o vk.so -shared

main.o: main.cpp
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -c main.cpp

api.o: src/api.h src/api.cpp src/json.hpp src/attachment.h
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -c src/api.cpp

attachment.o: src/attachment.h src/attachment.cpp src/json.hpp
	$(CXX) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -c src/attachment.cpp

clean:
	rm -rf *.o src/*.o hello