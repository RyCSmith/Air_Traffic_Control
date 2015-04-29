
all: Part5

Part5: Part5.cpp
	c++ Part5.cpp -o Part5

clean:
	rm -rf *.o

clobber: clean
	rm -rf Part5
