#pragma once

#include "instruction_base.h"

class InstructionPhaseTwist : public InstructionBase {
private:
    vector<Position3D> relative_space;
    const uint32_t lifetime = 2;

public:
    InstructionPhaseTwist(string qreg_name, string creg_name, const vector<string>& _creg_cond)
        : InstructionBase("PhaseTwist", InstructionType::MEAS_TWIST, { qreg_name }, _creg_cond, creg_name, true) {
        relative_space.push_back(Position3D(1, 0, 0));
    }

    static InstructionPhaseTwist* parse(const vector<string>& operands, const vector<string>& _creg_cond, string creg_output) {
        if (operands.size() != 1) {
            throw runtime_error("invalid format phase twist instruction");
        }
        return new InstructionPhaseTwist(operands[0], creg_output, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position3D>& qubit_position,
        const QubitSpace& qubit_space,
        const map<string, Position3D>&
    ) const {

        auto pos = qubit_position.at(qreg_names[0]);
        auto result = qubit_space.find_space_leftright(pos, relative_space);
        InstructionResolveInfo info;
        info.success = result.first;
        info.lock_position = result.second;
        info.lock_qubits = qreg_names;
        info.new_creg.push_back(creg_name_out);
        info.lock_lifetime = lifetime;
        return info;
    }
};
