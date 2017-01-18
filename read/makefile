
read: main.o readfile.o createNDfield.o plotting.o args.o functions.o momentofinertia.o
	gcc -o read main.o readfile.o createNDfield.o plotting.o args.o functions.o momentofinertia.o -lm

main.o: main.c read.h
	gcc -c main.c
	
readfile.o: readfile.c read.h
	gcc -c readfile.c

createNDfield.o: createNDfield.c read.h
	gcc -c createNDfield.c

momentofinertia.o: momentofinertia.c read.h
	gcc -c momentofinertia.c

plotting.o: plotting.c read.h
	gcc -c plotting.c

functions.o: functions.c read.h
	gcc -c functions.c

args.o: args.c read.h
	gcc -c args.c

clean:
	rm -f read main.o args.o readfile.o plotting.o createNDfield.o functions.o momentofinertia.o
