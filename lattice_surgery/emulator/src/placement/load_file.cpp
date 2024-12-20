
#include "utility.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <cassert>
using namespace std;

Instruction load_instruction(string line);

Instruction load_instruction(string line) {
    string inst_name;
    stringstream ss(line);
    ss >> inst_name;

    string target, temp;
    string mode;

    int target_count;
    if (inst_name == "H" || inst_name == "S" || inst_name == "MX" || inst_name == "MAGIC_MOVE" || inst_name == "MAGIC_MZZ") {
        target_count = 1;
    }
    else if (inst_name == "CX") {
        target_count = 2;
    }
    else {
        throw runtime_error("Unknown inst");
    }

    Instruction inst;
    inst.inst_name = inst_name;
    for (int i = 0; i < target_count; ++i) {
        ss >> target;
        inst.target.push_back(target);
    }
    while (!ss.eof()) {
        ss >> temp;
        if (temp.length() == 0)continue;
        if (temp == "_cond_") mode = "cond";
        else if (temp == "_out_") mode = "out";
        else {
            if (mode == "cond"){
                inst.condition.push_back(temp);
            }
            else if (mode == "out") {
                inst.output.push_back(temp);
            }
            else {
                throw runtime_error("Unknown mode");
            }
        }
    }
    return inst;
}


std::vector<Instruction> load_file(std::string filename) {
    // load instructions
    ifstream ifs(filename);
    vector<Instruction> inst_list;
    if (!ifs) {
        throw runtime_error("file not found");
    }
    while (!ifs.eof()) {
        string line;
        getline(ifs, line);
        if (line.length() == 0)continue;
        Instruction inst = load_instruction(line);
        inst_list.push_back(inst);
    }
    ifs.close();
    return inst_list;
}