#include <iostream>
#include "bits.h"
#include "console.h"
#include "filelib.h"
#include "huffman.h"
#include "simpio.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;


void huffmanConsoleProgram();

/*
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading.
 */

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    /*
     * In order to run the console program to compress/decompress whole files
     * change the above flag to NO_TESTS, which will cause the console program
     * to be invoked via the fuction call below.
     */
    huffmanConsoleProgram();

    cout << "All done, exiting" << endl;
    return 0;
}


/*
 * Sets up the output console and explains the program to the user.
 */
void intro() {
    cout << "Welcome to CS106B Shrink-It!" << endl;
    cout << "This program uses the Huffman coding algorithm for compression." << endl;
    cout << "Any type of file can be encoded using a Huffman code." << endl;
    cout << "Decompressing the result will faithfully reproduce the original." << endl;
}

/*
 * Prints a menu of choices for the user and reads/returns the user's response.
 */
string menu() {
    cout << endl;
    cout << "Your options are:" << endl;
    cout << "C) compress file" << endl;
    cout << "D) decompress file" << endl;
    cout << "Q) quit" << endl;

    cout << endl;
    return toUpperCase(trim(getLine("Enter your choice: ")));
}

const string kCompressedExtension = ".huf";
const string kDecompressedExtension = "unhuf.";

/*
 * Prompts for names of files to use for compress/decompress.
 */
bool getInputAndOutputFiles(string& inFilename, string& outFilename, bool compressing)
{
    inFilename = promptUserForFile("Input file name: ");
    string defaultName;
    if (compressing) {
        defaultName = inFilename + kCompressedExtension;
    } else {
        defaultName = kDecompressedExtension + getRoot(inFilename);
    }
    outFilename = trim(getLine("Output file name (Enter for " + defaultName + "): "));
    if (outFilename == "") {
        outFilename = defaultName;
    }
    if (inFilename == outFilename) {
        cout << "You cannot specify the same filename as both the input file" << endl;
        cout << "and the output file.  Canceling operation." << endl;
        return false;
    }
    if (fileExists(outFilename)) {
        return getYesOrNo(outFilename + " already exists. Overwrite? (y/n) ");
    }
    return true;
}

/*
 * Compress a file.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your compress function and displays information about size of
 * compressed output.
 */
void compressFile() {
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, true)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        string text = readEntireFile(inFilename);
        cout << "Compressing ..." << endl;
        EncodedData data = compress(text);
        ofstream out(outFilename, ios::binary);
        writeData(data, out);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " compressed bytes." << endl;
    } else {
        cout << "Compressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Decompress a file.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your decompress function and displays information about size of
 * decompressed output.
 */
void decompressFile() {
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, false)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        ifstream input(inFilename, ios::binary);
        EncodedData data = readData(input);
        cout << "Decompressing ..." << endl;
        string text = decompress(data);
        writeEntireFile(outFilename, text);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " decompressed bytes." << endl;
    } else {
        cout << "Decompressed output file was not found; perhaps there was an error." << endl;
    }
}

void huffmanConsoleProgram() {
    intro();
    while (true) {
        string choice = menu();
        if (choice == "Q") {
            break;
        } else if (choice == "C") {
            compressFile();
        } else if (choice == "D") {
            decompressFile();
        }
    }
}


// Do not remove or edit this test. It is here to to confirm that your code
// conforms to the expected function prototypes need for grading
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        string str;
        EncodingTreeNode *t;
        Queue<Bit> qb;
        Queue<char> qc;
        EncodedData data;

        t = buildHuffmanTree(str);
        deallocateTree(t);
        str = decodeText(t, qb);
        qb = encodeText(t, str);
        flattenTree(t, qb, qc);
        t = unflattenTree(qb, qc);
        data = compress(str);
        str =  decompress(data);
    }
}
