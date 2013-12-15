CC=g++
#LIBS= -lpthread #-mcmodel=medium -fPIC -m64 -fpermissive
#LIBOMP=-openmp
#LIBS=-lpthread -pipe -nostdinc -fno-builtin -m64 -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -gstabs
LIBS=-fopenmp -lpthread
all:
	$(CC) main.cpp MatrixGenerator.cpp CRS.cpp CCS.cpp timeSurvey.cpp -o main.o $(LIBS)

clean:
	rm -R *.o
	rm -R *.txt

cleanbkp:
	rm -R *~
