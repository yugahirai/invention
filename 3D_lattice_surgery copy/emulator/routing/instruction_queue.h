#pragma once

#include "common.h"
#include "instruction.h"

class InstructionQueue{
private:
    const size_t window_size;
    size_t inst_ptr;
    size_t inst_ptr_base;
    vector<InstructionBase*> inst_list;
    vector<bool> is_processed;
    map<string, bool> is_locked;

    uint32_t processed_count;
    vector<size_t> processed_index;

    virtual bool is_comment_line(string line){
        return line[0] == '#' || line.length() == 0;
    }

    virtual void load(string filename){
        fstream ifs(filename);
        string line;
        while(!ifs.eof()){
            getline(ifs, line);

            if(is_comment_line(line)){
                continue;
            }

            InstructionBase* inst = parse_command(line);
            inst_list.push_back(inst);
        }
        is_processed = vector<bool>(inst_list.size(), false);
    }

public:
    InstructionQueue(string filename, int _window_size) : window_size(_window_size){
        load(filename);
        inst_ptr = 0;
        inst_ptr_base = 0;
        processed_count = 0;
    };

    virtual ~InstructionQueue(){
        for (InstructionBase* val : inst_list){
            delete val;
        }
    }

    virtual void cursor_reset(){
        inst_ptr = inst_ptr_base;
        for (auto ite = is_locked.begin(); ite != is_locked.end(); ++ite){
            (*ite).second = false;
        }
        processed_index.clear();
    }

    virtual bool cursor_check_inside_window() const{
        return inst_ptr < min(inst_ptr_base + window_size, inst_list.size());
    }

    virtual void cursor_process(){
        is_processed[inst_ptr] = true;
        processed_count += 1;
        processed_index.push_back(inst_ptr);
    }

    virtual void cursor_iterate_next(){
        inst_ptr += 1;
        while (cursor_check_inside_window() && is_processed[inst_ptr]){
            inst_ptr += 1;
        }
    }

    virtual void cursor_update_base(){
        while (inst_ptr_base < inst_list.size() && is_processed[inst_ptr_base]){
            inst_ptr_base += 1;
        }
    }

    virtual const InstructionBase* get_current_instruction() const{
        if (inst_ptr >= inst_list.size()){
            throw range_error("cursor point out of queue.");
        }
        else{
            return inst_list[inst_ptr];
        }
    }

    virtual void lock_qubit(vector<string> qubit_names){
        for (auto name : qubit_names){
            auto ite = is_locked.find(name);
            if (ite == is_locked.end()){
                is_locked.insert(make_pair(name, true));
            }
            else{
                (*ite).second = true;
            }
        }
    }

    virtual bool check_is_locked(vector<string> qubit_names) const{
        bool flag = true;
        for (auto name : qubit_names){
            auto ite = is_locked.find(name);
            if (ite == is_locked.end()) continue;
            else{
                flag = flag & (!(*ite).second);
            }
        }
        return flag;
    }

    virtual bool is_finished() const{
        return inst_ptr_base >= inst_list.size();
    }

    virtual string get_queue_string() const{
        stringstream ss;
        size_t count = 0;
        for (size_t cursor = inst_ptr_base; cursor < min(inst_ptr_base + window_size, inst_list.size()); ++cursor){
            if (!is_processed.at(cursor)){
                ss << cursor << " : " << inst_list.at(cursor)->to_string() << endl;
                count += 1;
            }
        }
        for(; count < window_size; ++count){
            ss << endl;
        }
        return ss.str();
    }

    virtual pair<size_t, size_t> get_count() const{
        return make_pair(processed_count, inst_list.size());
    }

    virtual string get_processed_string() const {
        stringstream ss;
        for (size_t index : processed_index) {
            ss << index << " : " << inst_list.at(index)->to_string() << endl;
        }
        return ss.str();
    }


};