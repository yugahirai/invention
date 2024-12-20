#pragma once

#include <vector>
#include <map>
#include <string>


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


std::vector<Instruction> load_file(std::string filename);
AllocationInfo placement(const std::vector<Instruction>& inst_list, int num_factory, std::string hamiltonian_type);
void dump_placement_result(std::string result_filename, const AllocationInfo& allocation_info);

