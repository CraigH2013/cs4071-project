#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

/**
 * Formats and displays error message
 * @param  message error message
 * @return         int to return from main
 */
int displayFatalError(string message) {
  cout << endl << endl
       << "  Error: " << message << endl << endl;

  return 0;
}

/**
 * Splits a string with a delimeter into a vector of strings
 * @param s     string to split
 * @param delim delimeter to split the string with
 * @param elems vector to put split string into
 */
void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

/**
 * Splits a string with a delimeter into a vector of strings
 * @param s     string to split
 * @param delim delimeter to split the string with
 *
 * @return    vector of the split string
 */
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

/**
 * Removes all instances of a character in a string
 * @param  input string to remove characters from
 * @param  c     character to search and remove
 * @return       string with characters removed
 */
string removeChar(string input, char c) {
  input.erase(remove(input.begin(), input.end(), c), input.end());
  return input;
}

/**
 * Prints the matrix in a readable format
 * @param matrix the adjacency matrix of a graph
 */
template <typename T>
void printMatrix(vector<vector<T> > matrix, string title) {
  cout << endl << title << ": " << endl;
  int size = matrix[0].size();
  for (int i = 0; i < size; i++) {
    cout << "  ";
    for (int j = 0; j < size; j++) {
      cout << setw(3) << matrix[i][j] << " ";
    }
    cout << endl;
  }
}

/**
 * Retrieve all vertices that the given vertex is connected to
 * @param  graph the adjacency matrix of the graph
 * @param  vert  the vertex to search from
 * @return       the neighbors connected to the given vertex
 */
vector<int> getNeighbors(vector<vector<bool> > graph, int vert) {
  // array for neighbors
  vector<int> neighbors;

  // number of vertices in graph
  int numOfVertices = graph[0].size();

  // look through the column matching the vertex number
  for (int i = 0; i < numOfVertices; i++) {
    if (graph[i][vert] == true) {
      // add the vertex
      neighbors.push_back(i);
    }
  }

  return neighbors;
}

/**
 * Computes the distance between two vertices using a breadth-first traversal
 * @param  graph  adjacency matrix of the graph
 * @param  start  starting vertex
 * @param  end ending vertex
 * @return        The distance between start and end. Returns -1 if not found.
 */
int bfs(vector<vector<bool> > graph, int start, int end) {
  // queue for neighbor vertices
  queue<int> verts;

  // queue for tracking distance
  queue<int> dist;

  int numOfVertices = graph[0].size();

  // track vertices already visited
  vector<bool> visited(numOfVertices);

  // mark the starting vertex visited and push to queues
  visited[start] = true;
  verts.push(start);
  dist.push(0);

  int curVert, distance;
  vector<int> neighbors;

  while (!verts.empty()) {
    // get vertex and distance
    curVert = verts.front();
    distance = dist.front();

    // NOTE for debugging purposes only
    // cout << curVert << "(" << distance << ")" << endl;

    // if vertex equals end, return distance
    if (curVert == end) {
      return distance;
    }

    // remove vertex and distance from queues
    verts.pop();
    dist.pop();

    // get the neighbors of the current vertex
    neighbors = getNeighbors(graph, curVert);

    // add the neighbors and current distance to the queues
    for (int i = 0; i < neighbors.size(); i++) {
      if (!visited[neighbors[i]]) {
        visited[neighbors[i]] = true;
        verts.push(neighbors[i]);
        dist.push(distance + 1);
      }
    }
  }

  // the end vertex was never found, return -1
  return -1;
}

/**
 * Creates a matrix where ij is the distance between vertex i and vertex j
 * @param  graph adjacency matrix of the graph
 * @return       returns the distance matrix
 */
vector<vector<int> > distanceMatrix(vector<vector<bool> > graph) {
  int size = graph[0].size();
  vector<vector<int> > matrix(size, vector<int>(size, -1));

  // use bfs to compute the distance for each element of the matrix
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i][j] = bfs(graph, i, j);
    }
  }

  return matrix;
}

/**
 * Check if a graph is connected using it's distance matrix
 * @param  distMatrix distance matrix of a graph
 * @return            true if connected, false if not
 */
bool isConnected(vector<vector<int> > distMatrix) {
  int size = distMatrix[0].size();

  // a -1 in the dist matrix indicates that there is a disconnect
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (distMatrix[i][j] == -1) {
        return false;
      }
    }
  }
  return true;
}

int main(int argc, char const *argv[]) {

  cout << endl;

  // get number of vertices from the user
  cout << "Enter the number of vertices in your graph" << endl << endl
       << "VERTICES: ";

  string numInput;
  getline(cin, numInput);

  // convert number given by user from string to integer
  int numOfNodes = 0;
  try {
    numOfNodes = stoi(numInput);
  } catch (exception const & e) {
    return displayFatalError("Can not convert given number to an integer.");
  }

  cout << endl;

  // get the graph from the user
  cout << "Enter your graph " << endl
       << "  Undirected Example: 0-1, 2-0" << endl
       << "  Directed Example: 0->1, 1->2, 2->0" << endl << endl
       << "GRAPH: ";

  string graphString;
  getline(cin, graphString);

  // setup an Adjacency matrix with size given by user
  vector<vector<bool> > adjMat(numOfNodes, vector<bool>(numOfNodes, false));

  // setup a vector for the edges inputed by the user
  vector<string> edges = split(removeChar(graphString, ' '), ',');

  // go through each edge and set the coresponding adjacency
  bool isValid, isDirected;
  for (vector<string>::iterator it = edges.begin() ; it != edges.end(); ++it) {
    isValid = (*it).find("-") != string::npos;

    // make sure the edges are valid
    if (!isValid) {
      return displayFatalError("'" + *it
          + "' is not a valid edge. Use '-' or '->'");
    }

    // check if the edge given was in undirected or directed form
    isDirected = (*it).find("->") != string::npos;

    // save the edge in a pair format [i, j]
    vector<string> edgePair;
    if (isDirected) {
      edgePair = split(removeChar(*it, '>'), '-');
    } else {
      edgePair = split(*it, '-');
    }

    // get the Aij values
    int edgeI = stoi(edgePair[0]);
    int edgeJ = stoi(edgePair[1]);

    // make sure the ij values do not exceed or equal the number of nodes
    if (edgeI >= numOfNodes || edgeJ >= numOfNodes) {
      return displayFatalError(
          "Edge '" + to_string(edgeI) + "-" + (isDirected ? ">" : "") +
          to_string(edgeJ) +
          "' contains vertex that is greater than or equal to the "
          "number of nodes given (" + to_string(numOfNodes) + ").");
    }

    // set the adjacency matrix value for directed graph
    adjMat[edgeJ][edgeI] = true;

    // if it's undirected, set the other adjacency value too
    if (!isDirected) {
      adjMat[edgeI][edgeJ] = true;
    }

  }

  // print the adjacency matrix for the user to inspect
  printMatrix(adjMat, "Adjacency Matrix");

  printMatrix(distanceMatrix(adjMat), "Distance Matrix");

  cout << endl;

  return 0;
}
