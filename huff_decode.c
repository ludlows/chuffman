/*
codes for huffman decode tree
www.github.com/ludlows
2019-04
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "huff_decode.h"

// function to create huffman decode node
HDecodeNode* create_huff_decode_node() {
	HDecodeNode* p_node = (HDecodeNode*)malloc(sizeof(HDecodeNode));
	if (p_node != NULL) {
		p_node->left = NULL;
		p_node->right = NULL;
		p_node->symbol = 0;
		p_node->is_leaf = 0;
	}
	return p_node;
}


// function to free the memory allocated to the decode tree
void free_huff_decode_tree(HDecodeNode* node) {
	if (node == NULL) return;
	free_huff_decode_tree(node->left);
	free_huff_decode_tree(node->right);
	free(node);
	node = NULL;
}


//function to build huffman decode tree based on codebook.
// the format of codebook should be like:
// a 10000
// b 10001
// ....
// for each symbol, it creates nodes from the top to the bottom
void build_huff_decode_tree(FILE* fp, HDecodeNode* root_decode) {
	char symbol=0;
	char strcode[HUFF_MAX_LEN];
	int idx=0, length=0;
	int num_input=0;
	HDecodeNode* curr_node = NULL;
	while (!feof(fp)) {
		memset(strcode, 0, sizeof(strcode));
		symbol = 0;
		// num_input = fscanf_s(fp, "#%c %s\n", &symbol, 1, strcode, sizeof(strcode));
		num_input = fscanf(fp, "#%c %s\n", &symbol, strcode);
		if (num_input != 2) {
			break;
		}
		printf("reading:(%c,%s)successfully\n", symbol, strcode);
		// point curr_node to the root
		curr_node = root_decode;
		length = strlen(strcode);
		printf("length=%d\n", length);
		// create nodes one by one
		for (idx = 0; idx < length; idx++) {
			if (strcode[idx] == '0') {
				if (curr_node->left == NULL) {
					curr_node->left = create_huff_decode_node();
				}
				curr_node = curr_node->left;
			}
			else if(strcode[idx] == '1') { // strcode[idx] == '1'
				if (curr_node->right == NULL) {
					curr_node->right = create_huff_decode_node();
				}
				curr_node = curr_node->right;
			}
			else {
				printf("unexpected char %c\n", strcode[idx]);
				assert(0);
			}
		}

		assert(curr_node->is_leaf == 0);
		// at last assign the symbol to the leaf node
		curr_node->is_leaf = 1;
		curr_node->symbol = symbol;
		printf("successfully inserted symbol:(%c,%s)\n", symbol, strcode);
	}
}


// function to decode
// f_in is the file pointer to encoded file
// f_out is the file pointer to write decoded message
void huff_decode(FILE* f_in, FILE* f_out, HDecodeNode* root_decode) {
	char c = 0;
	// int num_read = 0;
	// assign the current node to root of decode tree
	HDecodeNode* curr_node = root_decode;
	while (!feof(f_in)) { // updating c bit by bit (mimic)
		// num_read = fscanf_s(f_in, "%c", &c, 1);
		c = getc(f_in);
		if(c == EOF) break;
		if (c == '0') {
			curr_node = curr_node->left;
		}
		else if (c == '1') {  // c == '1'
			curr_node = curr_node->right;
		}
		else {
			printf("\nchar(%c)rather than 1 or 0 appears\n", c);
			assert(0); // raise error;
		}
		if (curr_node->is_leaf) {
			//printf("%c", curr_node->symbol);
			fprintf(f_out, "%c", curr_node->symbol);
			// need to assign the curr_node to root_decode to start over
			curr_node = root_decode;
		}
	}
}

// interface to decode a file 
int huff_decode_ascii_file(const char* filename, const char* codebook_filename, const char* decoded_filename){
	// decode
	// get the root to decode
	FILE* f_in = NULL;
	FILE* f_out = NULL;
	
	// fopen_s(&f_in, "codebook.txt", "r");
	f_in = fopen(codebook_filename, "r");
	if(f_in == NULL){
		printf("cannot open %s\n.exit.\n", codebook_filename);
		return -1;
	}

	HDecodeNode* root_decode = create_huff_decode_node();
	build_huff_decode_tree(f_in, root_decode);

	fclose(f_in);
	f_in = NULL;
	

	// fopen_s(&f_in, "encoded.txt", "r");
	f_in = fopen(filename, "r");
	if(f_in == NULL){
		printf("cannot open %s\n.exit.\n", filename);
		return -1;
	}
	
	// fopen_s(&f_out, "decoded.txt", "w");
	f_out = fopen(decoded_filename, "w");
	if(f_out == NULL){
		printf("cannot open %s\n.exit.\n", decoded_filename);
		return -1;
	}
	huff_decode(f_in, f_out, root_decode);
	fclose(f_in);
	f_in = NULL;
	fclose(f_out);
	f_out = NULL;

	// clean memory on the heap
	free_huff_decode_tree(root_decode);
	return 0;
}