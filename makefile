CC=g++
#LIBS= -lpthread #-mcmodel=medium -fPIC -m64 -fpermissive
#LIBOMP=-openmp
#LIBS=-lpthread -pipe -nostdinc -fno-builtin -m64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -gstabs
all:
	$(CC) -o numGen.o numGen.cpp
clean:
	rm -R *.o

cleanbkp:
	rm -R *~
