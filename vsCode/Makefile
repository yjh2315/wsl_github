all: compile run

compile: sorting.c
	gcc sorting.c -o sorting

run: sorting
	./sorting input.txt output.txt

clean: sorting
	rm sorting
