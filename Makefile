ifish: *.c *.h
	cc *.c -o ifish

debug: *.c *.h
	cc -DDEBUG *.c -o ifish

all: ifish

.PHONY: clean

clean:
	rm ifish
