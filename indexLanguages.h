#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

Record *binarySearch (Page *page, int left, int right, char *key) {
	if (right >= 1) {
		if (strcmp (page->m_Reg[left].key, key) == 0) {
			return &page->m_Reg[left];
		}
		if (strcmp (page->m_Reg[right].key, key) == 0)
			return &page->m_Reg[right];
		int middle = (left + right - 1) / 2;
		if (strcmp (page->m_Reg[middle].key, key) == 0)
			return &page->m_Reg[middle];
		else {
			if (strcmp (page->m_Reg[middle].key, key) > 0)
				return binarySearch (page, left, middle - 1, key);
			else
				return binarySearch (page, middle + 1, right, key);
		}
	}	
}

class BTreeIndex {
private:
	BPage root;
	BPage currentPage;
	string dataFile;
public:
    Index(string filename) : dataFile (filename) {}
    
	void Execute(){
      ifstream file(dataFile, ios::binary);
	  ofstream indexFile ("index.dat", ios::binary);
      
	  string fileLine;
      Page newPage;
      
	  while(file.peek() != EOF){
        Record newRecord;
        newRecord.pdir = file.tellg();
        
		file >> newRecord.key;
        getline(file, fileLine);
        
		newRecord.offset = file.tellg() - newRecord.pdir;
        newRecord.offset--;
		insert (newRecord, idioma);
    }

	void FindKey (char *key) {
		ifstream index ("indexItalian.dat", ios::binary);
		while (index.peek () != EOF) {
			currentPage.Read (index);
			if (strcmp (currentPage.m_Reg[maxSize - 1].key, key) < 0)
				continue;
			else {
				Record *found = binarySearch (&currentPage, 0, maxSize - 1, key);
				if (found)
					cout << found->key << endl;
				else
					cout << "Key not found" << endl;
				return;
			}
		}
	}

	int insert (Record record) {
		currentPage.keys[currentSize] = record;
		
		if (currentSize > order)
			currentPage.split ();
	}

	void splitNode (BPage node) {
		
	}
};
