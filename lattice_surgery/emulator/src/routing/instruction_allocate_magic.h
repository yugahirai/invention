#pragma once

#include "instruction_base.h"

class InstructionAllocateMagic : public InstructionBase {
private:
    Position2D pos;

public:
    InstructionAllocateMagic(string name, Position2D _pos, const vector<string>& _creg_cond)
        : InstructionBase("AllocMagic", InstructionType::ALLOCATE_MAGIC, { name }, _creg_cond, CREG_OUT_NAME_DUMMY, false), pos(_pos) {}

    static InstructionAllocateMagic* parse(const vector<string>& operands, const vector<string>& _creg_cond) {
        if (operands.size() != 3) {
            throw runtime_error("invalid format allocate instruction");
        }
        string name = operands[0];
        int x = stoi(operands[1]);
        int y = stoi(operands[2]);
        Position2D pos(x, y);

        return new InstructionAllocateMagic(name, pos, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position2D>&,
        const QubitPlane& qubit_plane,
        const map<string, Position2D>&
    ) const {
        InstructionResolveInfo info;
        info.success = qubit_plane.is_available(pos);
        info.new_magic_factory.push_back(make_pair(qreg_names[0], pos));
        return info;
    }
};
