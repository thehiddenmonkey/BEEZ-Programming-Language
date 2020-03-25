#define ERR cout << "ERROR: " <<
#define WARN cout << "WARNING: " << 
#include "libraries.hpp"
#include "function.hpp"
using std::cout;
using std::endl;
bool hive = false;
bool inFunction = false;
std::string interpret;
std::string hiveType;
function uFunc;

int createHive() {
  hive = true;
  hiveType = interpret.substr(1, interpret.length() - 1);
  return 0;
}

std::string removeCS(std::string ogString, char CS) {
  std::string newOG;
  if (CS == 'C') {
    newOG = ogString;
    newOG.erase(newOG.find("?"));
  }
  else if (CS == 'S') {
    newOG = ogString;
    newOG.erase(std::remove_if(newOG.begin(), newOG.end(), ::isspace), newOG.end());
  }
  else {
    WARN "Compilation error; char CS defined incorrectly in std::string removeCS(std::string ogString, char CS)" << endl;
    return "0";
  }
  return newOG;
}

int main(int argc, char** argv) {
  std::string fileName;
  bool possibleCommand = false;
  if (argc > 1) {
      fileName = argv[1];
  }
  else {
      ERR "No input files given." << endl;
      return 0;
  }
  std::string argv1;
  std::stringstream ssArgv1;
  ssArgv1 << argv[1];
  ssArgv1 >> argv1;
  if (argv1.at(0) == '-') {
    possibleCommand = true;
    if (fileName == "-m" || fileName == "-man") {
      cout << "Welcome to b++, an interpreter created for the BEEZ Programming Language.\nGENERAL STRUCTURE OF COMMAND: b++ {file name or optional command here} {secondary optional command here}\nOPTIONAL COMMANDS: b++ -m or b++ -man: Brings up this help page." << endl;
      return 0;
    }
    else if (fileName == "-s" || fileName == "-settings")
  }
  if (boost::algorithm::ends_with(fileName, ".BUZZ") == false) {
    if (boost::algorithm::iends_with(fileName, ".BUZZ") == true) {
      fileName.erase(fileName.length() - 5, fileName.length());
    }
    fileName += ".BUZZ";
  }
  std::ifstream buzzFile(fileName.c_str());
  if (!buzzFile) {
    ERR "File '" << fileName << "' does not exist!" << endl;
    if (possibleCommand == true) {
      cout << "Did you mean to type in a command?" << endl;
    }
    return 0;
  }
  if (buzzFile.peek() == std::ifstream::traits_type::eof()) {
    ERR "File is blank." << endl;
    return 0;
  }
  while (getline(buzzFile, interpret)) {
    std::string varCheck = removeCS(interpret, 'S');
    if (varCheck == "") {
      ERR "Input file is literally just spaces.\nYou might be interested in another joke programming language: whitespace." << endl;
      return 0;
    }
    varCheck = removeCS(varCheck, 'C');
    if (varCheck == "") {
      ERR "Input fifle is literally just comments.\nWhat are you thinking? Just write something down in a txt file, jesus..." << endl;
      return 0;
    }
    if (varCheck.at(0) == '^') {
      if (hive == false) {
        if (inFunction == false) {
          createHive();
        }
        else {
          ERR "Cannot declare hive type inside of function." << endl;
          return 0;
        }
      }
      else {
        ERR "Hive type already declared." << endl;
        return 0;
      }
    }
    if (varCheck.substr(0, 3) == "BUZZ") {
      varCheck.erase(0, 3);
      if (varCheck.at(0) == '(') {
        if (varCheck.find(',') != std::string::npos) {
          int secondVarCheckFind;
          int varCheckFindI = 1;
          std::vector<std::string> functionArgs;
          while (varCheck.find(',') != std::string::npos) {
            varCheckFindI++;
            boost::iterator_range<std::string::iterator> varCheckFindMultiComma = boost::find_nth(varCheck, ",", varCheckFindI);
            char charVarCheckGoTo = varCheck.at(varCheck.find(','));
            std::stringstream ssVarCheckGoTo;
            ssVarCheckGoTo << charVarCheckGoTo;
            std::string varCheckGoTo;
            ssVarCheckGoTo >> varCheckGoTo;
            secondVarCheckFind = std::distance(varCheckGoTo.begin(), varCheckFindMultiComma.begin());
            if (secondVarCheckFind != 0) {
              functionArgs.push_back(varCheck.substr(varCheck.find(',') + 1, secondVarCheckFind));
            }
            else {
              functionArgs.push_back(varCheck.substr(varCheck.find(','), varCheck.find_last_of(")")));
            }
          }
          int commaF = varCheck.find(',');
          function uFunc(varCheck.substr(1, commaF - 1), functionArgs);
        }
        else {
          int commaF = varCheck.find(',');
          function uFunc(varCheck.substr(1, commaF - 1));
        }
        if (varCheck.back() == '{') {
          inFunction = true;
        }
        else if (varCheck.back() == '}') {
          uFunc.contents = interpret.substr(interpret.find('{') + 1, interpret.find('}') - 1);
        }
        else {
          WARN "Function declared without curly brackets." << endl;
        }  
      }
    }
  }
  return 0;
}
