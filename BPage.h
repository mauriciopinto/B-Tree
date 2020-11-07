#ifndef BPAGE_H
#define BPAGE_H

#include <iostream>
#include <fstream>
#include <string.h>

#define maxSize 100

using namespace std;

enum Idioma { italiano, frances, aleman, latin, portugues, espanol }; 

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

	void read (ifstream & file) {
		file.read ((char *) this, sizeof (Palabra));
	}

	void Print () {
		cout << "Palabra: " << key << " | Definicion: " << definicion << endl;
	}
};

struct Record {
	char key[25];
	long long pdir[6];
	long long offset;
};

class BPage {
public:
	int order;
	Record *keys;
	int currentSize;
	long long *children;
	bool isLeaf = true;;
	long long pdir;

	BPage () {};

	BPage (int order) {
		order = order;
		keys = new Record[order + 1];
		children = new long long[order + 2];
		currentSize = 0;
	}

	void write (ofstream &file, long long dir) {
		file.seekp (dir);
		file.write ((char *) &this->order, sizeof (int));
		for (int i = 0; i < order + 1; i++)
			file.write ((char *) &this->keys[i], sizeof (Record));
		file.write ((char *) &this->currentSize, sizeof (int));
		for (int i = 0; i < order + 2; i++)
			file.write ((char *) &this->children[i], sizeof (long long));
		file.write ((char *) &this->isLeaf, sizeof (bool));
		file.write ((char *) &this->pdir, sizeof (long long));
	}

	void read (ifstream &file, long long dir) {
		file.seekg (dir);
		file.read ((char *) &this->order, sizeof (int));
		for (int i = 0; i < order + 1; i++)
			file.read ((char *) &this->keys[i], sizeof (Record));
		file.read ((char *) &this->currentSize, sizeof (int));
		for (int i = 0; i < order + 2; i++)
			file.read ((char *) &this->children[i], sizeof (long long));
		file.read ((char *) &this->isLeaf, sizeof (bool));
		file.read ((char *) &this->pdir, sizeof (long long));
	}
};

#endif
