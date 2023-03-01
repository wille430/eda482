

CFLAGS = -Wno-implicit-function-declaration

main: startup.o
	echo "Linking and compiling to complete executable..."
	gcc $(CFLAGS) startup.o
	chmod +x main

startup.o: startup.c
	gcc $(CFLAGS) -c startup.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	echo "Removing build output files..."
	rm *.o
