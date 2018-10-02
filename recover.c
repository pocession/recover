//The program is for recovering JPEG files from forensic image

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    //store the file name
    char *infile = argv[1];

    // open input file and check if it's null
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //open output file for jpeg
    FILE *outptr = NULL;

    //initialize the counter
    int n = 0;

    //make a buffer to save the file name of JPEG read by this program
    char filename[8];

    //store bytes read from jpeg
    BYTE buffer[BLOCK_SIZE];

    while (true)
    {
        //read blocks from input; 512 bytes per block
        size_t byteread = fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr);

        //break the loop when reaching the end of file
        if (byteread == 0 && feof(inptr))
        {
            break;
        }

        //read header of this block and determine whehter it is a jpeg file
        //the first four bits of the fourth byte of a jpeg are always e, or 1110
        //the last four bits of the fourth byte of a jpeg are random, from 0~f, or from 0000 to 1111
        //By taking the first four bits of the fourth byte and use bitwise operator to confirm whether it is 1110
        //AND is bitwise operator => 1110 AND 1111 = 1110 = 0xe0
        bool jpeg_fileheader = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        //close the previous file if a jpeg is found
        if (jpeg_fileheader && outptr != NULL)
        {
            fclose(outptr);
            n++;
        }

        if (jpeg_fileheader)
        {
            //output a file with name of 3-digit number, starting from 000.jpg
            sprintf(filename, "%03i.jpg", n);
            outptr = fopen(filename, "w");
        }
        if (outptr != NULL)
        {
            fwrite(buffer, sizeof(BYTE), byteread, outptr);
        }

    }
    // close the input file
    fclose(inptr);
    // close the output file
    fclose(outptr);
    return 0;
}
