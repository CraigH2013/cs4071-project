/**
 * Assignment: 3
 * Group: 21
 * Team: Craig Harrison, Loc Tran, Zach Carey
 */

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <queue>
#include <set>

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
 * Prints the matrix in a readable format
 * @param matrix the adjacency matrix of a graph
 * @param title a title to print before the matrix
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
 * Prints the components in a readable format
 * @param comps the components
 * @param title a title to print before the components
 */
template <typename T>
void printComponents(vector<vector<T> > comps, string title) {
  cout << endl << title << ": ";
  for (int i = 0; i < comps.size(); i++) {
    cout << "[";
    for (int j = 0; j < comps[i].size(); j++) {
      cout << comps[i][j] << (j != comps[i].size() - 1 ? ", " : "");
    }
    cout << "]" << (i != comps.size() - 1 ? ", " : "");
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

/**
 * Get the diameter of a graph
 * @param  graph adjacency matrix of the graph
 * @return       the diameter of the graph
 */
int diameter(vector<vector<bool> > graph) {
  // get the distance matrix of the graph
  vector<vector<int> > distMatrix = distanceMatrix(graph);

  // make sure the graph is connected
  if (isConnected(distMatrix)) {
    // find the max distance in the distance matrix
    int size = distMatrix[0].size();
    int max = 0;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        max = distMatrix[i][j] > max ? distMatrix[i][j] : max;
      }
    }
    return max;
  }
  return -1;
}

/**
 * Gets the components of a graph
 * @param  graph the adjacency matrix of the graph
 * @return       the components of the graph
 */
vector<vector<int> > components(vector<vector<bool> > graph) {
  // get the distance matrix of the graph
  vector<vector<int> > distMatrix = distanceMatrix(graph);

  // vector containing sets to avoid duplicates in components
  vector<set<int> > comps;

  int size = distMatrix[0].size();

  // search the components to place each vertex
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (distMatrix[i][j] > 0) {
        // search for a set with j
        bool found = false;
        int indexI = -1;

        for (int k = 0; k < comps.size(); k++) {
          set<int> comp = comps[k];
          for (auto vert : comp) {
            // set found, add i to the set s
            if (vert == j) {
              found = true;
              comp.insert(i);
            }

            // keep track of the component that i was in
            if (vert == i) {
              indexI = k;
            }
          }
        }

        // if not found, check if i was found, or just add i and j to a new set
        if (!found) {
          if (indexI != -1) { // add j to the i set
            comps[indexI].insert(j);
          } else { // add a new set with both i and j
            set<int> newSet = {i, j};
            comps.push_back(newSet);
          }
        }
      }
    }
  }

  // copy the vector<set<int>> to a vector<vector<int>> for compatibility
  vector<vector<int> > finalComps;

  for (auto comp : comps) {
    vector<int> verts(comp.begin(), comp.end());
    finalComps.push_back(verts);
  }

  return finalComps;
}


int main(int argc, char const *argv[]) {
  // get the user input
  vector<string> args;

  if (argc == 2) { // user entered graph through cli as a string
    args = split(argv[1], ' ');
  } else if (argc > 1) { // user entered the values separatly
    for (int i = 1; i < argc; i++) {
      args.push_back(argv[i]);
    }
  } else { // the user wants to enter the values inside the progam
    cout << endl
         << "Enter the graph: ";

    string graphString;
    getline(cin, graphString);

    args = split(graphString, ' ');
  }

  // check for error in input
  if (args.size() % 2 != 0) {
    return displayFatalError("Invalid number of arguments");
  }
  if (stoi(args[args.size() - 1]) != -1) {
    return displayFatalError("Use a -1 at the end of the graph to signify end");
  }

  int numOfNodes = stoi(args[0]);

  // setup an Adjacency matrix with size given by user
  vector<vector<bool> > adjMat(numOfNodes, vector<bool>(numOfNodes, false));

  for (int i = 1; i < args.size() - 2; i += 2) {
    adjMat[stoi(args[i])][stoi(args[i+1])] = true;
    adjMat[stoi(args[i+1])][stoi(args[i])] = true;
  }

  // print the adjacency matrix of the graph
  printMatrix(adjMat, "Adjacency Matrix");

  // print the distance matrix of the graph
  vector<vector<int> > distMat = distanceMatrix(adjMat);
  printMatrix(distMat, "Distance Matrix");

  bool connected = isConnected(distMat);
  cout << endl;
  cout << "Connected: " << (connected ? "True" : "False") << endl;

  if (connected) {
    cout << endl;
    cout << "Diameter: " << diameter(adjMat);
  } else {
    printComponents(components(adjMat), "Components");
  }

  cout << endl << endl;

  return 0;
}
