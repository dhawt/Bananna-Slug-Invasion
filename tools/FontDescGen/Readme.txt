FontDescGen - A little proggie to build font descriptorfiles from textfiles.

Command line goes FondDescGen (infile) (outfile)

(infile) is the name of the text file containing cell->ascii character 
mappings.  Inputfile format consists of three integers in the beginning,
describing the number of characters in the font, followed by the width and 
height of each cell in the bitmap respectively.  Then, a series of lines with 
one character to a line, with the character on each line corresponding to the 
cell in the bitmap.  (See InputExample.txt for a font that contains the 
hexidecimal characters)

(outfile) is the file to output the resultant descriptorfile.  Output file
is in format consistant to INX Font Resourcefile format specification revision
3/28/02.

