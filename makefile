
read: main.o readfile.o createNDfield.o plotting.o args.o functions.o momentofinertia.o
	cc -o read main.o readfile.o createNDfield.o plotting.o args.o functions.o momentofinertia.o -lm

main.o: main.c read.h
	cc -c main.c
	
readfile.o: readfile.c read.h
	cc -c readfile.c

createNDfield.o: createNDfield.c read.h
	cc -c createNDfield.c

momentofinertia.o: momentofinertia.c read.h
	cc -c momentofinertia.c

plotting.o: plotting.c read.h
	cc -c plotting.c

functions.o: functions.c read.h
	cc -c functions.c

args.o: args.c read.h
	cc -c args.c

clean:
	rm -f main main.o readfile.o plotting.o createNDfield.o functions.o momentofinertia.o
