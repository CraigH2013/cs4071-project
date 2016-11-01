# cs4071-project
Design and Analysis of Algorithms (CS 4071) – Fall 2016 – Programming Project (Assignment 3) – Diameter and connected components of a network

[Assignment Details](./assignment.pdf)

## Compile and Run
```bash
$ g++ main.cpp -o main; ./main
```

## Usage
The program will first prompt the user for the number of vertices in their graph. In the case of this graph:

![graph 1](/images/graph-1.png)

The user would input: `4`

Then the program will ask the user to input the graph. For the above graph, the user would input:

```bash
0->1, 0->2, 2->1, 2->3
```

If the graph were undirected like the one below:

![graph 2](/images/graph-2.png)

The user would input:

```bash
0-1, 1-2
```
