#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include "encDec.h"

void transfer_data(int fd, char* data, int len) 
{
    int bytes_written = 0;
    while (bytes_written < len) 
    {
        int result = write(fd, data + bytes_written, len - bytes_written);
        if (result == -1) 
        {
            fprintf(stderr, "Error writing to pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        bytes_written += result;
    }
}

char* frame_creation(char *input, int remaining_len, char *prefix) 
{
    char *chunk = malloc(remaining_len + strlen(prefix) + 1);
    if (!chunk) 
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    memset(chunk, 0, remaining_len + strlen(prefix) + 1);

    strncpy(chunk, prefix, strlen(prefix));

    int input_pos = 0, chunk_pos = strlen(prefix);
    while (input_pos < remaining_len) {
        if (chunk_pos == remaining_len + strlen(prefix)) 
        {
            chunk[chunk_pos] = '\0';
            chunk_pos = strlen(prefix);
        }
        chunk[chunk_pos++] = input[input_pos++];
    }
    chunk[chunk_pos] = '\0';

    return chunk;
}
int generate_frames(char* input_filename,int pipefd[2]) 
{
     printf("Framing started \n");
     FILE* input_file = fopen(input_filename, "r+");
     if (input_file == NULL) 
        {
        printf("Error opening file %s!\n",input_filename);
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

 
   int input_len = strlen(file_contents);
   char f_input[input_len];
   memset(f_input, 0, input_len);
   //char f_prefix[100]={0};
   char *f_output;
   int chunk_size=BLOCK_LEN*8,a;
   int num_chunks = (input_len + chunk_size - 1) / chunk_size;
   strcpy(f_input, file_contents); 
    
   FILE* prefix_file = fopen("Header-Frame.binf", "rb+");
   if (prefix_file == NULL) 
   {
    printf("Error opening Header-Frame.binf !\n ");
    return 1;
   }
   
      // Determining the size of file
    fseek(prefix_file, 0, SEEK_END);
    long prefix_file_size = ftell(prefix_file);
    fseek(prefix_file, 0, SEEK_SET);
    
    char *f_prefix = (char *) malloc(prefix_file_size + 1);
    if (f_prefix == NULL) 
    {
        printf("Error allocating memory\n");
        fclose(prefix_file);
        return 1;
    }

    // Read the entire file into memory
    fread(f_prefix, prefix_file_size, 1, prefix_file);

    // Add a null terminator to the end of the string
    f_prefix[prefix_file_size] = '\0';
  
   f_output = (char*) malloc(num_chunks * (chunk_size + strlen(f_prefix)) + 1);
   
   if (!f_output) 
   {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    memset(f_output, 0, num_chunks * (chunk_size + strlen(f_prefix)) + 1);
   
    int output_pos = 0;
    for (a = 0; a < num_chunks; a++) 
    {
        char *chunk;
        if (a == num_chunks - 1) 
        {
            chunk = frame_creation(&f_input[a*chunk_size], input_len % chunk_size, f_prefix);
        } else {
            chunk = frame_creation(&f_input[a*chunk_size], chunk_size, f_prefix);
        }
        strcat(f_output, chunk);
        output_pos += strlen(chunk);
        free(chunk);
    }
    int total_len=0;
    total_len=strlen(f_output);
    
    transfer_data(pipefd[1], f_output, total_len);
    
   
    free(file_contents);
    file_contents=NULL;
    free(f_output);
    f_output=NULL;
    fclose(input_file);
    free(f_prefix);
    f_prefix=NULL;
    fclose(prefix_file);
    printf("Framing ended \n");
    
}

void receive_data(int fd, char* data, int len) 
{
    int bytes_read = 0;
    while (bytes_read < len) 
    {
        int result = read(fd, data + bytes_read, len - bytes_read);
        if (result == -1) 
        {
            fprintf(stderr, "Error reading from pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } 
        else if (result == 0) 
        {
            break;
        }
        bytes_read += result;
    }
}

char* frame_removal(char *input, char *prefix) 
{
    printf("Deframing started \n");
    char *output = malloc(strlen(input) + 1);
    if (!output) 
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    memset(output, 0, strlen(input) + 1);

    int input_pos = 0, prefix_pos = 0;
    int prefix_len = strlen(prefix);

    while (input_pos < strlen(input)) 
    {
        if (strncmp(&input[input_pos], prefix, prefix_len) == 0) 
        {
            input_pos += prefix_len;
        } 
        else 
        {
            output[prefix_pos++] = input[input_pos++];
        }
    }

    output[prefix_pos] = '\0';

    return output;
}

int deFrame(int pipefd[2], char* output_filename) 
{
    FILE* output_file = fopen(output_filename, "w+");
    char input_buffer[BUFFER_SIZE * 10];
    char *output_buffer;
    int input_len, i, j;
    
    FILE* prefix_file = fopen("Header-Frame.binf", "rb+");
     if (prefix_file == NULL) 
   {
    printf("Error opening Header-Frame.binf!\n ");
    return 1;
   }
   
 
     // Determining the size of file
    fseek(prefix_file, 0, SEEK_END);
    long prefix_file_size = ftell(prefix_file);
    fseek(prefix_file, 0, SEEK_SET);
    
    char *prefix = (char *) malloc(prefix_file_size + 1);
    if (prefix == NULL) 
    {
        printf("Error allocating memory\n");
        fclose(prefix_file);
        //return 1;
    }

    // Read the entire file into memory
    fread(prefix, prefix_file_size, 1, prefix_file);

    // Add a null terminator to the end of the string
    prefix[prefix_file_size] = '\0';

      
       receive_data(pipefd[0], input_buffer, BUFFER_SIZE * 10);
       output_buffer=frame_removal(input_buffer,prefix);
       fprintf(output_file, "%s", output_buffer);
       
         free(prefix);
         prefix=NULL;
         fclose(prefix_file);
         fclose(output_file);
         printf("Deframing ended \n");
}

int main(int argc, char *argv[])
{
    char *input_file_name;

    if (argc != 2)  // check if the command prompt has the correct number of arguments
    {
        printf("Please provide the input file name as an argument.\n");
        return 1;
    }
    input_file_name = argv[1];
    
    pid_t pid_j=fork();   
    if(pid_j<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_j==0)
    {
    	execl("generatePrefix", "generatePrefix", NULL );
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
      printf("prefix generation successfull \n");
    } 
    wait(NULL);
 
    int pro_to_con[2];
    int con_to_pro[2];
    pid_t pid1;
    
    if (pipe(pro_to_con) == -1) 
    {
        fprintf(stderr, "Error creating pipe: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    if (pipe(con_to_pro) == -1)
    {
        fprintf(stderr, "Error creating pipe: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    pid1 = fork();
    
    if (pid1 == -1) 
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if (pid1 == 0) // In Producer process
    { 
    close(pro_to_con[0]); // Close read end of producer to consumer pipe
     /* encoding service starts*/    
    pid_t pid_a=fork();   
    if(pid_a<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_a==0)
    {
    	execl("encodeService", "encodeService",input_file_name,"samplefile.binf", NULL );
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
      printf("Encoding service 1 successfull \n");
    } 
    wait(NULL);
    /* encoding service ends*/ 
    generate_frames("samplefile.binf", pro_to_con);
        
    close(pro_to_con[1]); // Close write end of producer to consumer pipe
    exit(EXIT_SUCCESS);
    }
    else if(pid1 >0)// In Consumer process
    {
    close(pro_to_con[1]); // Close write end of producer to consumer pipe

    deFrame(pro_to_con, "o_after_deframe.binf");
    
    /* decoding service starts */ 
    pid_t pid_d=fork();   
    if(pid_d<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_d==0)
    {
    	execl("decodeService", "decodeService","o_after_deframe.binf","samplefile.outf", NULL );
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
      printf("decoding service 1 successfull \n");
    } 
    wait(NULL);
    
    /* decoding service ends */ 
   
    pid_t pid_e=fork(); 
    if(pid_e<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_e==0)
    {
    	execl("convertToUpper", "convertToUpper", "samplefile.outf", NULL );
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
    } 
    wait(NULL);
    
     close(pro_to_con[0]); // Close read end of producer to consumer pipe
        
    pid_t pid2 = fork(); // Fork a new process to write the encoded binary data
    if (pid2 == -1)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if (pid2 == 0) // In the 2nd child process
    {
      	
    	close(con_to_pro[0]); // Close read end of consumer to producer pipe
        
     /* encoding service starts*/ 
    pid_t pid_a=fork();   
    if(pid_a<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_a==0)   // Write the encoded binary data to filename.chck
    {
    	execl("encodeService", "encodeService","samplefile.outf","samplefile.chck", NULL );  
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
      printf("Encoding service 2 successfull \n");
    } 
    wait(NULL);
     /* encoding service ends*/ 
   
    generate_frames("samplefile.chck", con_to_pro); //generating frames
        
    close(con_to_pro[1]); // Close write end of consumer to producer pipe
        
    exit(EXIT_SUCCESS);
    }
    else // In producer again
    {
    
    close(con_to_pro[1]); // Close write end of consumer to producer pipe
     
    deFrame(con_to_pro, "o_after_deframe2.binf");
    
    /* decoding service starts*/ 
    
    pid_t pid_i=fork();   
    if(pid_i<0)
    {
        fprintf(stderr, "Error creating fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else if(pid_i==0)
    {
    	execl("decodeService", "decodeService","o_after_deframe2.binf","samplefile.done", NULL );
    	exit(EXIT_SUCCESS);
    }
    else
    {
      wait(NULL);
      printf("decoding service 2 successfull \n");
    } 
    wait(NULL);
    /* decoding service ends */ 
    close(con_to_pro[0]); // Close read end of consumer to producer pipe
    exit(EXIT_SUCCESS);
   }
 }

    return 0;    
}
