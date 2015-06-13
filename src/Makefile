all: main.cpp
	g++ main.cpp -o ../bin/2001Console -std=c++11
	make XteWrapper
XteWrapper: XteWrapper.h XteWrapper.cpp
	g++ XteWrapper.cpp -o XteWrapper -lm -std=c++11
