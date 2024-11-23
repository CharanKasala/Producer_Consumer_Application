#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include "encDec.h"

int main()
{
	generate_prefix(); 
}
char* generate_prefix()		//generate header for frames based on defined SYN CHARACTER AND BLOCK LENGTH
{
   char f_prefix[BUFFER_SIZE]={0};
   strcat(f_prefix,char_to_binary8(SYN_CHAR));
   strcat(f_prefix,char_to_binary8(SYN_CHAR));
   strcat(f_prefix,char_to_binary8(BLOCK_LEN));
      
   FILE* buffer_file_name = fopen("Header-Frame.binf", "wb+");
   if (buffer_file_name == NULL) 
   {
    printf("Error opening Header-Frame.binf\n");
   }
   
   fwrite(f_prefix, sizeof(int),strlen(f_prefix), buffer_file_name);
   fclose(buffer_file_name);
}
char *char_to_binary8(char c)
{
    char *binary = malloc(8 * sizeof(char)); // allocate memory for binary string
    int i;
    for (i = 7; i >= 0; --i)
    {
        binary[7-i] = ((c >> i) & 1) + '0'; // convert binary digit to character
    }
    binary[8] = '\0'; // add null terminator to binary string
    return binary;
}

