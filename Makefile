#CC = g++
#CFLAGS = -c -fPIC 
#PROG = changeViewOrientation
#SULFIX1 =.o
#SULFIX2 =.cpp

#$(CC) $(CFLAGS) $(SRC) $(SULFIX2) -o $(SRC) $(SULFIX1)


g++ -c -fPIC handle3ddataset.cpp -o ../bin/handle3ddataset.o 
g++ -shared -o ../bin/libhandle3ddataset.so ../bin/handle3ddataset.o