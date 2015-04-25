#include "dictree.h"
#include <stdio.h>

int main(int argc, char * argv[]) {
	char * letters[] = {"this", "wats", "oahg", "fgdt", 0};
	char * words[] = {"this", "that", "fat", "two", "hel30", 0};
	char * test_words[] = {"this", "th", "that", "xxsfda", "das34hdfa", "hel", "fat", "two", 0};
	dictree_node_t root;
	printf("sizeof(root) -> %lu\n", sizeof(root));
	if(dictree_init(&root) != 0) {
		fprintf(stderr, "dictree_init fail!\n");
		return 1;
	}
	if(dictree_load_from_strings(&root, words) != 0) {
		fprintf(stderr, "dictree_load_from_strings fail!\n");
		return 1;
	}
	char ** pstr = &test_words[0];
	while(*pstr) {
		printf("search -> %s\n", *pstr);
		if(dictree_search(&root, *pstr) == 0) {
			printf("'%s' found!\n", *pstr);
		} else {
			printf("'%s' not found!\n", *pstr);
		}
		pstr++;
	}
	dictree_destroy(&root);
	return 0;
}
