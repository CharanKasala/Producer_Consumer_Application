#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include "encDec.h"
int main(int argc, char *argv[])
{
	enc_data(argv[1],argv[2]);
}

char *char_to_binary7(char c)
{
    char *binary = malloc(8 * sizeof(char)); // allocate memory for binary string
    int i;
    for (i = 6; i >= 0; --i)
    {
        binary[6-i] = ((c >> i) & 1) + '0'; // convert binary digit to character
    }
    binary[7] = '\0'; // add null terminator to binary string
    return binary;
}

char* add_parity_bit(char binary[BUFFER_SIZE])
{
    int i, j, count;
    char chunk[8];
    char* result = malloc(BUFFER_SIZE * sizeof(char));
    if (result == NULL) // check if allocation was successful
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    for (i = 0; i < strlen(binary); i += 7)
    {
        strncpy(chunk, binary+i, 7); // copy next 7 bits into chunk
        chunk[7] = '\0'; // add null terminator to chunk
        count = 0;
        for (j = 0; j < 7; ++j)
        {
            if (chunk[j] == '1') // count number of ones in binary string
            {
                ++count;
            }
        }
        if (count % 2 == 0) // add a zero parity bit if number of ones is even
        {
            memmove(chunk+1, chunk, 8); // shift string right by one
            chunk[0] = '1'; // set new MSB to 0
        }
        else // add a one parity bit if number of ones is odd
        {
            memmove(chunk+1, chunk, 8); // shift string right by one
            chunk[0] = '0'; // set new MSB to 1
        }
        strcat(result, chunk); // append modified chunk to result string
    }
    return result;
}

int enc_data(char* input_filename, char* output_filename)
{
    FILE* input_file = fopen(input_filename, "r+");
      if (input_file == NULL) 
      {
      printf("Failed to open file %s \n", input_filename);
      return 1;
      }
    FILE* output_file = fopen(output_filename, "w+");
     if (output_file == NULL) 
      {
      printf("Failed to open file %s \n", output_filename);
      return 1;
      }
    /*binary conversion begins*/
   int i, str_len;
   char buffer[BUFFER_SIZE];
   char *binary_buffer= malloc(BUFFER_SIZE * 8 * sizeof(char)); // allocate memory for binary buffer
   while (fgets(buffer, BUFFER_SIZE, input_file) != NULL) 
   {
      //buffer[strcspn(buffer, "\n")] = '\0'; // remove newline character from buffer
      str_len = strlen(buffer);
      for (i = 0; i < str_len; ++i) 
      {
         char c = buffer[i] & 0x7F;
         char *binary = char_to_binary7(c);
         strcat(binary_buffer, binary); // concatenate binary data to binary buffer
         free(binary); // free memory allocated by char_to_binary function
      }
   }
     int binary_len=strlen(binary_buffer);
      
     /*binary conversion begins*/
     
    /* parity addition begins*/
    
   char *binary_with_parity= malloc(strlen(binary_buffer) * 2 + 1);
   binary_with_parity = add_parity_bit(binary_buffer);
   int par_len = strlen(binary_with_parity);
      
   /* parity addition ends*/
   
    fprintf(output_file, "%s", binary_with_parity);
    free(binary_with_parity);
    binary_with_parity=NULL;
    free(binary_buffer);
    binary_buffer=NULL;
    fclose(input_file);
    fclose(output_file);
}
