#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dictree.h"

enum {
	NODE_TYPE_MIDDLE = 0,
	NODE_TYPE_LEAF
};
static inline dictree_node_t * new_dictree_node(void) {
	dictree_node_t * _new_dict_node = (dictree_node_t*)malloc(sizeof(dictree_node_t));
	if(!_new_dict_node) {
		perror("malloc");
		return NULL;
	}
	_new_dict_node->type = NODE_TYPE_MIDDLE;
	memset(_new_dict_node->nodes, 0, sizeof(dictree_node_t*)*ALPHA_NR);
	return _new_dict_node;
}

int dictree_init(dictree_node_t * root) {
	if(!root) {
		return 1;
	}
	memset(root->nodes, 0, sizeof(dictree_node_t*)*ALPHA_NR);
	return 0;
}

static inline void free_dictree_node(dictree_node_t * node) {
	if(!node) return;
	int child_idx;
	for(child_idx=0;child_idx<ALPHA_NR;child_idx++) {
		free_dictree_node(node->nodes[child_idx]);
		node->nodes[child_idx] = NULL;
	}
	free(node);
}

void dictree_destroy(dictree_node_t * root) {
	if(!root) {
		return;
	}
	int child_idx;
	for(child_idx=0;child_idx<ALPHA_NR;child_idx++) {
		free_dictree_node(root->nodes[child_idx]);
		root->nodes[child_idx] = NULL;
	}
}

enum {
	DICT_TREE_SEARCH_MIDDLE = 0,
	DICT_TREE_SEARCH_LEAF,
	DICT_TREE_SEARCH_NEED_GROW,
	DICT_TREE_SEARCH_FAIL
};
static int _dictree_search(dictree_node_t * root, char * str, dictree_node_t ** ppnode, char ** ppstr) {
	if(!root) return DICT_TREE_SEARCH_FAIL;
	char * p = str;
	dictree_node_t * node = root;
	while(*p) {
		if(!isalpha(*p)) {
			return DICT_TREE_SEARCH_FAIL;
		}
		int idx = *p - (*p>='a'?'a':'A');
		dictree_node_t * child_node = node->nodes[idx];
		if(!child_node) {
			if(ppnode) {
				*ppnode = node;
			}
			if(ppstr) {
				*ppstr = p;
			}
			// need grow
			return DICT_TREE_SEARCH_NEED_GROW;
		}
		node = child_node;
		p++;
	}
	// search ok
	if(node->type == NODE_TYPE_MIDDLE) {
		if(ppnode) {
			*ppnode = node;
		}
		return DICT_TREE_SEARCH_MIDDLE;
	}
	return DICT_TREE_SEARCH_LEAF;
}

int dictree_insert(dictree_node_t * root, char * str) {
	fprintf(stderr, "insert -> %s\n", str);
	dictree_node_t * pnode;
	char * pstr;
	int i = _dictree_search(root, str, &pnode, &pstr);
	switch(i) {
		case DICT_TREE_SEARCH_LEAF:
			// already inserted
			return 0;
		case DICT_TREE_SEARCH_NEED_GROW:
			// grow
			while(*pstr) {
				if(!isalpha(*pstr)) {
					break;
				}
				int idx = *pstr - (*pstr>='a'?'a':'A');
				assert(pnode->nodes[idx]==NULL);
				if((pnode->nodes[idx] = new_dictree_node()) == NULL) {
					return 1;
				}
				pstr++;
				pnode = pnode->nodes[idx];
			}
		case DICT_TREE_SEARCH_MIDDLE:
			// need set leaf
			pnode->type = NODE_TYPE_LEAF;
			return 0;
		default:
			// fail
			return 2;
	}
}

int dictree_search(dictree_node_t * root, char * str) {
	int i = _dictree_search(root, str, NULL, NULL);
	return i==DICT_TREE_SEARCH_LEAF?0:1;
}

int dictree_load_from_strings(dictree_node_t * root, char * strs[]) {
	char ** pstr = &strs[0];
	while(*pstr) {
		if(dictree_insert(root, *pstr) != 0) {
			return 1;
		}
		pstr++;
	}
	return 0;
}

int dictree_load_from_file(dictree_node_t * root, char * filename) {
	return 0;
}
