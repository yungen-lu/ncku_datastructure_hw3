#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>
enum class printType { preOrder, postOrder };
enum class inputType { preOrder_inOrder, postOrder_inOrder };
class TreeNode {
public:
  int value = 0;
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
  TreeNode() = default;
  explicit TreeNode(int x) : value(x) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : value(x), left(left), right(right) {}
  ~TreeNode() {
    delete left;
    delete right;
  }
};
class preorderAndInorder {
public:
  TreeNode *createTreeWithTwoVectors(const std::vector<int> &preorderVector,
                                     const std::vector<int> &inorderVector);

private:
  TreeNode *traverse(const std::vector<int> &preorderVector,
                     unsigned int preorderVectorStart,
                     unsigned int preorderVectorEnd,
                     const std::vector<int> &inorderVector,
                     unsigned int inorderVectorStart,
                     unsigned int inorderVectorEnd);
};
class postorderAndInorder {
public:
  TreeNode *createTreeWithTwoVectors(const std::vector<int> &postorderVector,
                                     const std::vector<int> &inorderVector);

private:
  TreeNode *traverse(const std::vector<int> &postorderVector,
                     unsigned int postorderVectorStart,
                     unsigned int postorderVectorEnd,
                     const std::vector<int> &inorderVector,
                     unsigned int inorderVectorStart,
                     unsigned int inorderVectorEnd);
};
class FileIO {
public:
  ~FileIO() {
    closeInputFile();
    closeOutputFile();
  }
  void getFileNameFromInput();
  void openFile();
  void createFile();
  void writeFile(const std::vector<int> &vectorOfNode);
  std::vector<std::string> convertFileToVectorOfStrings();

private:
  std::string inputFileName;
  std::ifstream inputFileBuffer;
  std::string outputFileName;
  std::ofstream outputFileBuffer;
  void closeInputFile();
  void closeOutputFile();
};
class DataSet {
public:
  inputType type;
  std::vector<int> firstVectorOfNode;
  std::vector<int> secondVectorOfNode;
  int nodeCount;

private:
};

class Parser {
public:
  std::vector<DataSet>
  parseVectorOfStrings(std::vector<std::string> vectorOfStrings);

private:
  int getDataSetsCount(const std::string &line);
  inputType getInputType(const std::string &line);
  int getNodeCount(const std::string &line);
  std::vector<int> getVectorOfNode(const std::string &line,
                                   const int nodeCount);

  int startIndex = 0;
};
/**
 * pass in preorderVector and inorderVector and record it's start and end by
 * varibles to represent vector as subvector in each scope. Beware that the
 * `Start` keyword is the index of the starting position of the subvector, and
 * `End` keyword is the index + 1 of the ending position. The reason that we
 * need to do this is to avoid edge case, for intstance, if the middleIndex is
 * at the start of the subvector, the new Start position will be out of bounds.
 */
TreeNode *preorderAndInorder::traverse(const std::vector<int> &preorderVector,
                                       unsigned int preorderVectorStart,
                                       unsigned int preorderVectorEnd,
                                       const std::vector<int> &inorderVector,
                                       unsigned int inorderVectorStart,
                                       unsigned int inorderVectorEnd) {
  if (preorderVectorStart == preorderVectorEnd) { // there is no subvector
    return nullptr;
  }
  int rootValue = preorderVector.at(
      preorderVectorStart); // the element at `preorderVectorStart` in
                            // preorderVector is the root in current scope
  TreeNode *newNode = new TreeNode(rootValue);
  if ((preorderVectorEnd - preorderVectorStart) ==
      1) { // there is only one element in current subvector
    return newNode;
  }
  unsigned int middleIndex; // find the middleIndex in the inorderVector
                            // subvector, and divide the subvector
  for (middleIndex = inorderVectorStart; middleIndex <= inorderVectorEnd;
       middleIndex++) {
    if (inorderVector.at(middleIndex) == rootValue) {
      break;
    }
  }
  unsigned int leftInorderVectorStart = inorderVectorStart;
  unsigned int leftInorderVectorEnd = middleIndex;

  unsigned int rightInorderVectorStart = leftInorderVectorEnd + 1;
  unsigned int rightInorderVectorEnd = inorderVectorEnd;

  unsigned int leftPreorderVectorStart = preorderVectorStart + 1;
  unsigned int leftPreorderVectorEnd =
      leftPreorderVectorStart + (middleIndex - inorderVectorStart);

  unsigned int rightPreorderVectorStart = leftPreorderVectorEnd;
  unsigned int rightPreorderVectorEnd = preorderVectorEnd;
  newNode->left =
      traverse(preorderVector, leftPreorderVectorStart, leftPreorderVectorEnd,
               inorderVector, leftInorderVectorStart, leftInorderVectorEnd);
  newNode->right =
      traverse(preorderVector, rightPreorderVectorStart, rightPreorderVectorEnd,
               inorderVector, rightInorderVectorStart, rightInorderVectorEnd);
  return newNode;
}
TreeNode *preorderAndInorder::createTreeWithTwoVectors(
    const std::vector<int> &preorderVector,
    const std::vector<int> &inorderVector) {
  return traverse(preorderVector, 0, preorderVector.size(), inorderVector, 0,
                  inorderVector.size());
}

void traverseTreeAndPushNodesToVector(TreeNode *nodeRoot, printType type,
                                      std::vector<int> &vectorOfNodeValue) {
  if (nodeRoot == nullptr) {
    return;
  }
  if (type == printType::preOrder) {
    vectorOfNodeValue.push_back(nodeRoot->value);
  }
  traverseTreeAndPushNodesToVector(nodeRoot->left, type, vectorOfNodeValue);
  traverseTreeAndPushNodesToVector(nodeRoot->right, type, vectorOfNodeValue);
  if (type == printType::postOrder) {
    vectorOfNodeValue.push_back(nodeRoot->value);
  }
}
int Parser::getDataSetsCount(const std::string &line) {
  int dataSetsCount;
  std::istringstream lineStream(line);
  if (lineStream >> dataSetsCount) {
    return dataSetsCount;
  } else {
    throw std::invalid_argument("can not get datasetscount");
  }
}
inputType Parser::getInputType(const std::string &line) {
  if (line == "preorder-inorder") {
    return inputType::preOrder_inOrder;
  } else if (line == "postorder-inorder") {
    return inputType::postOrder_inOrder;
  } else {
    throw std::invalid_argument("can not get inputtype");
  }
}
int Parser::getNodeCount(const std::string &line) {
  int nodeCount;
  std::istringstream lineStream(line);
  if (lineStream >> nodeCount) {
    return nodeCount;
  } else {
    throw std::invalid_argument("can not get node count");
  }
}
std::vector<int> Parser::getVectorOfNode(const std::string &line,
                                         const int nodeCount) {
  std::vector<int> vectorOfNode;
  int node;
  std::istringstream lineStream(line);
  for (int i = 0; i < nodeCount; i++) {
    if (lineStream >> node) {
      vectorOfNode.push_back(node);
    } else {
      throw std::invalid_argument("get get node value");
    }
  }
  return vectorOfNode;
}
// preorderAndInorder
std::vector<DataSet>
Parser::parseVectorOfStrings(std::vector<std::string> vectorOfStrings) {
  std::vector<DataSet> vectorOfDataSets;
  int dataSetsCount = getDataSetsCount(vectorOfStrings.at(startIndex++));
  for (int i = 0; i < dataSetsCount; i++) {
    DataSet dataSet;
    dataSet.type = getInputType(vectorOfStrings.at(startIndex++));
    dataSet.nodeCount = getNodeCount(vectorOfStrings.at(startIndex++));
    dataSet.firstVectorOfNode =
        getVectorOfNode(vectorOfStrings.at(startIndex++), dataSet.nodeCount);
    dataSet.secondVectorOfNode =
        getVectorOfNode(vectorOfStrings.at(startIndex++), dataSet.nodeCount);
    vectorOfDataSets.push_back(dataSet);
  }
  return vectorOfDataSets;
}

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
  // file.closeInputFile();
  Parser parser;
  std::vector<DataSet> vectorOfDataSets =
      parser.parseVectorOfStrings(vectorOfStrings);
  // file.closeInputFile();
  for (auto &dataSet : vectorOfDataSets) {
    TreeNode *nodeRoot = nullptr;
    printType type;
    std::vector<int> vectorOfNodeValue;
    switch (dataSet.type) {
    case inputType::preOrder_inOrder: {
      type = printType::postOrder;
      preorderAndInorder sol;
      nodeRoot = sol.createTreeWithTwoVectors(dataSet.firstVectorOfNode,
                                              dataSet.secondVectorOfNode);
      break;
    }
    case inputType::postOrder_inOrder: {
      type = printType::preOrder;
      postorderAndInorder sol;
      nodeRoot = sol.createTreeWithTwoVectors(dataSet.firstVectorOfNode,
                                              dataSet.secondVectorOfNode);
      break;
    }
    }
    traverseTreeAndPushNodesToVector(nodeRoot, type, vectorOfNodeValue);
    file.writeFile(vectorOfNodeValue);
  }
  // file.closeOutputFile();
}
TreeNode *postorderAndInorder::traverse(const std::vector<int> &postorderVector,
                                        unsigned int postorderVectorStart,
                                        unsigned int postorderVectorEnd,
                                        const std::vector<int> &inorderVector,
                                        unsigned int inorderVectorStart,
                                        unsigned int inorderVectorEnd) {
  if (postorderVectorStart == postorderVectorEnd) {
    return nullptr;
  }
  int rootValue = postorderVector.at(
      postorderVectorEnd - 1); // the element at `postorderVectorEnd - 1` in
                               // postorderVector is the root in current scope
  TreeNode *newNode = new TreeNode(rootValue);
  if ((postorderVectorEnd - postorderVectorStart) == 1) {
    return newNode;
  }
  unsigned int middleIndex;
  for (middleIndex = inorderVectorStart; middleIndex <= inorderVectorEnd;
       middleIndex++) {
    if (inorderVector.at(middleIndex) == rootValue) {
      break;
    }
  }
  unsigned int leftInorderVectorStart = inorderVectorStart;
  unsigned int leftInorderVectorEnd = middleIndex;

  unsigned int rightInorderVectorStart = leftInorderVectorEnd + 1;
  unsigned int rightInorderVectorEnd = inorderVectorEnd;

  unsigned int leftpostorderVectorStart = postorderVectorStart;
  unsigned int leftpostorderVectorEnd =
      leftpostorderVectorStart + (middleIndex - inorderVectorStart);

  unsigned int rightpostorderVectorStart = leftpostorderVectorEnd;
  unsigned int rightpostorderVectorEnd = postorderVectorEnd - 1;
  newNode->left = traverse(postorderVector, leftpostorderVectorStart,
                           leftpostorderVectorEnd, inorderVector,
                           leftInorderVectorStart, leftInorderVectorEnd);
  newNode->right = traverse(postorderVector, rightpostorderVectorStart,
                            rightpostorderVectorEnd, inorderVector,
                            rightInorderVectorStart, rightInorderVectorEnd);
  return newNode;
}
TreeNode *postorderAndInorder::createTreeWithTwoVectors(
    const std::vector<int> &postorderVector,
    const std::vector<int> &inorderVector) {
  return traverse(postorderVector, 0, postorderVector.size(), inorderVector, 0,
                  inorderVector.size());
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
void FileIO::getFileNameFromInput() {
  std::cout << "Please input the map file: ";
  std::cin >> inputFileName;
  if (std::cin.fail()) {
    handleCinError();
  }
}
void FileIO::createFile() {
  outputFileName =
      std::regex_replace(inputFileName, std::regex("input"), "output");
  outputFileBuffer.open(outputFileName);
  if (outputFileBuffer.is_open()) {
    std::cout << "outputfile opened" << std::endl;
  } else {
    throw std::invalid_argument("outputfile did not open");
  }
}
void FileIO::writeFile(const std::vector<int> &vectorOfNode) {
  outputFileBuffer << vectorOfNode.at(0);
  for (unsigned long i = 1; i < vectorOfNode.size(); i++) {
    outputFileBuffer << ' ' << vectorOfNode.at(i);
  }
  outputFileBuffer << std::endl;
}
void FileIO::closeOutputFile() { outputFileBuffer.close(); }
