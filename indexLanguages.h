#include "BPage.h"

// PageInfo contiene información del page (se usa como valor de retorno de findNodeByKey)
struct PageInfo {
	BPage parent;          // Padre del page 
	int parentPosition;    // Posicion del page entre los hijos del padre
	BPage current;         // Page actual
	bool parentExists;     // true si hay un padre, false en caso contrario
	int currentPosition;   // Posicion del page donde se encuentra el key
	bool keyExists;		   // true si el key existe en el page, false si no existe
};

pair<int, bool> binarySearch (BPage page, int left, int right, char key[]) {
	if (page.currentSize == 0)
		return make_pair (0, false);
	if (left == right)
		return make_pair(strcmp (page.keys[left].key, key) > 0 ? left : left + 1, strcmp (page.keys[left].key, key) == 0 ? true : false);
	if (strcmp (page.keys[left].key, key) == 0)
		return make_pair (left, true);
	if (strcmp (page.keys[right].key, key) == 0)
		return make_pair (right, true);
	int middle = (left + right - 1) / 2;
	if (strcmp (page.keys[middle].key, key) == 0)
		return make_pair (middle, true);
	else {
		if (strcmp (page.keys[middle].key, key) > 0)
			return binarySearch (page, left, middle, key);
		else
			return binarySearch (page, middle + 1, right, key);
	}
}

class BTreeIndex {
private:
	int order;
	BPage root;
	string dataFile;
public:
    BTreeIndex(string filename, int order) : dataFile (filename), order (order) {
		root.order = order;
		root.currentSize = 0;
		root.keys = new Record[order + 1];
		root.children = new long long[order + 2];
		root.pdir = 0;
	}	

	void execute(Idioma idioma){
      ifstream file(dataFile, ios::binary);
	  ofstream indexFile ("index.dat", ios::binary);
      
	  int initialPosition = indexFile.tellp ();
	  string fileLine;
      BPage newPage (order);
      char newKey[25];
	  long long newDir;
	  long long newOffset;

	  while(file.peek() != EOF){
        newDir = file.tellg();
        
		file >> newKey;
		newDir = file.tellg ();
        getline(file, fileLine);
        
		newOffset = file.tellg() - newDir;
        newOffset--;
		insert (newKey, newDir, newOffset, idioma, indexFile);
    	}
	}
	void FindKey (char *key) {
		ifstream index ("index.dat", ios::binary);
		/*while (index.peek () != EOF) {
			currentPage.read (index);
		}*/
	}

	PageInfo findNodeByKey (char key[]) {
		ifstream index ("index.dat", ios::binary);
		PageInfo info;
		root.read (index, root.pdir);

		// Se busca el key en el root y se retorna la posicion donde debe ir y si se encuentra
		pair<int, bool> position = binarySearch (root, 0, root.currentSize - 1, key);
		
		// Si el root es una hoja, se determina que no hay padre 
		if (root.isLeaf) {
			info.parentExists = false;
			info.current = root;
			info.currentPosition = position.first;
			info.keyExists = position.second;
		}

		// Si el root no es hoja, se recorre el arbol hasta encontrar la hoja correspondiente
		else {
			BPage parentPage = root;
			BPage nextPage;
			nextPage.read (index, parentPage.children[position.first + 1]);
			while (!nextPage.isLeaf) {
				parentPage = nextPage;
				position = binarySearch (parentPage, 0, parentPage.currentSize - 1, key);
				nextPage.read (index, parentPage.children[position.first + 1]);
			}
			info.parent = parentPage;
			info.parentPosition = position.first;
			info.parentExists = true;
			info.current = nextPage;
			position = binarySearch (nextPage, 0, nextPage.currentSize - 1, key);
			info.currentPosition = position.first;
			info.keyExists = position.second;
		}
		return info;
	}

	void insert (char key[], long long pdir, long long offset, Idioma idioma, ofstream &index) {
		// Se encuentra al page donde debe estar el key y la posicion en donde debe estar,
		// y también al page padre y la posicion del hijo que apunta al page actual.
		PageInfo info = findNodeByKey (key);

		// Se extrae la información de los pares
		BPage parent = info.parent;
		int parentPosition = info.parentPosition;
		bool parentExists = info.parentExists;
		BPage current = info.current;
		int currentPosition = info.currentPosition;
		bool exists = info.keyExists;
		
		cout << "inserting key: " << key << endl;
		cout << current.currentSize << ": ";
		for (int i = 0; i <= order; i++) {
			cout << current.keys[i].key << " ";
		}
		cout << endl;

		// Si existe el key, se debe revisar si ya existe una definicion en el idioma
		// seleccionado.
		if (exists) {
			// Si no existe, se asigna la direccion de la definicion correspondiente al idioma
			if (current.keys[currentPosition].pdir[idioma] == 0)
				current.keys[currentPosition].pdir[idioma] = pdir;
		}
		
		//Si no existe el key, se inserta en la posicion correspondiente
		else {
			if (currentPosition < current.currentSize) {
				int i = current.currentSize;
				while (i > currentPosition) {
					current.keys[i] = current.keys[i - 1];
					i--;
				}
			}

			Record newRecord;
			strcpy (newRecord.key, key);
			newRecord.pdir[idioma] = pdir;
			newRecord.offset = offset;
			current.keys[currentPosition] = newRecord;
			current.currentSize++;
		}
		
		// Si se sobrepasa la capacidad del page, se divide
		if (current.currentSize > order)
			splitLeaf (current, parent, parentPosition, parentExists, index);
		current.write (index, current.pdir);
		index.flush ();
	}

	void splitPage (BPage &current) {
		cout << "Hasta aqui llegue" << endl;
		return;
	}
	
	void splitLeaf (BPage &current, BPage &parent, int parentPosition, bool parentExists, ofstream &index) {
		// Se calcula la posicion del key que se ascenderá y se crean los dos nuevos nodos
		int midPosition = (order + 1) / 2;
		BPage newPageRight (order);
		BPage newPageLeft (order);
		
		// Si no hay parent, se crea un nuevo root
		if (!parentExists) {
			parent = BPage (order);
			parentPosition = 0;
		}
		
		// Se mueven los keys del parent para hacer espacio para la nueva key
		int i = parent.currentSize;
		while (i > parentPosition) {
			parent.keys[i] = parent.keys[i - 1];
			i--;
		}
		parent.keys[parentPosition] = current.keys[midPosition];
		parent.currentSize++;

		// Se mueven los children del parent para hacer espacio para el nuevo child
		int j = i + 1;
		while (j > parentPosition + 1) {
			parent.children[i] = parent.children[i - 1];
			i--;
		}

		// Se copian los keys correspondientes a los nuevos nodos
		newPageLeft.pdir = index.tellp ();
		for (int k = 0; k < midPosition; k++) {
			newPageLeft.keys[k] = current.keys[k];
		}
		newPageLeft.write (index, newPageLeft.pdir);

		newPageRight.pdir = index.tellp ();
		for (int k = midPosition + 1; k < current.currentSize; k++) {
			newPageRight.keys[k] = current.keys[k];
		}
		newPageRight.write (index, newPageRight.pdir);

		// Se agregan los dos nodos como hijos del padre
		parent.children[parentPosition] = newPageLeft.pdir;
		parent.children[parentPosition + 1] = newPageRight.pdir;
		// Si no hay un padre, significa que se debe crear el nuevo root
		if (!parentExists)
			parent.pdir = 0;
		parent.write (index, parent.pdir);
		index.flush ();

		// Si el padre excede el tamaño máximo, se divide
		if (parent.currentSize > order)
			splitPage (parent);
	}
};
