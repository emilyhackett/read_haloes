
esys: main.o readfile.o momentofinertia.o functions.o plotting.o
	gcc -o esys main.o readfile.o momentofinertia.o functions.o plotting.o -lm

main.o: main.c read.h
	gcc -c main.c
	
readfile.o: readfile.c read.h
	gcc -c readfile.c

momentofinertia.o: momentofinertia.c read.h
	gcc -c momentofinertia.c

functions.o: functions.c read.h
	gcc -c functions.c

plotting.o: plotting.c read.h
	gcc -c plotting.c

clean:
	rm -f esys main.o readfile.o momentofinertia.o functions.o plotting.o
