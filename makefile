main: photoeditor.o bmp.o
	gcc -o bip photoeditor.o bmp.o
photοeditor.o: bmp.h photoeditor.c
	gcc -c photoeditor.c
bmp.o: bmp.c
	gcc -c bmp.c