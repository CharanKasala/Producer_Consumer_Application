#ifndef ENCDEC_H
#define ENCDEC_H

#define BUFFER_SIZE 20000
#define SYN_CHAR 22
#define BLOCK_LEN 4

char *char_to_binary7(char c); /*function to convert character to 7 bit binary code*/

char *char_to_binary8(char c); /*function to convert character to 8 bit binary code*/

char* generate_prefix();       /*function to generate header for frame based on SYN CHAR and block length*/

char* add_parity_bit(char binary[BUFFER_SIZE]); /* function to add parity bit to binary data */

int enc_data(char* input_filename, char* output_filename); /* function to encode data (binary and parity addition)*/ 

void transfer_data(int fd, char* data, int len);  /*function to transfer data i.e, frames over pipe */

char* frame_creation(char *input, int remaining_len, char *prefix); /*function for creating of frames by adding header to data*/

int generate_frames(char* input_filename,int pipefd[2]) ; /*function for generation of frames*/

void receive_data(int fd, char* data, int len) ; /*function to receive data i.e, frames over the pipe */

char* frame_removal(char *input, char *prefix) ; /*function for remove header from frames transmitted */

int deFrame(int pipefd[2], char* output_filename); /*function for receiving frames over the pipe and processing received frames */

char* chk_remove_parity(char* binary_string, int n); /* function to check and remove the parity bits*/

char* binary_to_ascii(char* binary_string); /* function for converting binary to ASCII characters*/

int dec_data(char* input_filename, char* output_filename); /* function for decoding data (removal of parity and conversion to ascii)*/

int convert_toupper(char* filename); /* function to convert lower case to upper case */


#endif /*ENCDEC_H*/

