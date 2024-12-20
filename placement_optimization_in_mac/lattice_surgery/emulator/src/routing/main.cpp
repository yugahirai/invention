#include "utility.h"
#include "simulator.h"
#include <thread>
#include <chrono>

#ifdef _MSC_VER
#include <crtdbg.h>
#include <Windows.h>
#include <stdlib.h>
void setMyCursorPos(int x, int y) {
    HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hCons, pos);
}
#endif

void run(string input_name, string placement_name, string result_name, int max_factory, int queue_size);
void run_with_visualize(string input_name, string placement_name, string result_name, int max_factory, int queue_size);
void debug();

void run(string instruction_filename, string placement_filename, string result_name, int max_factory, int queue_size) {
    string temp_name = "temp.txt";
    SimulatorConfig config = load_config_file(instruction_filename, placement_filename, temp_name, max_factory, queue_size);
    Simulator simulator(config);
    vector<SimulatorStatus> history;
    while (!simulator.is_finished()) {
        simulator.step();
        history.push_back(simulator.get_status());
    }
    dump_simulator_history(result_name, history);
    dump_simulator_distance(simulator.get_distance());
}

void run_with_visualize(string instruction_filename, string placement_filename, string result_name, int max_factory, int queue_size) {
    string temp_name = "temp.txt";
    SimulatorConfig config = load_config_file(instruction_filename, placement_filename, temp_name, max_factory, queue_size);
    Simulator simulator(config);
    vector<SimulatorStatus> history;
    bool halt_beat = true;
    char key_value;
    while (!simulator.is_finished()) {
        simulator.step();

        simulator.visualize(true, false, false, false, false, false);
        if (halt_beat) {
            key_value = getchar();
            if(key_value == 32) halt_beat = false;
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(0));
        }

        history.push_back(simulator.get_status());
    }
    dump_simulator_history(result_name, history);
    dump_simulator_distance(simulator.get_distance());
}
void debug() {
#ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    string input_name = "inst.txt";
    string result_name = "result.txt";
    bool halt_beat = false;
    bool visualize = true;
    bool show_all = false;
    //SimulatorConfig config = generate_random_test(input_name);
    //SimulatorConfig config = generate_magic_test(input_name);
    //SimulatorConfig config = generate_allocate_test(input_name);
    
    string instruction_filename = "../../../../circuit_synth/circuit/result_SELECT_10_Heisenberg2D_cylinder_0.5_0.5_2.raw";
    string placement_filename = "../placement/placement.txt";
    string temporal_filename = "./temp.txt";
    SimulatorConfig config = load_config_file(instruction_filename, placement_filename, temporal_filename, 16, 100);
    Simulator simulator(config);
    vector<SimulatorStatus> history;

    //if (visualize) simulator.visualize();
    if (halt_beat) (void)getchar();

    while (!simulator.is_finished()) {
        simulator.step();

        if (visualize && show_all) simulator.visualize(true, true, true, true, true, true);

        if (visualize && !show_all) {
#ifdef _MSC_VER
            setMyCursorPos(0, 0);
#endif
            simulator.visualize(true, false, false, false, false, false);
            //Sleep(32);
        }
        history.push_back(simulator.get_status());
        if (halt_beat) (void)getchar();
    }
    dump_simulator_history(result_name, history);
}

int main(int argc, char** argv) {
    if (argc == 0){
        debug();
    }else if (argc == 6) {
        string input_name = argv[1];
        string placement_name = argv[2];
        string result_name = argv[3];
        int factory_count = atoi(argv[4]);
        int queue_size = atoi(argv[5]);
        #ifndef FLAG_VISUALIZE
        run(input_name, placement_name, result_name, factory_count, queue_size);
        #else
        run_with_visualize(input_name, placement_name, result_name, factory_count, queue_size);
        #endif
    }else {
        cerr << "Usage: <input_filename> <placement_filename> <result_filename> <num_factory> <queue_size>" << endl;
    }
    return 0;
}