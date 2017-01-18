
readSkel: main.o readNDskel.o myendian.o plotSkel.o arrayCheck.o readSkel.h
	gcc -o readSkel main.o readNDskel.o myendian.o plotSkel.o arrayCheck.o -lm

main.o: main.c readSkel.h
	gcc -c main.c

readNDskel.o: readNDskel.c readSkel.h
	gcc -c readNDskel.c 

plotSkel.o: plotSkel.c readSkel.h
	gcc -c plotSkel.c

myendian.o: myendian.c readSkel.h
	gcc -c myendian.c

arrayCheck.o: arrayCheck.c readSkel.h
	gcc -c arrayCheck.c

clean:
	rm -f readSkel main.o readNDskel.o myendian.o plotSkel.o arrayCheck.o
