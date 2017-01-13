makeND: main.o readfile.o createNDfield.o
	gcc -o makeND main.o readfile.o createNDfield.o -lm

main.o: main.c makeNDfile.h
	gcc -c main.c

readfile.o: readfile.c makeNDfile.h
	gcc -c readfile.c

createNDfield.o: createNDfield.c makeNDfile.h
	gcc -c createNDfield.c

clean: 
	rm makeND main.o readfile.o createNDfield.o
