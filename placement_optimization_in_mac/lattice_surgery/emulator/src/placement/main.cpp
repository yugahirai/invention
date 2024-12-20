#include "utility.h"
#include <string>
#include <iostream>

using namespace std;

void run(string input_name, string result_name, int num_factory, string ham_type);

void run(string input_name, string result_name, int num_factory, string ham_type) {
    auto instruction_list = load_file(input_name);
    auto allocation_info = placement(instruction_list, num_factory, ham_type);
    cout << "instruction: "  << input_name << endl;
    cout << "result_name: "  << result_name << endl;
    cout << "num_factory: " << num_factory << endl;
    cout << "hamiltonian_type: " << ham_type << endl;
    dump_placement_result(result_name, allocation_info);
    cout << "finish" << endl;
}

int main(int argc, char** argv) {
    string input_name = "../../../../circuit_synth/circuit/result_SELECT_10_Heisenberg2D_cylinder_0.5_0.5_2.raw";
    string result_name = "./placement.txt";
    int chip_width = 100;
    int chip_height = 100;
    int num_factory = 5;
    string ham_type = "Heisenberg2D";

    if (argc == 5) {
        input_name = argv[1];
        result_name = argv[2];
        num_factory = atoi(argv[3]);
        ham_type = argv[4];
    }
    else if(argc != 1) {
        cerr << "Usage: <input_filename> <result_filename> <num_factory> <hamiltonian_type>" << endl;
        return 0;
    }
    run(input_name, result_name, num_factory, ham_type);
    return 0;
}

