#pragma once

#include "instruction_base.h"

enum class SurgeryConnectionType : int{
    X = 0,
    Y = 1,
    Z = 2
};

class InstructionSurgery : public InstructionBase{
private:
    const vector<SurgeryConnectionType> connection;
    const bool use_magic;
    const uint32_t lock_time;
    
    InstructionSurgery(
        const vector<string>& _qreg_names,
        const vector<SurgeryConnectionType>& _connection_types,
        string _creg_name_out,
        const vector<string>& _creg_cond,
        bool _use_magic,
        uint32_t _lock_time
    ) : InstructionBase(string("Surgery") + (_use_magic?"Magic" : ""), InstructionType::SURGERY, _qreg_names, _creg_cond, _creg_name_out, true),
        connection(_connection_types), use_magic(_use_magic), lock_time(_lock_time){};

    virtual PathConnectionType convert_connection(SurgeryConnectionType surgery_type) const{
        if (surgery_type == SurgeryConnectionType::X){
            return PathConnectionType::VERTICAL;
        }
        else if (surgery_type == SurgeryConnectionType::Z){
            return PathConnectionType::HORIZONTAL;
        }
        else if (surgery_type == SurgeryConnectionType::Y){
            throw runtime_error("Y connection is not supported.");
        }
        throw runtime_error("Unknown surgery type.");
    }

    virtual InstructionResolveInfo _resolve_topology_with_magic(
        const QubitSpace& qubit_space,
        Position3D org,
        PathConnectionType org_type,
        const vector<pair<Position3D, PathConnectionType>>& residual_,
        const map<string, Position3D>& factory_position
    ) const{
        InstructionResolveInfo info;
        info.success = false;
        vector<pair<Position3D, PathConnectionType>> residual(residual_);

        for (const auto& factory : factory_position){
            residual.push_back(make_pair(factory.second, PathConnectionType::BOTH));
            auto result = qubit_space.find_path(org, org_type, residual);

            residual.pop_back();
            if (result.first){
                info.success = result.first;
                info.lock_position = result.second;
                info.lock_qubits = qreg_names;
                info.lock_lifetime = lock_time;
                info.use_factory.push_back(factory.first);
                info.new_creg.push_back(creg_name_out);
                break;
            }
        }
        return info;
    }

    virtual InstructionResolveInfo _resolve_topology_without_magic(
        const QubitSpace& qubit_space,
        Position3D org,
        PathConnectionType org_type,
        const vector<pair<Position3D, PathConnectionType>>& residual
    ) const{
        auto result = qubit_space.find_path(org, org_type, residual);

        InstructionResolveInfo info;
        info.success = result.first;
        info.lock_position = result.second;
        info.lock_qubits = qreg_names;
        info.lock_lifetime = lock_time;
        info.new_creg.push_back(creg_name_out);
        return info;
    }

public:
    static InstructionSurgery* create_instruction(
        const vector<pair<string, SurgeryConnectionType>>& _qreg_name_and_type,
        const vector<string>& _creg_names,
        string creg_name_out,
        bool use_magic,
        uint32_t _lock_time
    ){
        vector<pair<string, SurgeryConnectionType>> qreg_name_and_type(_qreg_name_and_type);
        sort(qreg_name_and_type.begin(), qreg_name_and_type.end());
        vector<string> qreg_names(qreg_name_and_type.size());
        vector<SurgeryConnectionType> qreg_types(qreg_name_and_type.size());
        for (size_t i = 0; i < qreg_name_and_type.size(); ++i){
            qreg_names[i] = qreg_name_and_type[i].first;
            qreg_types[i] = qreg_name_and_type[i].second;
        }
        auto obj = new InstructionSurgery(qreg_names, qreg_types, creg_name_out, _creg_names, use_magic, _lock_time);
        return obj;
    }

    static InstructionSurgery* parse(const vector<string>& operands, const vector<string>& creg_cond, string creg_output, bool use_magic, uint32_t _lock_time){
        if (operands.size() % 2 == 1){
            throw runtime_error("invalid format surgery instruction 1");
        }
        vector<pair<string, SurgeryConnectionType>> qreg_input;
        for (size_t i = 0; i < operands.size(); i += 2){
            string name = operands[i];
            string type = operands[i + 1];
            if (type == "X"){
                qreg_input.push_back(make_pair(name, SurgeryConnectionType::X));
            }
            else if (type == "Z"){
                qreg_input.push_back(make_pair(name, SurgeryConnectionType::Z));
            }
            else if (type == "Y"){
                qreg_input.push_back(make_pair(name, SurgeryConnectionType::Y));
            }
            else{
                cout << type << endl;
                throw runtime_error("invalid format surgery instruction 2");
            }
        }
        if ((use_magic && qreg_input.size() < 1) || (!use_magic && qreg_input.size() < 2)){
            throw runtime_error("invalid format surgery instruction 3");
        }
        return InstructionSurgery::create_instruction(qreg_input, creg_cond, creg_output, use_magic, _lock_time);
    }

    virtual InstructionResolveInfo resolve_topology(
        const map<string, Position3D>& qubit_position,
        const QubitSpace& qubit_space,
        const map<string, Position3D>& factory_position
    ) const{
        auto org = qubit_position.at(qreg_names[0]);
        PathConnectionType org_type = convert_connection(connection[0]);

        vector<pair<Position3D, PathConnectionType>> residual;
        for (uint32_t idx = 1; idx < qreg_names.size(); ++idx){
            //cout << "hey there" << endl;
            Position3D trg = qubit_position.at(qreg_names[idx]);
            //cout << "target;" << "(" << trg.x << "," << trg.y << "," << trg.z << ")" << endl;
            PathConnectionType trg_type = convert_connection(connection[idx]);
            residual.push_back(make_pair(trg, trg_type));
        }
        if (use_magic){
            return _resolve_topology_with_magic(qubit_space, org, org_type, residual, factory_position);
        }
        else{
            return _resolve_topology_without_magic(qubit_space, org, org_type, residual);
        }
    }
    virtual string to_string() const{
        string org = InstructionBase::to_string();
        stringstream ss;

        ss << org;
        ss << " lock:" << lock_time;
        if (use_magic) ss << " magic";
        return ss.str();
    }
};