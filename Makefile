ifish: *.c *.h
	cc *.c -o ifish

all: ifish

.PHONY: clean

clean:
	rm ifish
