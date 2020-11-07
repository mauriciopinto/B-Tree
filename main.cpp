#include "indexLanguages.h"

int main () {
	BTreeIndex index ("dicts/italian.txt", 3);
	index.execute (italiano);
}
