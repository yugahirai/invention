
#include "utility.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>
#include <algorithm>
#include <tuple>
using namespace std;


class QubitInfo {
public:
    QubitInfo() : name(""), type(""), index(0), thread(0), x(0), y(0), s(0) {};
    string name;
    string type;
    int index;
    int thread;
    int x;
    int y;
    int z;
    int s;
};
class MappingInfo {
public:
    int num_control_thread;
    int num_control_count;
    int system_width;
    int system_height;

};

QubitInfo match_name(string name);
tuple<vector<QubitInfo>, MappingInfo> fetch_qubit_info_list(const std::vector<Instruction>& inst_list);

QubitInfo match_name(string name) {
    regex re1(R"(control_(\d+)_th_(\d+))");
    regex re2(R"(control_ancilla_(\d+)_th_(\d+))");
    regex re3(R"(system_(\d+)_(\d+))");
    regex re4(R"(system_(\d+)_(\d+)_(\d+))");
    smatch m;
    QubitInfo qubit;
    qubit.name = name;
    if (regex_match(name, m, re1)) {
        qubit.type = "control";
        qubit.index = stoi(m[1]);
        qubit.thread = stoi(m[2]);
    }
    else if (regex_match(name, m, re2)) {
        qubit.type = "control_ancilla";
        qubit.index = stoi(m[1]);
        qubit.thread = stoi(m[2]);
    }
    else if (regex_match(name, m, re3)) {
        qubit.type = "system";
        qubit.x = stoi(m[1]);
        qubit.y = stoi(m[2]);
    }
    else if (regex_match(name, m, re4)) {
        qubit.type = "system";
        qubit.x = stoi(m[1]);
        qubit.y = stoi(m[2]);
        qubit.s = stoi(m[3]);
    }
    else {
        assert(false);
    }
    return qubit;
}

tuple<vector<QubitInfo>, MappingInfo> fetch_qubit_info_list(const std::vector<Instruction>& inst_list) {
    set<string> qubit_names;
    for (const auto& inst : inst_list) {
        for (auto name : inst.target) qubit_names.insert(name);
    }

    int max_control_thread = 0;
    int max_control_index = 0;
    int max_system_x = 0;
    int max_system_y = 0;
    vector<QubitInfo> qubit_info_list;
    for (string name : qubit_names) {
        QubitInfo info = match_name(name);
        if (info.type == "control" || info.type == "control_ancilla") {
            max_control_thread = max(max_control_thread, info.thread);
            max_control_index = max(max_control_index, info.index);
        }
        else if (info.type == "system") {
            max_system_x = max(max_system_x, info.x);
            max_system_y = max(max_system_y, info.y);
        }
        else {
            throw runtime_error("Unknown qubit name");
        }
        qubit_info_list.push_back(info);
    }
    MappingInfo mapping_info;
    mapping_info.num_control_thread = max_control_thread+1;
    mapping_info.num_control_count = max_control_index+1;
    mapping_info.system_width = max_system_x+1;
    mapping_info.system_height = max_system_y+1;
    return { qubit_info_list , mapping_info };
}

void allocate_magic_space(AllocationInfo& alloc_inst_list, int num_factory) {
    const int magic_space = 1;
    int magic_number = 0;
    // determine loations
    for (int n = 0; n < 4; ++n){
    for (int i = 0; i < num_factory; ++i) {
        Allocation alloc_inst;
        alloc_inst.type = "MAGIC";
        alloc_inst.name = "magic" + to_string(magic_number);
        alloc_inst.x = 0;
        alloc_inst.y = 18 + magic_space * i;
        alloc_inst.z = n;
        alloc_inst_list.push_back(alloc_inst);
        magic_number++;
    }
    }
}

void placement_heisenberg_2d(AllocationInfo& alloc_inst_list, const vector<QubitInfo>& qubit_info_list, const MappingInfo& mapping_info, int num_factory) {
    const int magic_space = 1;
    const int block_offset = 3;
    const int magic_offset = block_offset;
    const int system_offset = block_offset * 2 + (mapping_info.num_control_count - 1) * 3 / 2;
    const int height = max(max(magic_space * num_factory + 1, 4 * mapping_info.num_control_thread + 2), (mapping_info.system_height - 1) * 3 / 2 + 3);
    const int width = system_offset + mapping_info.system_width * 3 / 2 + 2;

    for (const auto& qubit_info : qubit_info_list) {
        Allocation alloc_inst;
        if (qubit_info.type == "control") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3/2;
            alloc_inst.y = 1 + qubit_info.thread * 4;
            alloc_inst.z = 1;
        }
        else if (qubit_info.type == "control_ancilla") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3/2;
            alloc_inst.y = 3 + qubit_info.thread * 4;
            alloc_inst.z = 1;
        }
        else if (qubit_info.type == "system") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + 2*qubit_info.x;
            alloc_inst.y = 15 + 1 + 2*qubit_info.y;
            alloc_inst.z = 0;
        }
        else {
            throw runtime_error("Unknown model");
        }
        assert(alloc_inst.x < width);
        assert(alloc_inst.y < height);
        alloc_inst_list.push_back(alloc_inst);
    }
}

void placement_heisenberg_1d(AllocationInfo& alloc_inst_list, const vector<QubitInfo>& qubit_info_list, const MappingInfo& mapping_info, int num_factory) {
    const int magic_space = 1;
    const int block_offset = 3;
    const int magic_offset = block_offset;
    const int system_offset = block_offset * 2 + (mapping_info.num_control_count - 1) * 3 / 2;

    const int temp_height = max(magic_space * num_factory + 1, 4 * mapping_info.num_control_thread + 2);

    assert(mapping_info.system_width == 2);
    const int num_line = max((temp_height - 2) / 3, 2);

    const int line_mod = (mapping_info.system_height - 1) / num_line + 1;
    const int width = system_offset + line_mod * 3 / 2 + 2;
    const int height = max(temp_height, 3 * ((mapping_info.system_height - 1) / line_mod) + 4);
    for (const auto& qubit_info : qubit_info_list) {
        Allocation alloc_inst;
        if (qubit_info.type == "control") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3 / 2;
            alloc_inst.y = 1 + qubit_info.thread * 4;
        }
        else if (qubit_info.type == "control_ancilla") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3 / 2;
            alloc_inst.y = 3 + qubit_info.thread * 4;
        }
        else if (qubit_info.type == "system") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;

            alloc_inst.x = system_offset + (qubit_info.y % line_mod) * 3 / 2;
            alloc_inst.y = 1 + 3 * (qubit_info.y / line_mod) + qubit_info.x;
        }
        else {
            throw runtime_error("Unknown model");
        }
        if (!(alloc_inst.x < width)) throw runtime_error("invalid size");
        if (!(alloc_inst.y < height)) throw runtime_error("invalid size");
        alloc_inst_list.push_back(alloc_inst);
    }
}

void placement_fermihubbard_2d(AllocationInfo& alloc_inst_list, const vector<QubitInfo>& qubit_info_list, const MappingInfo& mapping_info, int num_factory) {
    const int magic_space = 1;
    const int block_offset = 3;
    const int magic_offset = block_offset;
    const int system_offset = block_offset * 2 + (mapping_info.num_control_count - 1) * 3 / 2;

    const int height = max(max(magic_space * num_factory + 1, 4 * mapping_info.num_control_thread + 2), (mapping_info.system_height - 1) * 3 / 2 + 3);
    const int width = system_offset + mapping_info.system_width * 3 + 2;
    for (const auto& qubit_info : qubit_info_list) {
        Allocation alloc_inst;
        if (qubit_info.type == "control") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3 / 2;
            alloc_inst.y = 1 + qubit_info.thread * 4;
        }
        else if (qubit_info.type == "control_ancilla") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = magic_offset + qubit_info.index * 3 / 2;
            alloc_inst.y = 3 + qubit_info.thread * 4;
        }
        else if (qubit_info.type == "system") {
            alloc_inst.type = "ALLOCATE";
            alloc_inst.name = qubit_info.name;
            alloc_inst.x = system_offset + qubit_info.x * 3 + qubit_info.s;
            alloc_inst.y = 1 + qubit_info.y * 3 / 2;
        }
        else {
            throw runtime_error("Unknown model");
        }
        assert(alloc_inst.x < width);
        assert(alloc_inst.y < height);
        alloc_inst_list.push_back(alloc_inst);
    }
}

AllocationInfo placement(const std::vector<Instruction>& inst_list, int num_factory, string hamiltonian_type){
    MappingInfo mapping_info;
    vector<QubitInfo> qubit_info_list;
    std::tie(qubit_info_list, mapping_info) = fetch_qubit_info_list(inst_list);

    AllocationInfo alloc_inst_list;
    allocate_magic_space(alloc_inst_list, num_factory);

    if (hamiltonian_type == "Heisenberg2D") {
        placement_heisenberg_2d(alloc_inst_list, qubit_info_list, mapping_info, num_factory);
    }
    else if (hamiltonian_type == "Heisenberg1D") {
        placement_heisenberg_1d(alloc_inst_list, qubit_info_list, mapping_info, num_factory);
    }
    else if (hamiltonian_type == "FermiHubbard2D") {
        placement_fermihubbard_2d(alloc_inst_list, qubit_info_list, mapping_info, num_factory);
    }
    else {
        throw runtime_error("Unknown model");
    }

    return alloc_inst_list;
}

