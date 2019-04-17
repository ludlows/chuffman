/*
codes for huffman encode tree
www.github.com/ludlows
2019-04
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "huff_encode.h"



// allocate the memory for huffman encode node
HEncodeNode* create_huff_encode_node(char symbol, double freq, int is_leaf) {
	HEncodeNode* p_node = (HEncodeNode*)malloc(sizeof(HEncodeNode));
	if (p_node != NULL) {
		p_node->left = NULL;
		p_node->right = NULL;
		p_node->parent = NULL;
		p_node->next = NULL;
		p_node->freq = freq;
		p_node->is_leaf = is_leaf;
		p_node->symbol = symbol;
	}
	return p_node;
}


// function used to insert a node into priority queue
void insert_huff_pqueue(HEncodeNode* node, HEncodeNode** q_head) {
	printf("inserting node: (%c, %f)\n", node->symbol, node->freq);
	if (*q_head == NULL) { // when the pqueue is empty
		*q_head = node;
		return;
	}
	HEncodeNode* curr_node = NULL;
	HEncodeNode* prev_node = NULL;
	curr_node = *q_head;
	while ((curr_node != NULL) && (curr_node->freq < node->freq)) {
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
	// insert the node at the begining of pqueue
	if (curr_node == *q_head) {
		node->next = *q_head;
		*q_head = node;
	}
	else { // insert node between prev_node and curr_node
		prev_node->next = node;
		node->next = curr_node;
	}
}



// function used to display the priority queue
void disp_huff_pqueue(HEncodeNode* q_head) {
	printf("priority queue: ");
	while (q_head) {
		printf("(%c, %f),", q_head->symbol, q_head->freq);
		q_head = q_head->next;
	}
	printf("\n");
}


// functions used to pop element from pqueue
HEncodeNode* pop_huff_pqueue(HEncodeNode** q_head) {
	if (*q_head == NULL) return NULL;
	HEncodeNode* p_node = *q_head;
	*q_head = (*q_head)->next;
	printf("popped: (%c, %f)\n", p_node->symbol, p_node->freq);
	return p_node;
}


// functions used to generate codebook recursively
// codebook: char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer 
void generate_huff_codebook(HEncodeNode* root, int depth, char* codebook) {
	if (root->is_leaf) { // we reach the bottom of the encode tree
		int len = depth;
		char symbol = root->symbol;
		// add 0 at the end of string
		*(codebook + ((size_t)symbol) * HUFF_MAX_LEN + len) = 0;
		// start from the bottom (leaf) to the top (root)
		HEncodeNode* parent = root->parent;
		while ((parent != NULL) && (len > 0)) {
			// root is left of parent
			if (root == parent->left) {
				*(codebook + ((size_t)symbol) *  HUFF_MAX_LEN + (--len)) = '0';
			}
			else { // root is right of parent
				*(codebook + ((size_t)symbol) *  HUFF_MAX_LEN + (--len)) = '1';
			}
			root = parent;
			parent = root->parent;
		}
		// display
		printf("built code: (%c, %s)\n", symbol, codebook + ((size_t)symbol) *  HUFF_MAX_LEN);
	}
	else {
		generate_huff_codebook(root->left, depth + 1, codebook);
		generate_huff_codebook(root->right, depth + 1, codebook);
	}
}


// functions used to write codebook into file
// codebook:  char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer 
void write_huff_codebook(FILE* f_out, char* codebook) {
	int i = 0;
	for (i = 0; i < HUFF_MAX_SYMBOLS; i++) {
		if (*(codebook + i * HUFF_MAX_LEN)) { // when the strcode of symbol char i is not empty
			fprintf(f_out, "#%c %s\n", i, codebook + i * HUFF_MAX_LEN);
		}
	}
}

// functions used to write encoded bit stream into file
// codebook: char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer like &codebook[0][0]
void write_huff_encode_stream(FILE* f_out, char* str, char* codebook) {
	while (*str) {
		fprintf(f_out, "%s", codebook + (size_t)(*str) * HUFF_MAX_LEN);
		str++;
	}
}



// functions used to free the memory of huffman encode tree.
// this function is implemented post-order traversal on recursive way.
void free_huff_encode_tree(HEncodeNode* root) {
	if (root == NULL) return;
	free_huff_encode_tree(root->left);
	free_huff_encode_tree(root->right);
	free(root);
	root = NULL;
}


// functions used to build huffman encode tree
void build_huff_encode_tree(const char* str_arr, const double* freq_arr, size_t len, HEncodeNode** q_head) {
	size_t i = 0;
	HEncodeNode* left = NULL;
	HEncodeNode* right = NULL;
	HEncodeNode* parent = NULL;
	// insert all elements into this pqueue
	// here in this step, we build leaf nodes
	for (i = 0; i < len; i++) {
		insert_huff_pqueue(
			create_huff_encode_node(str_arr[i], freq_arr[i], 1),
			q_head);
	}
	// pop up len-1 elements and build nodes from bottom to top
	for (i = 0; i < len - 1; i++) {
		left = pop_huff_pqueue(q_head);
		right = pop_huff_pqueue(q_head);
		parent = create_huff_encode_node(0, left->freq + right->freq, 0);
		parent->left = left;
		parent->right = right;
		left->parent = parent;
		right->parent = parent;
		insert_huff_pqueue(parent, q_head);
	}
}


// functions used to buid huffman encode tree with freq array only
// however the length of freq array should be fixed as 256
// freq_arr is innitialed by 0.0f
void build_huff_encode_tree256(double* freq_arr, size_t len, HEncodeNode** q_head) {
	assert(len == 256 && 256 <= HUFF_MAX_SYMBOLS);
	size_t i = 0;
	HEncodeNode* left = NULL;
	HEncodeNode* right = NULL;
	HEncodeNode* parent = NULL;
	// insert all elements into this pqueue
	// here in this step, we build leaf nodes
	size_t valid_char_num = 0;
	for (i = 0; i < len; i++) {
		if (freq_arr[i] > 0.0f) { // ignore the non-existing nodes 
			insert_huff_pqueue(
				create_huff_encode_node((char)i, freq_arr[i], 1),
				q_head);
			valid_char_num++;
		}
	}
	// pop up valid_char_num-1 elements and build nodes from bottom to top
	for (i = 0; i < valid_char_num - 1; i++) {
		left = pop_huff_pqueue(q_head);
		right = pop_huff_pqueue(q_head);
		parent = create_huff_encode_node(0, left->freq + right->freq, 0);
		parent->left = left;
		parent->right = right;
		left->parent = parent;
		right->parent = parent;
		insert_huff_pqueue(parent, q_head);
	}
}


// functions used to write encoded file when reading from original file
void write_huff_encode_stream_from_file(FILE* f_in, FILE* f_out, char* codebook) {
	char c;
	while (!feof(f_in)) {
		// fscanf_s(f_in, "%c", &c, 1);
		c = getc(f_in);
		if(c == EOF) break;
		fprintf(f_out, "%s", codebook + (size_t)c * HUFF_MAX_LEN);
	}
}

// function used to count ASCII characters 
void huff_count_char(double* freq_arr, FILE* f_in, size_t len) {
	assert(len == 256 && len <= HUFF_MAX_SYMBOLS);
	char c;
	double s = 0.0f;
	while ((c=getc(f_in)) != EOF) {
		freq_arr[(size_t)c] += 1.0f;
		s += 1.0f;
	}
	size_t i = 0;
	for (i = 0; i < len; i++) {
		if (freq_arr[i] > 0.0f) {
			freq_arr[i] = freq_arr[i] / s;
		}
	}
}


// interface to encode a file 
int huff_encode_ascii_file(const char* filename, const char* codebook_filename, const char* encoded_filename){
	FILE* f_in = fopen(filename, "r");
	if(f_in == NULL){
		printf("cannot open %s\n.exit.\n", filename);
		return -1;
	}
	int i = 0;
	double freq_arr[HUFF_MAX_SYMBOLS];
	for (i = 0; i < HUFF_MAX_SYMBOLS; i++) {
		freq_arr[i] = 0.0f;
	}
	huff_count_char(freq_arr, f_in, HUFF_MAX_SYMBOLS);
	fclose(f_in);
	f_in = NULL;
    
	HEncodeNode* q_head = NULL;
	HEncodeNode* root_encode = NULL;
	
	char codebook[HUFF_MAX_SYMBOLS][HUFF_MAX_LEN];
	memset(codebook, 0, sizeof(codebook));
	build_huff_encode_tree256(freq_arr, HUFF_MAX_SYMBOLS, &q_head);
	root_encode = pop_huff_pqueue(&q_head);
	generate_huff_codebook(root_encode, 0, &codebook[0][0]);
	
	FILE* f_out = NULL;
	// fopen_s(&f_out, "codebook.txt", "w");
	f_out = fopen(codebook_filename, "w");
	if(f_out == NULL){
		printf("cannot open %s\n.exit.\n", codebook_filename);
		return -1;
	}
	write_huff_codebook(f_out, &codebook[0][0]);
	fclose(f_out);

	// fopen_s(&f_in, "book.txt", "r");
	// fopen_s(&f_out, "encoded.txt", "w");
	f_in = fopen(filename, "r");
	if(f_in == NULL){
		printf("cannot open %s\n.exit.\n", filename);
		return -1;
	}

	f_out = fopen(encoded_filename, "w");
	if(f_out == NULL){
		printf("cannot open %s\n.exit.\n", encoded_filename);
		return -1;
	}
	write_huff_encode_stream_from_file(f_in, f_out, &codebook[0][0]);
	fclose(f_in);
	fclose(f_out);
	free_huff_encode_tree(root_encode);
	return 0;
}


