#include "utility.h"
#include <fstream>
using namespace std;

void dump_placement_result(std::string result_filename, const AllocationInfo& allocation_info) {
    ofstream ofs(result_filename);
    for (const auto& alloc_inst : allocation_info) {
        ofs << alloc_inst.type << " " << alloc_inst.name << " " << alloc_inst.x << " " << alloc_inst.y << " " << alloc_inst.z << endl;
    }
}

