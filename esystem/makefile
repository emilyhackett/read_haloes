
esys: main.o readfile.o momentofinertia.o functions.o plotting.o
	gcc -o esys main.o readfile.o momentofinertia.o functions.o plotting.o -lm

main.o: main.c esys.h
	gcc -c main.c
	
readfile.o: readfile.c esys.h
	gcc -c readfile.c

momentofinertia.o: momentofinertia.c esys.h
	gcc -c momentofinertia.c

functions.o: functions.c esys.h
	gcc -c functions.c

plotting.o: plotting.c esys.h
	gcc -c plotting.c

clean:
	rm -f esys main.o readfile.o momentofinertia.o functions.o plotting.o
