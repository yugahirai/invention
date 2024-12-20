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


SimulatorConfig generate_random_test(string filename) {
    int width = 30;
    int height = 30;
    int queue_size = 10;

    fstream ofs(filename, ios::out);
    vector<pair<int, int>> coord;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            coord.push_back(make_pair(x, y));

    mt19937 mt(0);
    shuffle(coord.begin(), coord.end(), mt);

    int num_qubit = 20;
    for (int i = 0; i < num_qubit; ++i) {
        ofs << "ALLOCATE q" << i << " " << coord[i].first << " " << coord[i].second << endl;
    }

    vector<int> index_list(num_qubit);
    for (int i = 0; i < num_qubit; ++i) index_list[i] = i;
    for (int i = 0; i < 100; ++i) {
        shuffle(index_list.begin(), index_list.end(), mt);
        ofs << "SURGERY q" << index_list[0] << " Z q" << index_list[1] << " X" << endl;
        shuffle(index_list.begin(), index_list.end(), mt);
        ofs << "SURGERY q" << index_list[0] << " Z q" << index_list[1] << " X q" << index_list[2] << " Z" << endl;
        shuffle(index_list.begin(), index_list.end(), mt);
        ofs << "HADAMARD q" << index_list[0] << endl;
        shuffle(index_list.begin(), index_list.end(), mt);
        ofs << "MEAS_TWIST q" << index_list[0] << endl;

    }

    for (int i = 0; i < num_qubit; ++i) {
        ofs << "DEST_MEAS q" << i << " X" << endl;
    }
    ofs.close();

    return SimulatorConfig(width, height, filename, queue_size);
}

SimulatorConfig generate_magic_test(string filename) {
    int width = 30;
    int height = 30;
    int queue_size = 30;

    fstream ofs(filename, ios::out);
    vector<pair<int, int>> coord;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            coord.push_back(make_pair(x, y));

    mt19937 mt(0);
    shuffle(coord.begin(), coord.end(), mt);

    ofs << "ALLOCATE q0 10 10" << endl;
    ofs << "ALLOCATE q1 15 15" << endl;
    ofs << "ALLOCATE q2 20 20" << endl;
    ofs << "MAGIC m0 0 0" << endl;
    ofs << "MAGIC m1 5 0" << endl;
    ofs << "MAGIC m2 7 0" << endl;


    ofs << "CNOT_SURGERY q0 X q1 X _out_ c0" << endl;
    ofs << "CNOT_SURGERY q1 X q2 X _cond_ c0" << endl;

    ofs << "MAGIC_SURGERY q0 X _out_ c1" << endl;
    ofs << "MAGIC_SURGERY q1 X _cond_ c0 c1" << endl;
    ofs << "MAGIC_SURGERY q2 X" << endl;

    ofs << "DEST_MEAS q0 X" << endl;
    ofs << "DEST_MEAS q1 X" << endl;

    ofs.close();

    return SimulatorConfig(width, height, filename, queue_size);
}

SimulatorConfig generate_allocate_test(string filename) {
    int width = 30;
    int height = 30;
    int queue_size = 30;

    fstream ofs(filename, ios::out);
    vector<pair<int, int>> coord;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            coord.push_back(make_pair(x, y));

    mt19937 mt(0);
    shuffle(coord.begin(), coord.end(), mt);

    ofs << "ALLOCATE q0 10 10" << endl;
    ofs << "ALLOCATE q1 15 15" << endl;
    ofs << "ALLOCATE q2 20 20" << endl;


    ofs << "ALLOCATE temp 11 10" << endl;
    ofs << "SURGERY q0 Z temp Z" << endl;
    ofs << "SURGERY q1 X temp X" << endl;
    ofs << "DEST_MEAS temp Z" << endl;
    ofs << "RELEASE temp" << endl;
    ofs.close();

    return SimulatorConfig(width, height, filename, queue_size);
}
