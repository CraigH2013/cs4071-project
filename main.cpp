#include <string>
#include <sstream>
#include <vector>
#include <iostream>

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

void printMatrix(vector<vector<bool> > matrix, int size) {
  cout << endl << "Adjacency Matrix: " << endl;
  for (int i = 0; i < size; i++) {
    cout << "  ";
    for (int j = 0; j < size; j++) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
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
  printMatrix(adjMat, numOfNodes);

  cout << endl;

  return 0;
}
