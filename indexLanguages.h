#include <iostream>
#include <fstream>
#include <list>
#include <string.h>

#define maxSize 100

using namespace std;

class Palabra{
  private:
    char key[25];
    string definicion;
  public:
    Palabra(char * k, string d){
      strcpy(key, k);
      definicion = d;
    }
    void Write(ofstream & file) {
      file.write((char*)this, sizeof(Palabra));
    }   
    void Read(ifstream & file) {
      file.read((char*)this, sizeof(Palabra));
    }    
    void Print() {
      cout<< "Palabra: " << key <<" | Definicion: "<< definicion << endl;
    }     
};

struct Record{
  char key[25];
  long long pdir;
  long long offset;
};

struct Page{
    Record m_Reg[maxSize];
    int m_size;
    int m_currentSize;
    unsigned long m_nextPage;
    Page(){
      m_size = maxSize;
      m_currentSize = 0;
      //m_Reg = new Record[m_size];
    }
    void Write(ofstream & file){ // Abrir el archivo con append
      file.write((char*)this, sizeof(Page));
    }
    void Read(ifstream & file){
      file.read((char*)this, sizeof(Page));
    }
};

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

class Index{
  private:
    Page currentPage;
    string m_DataFile;
  public:
    Index(string filename){
      m_DataFile = filename;
    }
    void Execute(){
      ifstream file(m_DataFile, ios::binary);
      string fileLine;
      Page newPage;
      ofstream indexFile("indexItalian.dat", ios::binary | ios::app);
      while(file.peek() != EOF){
        Record newRecord;
        newRecord.pdir = file.tellg();
        file >> newRecord.key;
        getline(file, fileLine);
        // cout << newRecord.key << " ";
        newRecord.offset = file.tellg() - newRecord.pdir;
        newRecord.offset--;
        // cout << newRecord.pdir << " ";
        // cout << newRecord.offset << endl;
        newPage.m_Reg[newPage.m_currentSize] = newRecord;
        newPage.m_currentSize++;
        if(newPage.m_currentSize == maxSize)
        {
          newPage.m_nextPage += sizeof(Page);
          newPage.Write(indexFile);
          newPage.m_currentSize = 0;
        }
      }
      newPage.m_nextPage = -1;
      newPage.Write(indexFile);
    }
    /*void ReadFirstPage(){
      ifstream indexFile("indexItalian.dat", ios::binary);
      Page firstPage(100);
	  Record tempArray[100];
      indexFile.read((char *) & tempArray, sizeof(Record) * 100);
      for (int i = 0; i < 100; i++)
		cout << tempArray[i].key << endl;
    }*/

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
};
