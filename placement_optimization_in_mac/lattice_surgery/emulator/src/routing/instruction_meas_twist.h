#pragma once

#include "instruction_base.h"

class InstructionMeasurementTwist : public InstructionBase {
private:
    vector<Position2D> relative_space;
    const uint32_t lifetime = 2;

public:
    InstructionMeasurementTwist(string qreg_name, string creg_name, const vector<string>& _creg_cond)
        : InstructionBase("MeasTwist", InstructionType::MEAS_TWIST, { qreg_name }, _creg_cond, creg_name, true) {
        relative_space.push_back(Position2D(0, 1));
        relative_space.push_back(Position2D(1, 1));
        relative_space.push_back(Position2D(1, 0));
    }

    static InstructionMeasurementTwist* parse(const vector<string>& operands, const vector<string>& _creg_cond, string creg_output) {
        if (operands.size() != 1) {
            throw runtime_error("invalid format meas twist instruction");
        }
        return new InstructionMeasurementTwist(operands[0], creg_output, _creg_cond);
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
        info.new_creg.push_back(creg_name_out);
        info.lock_lifetime = lifetime;
        return info;
    }
};



