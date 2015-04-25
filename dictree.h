#ifndef _DICTREE_H
#define _DICTREE_H

#define ALPHA_NR	26

typedef struct dictree_node_t {
	int type;
	struct dictree_node_t * nodes[ALPHA_NR];
} dictree_node_t;

// return 0 on success, others on error
int dictree_init(dictree_node_t * root);
void dictree_destroy(dictree_node_t * root);

// return 0 on success, 1 on memory error, 2 on some other errors
int dictree_insert(dictree_node_t * root, char * str);
// return 0 on success, 1 on failure
int dictree_search(dictree_node_t * root, char * str);

// return 0 on success, 1 on failure
int dictree_load_from_strings(dictree_node_t * root, char * strs[]);
int dictree_load_from_file(dictree_node_t * root, char * filename);
#endif
