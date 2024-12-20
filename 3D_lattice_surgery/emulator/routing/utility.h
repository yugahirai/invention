#pragma once

#include "simulator.h"

SimulatorConfig load_config_file(string instruction_filename, string placement_filename, string temp_name, int max_factory, int queue_size);
void dump_simulator_history(string filename, const vector<SimulatorStatus>& status_list);
void dump_simulator_distance(vector<uint32_t> simulator_distance);