
# Producer-Consumer Application with Interprocess Communication

This project demonstrates a producer-consumer application in C using interprocess communication with `fork()`, `pipe()`, and `exec()` system calls. It converts lowercase characters in input files to uppercase and employs a layered architecture with distinct physical, data link, and application layers.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Instructions to execute](#instructions-to-execute)
- [File Descriptions](#file-descriptions)

---

## Overview

This application implements a producer-consumer model where:
1. The **Producer** encodes input data, adds a parity bit, frames the data, and sends it through a pipe to the **Consumer**.
2. The **Consumer** decodes, converts lowercase to uppercase, re-encodes, and sends the result back to the **Producer**.
3.  The **Producer** decodes the received data and writes it to the output file.

The system follows the ASCII standard with odd parity and uses temporary files for debugging and results.

---

## Key Features

- Encodes and decodes data with parity bit and framing.
- Converts lowercase letters to uppercase.
- Modular and layered design. It has distinct physical, data link, and application layers
- Uses pipes for interprocess communication.

## System Architecture

The application follows a layered architecture, with each layer handling specific responsibilities:

### Physical Layer
- Converts characters to binary bit patterns and vice versa.
- Manages parity bit addition, verification, and removal.

### Data Link Layer
- Frames data by adding control characters (`SYN`, `LENGTH`, etc.).
- Deframes received data to extract meaningful content.

### Application Layer
- Reads input data files (`samplefile.inpf`) in the producer.
- Writes processed data to output files (`samplefile.outf` and `samplefile.done`) in the consumer.

---

## Producer Functions

1. **Read Input Data**:
   - Reads ASCII characters from input files with the `.inpf` extension.

2. **Encode Data**:
   - Converts each ASCII character into a 7-bit binary string.
   - Adds a parity bit (odd parity) to each character to ensure data integrity.

3. **Frame Data**:
   - Groups the encoded data into frames:
     - 2 `SYN` characters (`ASCII 22`).
     - 1 control character for length indication.
     - Up to 32 data characters per frame (padded if necessary).

4. **Transmit Frames**:
   - Writes the encoded frames to a pipe to be transmitted to the Consumer.

5. **Receive Processed Data**:
   - Reads processed frames from a second pipe shared with the Consumer.
   - Deframes the data, removes parity bits, and decodes it back into ASCII.

6. **Write Final Output**:
   - Writes the final decoded data to an output file (`.done`).

---

## Consumer Functions

1. **Receive Frames**:
   - Reads encoded frames sent by the Producer through the pipe.

2. **Deframe Data**:
   - Removes control characters (SYN, length) to retrieve the actual data.

3. **Check and Remove Parity**:
   - Verifies and removes the parity bit for each character.

4. **Decode Data**:
   - Converts binary data back into ASCII characters.

5. **Convert to Uppercase**:
   - Converts all lowercase letters in the data to uppercase.

6. **Re-encode Data**:
   - Re-encodes the modified data (with uppercase characters) into binary.
   - Adds a parity bit and frames the data.

7. **Transmit Processed Data**:
   - Sends the re-encoded frames back to the Producer via another pipe.

---
## Instructions to execute:

1. Clone this repository:
   ```bash
   git clone https://github.com/CharanKasala/Producer_Consumer_Application.git
   cd <repository-folder>
   
2. Make sure that all the below mentioned filesare in the same folder (or) directory.
   
         1) encDec.h
         2) generatePrefix.c
         3) convertToUpper.c
         4) encodeService.c
         5) decodeService.c
         6) ProducerConsumer.c
         7) Samplefile.inpf

3. Create necessary executable files using the following commands :

         gcc generatePrefix.c -o generatePrefix

         gcc convertToUpper.c -o convertToUpper

         gcc encodeService.c -o encodeService

         gcc decodeService.c -o decodeService

         gcc ProducerConsumer.c -o ProducerConsumer
   
4. Run the program:
   
         ./ProducerConsumer samplefile.inpf

5. After execution, the following files will be generated:

         samplefile.binf
   
         samplefile.outf
   
         samplefile.chck
   
         samplefile.done

   samplefile.done contains the Upper

## File Descriptions:

- **Input File**:

  - **`samplefile.inpf`**: Contains the original ASCII data to process.
   
- **Temporary Files**:
 
  - **`samplefile.binf`**: Encoded binary data with parity.
     
  - **`samplefile.outf`**: Data with lowercase converted to uppercase.
    
  - **`samplefile.chck`**: Re-encoded output from the consumer.

- **Output File**:
 
  - **`samplefile.done`**: Final decoded and verified data.
