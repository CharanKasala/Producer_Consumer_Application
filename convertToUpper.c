#include <stdio.h>
#include <ctype.h>
#include "encDec.h"
int main(int argc, char *argv[])
{
	convert_toupper(argv[1]);
}
int convert_toupper(char* filename) 
{
	char c;
	FILE* input_file = fopen(filename, "r+");
        if (input_file == NULL) 
        {
        printf("Error opening file! %s\n",filename);
        return 1;
    	}

    while ((c = fgetc(input_file)) != EOF) 
    {
        fseek(input_file, -1, SEEK_CUR);  // move file pointer back one position
        fputc(toupper(c), input_file);
    }

    fclose(input_file);
}
