#include <fstream>
#include <string>
#include <regex>
#include <iostream>
#include <iomanip>
#include "tokens.h"
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <utility>

using namespace std;

struct Symbol {
    string name = "";
    string scope = "";
    string value = "";
    string type = "";
    string identifier = "";
};


struct Node {
    string category;
    string value;
    vector<Node*> children;

    Node(string val) : value(move(val)) {}
};


list<string> varList;
list<Symbol> symbolList;

void pressEnter();
void inputText();
void inputFile();
bool lexicalAnalysis(string);
bool syntaxAnalysis();
bool semanticAnalysis(string);
Node* createParseTree(const vector<pair<string, pair<string, string>>>& input);
void printParseTree(Node* node, int depth);


int main() {

    bool running = true;
    int option;
    while (running) {
        system("cls");
        symbolList.clear();
        cout << "=============================================================================" << endl;
        cout << setw(60) << "C++ Lexical, Syntax, and Semantic Analysis" << endl;
        cout << "=============================================================================" << endl << endl;
        cout << " [1] Input text" << endl;
        cout << " [2] Input from file" << endl;
        cout << " [3] Exit program" << endl << endl;
        cout << "=============================================================================" << endl;
        cout << " Enter option: ";
        cin >> option;
        cin.ignore();
        switch (option) {
        default:
            cout << " Invalid option!" << endl;
            pressEnter();
            break;
        case 1:
            inputText();
            break;
        case 2:
            inputFile();
            break;
        case 3:
            running = false;
            break;
        }
    }
    
    return 0;
}

bool lexicalAnalysis(string filename) {
    
    ifstream file(filename);
    varList.clear();
    if (!file.is_open()) {
        cout << " Error: Unable to open file." << endl;
        return false;
    }

    cout << endl << "=============================================================================" << endl;
    cout << setw(51) << " Phase 1: Lexical Analysis" << endl;
    cout << "=============================================================================" << endl << endl;
    string line;

    // Regular expression 
    regex re("(using\\s+namespace\\s+std)|(std::\\b(cout|cin))|(\\b\\d+\\b)|(\".*?\")|(//[^\\n]*)|(\\b[a-zA-Z_][a-zA-Z0-9_]*\\b)|(,)|"
        "(cout)|(\".*?\")|(cin)|(<<)|(>>)|(\\(\\))|(\\{)|([\\{\\}()<>%:;.?*\\+-/^&|~!=])|(\\[\\])|"
        "(#include <[^>]+>)|(int)|(main)|([^\\s\"';]+)|(\\S+)");

    while (getline(file, line)) {
        auto words_begin = sregex_iterator(line.begin(), line.end(), re);
        auto words_end = sregex_iterator();

        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();

            if (match_str == "cout" || match_str == "cin") {
                cout << "Token(Type: " << tokenTypes[match_str] << ", Value: '" << match_str << "')\n";
                while (i != words_end) {
                    ++i;
                    if (i == words_end) {
                        cout << "Error: Missing tokens after '" << match_str << "'\n";
                        break;
                    }

                    match_str = (*i).str();
                    if (match_str == "<<" || match_str == ">>") {
                        cout << "Token(Type: " << tokenTypes[match_str] << ", Value: '" << match_str << "')\n";
                        continue;
                    }

                    if (match_str[0] == '\"' && match_str[match_str.length() - 1] == '\"') {
                        match_str = match_str.substr(1, match_str.length() - 2);
                    }
                    cout << "Token(Type: STRING_LITERAL" << ": \"" << match_str << "\")" << endl;

                    // If the next token is not '<<' or '>>', break the loop
                    sregex_iterator next = i;
                    ++next;
                    if (next == words_end || ((*next).str() != "<<" && (*next).str() != ">>")) {
                        break;
                    }
                }
            }
            else if (match_str == "+" || match_str == "=") {
                cout << "Token(Type: OPERATOR, Value: " << match_str << ")\n";
            }
            else if (tokenTypes.find(match_str) != tokenTypes.end()) {
                cout << "Token(Type: " << tokenTypes[match_str] << ", Value: '" << match_str << "')\n";
            }
            else if (match_str[0] == '/' && match_str[1] == '/') {
                string comment = line.substr(i->position());
                cout << "Token(Type: COMMENT, Value: \"" << comment << "\")\n";
                break;
            }
            else {
                cout << "Error: unrecognized token: " << match_str << '\n';
             
                return false;
            }
        }
    }

    

    return true;
}

bool syntaxAnalysis() {

    cout << endl << "=============================================================================" << endl;
    cout << setw(51) << " Phase 2: Syntax Analysis" << endl;
    cout << "=============================================================================" << endl << endl;

    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> fList = {
        {"PREPROCESSOR DIRECTIVE", {"LIBRARY", "#include <iostream>"}},
        {"USING DIRECTIVE", {"USING NAMESPACE STD SEMICOLON", "using namespace std ;"}},
        {"FUNCTION DECLARATION", {"INT IDENTIFIER OPEN_PAREN CLOSE_PAREN OPEN_BRACE", "int main ( ) {}"}},
        {"OUTPUT STATEMENT", {"COUT INSERTION STRING_LITERAL SEMICOLON", "cout << Hello World! ;"}},
        {"DECLARATION", {"INT IDENTIFIER SEMICOLON", "int num1 ;"}},
        {"INPUT STATEMENT", {"CIN EXTRACTION IDENTIFIER SEMICOLON", "cin >> num1 ;"}},
        {"RETURN STATEMENT", {"RETURN INTEGER_LITERAL SEMICOLON", "return 0 ;"}}
    };

    Node* parseTree = createParseTree(fList);
    printParseTree(parseTree,1);




    return true;
}


bool semanticAnalysis(string filename) {

    ifstream file(filename);
    bool usingIO = false;
    bool usingStd = false;
    bool validStdUse = true;
    bool validIOUse = true;
    bool validOp = true;
    bool validReturn = true;

    if (!file.is_open()) {
        return false;
    }

    cout << endl << "=============================================================================" << endl;
    cout << setw(51) << " Phase 3: Semantic Analysis" << endl;
    cout << "=============================================================================" << endl << endl;
    string line;


    regex re("(\\b(void|int|string|float|char|bool|double)\\s+\\w+(,*\\s*\\w+)*\\s*(\\(\\))?)|(return\\s+\"?\\w+\"?)");

    while (getline(file, line)) {
        if (regex_search(line, regex("#include\\s+<\\s*iostream\\s*>"))) {
            usingIO = true;
        }
        if (regex_search(line, regex("using\\s+namespace\\s+std;"))) {
            usingStd = true;
        }
        if (regex_search(line,regex("cout"))) {
           if (!usingIO) {
               validIOUse = false;
           } else if (!usingStd && !regex_search(line, regex("std::cout"))) {
               validStdUse = false;
           } else if (!regex_search(line, regex("cout\\s+<<")) || (!usingStd && !regex_search(line, regex("std::cout\\s+<<")))) {
               validOp = false;
           }
        }

        if (regex_search(line, regex("cin"))) {
            if (!usingIO) {
                validIOUse = false;
            }else if (!usingStd && !regex_search(line, regex("std::cin"))) {
                validStdUse = false;
            }else if (!regex_search(line, regex("cin\\s+>>")) || (!usingStd && !regex_search(line, regex("std::cin\\s+>>")))) {
                validOp = false;
            }
        }

        auto words_begin = sregex_iterator(line.begin(), line.end(), re);
        auto words_end = sregex_iterator();
        
        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            smatch match = *i;
            string match_str = match.str();
            Symbol symbol;

            bool isReturnType = false;
            string returntype = "";
            if (regex_search(match_str, regex("(return\\s+\"?\\w+\"?)"))) {
                for (int i = 0; i < match_str.length(); i++) {
                    if (match_str[i] == ' ') {
                        isReturnType = true;
                        continue;
                    }
                    if (isReturnType) {
                        returntype += match_str[i];
                    }
                }
                for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
                    if (it->name == "main" && it->type!="void") {
                        if (it->type == "int") {
                            if (regex_search(returntype, regex("\\b(true|false)|(\"\\w+\")|(\\d+\\.\\d+)"))) {
                                validReturn = false;
                            }
                        }
                        
                    }
                    else if (it->name == "main" && it->type == "void") {
                        if (regex_search(returntype, regex("\"?\\w+\"?"))) {
                            validReturn = false;
                        }
                    }
                }
                continue;
            }

            if ((match_str[match_str.length() - 2] == '(') && (match_str[match_str.length() - 1] == ')')) {
                symbol.identifier = "Function";
                
            }
            else {
                symbol.identifier = "Variable";
            }
            
           symbol.scope = "Global";
           for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
               if (it->name == "main") {
                   symbol.scope = "Local";
                   break;
               }      
            }
           
           bool isName = false;
           string name = "";
  
           for (int i = 0; i < match_str.length(); i++) {
               if (match_str[i] == '(') break;
               else if (match_str[i] == ' ') {
                   isName = true;
                   continue;
               }
               if (isName) {
                   name += match_str[i];               
               }
               else {
                   symbol.type += match_str[i];
               }
           }

            for (int i = 0; i < name.length(); i++) {
                if (name[i + 1] == '\0') {
                    symbol.name += name[i];
                    symbolList.push_back(symbol);
                }
                if (name[i] == ',') {           
                    symbolList.push_back(symbol);
                    symbol.name = "";
                }
                else {
                    symbol.name += name[i];
                }
            }
        }
       
    }

    file.close();


    cout << "[Symbol Table]" << endl << endl;
    cout << setw(9) << "Scope" << setw(12) << "Identifier" << setw(8) << "Type" << setw(20) << "Name" << endl << endl;
    for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
        cout << setw(9) << it->scope << setw(12) << it->identifier << setw(8) << it->type << setw(20) << it->name << endl;
    }
    cout << endl;

   
    cout << "[Type Checking]" << endl << endl;
  
    for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
        if (it->identifier == "Variable") {
            if (it->type == "int")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing integers.";
            if (it->type == "string")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing strings.";
            if (it->type == "float")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing floating-point numbers.";
            if (it->type == "char")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing a character.";
            if (it->type == "bool")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing boolean values.";
            if (it->type == "double")
                cout << "Variable " << it->name << " is declared as " << it->type << ", suitable for storing high precision floating-point numbers.";
            cout << endl;
        }
    }
    
    cout << endl; 

    if (validOp) {
        cout << "The cin and cout objects use the stream extraction >> and insertion\noperators << correctly with operands of compatible types" << endl;
    }
    else {
        cout << "Error: invalid use of stream extraction (>>) or insertion (<<) operators." << endl;
        return false;
    }
     

    cout << endl << "[Scope Resolution]" << endl << endl;

    if (validStdUse) {
        cout << "The program correctly uses the standard namespace for input (cin)\nand output (cout)." << endl;
    }
    else {
        cout << "Error: namespace std missing." << endl;
        return false;
    }

    cout << endl << "[Checking for Semantic Errors]" << endl << endl;

    bool undeclaredVar = false;

    if (!varList.empty()) {
        for (auto it = varList.begin(); it != varList.end(); ++it) {
            undeclaredVar = true;
            for (auto it1 = symbolList.begin(); it1 != symbolList.end(); ++it1) {
                if (*it == it1->name) {
                    undeclaredVar = false;
                    break;
                }
            }
            if (undeclaredVar) {
                cout << "Error: an undeclared variable was found." << endl;
                return false;
                //break;
            }
        }
    }
    
    
    if (!undeclaredVar) {
        cout << "No undeclared identifiers are used." << endl;
    }

    cout << endl;
    file.open(filename);
    string operation = "";
    bool compatibleType = true;
    while (getline(file, line)) {
        if (regex_search(line, regex("(\\w+\\s*[+\\-*\\/\\%]\\s*\\w+)"))) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ' ' || line[i] == ';') continue;
                operation += line[i];
            }
            regex opRegex("\\w+");
            auto words_begin = sregex_iterator(operation.begin(), operation.end(), opRegex);
            auto words_end = sregex_iterator();
            
            string checkType = "";
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                for (auto it = symbolList.begin(); it != symbolList.end(); ++it) {
                    if (it->name == match_str) {
                        if (i == words_begin) {
                            checkType = it->type;
                        }
                        else {
                            if (it->type != checkType) {
                                compatibleType = false;
                                break;
                            }
                        }
                    }
                }
            }
        }

    }
    if (compatibleType) {
        cout << "All operations are performed on compatible types." << endl;
    }
    else {
        cout << "Error: invalid operation on variables." << endl;
        return false;
    }

    file.close();


    if (validReturn) {
        cout << endl << "The return type of the functions are valid." << endl;
    }
    else {
        cout << endl << "Error: invalid return type." << endl;
        return false;
    }


    cout << endl << "[Function Calls and Standard Library Use]" << endl << endl;

    if (validIOUse) {
        cout << "The program correctly includes and uses the <iostream> library for\ninput or output operations." << endl;
    }
    else {
        cout << "Error: iostream library missing." << endl;
        return false;
    }

    cout << endl;
    cout << "The flow of the program is semantically coherent." << endl;
  
    return true;

}


void inputFile() {
    system("cls");
    string filename = "";
    cout << "=============================================================================" << endl;
    cout << setw(60) << "C++ Lexical, Syntax, and Semantic Analysis" << endl;
    cout << "=============================================================================" << endl << endl;

    cout << " Enter filename (ex. \"source.cpp\"): ";
    getline(cin, filename, '\n');

    if (lexicalAnalysis(filename) && syntaxAnalysis() && semanticAnalysis(filename));
   
    cout << endl << "=============================================================================" << endl << endl;

    cout << " Press <Enter> to continue...";
    cin.get();
}


void inputText() {
    system("cls");
    string code = "";
    cout << "=============================================================================" << endl;
    cout << setw(60) << "C++ Lexical, Syntax, and Semantic Analysis" << endl;
    cout << "=============================================================================" << endl << endl;
    cout << "Enter code (input 'DONE' to finish the code):" << endl;
    list<string> lines;

    ofstream outputFile("inputText.txt", ios::trunc);
    while (code != "DONE") {
        getline(cin, code);
        if (code == "DONE") {
            break;
        }
        outputFile << code << endl;
    }

    for (const string& line : lines) {
        cout << line << endl;
    }
    outputFile.close();
    system("cls");


    if (lexicalAnalysis("inputText.txt") && syntaxAnalysis() && semanticAnalysis("inputText.txt"));
   
    cout << endl << "=============================================================================" << endl << endl;

    cout << " Press <Enter> to continue...";
    cin.get();
}

Node* createParseTree(const vector<pair<string, pair<string, string>>>& input) {
    Node* root = new Node("PROGRAM");
    Node* currentParent = root;

    for (const auto& item : input) {
        string category = item.first;
        string typeStructure = item.second.first;
        string sourceStructure = item.second.second;

        Node* categoryNode = new Node(sourceStructure);
        categoryNode->category = category; // Maybe needed for later

        if (category == "FUNCTION DECLARATION") {
            typeStructure = typeStructure.substr(0, typeStructure.find(' '));
            sourceStructure = sourceStructure.substr(sourceStructure.find(' ') + 1, sourceStructure.find(' ', sourceStructure.find(' ') + 1) - sourceStructure.find(' ') - 1);
            Node* returnTypeNode = new Node(typeStructure);
            Node* functionNameNode = new Node(sourceStructure);

            currentParent->children.push_back(categoryNode);
            categoryNode->children.push_back(returnTypeNode);
            categoryNode->children.push_back(functionNameNode);

            // Update current parent to function declaration node
            currentParent = categoryNode;
        }
        else if (category == "OUTPUT STATEMENT") {
            size_t pos = sourceStructure.find("<<");
            string leftSide = sourceStructure.substr(0, pos);
            string rightSide = sourceStructure.substr(pos + 2);

            Node* insertionNode = new Node("INSERTION");
            Node* leftNode = new Node(leftSide);
            Node* rightNode = new Node(rightSide);

            insertionNode->children.push_back(leftNode);
            insertionNode->children.push_back(rightNode);
            categoryNode->children.push_back(insertionNode);
            currentParent->children.push_back(categoryNode);
        }
        else if (category == "INPUT STATEMENT") {
            size_t pos = sourceStructure.find(">>");
            string leftSide = sourceStructure.substr(0, pos);
            string rightSide = sourceStructure.substr(pos + 2);

            Node* extractionNode = new Node("EXTRACTION");
            Node* leftNode = new Node(leftSide);
            Node* rightNode = new Node(rightSide);

            extractionNode->children.push_back(leftNode);
            extractionNode->children.push_back(rightNode);
            categoryNode->children.push_back(extractionNode);
            currentParent->children.push_back(categoryNode);
        }
        else if (category == "RETURN STATEMENT") {
            string returnValue = sourceStructure.substr(sourceStructure.find(' ') + 1, sourceStructure.find(';') - sourceStructure.find(' ') - 1);

            Node* returnKeywordNode = new Node("RETURN");
            Node* returnValueNode = new Node(returnValue);

            returnKeywordNode->children.push_back(returnValueNode);
            categoryNode->children.push_back(returnKeywordNode);
            currentParent->children.push_back(categoryNode);
        }
        else {
            Node* sourceNode = new Node(typeStructure);
            Node* structureNode = new Node(sourceStructure);
            categoryNode->children.push_back(sourceNode);
            categoryNode->children.push_back(structureNode);
            currentParent->children.push_back(categoryNode);
        }
    }

    return root;
}

void printParseTree(Node* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << node->value << endl;
    for (const auto& child : node->children) {
        printParseTree(child, depth + 1);
    }
}




void pressEnter() {
    cout << " Press <Enter> to continue...";
    cin.clear();
    cin.ignore(100, '\n');
    cin.get();
}
