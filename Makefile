all: bin/meshconverter

bin/meshconverter:
	mkdir -p bin/
	gcc -Wall -Wextra -Wpedantic -g meshconverter.c -o bin/meshconverter

run: bin/meshconverter Box01.mesh
	./bin/meshconverter Box01.mesh

clean:
	rm -fr bin/
