# chuffman

huffman coding implemented in C for educational purpose from scratch.

Using Encoding Tree and Decoding Tree to achieve information Compression and Extraction.

The code works well with ASCII texts. 

# usage

$gcc *.c -o chuffman -Wall

$./chuffman

## for huffman encoding

$./chuffman -e book.txt -b codebook.txt -o encoded.txt

you can also use 

$./chuffman -e book.txt

but the default filename of codebook is codebook.txt

the default filename of encoded file is output.txt

## for huffman decoding

$./chuffman -d encoded.txt -b codebook.txt -o origin.txt

you can also use

$./chuffman -d encoded.txt

but the default filename of codebook is codebook.txt

the default filename of decoded file is output.txt

# Example

here is an example about compiling the code and using it to encode and decode file 'example/book.txt'.

[![1:39](http://img.youtube.com/vi/7GttPQ7yFuw/0.jpg)](http://www.youtube.com/watch?v=7GttPQ7yFuw "Huffman")
