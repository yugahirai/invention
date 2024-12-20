#pragma once

#include "common.h"

enum class ClassicalRegisterStatus {
    UNCORRECTED,
    CORRECTED
};

class ClassicalRegister {
private:
    ClassicalRegisterStatus status;
    uint32_t value;
    uint32_t unique_id;
    string name;
    uint32_t reaction_elapsed;
public:
    ClassicalRegister() = delete;
    ClassicalRegister(uint32_t _id, string _name, uint32_t reaction_time) 
        : status(ClassicalRegisterStatus::UNCORRECTED), value(0),
          unique_id(_id), name(_name), reaction_elapsed(reaction_time) {}
    virtual string get_name() const { return name; }
    virtual void reset(uint32_t id) {
        unique_id = id;
        status = ClassicalRegisterStatus::UNCORRECTED;
        value = 0;
    }
    virtual ClassicalRegisterStatus get_status() { return status; };
    virtual void step() {
        if (reaction_elapsed > 0) {
            reaction_elapsed -= 1;
        }
        if (reaction_elapsed == 0) {
            status = ClassicalRegisterStatus::CORRECTED;
        }
    }
    virtual bool is_corrected() const {
        return (status == ClassicalRegisterStatus::CORRECTED);
    }
    virtual uint32_t get_elapsed() const {
        return reaction_elapsed;
    }
};

class ClassicalRegisterMap {
    uint32_t creg_id_counter = 0;
    const uint32_t reaction_time;
    map<string, ClassicalRegister> creg_list;
    virtual void check_name_available(string name) const {
        if (creg_list.find(name) != creg_list.end()) {
            throw runtime_error("creg is assigned before use: " + name);
        }
    }
    virtual void add_classical_register(string name) {
        if (creg_list.find(name) == creg_list.end()) {
            ClassicalRegister cr(creg_id_counter, name, reaction_time);
            creg_list.insert(make_pair(name, cr));
        }
        else {
            creg_list.at(name).reset(creg_id_counter);
        }
        creg_id_counter += 1;
    }
public:
    ClassicalRegisterMap(uint32_t _reaction_time) : reaction_time(_reaction_time) {};
    virtual void step() {
        for (auto& creg : creg_list) {
            creg.second.step();
        }
    }
    virtual void resolve(const InstructionResolveInfo& info) {
        for (auto name : info.new_creg) {
            if (name == CREG_OUT_NAME_DUMMY) continue;
            check_name_available(name);
            add_classical_register(name);
        }
    }
    virtual vector<string> get_resolved_names() const {
        vector<string> resolved_names;
        for (const auto& creg : creg_list) {
            if(creg.second.is_corrected())
                resolved_names.push_back(creg.second.get_name());
        }
        return resolved_names;
    }
    virtual bool is_finished() const {
        bool flag = true;
        for (auto pair : creg_list) {
            flag = flag & pair.second.is_corrected();
        }
        return flag;
    }
    virtual string get_status_string() const {
        stringstream ss;
        for (auto pair : creg_list) {
            string status_str = (pair.second.get_status() == ClassicalRegisterStatus::CORRECTED) ? "CORRECTED" : "UNCORRECTED";
            ss << pair.first << " " << status_str << " prog:" << pair.second.get_elapsed() << endl;
        }
        return ss.str();
    }
};