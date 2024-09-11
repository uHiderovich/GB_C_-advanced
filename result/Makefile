all: drone

drone: main.o drone.o
	gcc -O2 -o drone main.o drone.o

main.o: main.c drone.h
	gcc -c -o main.o main.c

drone.o: drone.c
	gcc -c -o drone.o drone.c

clear:
	del *.o
	del drone.exe

	# rm -rf *.o drone
