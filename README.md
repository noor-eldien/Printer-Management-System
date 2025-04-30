# Printer-Management-System

## Overview
A C++ file-based program that simulates a printer queue managing multiple print jobs submitted from different computers to a single printer.
The printer reads print job information from a file named "Printer.txt" and processes them based on their order of arrival and execution time, And provides summary and details at the end of the process about the whole print jobs done.

## Features

- **Priority Queue Management:** Jobs are prioritized based on their arrival time. If jobs arrive at the same time, the one with a shorter execution time is executed first.
- **Waiting Time Calculation:** The program calculates the waiting time for each job and the average waiting time across all jobs.
- **Efficient Queue Operations:** Implemented using a linked list to efficiently manage the queue of print jobs.
- **Input File Reading:** Reads print job information from a file named "Printer.txt".
- **Output Summary Generation:** Produces a detailed summary of the printer queue management, including job execution details, total printer occupancy time, and average waiting time.

## Program Input/Output Examples

### Input Format

The program reads data from a file named `Printer.txt`. The first line of the file contains the **number of print jobs**. Each subsequent line represents a print job with three space-separated fields:

1. **Computer Name:** A string representing the name of the computer (e.g., `PC1`, `PC2`).
2. **Time:** A string in the format `hh:mm` representing the time when the print command was issued.
3. **Execution Time:** An integer representing the time required to complete the print job in minutes.

**Example Input File:**
```
6
PC2 09:30 4
PC1 14:15 2
PC9 11:09 3
PC2 11:09 1
PC3 09:31 3
PC5 14:14 5
```
### Output Format

**Output:**
```
Jobs Executed: 6
PC2 09:30 4 executed at 09:30 waiting time: 0
PC3 09:31 3 executed at 09:34 waiting time: 3
PC2 11:09 1 executed at 11:09 waiting time: 0
PC9 11:09 3 executed at 11:10 waiting time: 1
PC5 14:14 5 executed at 14:14 waiting time: 0
PC1 14:15 2 executed at 14:19 waiting time: 4
The printer was occupied for 18 minutes
Average waiting time for all jobs is 1.33 minutes
```
