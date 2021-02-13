#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#define BLOCKSIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // checking to make sure user placed two words at the cmd-line
    if (argc != 2)
    {
        printf("Expected one cmd-line arg");
        return 1;
    }

    // opening the memory file which contains the JPEGs
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("File could not be opened or found.");
        return 1;
    }

    // initializing variables
    BYTE bytes[BLOCKSIZE]; // this array holds the memory in 512 blocks
    int counter = 0; // counts the number of JPEG files written
    char filename[8]; // name of the file in the format ###.jpg

    // this file will hold the jpeg file
    FILE *img;

    // this is for the else statement, in order to make sure empty blocks don't get accidentally read upon
    bool first_jpeg = false;

    // loop that works until file cannot be read further
    while (fread(bytes, BLOCKSIZE, 1, file)) // reading 512 blocks of 1 byte each of memory from the file into the bytes array
    {
        // reading the file into an array of 512 bytes
        //fread(bytes, BLOCKSIZE, 1, file);

        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            // first three bytes have to be 0xff 0xd8 0xff to be considered new jpeg
            if (counter != 0)
            {
                fclose(img);
            }

            // moving name to string variable
            sprintf(filename, "%03i.jpg", counter);
            counter++;

            // writing the information in image to the file
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("File could not be opened or found.");
                return 1;
            }

            fwrite(bytes, BLOCKSIZE, 1, img);
        }
        else if (first_jpeg == true)
        {
            fwrite(bytes, BLOCKSIZE, 1, img);
        }
        first_jpeg = true;
    }
    // close file and image
    fclose(file);
    fclose(img);
    return 0;
}
