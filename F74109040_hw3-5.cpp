#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
class AdjacentNode {
public:
  int des;
  unsigned int weight;
};
struct edge {
  int first;
  int second;
  unsigned int weight;
};
class Graph {
public:
  explicit Graph(int a) : verties(a) {}
  unsigned int verties = 0;
  unsigned int start = 0;
  void addEdge(int f, int s, int w);
  void addEdge(edge edge);
  void addStart(unsigned int i);
  std::map<int, int> algo();
  void algo(int i, std::map<int, int> &shortestDistance);
  std::vector<std::pair<int, int>> sort(std::map<int, int> &shortestDistance);
  std::map<int, std::vector<AdjacentNode>> mapOfAdjacentList;
  std::unordered_set<int> visited;
};
class FileIO {
public:
  void getFileNameFromInput();
  void openFile();
  void createFile();
  void writeFile(std::vector<std::pair<int, int>> result);
  void closeInputFile();
  void closeOutputFile();
  std::vector<std::string> convertFileToVectorOfStrings();

private:
  std::string inputFileName;
  std::ifstream inputFileBuffer;
  std::string outputFileName;
  std::ofstream outputFileBuffer;
};
class Parser {
public:
  Graph parseVectorOfStrings(std::vector<std::string> vectorOfStrings);

private:
  unsigned int getVerticesCount(std::string &line) const;
  unsigned int startIndex = 0;
  edge getEdge(std::string &line) const;
  unsigned int getStartRoot(std::string &line) const;
};

void handleCinError() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  throw std::invalid_argument("Invalid input, please enter valid value");
}
int main() {
  FileIO file;
  file.getFileNameFromInput();
  file.openFile();
  file.createFile();
  std::vector<std::string> vectorOfStrings =
      file.convertFileToVectorOfStrings();
  file.closeInputFile();
  Parser parser;
  Graph myGraph = parser.parseVectorOfStrings(vectorOfStrings);
  std::map<int, int> shortestDistance = myGraph.algo();
  std::vector<std::pair<int, int>> result = myGraph.sort(shortestDistance);
  file.writeFile(result);
  file.closeOutputFile();
  return 0;
}
/**
 * add edge to the vector according to the f, s, w
 */
/**
 * add edge to the vecot with `AdjacentList` data type
 */
void Graph::addEdge(edge edge) {
  auto findedFirst = mapOfAdjacentList.find(edge.first);
  if (findedFirst == mapOfAdjacentList.end()) {
    // can not find
    mapOfAdjacentList.insert(std::pair<int, std::vector<AdjacentNode>>(
        edge.first, {{edge.second, edge.weight}}));
  } else {
    findedFirst->second.push_back({edge.second, edge.weight});
  }
  auto findedSecond = mapOfAdjacentList.find(edge.second);
  if (findedSecond == mapOfAdjacentList.end()) {
    // can not find
    mapOfAdjacentList.insert(std::pair<int, std::vector<AdjacentNode>>(
        edge.second, {{edge.first, edge.weight}}));
  } else {
    findedSecond->second.push_back({edge.first, edge.weight});
  }
}
void Graph::addStart(unsigned int i) { start = i; }

std::map<int, int> Graph::algo() {
  std::map<int, int> shortestDistance;
  algo(start, shortestDistance);
  return shortestDistance;
}
void Graph::algo(int i, std::map<int, int> &shortestDistance) {
  visited.insert(i);
  std::vector<AdjacentNode> &vectorOfNeighboors = mapOfAdjacentList.at(i);
  if (!shortestDistance.count(i)) {
    shortestDistance.insert(std::pair<int, int>(i, 0));
  }
  // int distance = shortestDistance.count(i) ? shortestDistance.at(i) : 0;
  int distance = shortestDistance.at(i);
  for (const auto &a : vectorOfNeighboors) {
    if (!visited.count(a.des)) {
      shortestDistance.insert(std::pair<int, int>(a.des, a.weight + distance));
      algo(a.des, shortestDistance);
    }
  }
}
bool compare(std::pair<int, int> &a, std::pair<int, int> &b) {
  return a.first < b.first;
}
std::vector<std::pair<int, int>>
Graph::sort(std::map<int, int> &shortestDistance) {
  std::vector<std::pair<int, int>> vectorOfPairs;
  for (const auto &e : shortestDistance) {
    vectorOfPairs.push_back(e);
  }
  std::sort(vectorOfPairs.begin(), vectorOfPairs.end(), compare);
  return vectorOfPairs;
}

void FileIO::openFile() {
  inputFileBuffer.open(inputFileName);
  if (inputFileBuffer.is_open()) {
    std::cout << "Load file success." << std::endl << std::endl;
  } else {
    std::cerr << "can't open file name : " << inputFileName << std::endl;
    inputFileBuffer
        .close(); // if can not open file get a new filename and try again
    getFileNameFromInput();
    openFile();
  }
}
void FileIO::closeInputFile() { inputFileBuffer.close(); }
std::vector<std::string> FileIO::convertFileToVectorOfStrings() {
  std::string lineBuffer;
  std::vector<std::string> vectorOfStrings;
  while (std::getline(inputFileBuffer, lineBuffer)) {
    vectorOfStrings.push_back(lineBuffer);
  }
  return vectorOfStrings;
}
/**
 * close input file fstream
 */
void FileIO::createFile() {
  outputFileName =
      std::regex_replace(inputFileName, std::regex("input"), "output");
  outputFileBuffer.open(outputFileName);
  if (outputFileBuffer.is_open()) {
    std::cout << "outputfile"
              << "`" << outputFileName << "`"
              << "opened" << std::endl;
  } else {
    throw std::invalid_argument("outputfile did not open");
  }
}
/**
 * write result to file
 */
void FileIO::writeFile(std::vector<std::pair<int, int>> result) {
  for (const auto &e : result) {
    outputFileBuffer << e.first << ' ' << e.second << std::endl;
  }
}
/**
 * close output file filestream
 */
void FileIO::closeOutputFile() { outputFileBuffer.close(); }

// private methods

/**
 * tries to get the file name
 * @throws throw an exception when user enter EOF
 */
void FileIO::getFileNameFromInput() {
  std::cout << "Please input the map file: ";
  std::cin >> inputFileName;
  if (std::cin.fail()) {
    handleCinError();
  }
}
unsigned int Parser::getVerticesCount(std::string &line) const {
  std::istringstream lineStream(line);
  unsigned int verticesCount;
  if (lineStream >> verticesCount) {
    return verticesCount;
  } else {
    throw std::invalid_argument("can not get vertices");
  }
}
edge Parser::getEdge(std::string &line) const {
  std::istringstream lineStream(line);
  int start;
  int des;
  unsigned int weight;
  if (lineStream >> start >> des >> weight) {
    return {start, des, weight};
  } else {
    throw std::invalid_argument("can not get edges");
  }
}
unsigned int Parser::getStartRoot(std::string &line) const {
  std::istringstream lineStream(line);
  unsigned int start;
  if (lineStream >> start) {
    return start;
  } else {
    throw std::invalid_argument("can not get start root");
  }
}
Graph Parser::parseVectorOfStrings(std::vector<std::string> vectorOfStrings) {
  unsigned int verties = getVerticesCount(vectorOfStrings.at(startIndex++));
  Graph GraphObject(verties);
  for (unsigned int i = 0; i < verties - 1; i++) {
    edge tmp = getEdge(vectorOfStrings.at(startIndex++));
    GraphObject.addEdge(tmp);
  }
  unsigned int start = getStartRoot(vectorOfStrings.at(startIndex++));
  GraphObject.addStart(start);
  return GraphObject;
}
