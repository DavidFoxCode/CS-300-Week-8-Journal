# CS-300 Data Structures and Algorithms Portfolio

**Amin Ahmed** · CS-300: Data Structures and Algorithms · Southern New Hampshire University

This repository has my two course catalog projects from CS-300, where Project One is the runtime and memory analysis I used to pick a data structure, and Project Two is the C++ program I built with that data structure to load ABCU's course file, print every course in alphanumeric order, and look up any course with its prerequisites.

## What's in Here

| File | What it is |
|---|---|
| `Course Catalog Pseudocode and Runtime Analysis.pdf` (and the `.docx`) | Project One: pseudocode for a vector, a hash table, and a binary search tree, plus the runtime and memory analysis, pros and cons, and my recommendation |
| `ProjectTwo.cpp` | Project Two: the course planner program, built with a binary search tree |
| `CS 300 ABCU_Advising_Program_Input.csv` | The course data file the program loads |

## Running Project Two

You just need a C++ compiler like g++, and then from inside this folder you can run:

```
g++ -std=c++11 -o ProjectTwo ProjectTwo.cpp
./ProjectTwo
```

On Windows, run `.\ProjectTwo.exe` for the second line, then pick option 1 and enter `CS 300 ABCU_Advising_Program_Input.csv`, pick 2 to print the course list, or pick 3 and type a course number like `csci400` to see its title and prerequisites.

Here's what a run looks like:

```
What would you like to do? 2
Here is the course list:

CSCI100, Introduction to Computer Science
CSCI101, Introduction to Programming in C++
CSCI200, Data Structures
CSCI300, Introduction to Algorithms
CSCI301, Advanced Programming in C++
CSCI350, Operating Systems
CSCI400, Large Software Development
MATH201, Discrete Mathematics

What would you like to do? 3
What course do you want to know about? csci400
CSCI400, Large Software Development
Prerequisites:
  CSCI301, Advanced Programming in C++
  CSCI350, Operating Systems
```

## Runtime Analysis Summary

| | Vector | Hash Table | Binary Search Tree |
|---|---|---|---|
| Load all courses | O(n²) | O(n) average, O(n²) worst | O(n log n) average, O(n²) worst |
| Find one course | O(n) | O(1) average, O(n) worst | O(log n) average, O(n) worst |
| Print all courses in order | O(n log n) | O(n log n) | O(n) |
| Memory | O(n) | O(n) | O(n) |

I went with the binary search tree because it's the only one that's good at both main jobs, finding a course in O(log n) on average and printing every course in order in O(n) with an in-order traversal, and the full analysis with the line-by-line cost tables and the memory comparison is in the Project One PDF.

## Reflection

### What problem was I solving?

The advisors at ABCU needed a program that reads their course file and lets them print every Computer Science and math course in alphanumeric order or look up one course to see its title and its prerequisites, and before that, the labs had me storing and searching thousands of property auction bids with a linked list, a hash table, a binary search tree, and vector sorting, which all built up to picking the right structure for the course planner.

### How did I approach it, and why do data structures matter?

I started by writing pseudocode for three versions of the program (a vector, a hash table, and a binary search tree) and working out the Big-O for loading, searching, and printing each one, and that showed me the vector needs O(n) to find a course and a full sort every time it prints, the hash table finds a course in O(1) on average but keeps no order, and the binary search tree finds a course in O(log n) on average and prints everything in order in O(n), so I built Project Two with the tree, and that's why understanding data structures matters, since the same program can be fast or slow depending on how the data is stored, and picking the structure that fits what the program does most is one of the biggest decisions you make.

### What was hard, and how did I get past it?

The real data caused most of my problems, since the course file has Windows line endings and extra commas like `MATH201,Discrete Mathematics,,` that would have turned into blank prerequisites, so I trimmed every field and skipped the empty ones, and because a course can list a prerequisite that shows up later in the file, I check every prerequisite against the tree only after the whole file is loaded, and in the bid labs I ran into the Fund column being in a different spot in the large file and amounts over $1,000 being read as 0, so I looked the column up by its header name and stripped out the commas and quotes before converting, and removing a node with two children from the binary search tree took some work until I used the smallest node in its right subtree as the replacement.

### How did these projects change the way I design software?

These projects taught me to plan before I code, because writing the pseudocode and runtime analysis first meant I already knew Project Two would use a binary search tree and what each function needed to do before I wrote any C++, and I also learned to break a program into small pieces like `loadCourses`, `find`, `validate`, and `printCourse` so each one does one job, and to test the weird cases on purpose, like a missing file, a line with no title, a prerequisite that isn't in the file, a duplicate course, and letters typed into the menu.

### How did this change the way I write code?

I write code that's easier to read and change now by using clear names like `courseNumber` and `prerequisites`, keeping functions short, and not hardcoding things that might change, like asking the user for the file name instead of using one fixed file, and in the bid labs, finding a column by its header name instead of a fixed position, and I make sure bad input gets a clear message instead of a crash, so if someone else picks up my code later or the course file changes, the program still makes sense and still works.
