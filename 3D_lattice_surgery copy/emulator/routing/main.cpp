#include "utility.h"
#include "simulator.h"
#include <thread>
#include <chrono>

void run(string instruction_filename, string placement_filename, string result_name, int max_factory, int queue_size){
    string temp_name = "../routing/temp.txt";
    SimulatorConfig config = load_config_file(instruction_filename, placement_filename, temp_name, max_factory, queue_size);
    Simulator simulator(config);
    vector<SimulatorStatus> history;
    while (!simulator.is_finished()){
        simulator.step();
        history.push_back(simulator.get_status());
    }
    dump_simulator_history(result_name, history);
    dump_simulator_distance(simulator.get_distance());
}

void run_with_visualize(string instruction_filename, string placement_filename, string result_name, int max_factory, int queue_size){
    string temp_name = "../routing/temp.txt";
    SimulatorConfig config = load_config_file(instruction_filename, placement_filename, temp_name, max_factory, queue_size);
    Simulator simulator(config);
    vector<SimulatorStatus> history;
    bool halt_beat = true;
    char key_value;
    while (!simulator.is_finished()){
        simulator.step();

        simulator.visualize(true, false, false, false, false, false);
        if (halt_beat){
            key_value = getchar();
            if (key_value == 32) halt_beat = false;
        }
        else{
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        history.push_back(simulator.get_status());
    }
    dump_simulator_history(result_name, history);
    dump_simulator_distance(simulator.get_distance());
}

int main(int argc, char** argv){
    if (argc == 6) {
        string input_name = argv[1];
        string placement_name = argv[2];
        string result_name = argv[3];
        int factory_count = atoi(argv[4]);
        int queue_size = atoi(argv[5]);
        run_with_visualize(input_name, placement_name, result_name, factory_count, queue_size);
    }
    
    return 0;
};