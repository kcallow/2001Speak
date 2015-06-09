all: main.cpp
	g++ main.cpp -o Main -std=c++11
	make XteWrapper

XteWrapper: XteWrapper.h XteWrapper.cpp
	g++ XteWrapper.cpp -o XteWrapper -lm -std=c++11

