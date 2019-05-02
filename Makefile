

all: extractor

extractor:
	@gcc main.c -o exe

clean:
	@rm -f exe output
