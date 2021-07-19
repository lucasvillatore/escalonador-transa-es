all: main.c
	gcc -o escalona main.c

clean:
	rm -rf escalona main