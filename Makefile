#CC = g++
#CFLAGS = -c -fPIC 
#PROG = changeViewOrientation
#SULFIX1 =.o
#SULFIX2 =.cpp

#$(CC) $(CFLAGS) $(SRC) $(SULFIX2) -o $(SRC) $(SULFIX1)


all: makemyfiles

makemyfiles:
	g++ -c -fPIC handle3ddataset.h -I../../Array/ -o ../bin/handle3ddataset.o 










