CC=g++
CFLAGS=-g
ALLOBJ=main.o
DEPS=indexLanguages.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

IndexLangages: $(ALLOBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean

clean:
	rm -f IndexLanguages
	rm -f indexItalian.dat
	rm -f $(ALLOBJ)
