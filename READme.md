MyZip- my c++ take on a text compressor.

Objective:
-compress text
-make a faster program than my old zipper made in python
-explore the concept and the usage of data structures like huffman trees.
-make a small project that accualy has a usecase in daily life(althoug not better than any opensource project already outthere in the market or already installed in the OS)

fuction explanaition:
    HUFF.h: header file reponsable to creat the auxiliar structs and functions to build the huffman tree
    -struct Node: structure that builds all the nodes if left and right are null its bc is a leaf
    -Node* createNode: simple constructor to fill the node data
    -struct Compare: structure used to teach the priority queue to operate with the Node structure
    -Node* BuildTree: Simple function to build the HUFFman tree and returns the root node
    myzipper.cpp: main file with all the functions to read, build the huffman codes, compress and decompress files
    -std::map<char, int> reader: function used to read the content of a file and return the amount of times a certain char appeard inside that file
    -void genCods: recursive function that goes down the HUFFman tree and generate the codes for each char storing it in a new map
    -std::map<char,std::string> HUFFcodes: fuction used to organize the codes generated in the last function
    -void compress: fuction that reads the the original file and searchs the map with the huffman codes generated in the last function and writes that code in an 8 bit sequence therefor writing that sequence in the new zip file it also stores in the beggining of the zip file a header with the map so it can be decompressed later.
    -void decompress: function that reads the zip file and starts to reconstruct the original charfrequence map that is stored in the header of the zip file, aftere that it begins to form the chars acording to the huffman code and writing them in a new tct file.
    -int compOrDecomp:auxilar function to know if the file is to be compressed or decompressed(acording to the type of file it is)

Explenation of Comcepts;
    HUFFman Codes/trees: it uses a the amount of apearance of a certain simbol to to create a tree every leaf node is a char and its frequence in the file every non leaf node is filed with \0 and the sum between the frequence of the left and right node, by this logic the root node is filled with \0 in and the total amout of chars in the original file. to extract the codes from this tree all that is needed is to flow the tree until you find all the leaves, every time you go left you cout a 0 and every time you go right its a 1. this ensures that the more frequent a char is in the file the colsest to the root it is making it faster to compress and decompress the message.


How to compile:
    linux/WSL: make sure you have the g++ install (it should come pre-installed)
     open the terminal in the src paste.
     Ex: cd MyZipper/src
     after that compile the code using g++
     Ex: g++ myzipper.cpp -o myzip
     you can also use the makefile using make to compile 

How to Run:
    Start by puting the txt file that you want to compress or decompress inside the folder tahat has the ./myzip (file that resulted from compiling)
    after that run: ./myzip test-file.txt where test-file.txt is the name of your file, it will compress if its a txt or decompress if its a zip file.
    you can also use make run FILE=test-file.txt 

Efficiency: after trying to compress several 10Mb of that it compressed it to 5.1Mb every single time and the same from 20Mb to 10.2Mb i can say that it can compress up to 49%.

Next setps in the project:
    add to the top off the file what type of file it was before it was compressed so it can decompress it back to the type of file it was before
    add a way to compress other forms of media like images and audio