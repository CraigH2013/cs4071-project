# cs4071-project
Design and Analysis of Algorithms (CS 4071) – Fall 2016 – Programming Project (Assignment 3) – Diameter and connected components of a network

[Assignment Details](./assignment.pdf)

## Compile and Run
```bash
$ g++ -std=c++11 main.cpp -o main.out; ./main.out
```

## Usage
The program will prompt the user to input a graph. In the case of the graph below:

![graph 3](/images/graph-3.png)

The input would look like the following:

```bash
Enter the graph: 6 0 2 1 2 2 3 2 4 4 5 -1
```

The first number is the number of vertices, the last is to terminate the input, and the rest are pairs of vertices representing an edge in the graph.
