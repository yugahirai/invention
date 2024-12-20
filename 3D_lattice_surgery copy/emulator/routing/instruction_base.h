#pragma once

#include "common.h"
#include "position.h"
#include "qubit_space.h"

class InstructionResolveInfo{
public:
    bool success = false;
    int lock_lifetime = 0;
    vector<Position3D> lock_position;
    vector<pair<string, Position3D>> new_qubit;
    vector<pair<string, Position3D>> new_magic_factory;
    vector<string> lock_qubits;
    vector<string> remove_qubit;
    vector<string> new_creg;
    vector<string> use_factory;
};

enum class InstructionType{
    ALLOCATE,
    ALLOCATE_MAGIC,
    DESTRUCTIVE_MEASURE,
    SURGERY,
    MEAS_TWIST,
    HADAMARD
};

const string CREG_OUT_NAME_DUMMY = "_";

class InstructionBase{
protected:
    const string inst_name;
    const InstructionType inst_type;
    vector<string> qreg_names;
    vector<string> creg_cond;
    string creg_name_out;
    const bool require_qreg_resolved;
public:
    InstructionBase(string _inst_name, InstructionType _inst_type, const vector<string>& _qreg_names, const vector<string>& _creg_cond, string _creg_name_out, bool _require_qreg_resolved)
        : inst_name(_inst_name), inst_type(_inst_type), qreg_names(_qreg_names), creg_cond(_creg_cond), creg_name_out(_creg_name_out), require_qreg_resolved(_require_qreg_resolved){
        sort(creg_cond.begin(), creg_cond.end());
        sort(qreg_names.begin(), qreg_names.end());
    }

    virtual ~InstructionBase(){};

    virtual InstructionResolveInfo resolve_topology(const map<string, Position3D>& qubit_position, const QubitSpace& qubit_space, const map<string, Position3D>& factory_position) const = 0;

    virtual bool resolve_qreg_dependency(const map<string, Position3D>& qreg_info) const{
        vector<string> qreg_resolved;
        for (const auto& qreg : qreg_info){
            qreg_resolved.push_back(qreg.first);
        }
        sort(qreg_resolved.begin(), qreg_resolved.end());

        if (require_qreg_resolved){
            return includes(qreg_resolved.begin(), qreg_resolved.end(), qreg_names.begin(), qreg_names.end());
        }
        else{
            bool flag = true;
            for (string name : qreg_names){
                if (find(qreg_resolved.begin(), qreg_resolved.end(), name) != qreg_resolved.end()){
                    flag = false;
                }
                if (!flag) break;
            }
            return flag;
        }
    }

    virtual bool resolve_creg_dependency(const vector<string>& creg_ready) const{
        return includes(creg_ready.begin(), creg_ready.end(), creg_cond.begin(), creg_cond.end());
    }

    virtual const vector<string>& get_qreg_names() const{
        return qreg_names;
    }

    virtual string to_string() const{
        stringstream ss;
        ss << inst_name;
        for (auto name : qreg_names){
            ss << " " << name;
        }
        if (creg_cond.size() > 0){
            ss << " cond";
            for (auto name : creg_cond){
                ss << " " << name;
            }
        }
        if (creg_name_out != CREG_OUT_NAME_DUMMY){
            ss << " out " << creg_name_out;
        }
        return ss.str();
    }
};