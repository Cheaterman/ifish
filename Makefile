ifish: *.c *.h
	cc *.c -o ifish

debug: *.c *.h
	cc -DDEBUG -g -O0 *.c -o ifish

all: ifish

.PHONY: clean

clean:
	rm ifish
