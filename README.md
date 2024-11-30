
# Producer-Consumer Application with Interprocess Communication

This project demonstrates a producer-consumer application in C using interprocess communication with `fork()`, `pipe()`, and `exec()` system calls. It converts lowercase characters in input files to uppercase and employs a layered architecture with distinct physical, data link, and application layers.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Requirements](#system-requirements)
- [Setup](#setup)
- [How to Run](#how-to-run)
- [File Descriptions](#file-descriptions)
- [Testing](#testing)
- [Acknowledgments](#acknowledgments)

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

---

## System Requirements

- **Operating System**: Linux distribution
- **Compiler**: GCC
- **Programming Language**: C

---

## Setup

1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd <repository-folder>
