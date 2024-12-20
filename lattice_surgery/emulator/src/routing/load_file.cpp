#include "utility.h"


#include "utility.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <cassert>
using namespace std;



class Allocation {
public:
    std::string type;
    std::string name;
    int x;
    int y;
    Allocation() : type(""), name(""), x(0), y(0) {};
};

using AllocationInfo = std::vector<Allocation>;

class Instruction {
public:
    std::string inst_name;
    std::vector<std::string> target;
    std::vector<std::string> condition;
    std::vector<std::string> output;
};



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
            if (mode == "cond") {
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


std::vector<Instruction> load_instruction_file(std::string filename) {
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

Allocation load_placement(std::string line) {
    Allocation alloc;
    stringstream ss(line);
    ss >> alloc.type >> alloc.name >> alloc.x >> alloc.y;
    return alloc;
}

AllocationInfo load_placement_file(std::string filename) {
    // load placement
    ifstream ifs(filename);
    AllocationInfo alloc_inst_list;
    if (!ifs) {
        throw runtime_error("file not found");
    }
    while (!ifs.eof()) {
        string line;
        getline(ifs, line);
        if (line.length() == 0)continue;
        Allocation alloc = load_placement(line);
        alloc_inst_list.push_back(alloc);
    }
    ifs.close();
    return alloc_inst_list;

}

SimulatorConfig load_config_file(string instruction_filename, string placement_filename, string temp_name, int max_factory, int queue_size) {
    std::vector<Instruction> inst_list = load_instruction_file(instruction_filename);
    AllocationInfo alloc_inst_list = load_placement_file(placement_filename);

    int chip_width = 0;
    int chip_height = 0;

    ofstream ofs(temp_name);
    for (const auto& alloc_inst : alloc_inst_list) {
        ofs << alloc_inst.type << " " << alloc_inst.name << " " << alloc_inst.x << " " << alloc_inst.y << endl;
        chip_width = max(chip_width, alloc_inst.x);
        chip_height = max(chip_height, alloc_inst.y);
    }
    chip_width += 2;
    chip_height += 2;

    for (const auto& inst : inst_list) {
        string inst_name = inst.inst_name;

        if (inst_name == "H" || inst_name == "S" || inst_name == "MX" || inst_name == "MAGIC_MOVE" || inst_name == "MAGIC_MZZ") {
            assert(inst.target.size() == 1);
            if (inst_name == "H")
                ofs << "HADAMARD " << inst.target[0];
            else if (inst_name == "S") ofs << "PHASE_TWIST " << inst.target[0];
            else if (inst_name == "MX") ofs << "DEST_MEAS " << inst.target[0] << " X";
            else if (inst_name == "MAGIC_MOVE") ofs << "MAGIC_SURGERY " << inst.target[0] << " Z";
            else if (inst_name == "MAGIC_MZZ") ofs << "MAGIC_SURGERY " << inst.target[0] << " Z";
        }
        else if (inst_name == "CX") {
            ofs << "CNOT_SURGERY " << inst.target[0] << " Z " << inst.target[1] << " X";
        }
        if (inst.output.size() > 0) {
            assert(inst.output.size() == 1);
            ofs << " _out_ " << inst.output[0];
        }
        if (inst.condition.size() > 0) {
            ofs << " _cond_";
            for (auto name : inst.condition) {
                ofs << " " << name;
            }
        }
        ofs << endl;
    }
    ofs.close();

    return SimulatorConfig(chip_width, chip_height, temp_name, queue_size);
}