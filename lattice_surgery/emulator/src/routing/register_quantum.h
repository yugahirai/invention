#pragma once

#include "common.h"
#include "position.h"
#include "qubit_plane.h"
#include "instruction.h"

enum class QuantumRegisterStatus {
    FREE,
    USE,
    SURGERY_X,
    SURGERY_Z,
    HADAMARD,
    TWIST
};

class QuantumRegister {
private:
    const uint32_t m_id;
    const string name;
    vector<Position2D> pos_list;
    QuantumRegisterStatus status;
    uint32_t reserved_time;
public:
    QuantumRegister(uint32_t id, string _name, Position2D _pos) : m_id(id), name(_name), 
        status(QuantumRegisterStatus::FREE), reserved_time(0) {
        pos_list.push_back(_pos);
    };
    virtual Position2D get_position() const {
        return pos_list.at(0);
    }
    virtual void step() {
        if (reserved_time > 0) {
            reserved_time -= 1;
        }
    }
    virtual bool is_available() const {
        return (reserved_time == 0);
    }
    virtual void reserve(uint32_t _reserved_time) {
        reserved_time = _reserved_time;
    }
    virtual uint32_t get_lock_time() const {
        return reserved_time;
    }
};

class MagicFactory {
private:
    uint32_t m_id;
    string name;
    Position2D pos;
    const uint32_t code_beat_for_prepare;
    uint32_t elapsed;
    uint32_t stocked_magic;
    uint32_t generated_magic;
    bool is_locked;
public:
    MagicFactory(uint32_t id, string _name, Position2D _pos, uint32_t _code_beat_for_prepare)
        : m_id(id), name(_name), pos(_pos), code_beat_for_prepare(_code_beat_for_prepare), elapsed(_code_beat_for_prepare), stocked_magic(0), generated_magic(0), is_locked(false) {
        assert(_code_beat_for_prepare > 0);
    };
    virtual void step() {
        assert(elapsed>0);
        elapsed -= 1;
        if (elapsed == 0) {
            elapsed = code_beat_for_prepare;
            stocked_magic += 1;
            generated_magic += 1;
        }
        is_locked = false;
    }
    virtual uint32_t get_elapsed() const {
        return elapsed;
    }
    virtual uint32_t get_stock_count() const {
        return stocked_magic;
    }
    virtual uint32_t get_total_generated_count() const {
        return generated_magic;
    }
    virtual uint32_t is_available() const {
        return stocked_magic > 0 && !is_locked;
    }
    virtual Position2D get_position() const {
        return pos;
    }
    virtual void consume_magic() {
        stocked_magic -= 1;
        is_locked = true;
    }
};


class QuantumRegisterMap {
private:
    uint32_t id_counter = 0;
    map<string, QuantumRegister> qreg_list;
    map<string, MagicFactory> factory_list;
    QubitPlane qubit_plane;
    const uint32_t factory_time;
public:
    QuantumRegisterMap(uint32_t width, uint32_t height, uint32_t _factory_time) : qubit_plane(width, height), factory_time(_factory_time) {};
    virtual void add_register(string name, Position2D position) {

        // block duplicated name
        if (qreg_list.find(name) != qreg_list.end()) {
            string message = "register " + name + " is already allocated";
            throw std::invalid_argument(message);
        }

        // prevent double allocation
        if (!qubit_plane.is_available(position)) {
            string message = "register " + name + " tries to allocate reserved position";
            throw std::invalid_argument(message);
        }

        // add register
        QuantumRegister reg(id_counter, name, position);
        id_counter += 1;
        qreg_list.insert(make_pair(name, reg));
    }
    virtual void add_magic_factory(string name, Position2D position) {
        // block duplicated name
        if (qreg_list.find(name) != qreg_list.end()) {
            string message = "register " + name + " is already allocated";
            throw std::invalid_argument(message);
        }

        // prevent double allocation
        if (!qubit_plane.is_available(position)) {
            string message = "register " + name + " tries to allocate reserved position";
            throw std::invalid_argument(message);
        }

        MagicFactory factory(id_counter, name, position, factory_time);
        id_counter += 1;
        factory_list.insert(make_pair(name, factory));
    }
    virtual void release_register(string name) {

        // block double delete
        if (qreg_list.find(name) == qreg_list.end()) {
            string message = "register " + name + " is requested to delete but not exist";
            throw std::invalid_argument(message);
        }

        qreg_list.erase(name);
    }
    virtual void step() {
        qubit_plane.step();
        for (auto& qreg : qreg_list) {
            qreg.second.step();
        }
        for (auto& factory : factory_list) {
            factory.second.step();
        }
    };
    virtual const QubitPlane& get_qubit_plane() const {
        return qubit_plane;
    }
    virtual const map<string, Position2D> get_available_position_list() const {
        map<string, Position2D> result;
        for (const auto& qreg : qreg_list) {
            if (qreg.second.is_available()) {
                string name = qreg.first;
                Position2D pos = qreg.second.get_position();
                result.insert(make_pair(name, pos));
            }
        }
        return result;
    }
    virtual const map<string, Position2D> get_available_factory_list() const {
        map<string, Position2D> result;
        for (const auto& factory : factory_list) {
            if (factory.second.is_available()) {
                string name = factory.first;
                Position2D pos = factory.second.get_position();
                result.insert(make_pair(name, pos));
            }
        }
        return result;
    }
    virtual void resolve(const InstructionResolveInfo& info) {
        for (auto val : info.new_qubit) {
            add_register(val.first, val.second);
            qubit_plane.allocate_parent_block(val.second);
        }
        for (auto val : info.new_magic_factory) {
            add_magic_factory(val.first, val.second);
            qubit_plane.allocate_magic_block(val.second);
        }
        for (auto pos : info.lock_position) {
            qubit_plane.allocate_children_block(pos, info.lock_lifetime);
        }
        for (auto name : info.lock_qubits) {
            qreg_list.at(name).reserve(info.lock_lifetime);
        }
        for (auto name : info.use_factory) {
            factory_list.at(name).consume_magic();
        }
        for (auto name : info.remove_qubit) {
            auto pos = qreg_list.at(name).get_position();
            qubit_plane.deallocate_parent_block(pos);
            qreg_list.erase(name);
        }
    }

    virtual bool is_finished() const {
        bool flag = true;
        for (auto& qreg : qreg_list) {
            flag = flag & qreg.second.is_available();
        }
        return flag;
    }

    virtual string get_floor_string() const {
        return qubit_plane.get_floor_string();
    }
    virtual string get_status_string() const {
        stringstream ss;
        for (auto pair : qreg_list) {
            ss << pair.first << " : " << pair.second.get_position().to_string() 
                << " lock:" << pair.second.get_lock_time()
                << endl;
        }
        return ss.str();
    }
    virtual string get_factory_status_string() const {
        stringstream ss;
        for (auto pair : factory_list) {
            ss << pair.first << " : "
                << pair.second.get_position().to_string() 
                << " prog:" << pair.second.get_elapsed()
                << " stock/total=" << pair.second.get_stock_count() << "/" << pair.second.get_total_generated_count()
                << endl;
        }
        return ss.str();
    }
    virtual pair<size_t, size_t> get_magic_status() const {
        size_t stocked = 0;
        size_t total = 0;
        for (const auto& factory : factory_list) {
            stocked += factory.second.get_stock_count();
            total += factory.second.get_total_generated_count();
        }
        return make_pair(stocked, total);
    }
};