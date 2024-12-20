#pragma once

#include "instruction_base.h"

enum class DestructiveMeasurementBasis{
    X, Z
};

class InstructionDestructiveMeasure : public InstructionBase{
private:
    DestructiveMeasurementBasis basis;

public:
    InstructionDestructiveMeasure(string qreg_name, DestructiveMeasurementBasis _basis, string _creg_name_out, const vector<string>& _creg_cond)
        : InstructionBase("DestMeas", InstructionType::DESTRUCTIVE_MEASURE, { qreg_name }, _creg_cond, _creg_name_out, true), basis(_basis){};
    
    static InstructionDestructiveMeasure* parse(const vector<string>& operands, const vector<string>& _creg_cond, string creg_output){
        if (operands.size() != 2){
            throw runtime_error("invalid format destructive measurement.");
        }
        string qreg_name = operands[0];
        string meas_basis_str = operands[1];
        DestructiveMeasurementBasis basis;
        if (meas_basis_str == "X") basis = DestructiveMeasurementBasis::X;
        else if (meas_basis_str == "Z") basis = DestructiveMeasurementBasis::Z;
        else{
            throw runtime_error("invalid format destructive measurement.");
        }

        return new InstructionDestructiveMeasure(qreg_name, basis, creg_output, _creg_cond);
    }

    virtual InstructionResolveInfo resolve_topology(const map<string, Position3D>& qubit_position, const QubitSpace&, const map<string, Position3D>&) const{
        InstructionResolveInfo info;
        info.success = (qubit_position.find(qreg_names[0]) != qubit_position.end());
        if (info.success){
            info.new_creg.push_back(creg_name_out);
        }
    return info;
    }
};