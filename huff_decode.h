/*
codes for huffman decode tree
www.github.com/ludlows
2019-04
*/
#ifndef HUFF_DECODE_H
#define HUFF_DECODE_H

#include "huff_const.h"

// define the structure of huffman decode node
struct huff_decode_node {
	struct huff_decode_node* left;  // represents 0
	struct huff_decode_node* right; // represents 1
	int is_leaf;
	char symbol;
};

// a shor name of the struct above
typedef struct huff_decode_node HDecodeNode;


// function used to create huffman decode node
HDecodeNode* create_huff_decode_node();

// function used to free the memory allocated to the decode tree
void free_huff_decode_tree(HDecodeNode* node);


//function to build huffman decode tree based on codebook.
// the format of codebook should be like:
// a 10000
// b 10001
// ....
// for each symbol, it creates nodes from the top to the bottom
void build_huff_decode_tree(FILE* fp, HDecodeNode* root_decode);

// function to decode
// f_in is the file pointer to encoded file
// f_out is the file pointer to write decoded message
void huff_decode(FILE* f_in, FILE* f_out, HDecodeNode* root_decode);

// interface to decode a file 
int huff_decode_ascii_file(const char* filename, const char* codebook_filename, const char* decoded_filename);

#endif // !HUFF_DECODE_H

