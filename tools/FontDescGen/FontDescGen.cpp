// FontDescGen.cpp
//

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>

/*! Main.  Lovely, no?
 */
int main(int argc, char *argv[])
{
ifstream inFile;
FILE *outFile;
short numChars, charWidth, charHeight;
int wrt, readChars = 0, chunkLen;

// Print a welcome message:
cout << "FontDescGen - A little util to build font descriptors" << endl << endl;

// Check command line parms:
if (argc < 2 || argc > 3)
	{
	cout << "Command line goes: " << endl << " FontDescGen (inFile) (outfile)" << endl;
	cout << "Where (inputfile) is the file to build from, and (outfile) is the" << endl;
	cout << "resultant descriptorfile.  See Readme.txt for more info." << endl;
    return 1;
	 }

// Attempt to open the files, bomb if we cant:
inFile.open(argv[1], ifstream::in);
if (!inFile.good())
    {
    cout << "Error! Couldn't open " << argv[1] << " for input!\n";
	goto QUIT;
     }
if ((outFile = fopen(argv[2], "wb")) == NULL)
    {
    cout << "Error! Couldn't open " << argv[2] << "for output!\n";
    goto QUIT;
     }

// Read the number of chars, and width and height:
inFile >> numChars >> charWidth >> charHeight;

// Check for valid data:
if (numChars < 0 || charWidth < 0 || charHeight < 0)
	{
    cout << "Error! numChars, charWidth, or charHeight not valid (i.e., < 0)!\n";
    goto QUIT;
     }


// Build the header for the output:
wrt = fwrite("INXRESRC", 1, 8, outFile); if (wrt != 8) { cout << "Failed on write (1)\n"; goto QUIT; };
wrt = fwrite("INXFont\0", 1, 8, outFile); if (wrt != 8) { cout << "Failed on write (2)\n"; goto QUIT; };
wrt = fwrite("   ", 4, 1, outFile); if (wrt != 1) { cout << "Failed on write (3)\n"; goto QUIT; };	// Write a dummy ChunkLen
wrt = fwrite("\0", 1, 1, outFile); if (wrt != 1) { cout << "Failed on write (3.5)\n"; goto QUIT; };	// No resource id string
wrt = fwrite(&numChars, 2, 1, outFile); if (wrt != 1) { cout << "Failed on write (4)\n"; goto QUIT; };
wrt = fwrite(&charWidth, 2, 1, outFile); if (wrt != 1) { cout << "Failed on write (5)\n"; goto QUIT; };
wrt = fwrite(&charHeight, 2, 1, outFile); if (wrt != 1) { cout << "Failed on write (6)\n"; goto QUIT; };

// Loop while we have input:
readChars = 0;
while (1)
	{
	// Get the char and output to the descriptor:
	char inChar;
	inFile >> inChar;
	if (!inFile.good()) break;
	readChars ++;

	// Output the data to the descriptor:
	wrt = fwrite(&inChar, 1, 1, outFile); if (wrt != 1) { cout << "Failed on write (7)\n"; goto QUIT; };
	 }

// Go back and write in the chunk length:
fseek(outFile, 16, SEEK_SET);
chunkLen = readChars + 6;  // 6 bytes for numChars, width, and height, plus actual data (1 char/mapping)
wrt = fwrite(&chunkLen, 4, 1, outFile); if (wrt != 1) { cout << "Failed on write (8)\n"; goto QUIT; };

QUIT:

// Close the files:
inFile.close();
fclose(outFile);

// Check if we have a mismatch between specified number of chars, and what we read.  Display
// a warning if so:
if (numChars != readChars)
	cout << "Warning: Number of character mappings in input file does not agree with" << endl
		 << "number of characters specified in header; " << readChars << " read versus " << numChars << " specified!" << endl;

return 1;
 }
