
readSkel: main.o readNDskel.o myendian.o flattenSkel.o plotSkel.o readSkel.h
	cc -o readSkel main.o readNDskel.o myendian.o flattenSkel.o plotSkel.o -lm

main.o: main.c readSkel.h
	cc -c main.c

readNDskel.o: readNDskel.c readSkel.h
	cc -c readNDskel.c 

flattenSkel.o: flattenSkel.c readSkel.h
	cc -c flattenSkel.c

plotSkel.o: plotSkel.c readSkel.h
	cc -c plotSkel.c

myendian.o: myendian.c readSkel.h
	cc -c myendian.c

clean:
	rm -f readSkel main.o readNDskel.o myendian.o flattenSkel.o plotSkel.o
