#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
class Graph {
public:
  explicit Graph(int i)
      : vertices(i), matrix(vertices, std::vector<int>(vertices, 0)) {}
  void setMatrix(int row, int columun, int value);
  void setEntryPoint(int i);
  void printMatrix();
  void BFS();
  void BFS(int start, std::queue<int> &queue);
  std::vector<int> getVectorOfPath() const;

private:
  int vertices;
  int entryPoint = 0;
  std::vector<std::vector<int>> matrix;
  std::vector<int> vectorOfPath;
  std::unordered_set<int> visited;
};
class FileIO {
public:
  void getFileNameFromInput();
  void openFile();
  void createFile();
  void writeFile(const std::vector<int> &vectorOfPath);
  std::vector<std::string> convertFileToVectorOfStrings();
  void closeInputFile();
  void closeOutputFile();

private:
  std::string inputFileName;
  std::ifstream inputFileBuffer;
  std::string outputFileName;
  std::ofstream outputFileBuffer;
};
class Parser {
public:
  std::vector<Graph>
  parseVectorOfStrings(std::vector<std::string> vectorOfStrings);

private:
  struct verticesAndEntryPoint {
    int vertices;
    int entryPoint;
  };
  int getDataSetsCount(const std::string &line) const;
  verticesAndEntryPoint
  getVerticesCountAndEntryPoint(const std::string &line) const;
  void parseMatrixInput(int vertices, std::vector<std::string> vectorOfStrings,
                        Graph &targetGraph) const;
  int startIndex = 0;
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
  std::vector<Graph> vectorOfGraph =
      parser.parseVectorOfStrings(vectorOfStrings);
  for (auto &GraphObject : vectorOfGraph) {
    GraphObject.BFS();
    std::vector<int> vectorOfPath = GraphObject.getVectorOfPath();
    file.writeFile(vectorOfPath);
  }
  file.closeOutputFile();
}
/**
 * set adjacency matrix value according to the target
 */
void Graph::setMatrix(int row, int columun, int value) {
  matrix.at(row).at(columun) = value;
}
/**
 * set entry point
 */
void Graph::setEntryPoint(int i) { entryPoint = i; }
/**
 * print the whole adjacency matrix
 */
void Graph::printMatrix() {
  std::cout << "vertices: " << vertices << "entrypoint: " << entryPoint
            << std::endl;
  std::cout << "------------------" << std::endl;
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      std::cout << matrix.at(i).at(j) << " ";
    }
    std::cout << std::endl;
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
 * write vector of path data to file
 */
void FileIO::writeFile(const std::vector<int> &vectorOfPath) {
  outputFileBuffer << vectorOfPath.at(0);
  for (unsigned long i = 1; i < vectorOfPath.size(); i++) {
    outputFileBuffer << ' ' << vectorOfPath.at(i);
  }
  outputFileBuffer << std::endl;
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
  std::cout << "Please input the file name: ";
  std::cin >> inputFileName;
  if (std::cin.fail()) {
    handleCinError();
  }
}
/**
 * parse data from string and get data sets count
 */
int Parser::getDataSetsCount(const std::string &line) const {
  int dataSetsCount;
  std::istringstream lineStream(line);
  if (lineStream >> dataSetsCount) {
    return dataSetsCount;
  } else {
    throw std::invalid_argument("can not get data set count");
  }
}
/**
 * parse data from string and get vertices and entrypoint
 */
Parser::verticesAndEntryPoint
Parser::getVerticesCountAndEntryPoint(const std::string &line) const {
  std::istringstream lineStream(line);
  int vertices;
  int entryPoint;
  if (lineStream >> vertices >> entryPoint) {
    return {vertices, entryPoint};
  } else {
    throw std::invalid_argument("can not get vertices and entrypoint");
  }
}
/**
 *  parse the whole matrix from vector of strings
 */
void Parser::parseMatrixInput(int vertices,
                              std::vector<std::string> vectorOfStrings,
                              Graph &targetGraph) const {
  for (int i = 0; i < vertices; i++) {
    std::string line = vectorOfStrings.at(i + startIndex);
    std::istringstream lineStream(line);
    int vertex;
    for (int j = 0; j < vertices; j++) {
      if (lineStream >> vertex) {
        targetGraph.setMatrix(i, j, vertex);
      } else {
        throw std::invalid_argument("read matrix input failed");
      }
    }
  }
}
/**
 * parse data from vector of strings and return vector of data sets
 */
std::vector<Graph>
Parser::parseVectorOfStrings(std::vector<std::string> vectorOfStrings) {
  std::vector<Graph> vectorOfGraph;
  int dataSetsCount = getDataSetsCount(vectorOfStrings.at(startIndex++));
  for (int i = 0; i < dataSetsCount; i++) {
    Parser::verticesAndEntryPoint tmp;
    tmp = getVerticesCountAndEntryPoint(vectorOfStrings.at(startIndex++));
    int vertices = tmp.vertices;
    int entryPoint = tmp.entryPoint;
    Graph tmpGraph(vertices);
    tmpGraph.setEntryPoint(entryPoint);
    parseMatrixInput(vertices, vectorOfStrings, tmpGraph);
    startIndex += vertices;
    vectorOfGraph.push_back(tmpGraph);
  }
  return vectorOfGraph;
}

/**
 * the BFS function with default entrypoint and queue
 */
void Graph::BFS() {
  std::queue<int> queue;
  BFS(entryPoint, queue);
}
/**
 * the BFS function
 */
void Graph::BFS(int start, std::queue<int> &queue) {
  queue.push(start);
  visited.insert(start);
  vectorOfPath.push_back(start);
  while (!queue.empty()) {
    int poped = queue.front();
    queue.pop();
    std::vector<int> vectorOfNeighboors = matrix.at(poped);
    for (int i = 0; i < vertices; i++) {
      if (vectorOfNeighboors.at(i) && !visited.count(i)) {
        visited.insert(i);
        queue.push(i);
        vectorOfPath.push_back(i);
      }
    }
  }
}
std::vector<int> Graph::getVectorOfPath() const { return vectorOfPath; }
