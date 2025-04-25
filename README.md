# Matrix Multiplication Acceleration

This project implements various approaches to accelerate and optimize matrix multiplication using **C**. By leveraging techniques such as naive, multi-process, and multi-threading (joinable, detached, and mixed threads), the project demonstrates the efficiency of different parallel computing methods. Each approach has been carefully benchmarked to highlight performance improvements over the traditional naive solution.

## Features

- **Naive Matrix Multiplication**:
  - Implements a straightforward O(n³) matrix multiplication algorithm.
  - Serves as a baseline for performance comparison.

- **Multi-Processing Approach**:
  - Divides the workload among child processes using pipes for communication.
  - Demonstrates the use of process-based parallelism in C.

- **Joinable Threads**:
  - Implements threading with synchronization using joinable threads.
  - Ensures all threads complete their work before proceeding.

- **Detached Threads**:
  - Implements threading where threads run independently without joining.
  - Uses mutexes for shared resource synchronization.

- **Mixed Threads**:
  - Combines joinable and detached threads in a hybrid threading model.
  - Demonstrates the flexibility of thread management.

## Performance Benchmarking

Each approach has been benchmarked to measure execution time and efficiency under varying conditions. The project compares the naive approach with parallel methods to showcase the advantages of multi-threading and multi-processing in handling large matrices.

## Tech Stack

- **Language**: C.
- **Libraries Used**:
  - `<pthread.h>` for threading.
  - `<sys/types.h>` and `<sys/wait.h>` for process management.
  - `<stdio.h>` and `<stdlib.h>` for standard I/O and memory management.

## Prerequisites

- A C compiler (e.g., GCC) installed on your system.
- Basic understanding of matrix multiplication and parallel programming concepts.

## Clone the Repository
    ```bash
    git clone https://github.com/Lanamahd/Matrix-Multiplication-Acceleration.git
    cd Matrix-Multiplication-Acceleration

## How It Works

1. **Matrix Initialization**:  
  Matrices are filled with values derived from specific calculations to simulate real-world data.

2. **Naive Approach**:  
  A basic triple-nested loop is used for matrix multiplication.

3. **Child Processes**:  
  - The matrix is divided into chunks, each processed by a child process.  
  - Pipes are used for communication to transfer partial results.

4. **Joinable Threads**:  
  - Threads are created, each responsible for a portion of the matrix.  
  - A barrier is used to synchronize all threads.

5. **Detached Threads**:  
  - Threads execute independently, using mutexes for shared resources.

6. **Mixed Threads**:  
  - A combination of joinable and detached threads is used, showcasing advanced thread management techniques.

## Project Report

A detailed project report is available in the repository as **proj1Report_OS.pdf**, which includes:

- **Implementation Strategies**: Explanation of the design and development approaches used in the project.
- **Performance Comparisons**: Analysis of the performance of various matrix multiplication techniques.
- **Challenges and Solutions**: Discussion of challenges encountered during implementation and the solutions applied.


## Conclusion
This project provides a comprehensive exploration of matrix multiplication optimization using parallel computing techniques. By comparing multiple approaches, it serves as a valuable resource for understanding the trade-offs and benefits of various parallel processing methods. Feel free to explore, modify, and expand upon the implementation to suit your needs.
