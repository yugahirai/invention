#pragma once

#include "instruction_base.h"

class InstructionHadamard : public InstructionBase {
private:
    vector<Position2D> relative_space;
    const uint32_t lifetime = 3;

public:
    InstructionHadamard(string qreg_name, const vector<string>& _creg_cond)
        : InstructionBase("Hadamard", InstructionType::HADAMARD, { qreg_name }, _creg_cond, CREG_OUT_NAME_DUMMY, true) {
        relative_space.push_back(Position2D(0, 1));
    }

    static InstructionHadamard* parse(const vector<string>& operands, const vector<string>& _creg_cond) {
        if (operands.size() != 1) {
            throw runtime_error("invalid format hadamard instruction");
        }
        return new InstructionHadamard(operands[0], _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position2D>& qubit_position,
        const QubitPlane& qubit_plane,
        const map<string, Position2D>&
    ) const {

        auto pos = qubit_position.at(qreg_names[0]);
        auto result = qubit_plane.find_space_around(pos, relative_space);
        InstructionResolveInfo info;
        info.success = result.first;
        info.lock_position = result.second;
        info.lock_qubits = qreg_names;
        info.lock_lifetime = lifetime;
        return info;
    }
};



