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
  SetsNode(int parentValue) : parentValue(parentValue) {}
  int parentValue;
  bool isRoot = true;
  unsigned int childCount = 0;
};
class Graph {
public:
  Graph(int a) : verties(a) {}
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
  void closeInputFile();
  void createFile();
  void writeFile(unsigned int result);
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
  verticesAndEntryPoint getVerticesCountAndEdgeCount(std::string &line);
  unsigned int startIndex = 0;
  AdjacentList getAdjacentList(std::string &line);
};

void handleCinError() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  throw std::invalid_argument("Invalid input, please enter valid value");
}

int main() {
  // Graph test(7);
  // test.addEdge(0, 1, 28);
  // test.addEdge(0, 5, 10);
  // test.addEdge(1, 2, 16);
  // test.addEdge(5, 4, 25);
  // test.addEdge(6, 4, 24);
  // test.addEdge(1, 6, 14);
  // test.addEdge(3, 4, 22);
  // test.addEdge(2, 3, 12);
  // test.addEdge(3, 6, 18);

  // std::cout << test.algo() << std::endl;
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
void Graph::addEdge(int f, int s, int w) {
  vectorOfEdges.push(AdjacentList(f, s, w));
}
void Graph::addEdge(AdjacentList list) { vectorOfEdges.push(list); }
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
void FileIO::writeFile(unsigned int result) {
  outputFileBuffer << result;
  // outputFileBuffer << vectorOfPath.at(0);
  // for (unsigned long i = 1; i < vectorOfPath.size(); i++) {
  //   outputFileBuffer << ' ' << vectorOfPath.at(i);
  // }
  // outputFileBuffer << std::endl;
}
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
Parser::verticesAndEntryPoint
Parser::getVerticesCountAndEdgeCount(std::string &line) {
  std::istringstream lineStream(line);
  unsigned int verticesCount;
  unsigned int edgeCount;
  if (lineStream >> verticesCount >> edgeCount) {
    return {verticesCount, edgeCount};
  } else {
    throw std::invalid_argument("can not get vertices and entrypoint");
  }
}
Graph Parser::parseVectorOfStrings(std::vector<std::string> vectorOfStrings) {
  Parser::verticesAndEntryPoint tmp =
      getVerticesCountAndEdgeCount(vectorOfStrings.at(startIndex++));
  Graph GraphObject(tmp.verticesCount);
  for (unsigned int i = 0; i < tmp.edgeCount; i++) {
    GraphObject.addEdge(getAdjacentList(vectorOfStrings.at(startIndex++)));
  }
  return GraphObject;
}
AdjacentList Parser::getAdjacentList(std::string &line) {
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
