CC=g++
#LIBS= -lpthread #-mcmodel=medium -fPIC -m64 -fpermissive
#LIBOMP=-openmp
#LIBS=-lpthread -pipe -nostdinc -fno-builtin -m64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -gstabs
all:
	$(CC) -o main.o main.cpp MatrixGenerator.cpp CRS.cpp CCS.cpp

clean:
	rm -R *.o
	rm -R *.txt

cleanbkp:
	rm -R *~
