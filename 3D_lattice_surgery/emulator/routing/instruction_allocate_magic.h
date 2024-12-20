#pragma once

#include "instruction_base.h"

class InstructionAllocateMagic : public InstructionBase{
private:
    Position3D pos;

public:
    InstructionAllocateMagic(string name, Position3D _pos, const vector<string>& _creg_cond)
        : InstructionBase("AllocMagic", InstructionType::ALLOCATE_MAGIC, { name }, _creg_cond, CREG_OUT_NAME_DUMMY, false), pos(_pos){}

    static InstructionAllocateMagic* parse(const vector<string>& operands, const vector<string>& _creg_cond){
        if (operands.size() != 4){
            throw runtime_error("invalid format allocate magic instruction.");
        }
        string name = operands[0];
        int x = stoi(operands[1]);
        int y = stoi(operands[2]);
        int z = stoi(operands[3]);
        Position3D pos(x, y, z);

        return new InstructionAllocateMagic(name, pos, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(const map<string, Position3D>&, const QubitSpace& qubit_space, const map<string, Position3D>&) const{
        InstructionResolveInfo info;
        info.success = qubit_space.is_available(pos);
        info.new_magic_factory.push_back(make_pair(qreg_names[0], pos));
        return info;
    }
};