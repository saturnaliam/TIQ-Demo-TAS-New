files := src/main.c src/include/utils.c
build:
	gcc -o tas $(files)

run: build
	./tas.exe