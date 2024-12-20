#pragma once

#include "instruction_base.h"

class InstructionRelease : public InstructionBase {
private:

public:
    InstructionRelease(string qreg_name, const vector<string>& _creg_cond)
        : InstructionBase("Release", InstructionType::DESTRUCTIVE_MEASURE, { qreg_name }, _creg_cond, CREG_OUT_NAME_DUMMY, true)
    {}

    static InstructionRelease* parse(const vector<string>& operands, const vector<string>& _creg_cond) {
        if (operands.size() != 1) {
            throw runtime_error("invalid format release");
        }
        string qreg_name = operands[0];
        return new InstructionRelease(qreg_name, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position3D>& qubit_position,
        const QubitSpace&,
        const map<string, Position3D>&
    ) const {
        InstructionResolveInfo info;
        info.success = (qubit_position.find(qreg_names[0]) != qubit_position.end());
        if (info.success) {
            // Do not remove, do not lock qubit if success
            info.remove_qubit.push_back(qreg_names[0]);
        }
        return info;
    }
};