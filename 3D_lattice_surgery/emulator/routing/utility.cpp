#include "utility.h"

void dump_simulator_history(string filename, const vector<SimulatorStatus>& status_list) {
    ofstream ofs(filename);
    for (const auto& status : status_list) {
        ofs << status.num_code_beat_elapsed << " " << status.num_inst_total << " " << status.num_inst_processed << " " << status.num_magic_generated << " " << status.num_magic_stocked << endl;
    }
    ofs.close();
}

void dump_simulator_distance(vector<uint32_t> distance_list) {
    ofstream ofs("distance.txt");
    for (const auto& distance : distance_list) {
        ofs << distance << endl;
    }
    ofs.close();
}