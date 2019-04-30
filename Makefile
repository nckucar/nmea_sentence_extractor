

all: extractor

extractor:
	gcc main.c -o exe

clean:
	rm exe output