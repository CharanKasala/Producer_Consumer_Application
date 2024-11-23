
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
	dec_data(argv[1],argv[2]);
}
char* chk_remove_parity(char* binary_string, int n) 
{
    int i, j, k;
    int len = strlen(binary_string);
    // Check if the length is valid
    if (len != 8) {
        printf("Invalid input length\n");
        return NULL;
    }

    // Copy the binary string to result and remove parity bits
    char* result = malloc(8);
    for (i = 0, k = 0; i < len; i += 8, k += 7) 
    {
        // Check parity
        int parity = 0;
        for (j = i; j < i + 7; j++) 
        {
            if (binary_string[j] == '1') 
            {
                parity++;
            }
        }
        if (binary_string[i+7] == '1' && parity % 2 == 1) 
        {
            printf("Parity bit is incorrect\n");
            free(result);
            return NULL;
        } 
        else if (binary_string[i+7] == '0' && parity % 2 == 0) 
        {
            printf("Parity bit is incorrect\n");
            free(result);
            return NULL;
        }
        
        // Copy the 7-bit string without the parity bit
        for (j = i+1; j < i + 8; j++)
        {
            result[k + j - i - 1] = binary_string[j];
        }
    }
    result[k] = '\0';

    return result;
}

char* binary_to_ascii(char* binary_string) 
{
    // Check if the length is valid
    int len = strlen(binary_string);
    if (len % 7 != 0) 
    {
        printf("Invalid input length\n");
        return NULL;
    }

    // Allocate memory for the ASCII string
    char* ascii_string = malloc(len / 7 + 1);
    if (ascii_string == NULL) 
    {
        printf("Memory allocation error\n");
        return NULL;
    }

    // Convert binary codes to ASCII characters
    int i;
    for (i = 0; i < len; i += 7) 
    {
        char binary_code[8];
        strncpy(binary_code, binary_string + i, 7);
        binary_code[7] = '\0';
        ascii_string[i / 7] = strtol(binary_code, NULL, 2);
    }
    ascii_string[i / 7] = '\0';

    return ascii_string;
}

int dec_data(char* input_filename, char* output_filename) 
{
    FILE* input_file = fopen(input_filename, "r+");
     if (input_file == NULL) 
        {
        printf("Error opening file %s !\n",input_filename);
        return 1;
    	}
    FILE* output_file = fopen(output_filename, "w+");
     if (input_file == NULL) 
        {
        printf("Error opening file %s!\n",output_filename);
        return 1;
    	}

 
     // Determining the size of file
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    
    char *file_contents = (char *) malloc(file_size + 1);
    if (file_contents == NULL)
     {
        printf("Error allocating memory\n");
        fclose(input_file);
        return 1;
    }

    // Read the entire file into memory
    fread(file_contents, file_size, 1, input_file);

    // Add a null terminator to the end of the string
    file_contents[file_size] = '\0';

   
   int num_strings=0,b;
   //char *result_rm_pr;
   char binary_string[9]={0};
   int par_len1=0;
   par_len1=strlen(file_contents);
   num_strings = par_len1 / 8;		  // Determine the number of 8-bit strings in the input
   

   char *output_rem_par = malloc((num_strings * 8) + 1);
   output_rem_par[0] = '\0';
    for (b = 0; b < num_strings; b++) 
    {
        strncpy(binary_string, file_contents + (b * 8), 8);  // Copy the current 8-bit string to binary_string
        char *result_rm_pr = chk_remove_parity(binary_string,par_len1);		  // Remove the parity bit from binary_string
        // If result is not NULL, append it to the output string
        if (result_rm_pr != NULL)
        {
            strcat(output_rem_par, result_rm_pr);
            free(result_rm_pr);
        }
    }
    
    int len_rm_parity=0;
    char* ascii_string;
    
    len_rm_parity=strlen(output_rem_par);
    ascii_string = binary_to_ascii(output_rem_par);
    
    free(output_rem_par);
    output_rem_par=NULL;
    
    if (ascii_string != NULL) 
    {
        fprintf(output_file, "%s", ascii_string);
        free(ascii_string);
    }
    free(file_contents);
    fclose(input_file);
    fclose(output_file);
}

