#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

class AdjacentList {
public:
  AdjacentList(int firstNode, int secondNode, int weight)
      : firstNode(firstNode), secondNode(secondNode), weight(weight) {}
  int firstNode;
  int secondNode;
  int weight;
};
class MyGreater {
public:
  bool operator()(const AdjacentList &a, const AdjacentList &b) const {
    return a.weight > b.weight;
  }
};
class SetsNode {
public:
  explicit SetsNode(int parentValue) : parentValue(parentValue) {}
  int parentValue;
  bool isRoot = true;
  unsigned int childCount = 0;
};
class Graph {
public:
  explicit Graph(int a) : verties(a) {}
  unsigned int verties = 0;
  void addEdge(int f, int s, int w);
  void addEdge(AdjacentList list);

  std::priority_queue<AdjacentList, std::vector<AdjacentList>, MyGreater>
      vectorOfEdges;
  std::map<int, SetsNode> sets;
  unsigned int algo();
  int findParent(int target);
  void unionSets(int a, int b);
};
class FileIO {
public:
  void getFileNameFromInput();
  void openFile();
  void createFile();
  void writeFile(unsigned int result);
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
  struct verticesAndEntryPoint {
    unsigned int verticesCount;
    unsigned int edgeCount;
  };
  verticesAndEntryPoint getVerticesCountAndEdgeCount(std::string &line) const;
  unsigned int startIndex = 0;
  AdjacentList getAdjacentList(std::string &line) const;
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
  unsigned int result = myGraph.algo();
  file.writeFile(result);
  file.closeOutputFile();
  return 0;
}
/**
 * add edge to the vector according to the f, s, w
 */
void Graph::addEdge(int f, int s, int w) {
  vectorOfEdges.push(AdjacentList(f, s, w));
}
/**
 * add edge to the vecot with `AdjacentList` data type
 */
void Graph::addEdge(AdjacentList list) { vectorOfEdges.push(list); }
/**
 * the Kruskal’s algorithm
 */
unsigned int Graph::algo() {
  unsigned int count = 0;
  unsigned int weightCount = 0;
  while (count < verties - 1) {
    while (!vectorOfEdges.empty()) {
      AdjacentList list = vectorOfEdges.top();
      vectorOfEdges.pop();
      int firstNodeParent;
      int secondNodeParent;
      try {
        firstNodeParent = findParent(list.firstNode);
      } catch (const std::invalid_argument &e) {
        sets.insert(std::pair<int, int>(list.firstNode, list.firstNode));
        firstNodeParent = list.firstNode;
      }
      try {
        secondNodeParent = findParent(list.secondNode);
      } catch (const std::invalid_argument &e) {
        sets.insert(std::pair<int, int>(list.secondNode, list.secondNode));
        secondNodeParent = list.secondNode;
      }
      if (firstNodeParent != secondNodeParent) {
        unionSets(firstNodeParent, secondNodeParent);
        count++;
        weightCount += list.weight;
      }
    }
  }
  return weightCount;
}
/**
 * find the parent value in the set target value belong in
 */
int Graph::findParent(int target) {
  auto finded = sets.find(target);
  if (finded == sets.end()) {
    throw std::invalid_argument("can not find target");
  }
  while (finded->second.isRoot != true) {
    finded = sets.find(finded->second.parentValue);
  }
  return finded->second.parentValue;
}
/**
 * union two sets
 */
void Graph::unionSets(int a, int b) {
  SetsNode &aNode = sets.at(a);
  SetsNode &bNode = sets.at(b);
  if (aNode.childCount > bNode.childCount) {
    bNode.parentValue = aNode.parentValue;
    bNode.isRoot = false;
    aNode.childCount = aNode.childCount + bNode.childCount + 1;
  } else {
    aNode.parentValue = bNode.parentValue;
    aNode.isRoot = false;
    bNode.childCount += bNode.childCount + aNode.childCount + 1;
  }
}
/**
 * handle file input output,tries to open file and create a output file base on
 * the input file name
 */
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
/**
 * close input file fstream
 */
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
 * create output file, replacing `input` to `output` in the filename
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
void FileIO::writeFile(unsigned int result) { outputFileBuffer << result; }
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
  std::cout << "Please input the file name: ";
  std::cin >> inputFileName;
  if (std::cin.fail()) {
    handleCinError();
  }
}
/**
 * parse data from string and get vertices and entrypoint
 */
Parser::verticesAndEntryPoint
Parser::getVerticesCountAndEdgeCount(std::string &line) const {
  std::istringstream lineStream(line);
  unsigned int verticesCount;
  unsigned int edgeCount;
  if (lineStream >> verticesCount >> edgeCount) {
    return {verticesCount, edgeCount};
  } else {
    throw std::invalid_argument("can not get vertices and entrypoint");
  }
}
/**
 * parse data from vector of strings and return data set
 */
Graph Parser::parseVectorOfStrings(std::vector<std::string> vectorOfStrings) {
  Parser::verticesAndEntryPoint tmp =
      getVerticesCountAndEdgeCount(vectorOfStrings.at(startIndex++));
  Graph GraphObject(tmp.verticesCount);
  for (unsigned int i = 0; i < tmp.edgeCount; i++) {
    GraphObject.addEdge(getAdjacentList(vectorOfStrings.at(startIndex++)));
  }
  return GraphObject;
}
/**
 * parse data from string and get AdjacentList
 */
AdjacentList Parser::getAdjacentList(std::string &line) const {
  std::istringstream lineStream(line);
  int firstNode;
  int secondNode;
  int weight;
  if (lineStream >> firstNode >> secondNode >> weight) {
    return {firstNode, secondNode, weight};
  } else {
    throw std::invalid_argument("can not get verties");
  }
}
