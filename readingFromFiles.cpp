#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

struct IndexingStructure {
  vector<string> files;

  void IndexFiles() {
    int language = 0;
    for (auto file : files) {
      ifstream dictionary(file, ios::binary);
      string dictionaryLine, englishWord;
      while (dictionary.peek() != EOF) {
        dictionary >> englishWord;
        dictionary.ignore(); // This is to eliminate the tab that separates the characters. 
        /* Here comes the TODO: Insert in the B Tree
          bTree.insert(englishWord, dictionary.tellg(), language)
        */
        getline(dictionary, dictionaryLine);
        // Debugging line: cout << englishWord << " " << dictionaryLine << endl;
      }
      language++;
    }
  }
};

int main(int argc, char ** argv) {
  IndexingStructure languageIndexing;

  for (int i = 1; i < argc; i++) {
    string newFileName(argv[i]);
    languageIndexing.files.push_back(newFileName);
  }

  languageIndexing.IndexFiles();
}