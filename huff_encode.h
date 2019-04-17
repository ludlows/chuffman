/*
codes for huffman encode tree
www.github.com/ludlows
2019-04
*/
#ifndef HUFF_ENCODE_H
#define HUFF_ENCODE_H

#include "huff_const.h"


struct huff_encode_node {
	struct huff_encode_node* left;   // used in encode tree ; '0'
	struct huff_encode_node* right;  // used in encode tree ; '1'
	struct huff_encode_node* parent; // used in encode tree
	struct huff_encode_node* next;   // used in priority queue
	double freq;                     // frequency of symbol in file
	int    is_leaf;                  // if the node is a leaf node in encode tree
	char   symbol;                   // the symbol assigned to this node (char)
};

// claim the type of huffman encode node
typedef struct huff_encode_node HEncodeNode;

// allocate the memory for huffman encode node
HEncodeNode* create_huff_encode_node(char symbol, double freq, int is_leaf);

// function used to insert a node into priority queue
void insert_huff_pqueue(HEncodeNode* node, HEncodeNode** q_head);


// function used to display the priority queue
void disp_huff_pqueue(HEncodeNode* q_head);


// functions used to pop element from pqueue
HEncodeNode* pop_huff_pqueue(HEncodeNode** q_head);

// functions used to generate codebook recursively
// codebook: char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer 
void generate_huff_codebook(HEncodeNode* root, int depth, char* codebook);

// functions used to write codebook into file
// codebook:  char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer 
void write_huff_codebook(FILE* f_out, char* codebook);

// functions used to write encoded bit stream into file
// codebook: char [HUFF_MAX_SYMBOLS][HUFF_MAX_LEN]
// here we use 1st order pointer like &codebook[0][0]
void write_huff_encode_stream(FILE* f_out, char* str, char* codebook);

// functions used to free the memory of huffman encode tree.
// this function is implemented post-order traversal on recursive way.
void free_huff_encode_tree(HEncodeNode* root);

// functions used to build huffman encode tree
void build_huff_encode_tree(const char* str_arr, const double* freq_arr, size_t len, HEncodeNode** q_head);

// functions used to buid huffman encode tree with freq array only
// however the length of freq array should be fixed as 256
void build_huff_encode_tree256(double* freq_arr, size_t len, HEncodeNode** q_head);

// functions used to write encoded file when reading from original file
void write_huff_encode_stream_from_file(FILE* f_in, FILE* f_out, char* codebook);

// interface to encode a file 
int huff_encode_ascii_file(const char* filename, const char* codebook_filename, const char* encoded_filename);

// function used to count ASCII characters 
void huff_count_char(double* freq_arr, FILE* f_in, size_t len);

#endif // !HUFF_ENCODE_H

