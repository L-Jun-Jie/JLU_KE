#include <fstream>
#include <iostream>
#include <sstream>

#include "AnimalExpert.h"

void parse_input(string input_file, mymap& kb, myset& db) {
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        exit(0);
    }

    std::string line;
    bool is_db = true;
    while (std::getline(file, line)) {
        if (line.find("DB") == 0)
            continue;
        if (line.find("KB") == 0) {
            is_db = false;
            continue;
        }
        std::string w;
        std::istringstream iss(line);
        if (is_db) {
            while (iss >> w) {
                db.insert(w);
            }
        } else {
            myset lhs;
            std::string rhs;
            bool is_lhs = true;
            while (iss >> w) {
                if (w == "->") {
                    is_lhs = false;
                    continue;
                }
                if (is_lhs)
                    lhs.insert(w);
                else
                    rhs = w;
            }
            kb[lhs] = rhs;
            lhs.clear();
        }
    }
}

void print_usage(string program) {
    std::cout << "Usage: " << program << " [options]\n"
              << "Options:\n"
              << "  --help, -h       Display this help message\n"
              << "  --version, -v    Display the program version\n"
              << "  --input, -i      Input file path\n"
              << "  --goal, -g       Objectives of the derivation"
              << std::endl;
    exit(0);
}

void parseCmdLineArg(int argc, char* argv[], string& inputFile, string& goal) {
    if (argc == 1) {
        print_usage(argv[0]);
        exit(0);
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            exit(0);
        } else if (arg == "--version" || arg == "-v") {
            std::cout << "Program version: 1.0" << std::endl;
            exit(0);
        } else if (arg == "--input" || arg == "-i") {
            inputFile = argv[++i];
        } else if (arg == "--goal" || arg == "-g") {
            goal = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            exit(1);
        }
    }
}

int main(int argc, char* argv[]) {
    string inputFile, goal;
    parseCmdLineArg(argc, argv, inputFile, goal);

    myset db;
    mymap kb;
    parse_input(inputFile, kb, db);
    assert(!kb.empty());
    AnimalExpert expert = AnimalExpert(kb);
    expert.infer(db, goal);
    expert.display_static();

    return 0;
}