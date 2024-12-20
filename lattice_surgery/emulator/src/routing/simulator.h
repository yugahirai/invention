#pragma once

#include "register_quantum.h"
#include "register_classical.h"
#include "instruction_queue.h"

class SimulatorConfig {
public:
    int plane_width;
    int plane_height;
    string instruction_list_filename;
    int instruction_queue_window_size;
    int reaction_time;
    int factory_time;

    SimulatorConfig(int _plane_width, int _plane_height, string _filename, int _instruction_queue_window_size)
        :plane_width(_plane_width), plane_height(_plane_height), 
        instruction_list_filename(_filename), instruction_queue_window_size(_instruction_queue_window_size),
        reaction_time(1), factory_time(15){}
};

class SimulatorStatus {
public:
    size_t num_code_beat_elapsed;
    size_t num_inst_processed;
    size_t num_inst_total;
    size_t num_magic_stocked;
    size_t num_magic_generated;
};


class Simulator {
private:
    QuantumRegisterMap qreg;
    ClassicalRegisterMap creg;
    InstructionQueue inst_queue;
    uint32_t code_beat;
    vector<uint32_t> distance;

    virtual void process_instruction(const InstructionBase* inst) {
        const QubitPlane& qubit_plane = qreg.get_qubit_plane();
        const vector<string> creg_ready = creg.get_resolved_names();
        const map<string, Position2D> qreg_position_list = qreg.get_available_position_list();
        const map<string, Position2D> factory_position_list = qreg.get_available_factory_list();
        const vector<string> requested_qreg = inst->get_qreg_names();


        // check required creg are ready
        bool flag_creg_ready = inst->resolve_creg_dependency(creg_ready);

        // check all the relevant qubits are not locked
        bool flag_queue_ready = inst_queue.check_is_locked(requested_qreg);

        // check registered status of qreg is okay
        bool flag_qreg_ready = inst->resolve_qreg_dependency(qreg_position_list);

        if (flag_creg_ready && flag_qreg_ready && flag_queue_ready) {
            // resolve topology
            InstructionResolveInfo alloc_info = inst->resolve_topology(qreg_position_list, qubit_plane, factory_position_list);
            if (alloc_info.success) {
                qreg.resolve(alloc_info);
                creg.resolve(alloc_info);
                inst_queue.lock_qubit(alloc_info.lock_qubits);
                inst_queue.cursor_process();
                distance.push_back(alloc_info.lock_position.size());
            }
            else {
                // if fail, prohibit use of relevant qubits
                inst_queue.lock_qubit(requested_qreg);
            }
        }
        else {
            // if fail, prohibit use of relevant qubits
            inst_queue.lock_qubit(requested_qreg);
        }
    }

public:
    Simulator(const SimulatorConfig& _config) : 
        qreg(_config.plane_width, _config.plane_height, _config.factory_time), 
        creg(_config.reaction_time),
        inst_queue(_config.instruction_list_filename, _config.instruction_queue_window_size) ,
        code_beat(0)
    {};
    virtual ~Simulator() {}

    virtual void step() {
        qreg.step();
        creg.step();
        inst_queue.cursor_reset();

        while (inst_queue.cursor_check_inside_window()) {
            const InstructionBase* inst = inst_queue.get_current_instruction();
            process_instruction(inst);
            inst_queue.cursor_iterate_next();
        }
        inst_queue.cursor_update_base();
        code_beat += 1;
    }
    virtual bool is_finished() const {
        return inst_queue.is_finished() && creg.is_finished() && qreg.is_finished();
    }
    virtual void visualize(
        bool flag_floor = true, 
        bool flag_qreg = true, 
        bool flag_creg = true, 
        bool flag_factory = true, 
        bool flag_inst_queue = true, 
        bool flag_inst_process = true
    ) const {

        auto counter = inst_queue.get_count();
        cout << "--- CODE BEAT:" << code_beat << "  INST:" << counter.first << "/" << counter.second << " ---" << endl;
        if (flag_floor) {
            cout << "--- FLOOR STR ---" << endl;
            string floor_str = qreg.get_floor_string();
            cout << floor_str << endl;
        }
        if (flag_qreg) {
            cout << "--- QREG status ---" << endl;
            string qreg_status_str = qreg.get_status_string();
            cout << qreg_status_str << endl;
        }
        if (flag_creg) {
            cout << "--- CREG status ---" << endl;
            string creg_status_str = creg.get_status_string();
            cout << creg_status_str << endl;
        }
        if (flag_factory) {
            cout << "--- FACTORY status ---" << endl;
            string factory_status_str = qreg.get_factory_status_string();
            cout << factory_status_str << endl;
        }
        if (flag_inst_queue) {
            cout << "--- INST queue ---" << endl;
            string inst_queue_str = inst_queue.get_queue_string();
            cout << inst_queue_str << endl;
        }
        if (flag_inst_process) {
            cout << "--- INST processed ---" << endl;
            string inst_queue_str = inst_queue.get_processed_string();
            cout << inst_queue_str << endl;
        }
    }
    virtual SimulatorStatus get_status() const {
        auto count_inst = inst_queue.get_count();
        auto count_magic = qreg.get_magic_status();
        SimulatorStatus status;
        status.num_code_beat_elapsed = code_beat;
        status.num_inst_processed = count_inst.first;
        status.num_inst_total = count_inst.second;
        status.num_magic_stocked = count_magic.first;
        status.num_magic_generated = count_magic.second;
        return status;
    }

    virtual vector<uint32_t> get_distance(){
        return distance;
    }
};
