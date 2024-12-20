#pragma once

#include "instruction_base.h"

class InstructionAllocate : public InstructionBase {
private:
    Position2D pos;

public:
    InstructionAllocate(string name, Position2D _pos, const vector<string>& _creg_cond)
        : InstructionBase("Allocate", InstructionType::ALLOCATE, { name }, _creg_cond, CREG_OUT_NAME_DUMMY, false), pos(_pos) {}

    static InstructionAllocate* parse(const vector<string>& operands, const vector<string>& _creg_cond) {
        if (operands.size() != 3) {
            throw runtime_error("invalid format allocate instruction");
        }
        string name = operands[0];
        int x = stoi(operands[1]);
        int y = stoi(operands[2]);
        Position2D pos(x, y);

        return new InstructionAllocate(name, pos, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position2D>&,
        const QubitPlane& qubit_plane,
        const map<string, Position2D>&
    ) const {
        InstructionResolveInfo info;
        info.success = qubit_plane.is_available(pos);
        info.new_qubit.push_back(make_pair(qreg_names[0], pos));
        // do not lock created qubit if success
        return info;
    }

    virtual string to_string() const {
        string org = InstructionBase::to_string();
        stringstream ss;

        ss << org;
        ss << " pos: " << pos.x << " " << pos.y;
        return ss.str();
    }
};
