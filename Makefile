files := src/main.c src/include/utils.c src/include/tas.c
build:
	gcc -o tas $(files)