#pragma once

#include "common.h"
#include "qubit_plane.h"
#include "instruction_allocate.h"
#include "instruction_allocate_magic.h"
#include "instruction_dest_meas.h"
#include "instruction_meas_twist.h"
#include "instruction_hadamard.h"
#include "instruction_surgery.h"
#include "instruction_release.h"
#include "instruction_phase_twist.h"

enum class ParseMode {
    OPERAND,
    OUTPUT,
    CONDITION
};

const string _SYM_OPERAND = "_op_";
const string _SYM_OUTPUT = "_out_";
const string _SYM_CONDITION = "_cond_";

static InstructionBase* parse_command(string line) {
    stringstream ss(line);
    string inst_name;
    ss >> inst_name;
    vector<string> operands;
    string creg_output = CREG_OUT_NAME_DUMMY;
    vector<string> creg_cond;
    ParseMode mode = ParseMode::OPERAND;
    while (!ss.eof()) {
        string term;
        ss >> term;
        if (term == _SYM_OPERAND) {
            mode = ParseMode::OPERAND;
        }
        else if (term == _SYM_OUTPUT) {
            mode = ParseMode::OUTPUT;
        }
        else if (term == _SYM_CONDITION) {
            mode = ParseMode::CONDITION;
        }else{
            if (mode == ParseMode::OPERAND)
                operands.push_back(term);
            else if (mode == ParseMode::CONDITION)
                creg_cond.push_back(term);
            else if (mode == ParseMode::OUTPUT) {
                if (creg_output == CREG_OUT_NAME_DUMMY)
                    creg_output = term;
                else
                    throw runtime_error("multiple creg output is assigned");
            }
        }
    }

    if (inst_name == "ALLOCATE") {
        if (creg_output != CREG_OUT_NAME_DUMMY)
            throw runtime_error("Instruction ALLOCATE does not have creg output, but creg output is assigned");
        return InstructionAllocate::parse(operands, creg_cond);
    }
    else if (inst_name == "MAGIC") {
        if (creg_output != CREG_OUT_NAME_DUMMY)
            throw runtime_error("Instruction MAGIC does not have creg output, but creg output is assigned");
        return InstructionAllocateMagic::parse(operands, creg_cond);
    }
    else if (inst_name == "DEST_MEAS") {
        return InstructionDestructiveMeasure::parse(operands, creg_cond, creg_output);
    }
    else if (inst_name == "MEAS_TWIST") {
        return InstructionMeasurementTwist::parse(operands, creg_cond, creg_output);
    }
    else if (inst_name == "PHASE_TWIST") {
        return InstructionPhaseTwist::parse(operands, creg_cond, creg_output);
    }
    else if (inst_name == "HADAMARD") {
        if (creg_output != CREG_OUT_NAME_DUMMY)
            throw runtime_error("Instruction HADAMARD does not have creg output, but creg output is assigned");
        return InstructionHadamard::parse(operands, creg_cond);
    }
    else if (inst_name == "SURGERY") {
        return InstructionSurgery::parse(operands, creg_cond, creg_output, false, 1);
    }
    else if (inst_name == "MAGIC_SURGERY") {
        return InstructionSurgery::parse(operands, creg_cond, creg_output, true, 1);
    }
    else if (inst_name == "CNOT_SURGERY") {
        return InstructionSurgery::parse(operands, creg_cond, creg_output, false, 2);
    }
    else if (inst_name == "RELEASE") {
        if (creg_output != CREG_OUT_NAME_DUMMY)
            throw runtime_error("Instruction ALLOCATE does not have creg output, but creg output is assigned");
        return InstructionRelease::parse(operands, creg_cond);
    }
    else {
        throw runtime_error("Unknown instrcution name");
    }
}
