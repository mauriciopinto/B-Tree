CC=g++
CFLAGS=-g
ALLOBJ=main.o
DEPS=indexLanguages.h BPage.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

IndexLanguages: $(ALLOBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean

clean:
	rm -f IndexLanguages
	rm -f indexItalian.dat
	rm -f $(ALLOBJ)
