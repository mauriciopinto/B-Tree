#ifndef BPAGE_H
#define BPAGE_H

#include <iostream.h>
#include <fstream.h>
#include <string.h>

#define maxSize 100

enum Idioma { italiano, 

class Palabra {
private:
	char key[25];
	string definicion;
	Idioma idioma;
public:
	Palabra (char *k, string d) {
		strcpy (key, k);
		definicion = d;
	}

	void write (ofstream & file) {
		file.write ((char *) this, sizeof (Palabra));
	}

	void read (ifstream &ifile) {
		file.read ((char *) this, sizeof (Palabra));
	}

	void Print () {
		cout << "Palabra: " << key << " | Definicion: " definicion << endl;
	}
};

struct Record {
	char key[25];
	long long pdir[6];
	long long offset;
};

class BPage {
private:
	int order;
	Record *keys;
	int currentSize;
	unsigned long int *children;
public:
	BPage (int order) {
		keys = new Record[order];
		children = new unsigned long int[order+1];
		currentSize = 0;
	}

	void write (ofstream &file) {
		file.write ((char *) this, sizeof (Page));
	}

	void read (ifstream &file) {
		file.read ((char *) this, sizeof (Page));
	}
};

#endif
