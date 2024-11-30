
# Producer-Consumer Application with Interprocess Communication

This project demonstrates a producer-consumer application in C using interprocess communication with `fork()`, `pipe()`, and `exec()` system calls. It converts lowercase characters in input files to uppercase and employs a layered architecture with distinct physical, data link, and application layers.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Instructions to execute](#instructions-to-execute)
- [File Descriptions](#file-descriptions)

---

## Overview

This application implements a producer-consumer model where:
1. The **Producer** encodes input data, adds a parity bit, frames the data, and sends it through a pipe to the **Consumer**.
2. The **Consumer** decodes, converts lowercase to uppercase, re-encodes, and sends the result back to the **Producer**.

The system follows the ASCII standard with odd parity and uses temporary files for debugging and results.

---

## Features

- Encodes and decodes data with parity bit and framing.
- Converts lowercase letters to uppercase.
- Simulates transmission errors.
- Modular and layered design.
- Uses pipes for interprocess communication.


## Instructions to execute:

1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd <repository-folder>
   
2. Make sure that all the below mentioned filesare in the same folder (or) directory.
   
         1) encDec.h
         2) generatePrefix.c
         3) convertToUpper.c
         4) encodeService.c
         5) decodeService.c
         6) ProducerConsumer.c
         7) Samplefile.inpf

4. Create necessary executable files using the following commands :

         gcc generatePrefix.c -o generatePrefix

         gcc convertToUpper.c -o convertToUpper

         gcc encodeService.c -o encodeService

         gcc decodeService.c -o decodeService

         gcc ProducerConsumer.c -o ProducerConsumer
   
6. Run the program:
      ./ProducerConsumer samplefile.inpf
