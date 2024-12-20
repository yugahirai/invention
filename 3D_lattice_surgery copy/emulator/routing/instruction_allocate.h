#pragma once

#include "instruction_base.h"

class InstructionAllocate : public InstructionBase{
private:
    Position3D pos;

public:
    InstructionAllocate(string name, Position3D _pos, const vector<string>& _creg_cond)
        : InstructionBase("Allocate", InstructionType::ALLOCATE, { name }, _creg_cond, CREG_OUT_NAME_DUMMY, false), pos(_pos){}

    static InstructionAllocate* parse(const vector<string>& operands, const vector<string>& _creg_cond){
        if (operands.size() != 4){
            throw runtime_error("invalid format allocate instruction.");
        }
        
        string name = operands[0];
        int x = stoi(operands[1]);
        int y = stoi(operands[2]);
        int z;
        if (!name.find("system")){
            z = stoi(operands[3]);
        }
        else{
            z = stoi(operands[3]) + y / 44;
            y = y % 44;
        }

        Position3D pos(x, y, z);

        return new InstructionAllocate(name, pos, _creg_cond); // ← pay attention
    }

    virtual InstructionResolveInfo resolve_topology(const map<string, Position3D>&, const QubitSpace& qubit_space, const map<string, Position3D>&) const{
        InstructionResolveInfo info;
        info.success = qubit_space.is_available(pos);
        info.new_qubit.push_back(make_pair(qreg_names[0], pos));
        return info;
    }
};