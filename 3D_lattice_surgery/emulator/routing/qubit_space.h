#pragma once

#include "common.h"
#include "position.h"
#include "qubit_space_search.h"



class SpaceBlock{
private:
    bool is_parent;
    bool is_magic;
    string parent_name;
    int lock_lifetime;

public:
    SpaceBlock() : is_parent(false), is_magic(false), parent_name(""), lock_lifetime(0){}

    virtual void step(){
        if (lock_lifetime > 0){
            lock_lifetime -= 1;
        }
    }

    virtual bool is_locked() const{
        return lock_lifetime > 0 || is_parent;
    }

    virtual void set_children_block(int32_t lifetime) {
        is_parent = false;
        lock_lifetime = lifetime;
    }

    virtual void set_parent_block(){
        is_parent = true;
    }
    
    virtual void set_magic_block(){
        is_parent = true;
        is_magic = true;
    }

    virtual void unset_parent_block() {
        is_parent = false;
    }

    virtual bool get_is_parent() const{
        return is_parent;
    }

    virtual bool get_is_magic() const{
        return is_magic;
    }
};

class  QubitSpace{
private:
    const int32_t m_width;
    const int32_t m_height;
    const int32_t m_depth;
    vector<vector<vector<SpaceBlock>>> field;

    virtual pair<bool, vector<Position3D>> find_space_rotate(Position3D st, vector<Position3D> relative_space, RotationAngle angle) const{
        vector<Position3D> pos_list;
        for (const auto& relative : relative_space){
            Position3D rot_angle = relative.rotate(angle);
            auto pos = st + rot_angle;
            if (!is_available(pos)){
                return make_pair(false, vector<Position3D>());
            }
            pos_list.push_back(pos);
        }
        return make_pair(true, pos_list);
    }

    virtual SearchField create_search_map() const{
        SearchField sfield(m_width, m_height, m_depth);
        for (int32_t z = 0; z < m_depth; ++z){
            for (int32_t y = 0; y < m_height; ++y){
                for (int32_t x = 0; x < m_width; ++x){
                    if (field.at(z).at(y).at(x).is_locked()){
                        sfield.set_status(Position3D(x, y, z), NodeStatus::USED, PathConnectionType::BOTH);
                    }
                }
            }
        }
        return sfield;
    }
    
public:
    QubitSpace(int32_t width, int32_t height, int32_t depth) : m_width(width), m_height(height), m_depth(depth){
        field = vector<vector<vector<SpaceBlock>>>(depth, vector<vector<SpaceBlock>>(height, vector<SpaceBlock>(width)));
        cout << "field_size: " << field.size() << " " << field[0].size() << " " << field[0][0].size() << endl;
    }

    virtual void step(){
        for (int32_t z = 0; z < m_depth; ++z){
            for (int32_t y = 0; y < m_height; ++y){
                for (int32_t x = 0; x < m_width; ++x){
                    field.at(z).at(y).at(x).step();
                    //cout << "added_space: " << "(" << x << "," << y << "," << z << ")" << endl;
                }
            }
        }
    }

    virtual bool is_available(Position3D pos) const{
        if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height || pos.z < 0 || pos.z >= m_depth){
            return false;
        }
        auto block = get(pos);
        if (block.is_locked()){
            return false;
        }
        return true;
    }

    virtual pair<bool, vector<Position3D>> find_path(Position3D st, PathConnectionType con_st, Position3D en, PathConnectionType con_en) const{
        return find_path(st, con_st, { make_pair(en, con_en) });
    }

    virtual pair<bool, vector<Position3D>> find_path(Position3D st, PathConnectionType con_st, vector<pair<Position3D, PathConnectionType>> en_list) const{
        auto search_map = create_search_map();
        search_map.set_status(st, NodeStatus::START, con_st);
        for (const auto& pair : en_list){
            //cout << "set goal" << "(" << pair.first.x << "," << pair.first.y << "," << pair.first.z << ")" << endl;
            search_map.set_status(pair.first, NodeStatus::GOAL, pair.second);
        }
        auto result = search_map.bfs();
        return result;
    }

    virtual SpaceBlock get(const Position3D& pos) const{
        return field.at(pos.z).at(pos.y).at(pos.x);
    }

    virtual pair<bool, vector<Position3D>> find_space_around(Position3D st, vector<Position3D> relative_space) const{
        vector<RotationAngle> rotation_list = { RotationAngle::ROT_0, RotationAngle::ROT_90, RotationAngle::ROT_180, RotationAngle::ROT_270 };
        for (auto rotation : rotation_list){
            auto res = find_space_rotate(st, relative_space, rotation);
            if (res.first){
                return res;
            }
        }
        return make_pair(false, vector<Position3D>());
    }

    virtual pair<bool, vector<Position3D>> find_space_leftright(Position3D st, vector<Position3D> relative_space) const{
        vector<RotationAngle> rotation_list = {RotationAngle::ROT_0, RotationAngle::ROT_180 };
        for (auto rotation : rotation_list){
            auto res = find_space_rotate(st, relative_space, rotation);
            if (res.first){
                return res;
            }
        }
        return make_pair(false, vector<Position3D>());
    }


    virtual void allocate_parent_block(Position3D position){
        field[position.z][position.y][position.x].set_parent_block();
    }

    virtual void allocate_magic_block(Position3D position){
        field[position.z][position.y][position.x].set_magic_block();
    }

    virtual void deallocate_parent_block(Position3D position) {
        field[position.z][position.y][position.x].unset_parent_block();
    }

    virtual void allocate_children_block(Position3D position, int32_t lifetime) {
        field[position.z][position.y][position.x].set_children_block(lifetime);
    }

    virtual string get_floor_string() const{
        stringstream ss;
        string spaces(m_width - 5, ' ');

        

        for (int floor_num = 0; floor_num < m_depth; ++floor_num){
            cout << "FLOOR  " << floor_num << spaces;
        }

        cout << endl;

        for (int y = 0; y < m_height; ++y){
            for (int z = 0; z < m_depth; ++z){
                for (int x = 0; x < m_width; ++x){
                    if (field[z][y][x].is_locked()){
                        if (field[z][y][x].get_is_parent()){
                            if (field[z][y][x].get_is_magic()){
                                ss << "M";
                            }
                            else{
                                ss << "Q";
                            }
                        }
                        else{
                            ss << "*";
                        }
                    }
                    else{
                        ss << ".";
                    }
                }
                ss << "   ";
            }
            ss << endl;
        }
    return ss.str();
    }
};