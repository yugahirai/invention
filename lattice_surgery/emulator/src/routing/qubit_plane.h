#pragma once

#include "qubit_plane_search.h"

class PlaneBlock {
private:
    bool is_parent;
    bool is_magic;
    string parent_name;
    int lock_lifetime;
public:
    PlaneBlock()
        : is_parent(false), is_magic(false), parent_name(""), lock_lifetime(0) {
    }
    virtual bool is_locked() const {
        return lock_lifetime > 0 || is_parent;
    }
    virtual void set_children_block(int32_t lifetime) {
        is_parent = false;
        lock_lifetime = lifetime;
    }
    virtual void set_parent_block() {
        is_parent = true;
    }
    virtual void set_magic_block() {
        is_parent = true;
        is_magic = true;
    }
    virtual void unset_parent_block() {
        is_parent = false;
    }
    virtual bool get_is_parent() const {
        return is_parent;
    }
    virtual bool get_is_magic() const {
        return is_magic;
    }
    virtual void step() {
        if(lock_lifetime > 0)
            lock_lifetime -= 1;
    }
};

class QubitPlane {
private:
    const int32_t m_width;
    const int32_t m_height;
    vector<vector<PlaneBlock>> field;
    virtual PlaneBlock get(const Position2D& pos) const {
        return field.at(pos.y).at(pos.x);
    }
    virtual SearchField create_search_map() const {
        SearchField sfield(m_width, m_height);
        for (int32_t y = 0; y < m_height; ++y) {
            for (int32_t x = 0; x < m_width; ++x) {
                if (field.at(y).at(x).is_locked()) {
                    sfield.set_status(Position2D(x, y), NodeStatus::USED, PathConnectionType::BOTH);
                }
            }
        }
        return sfield;
    }

    virtual pair<bool, vector<Position2D>> find_space_rotate(Position2D st, vector<Position2D> relative_space, RotationAngle angle) const {
        vector<Position2D> pos_list;
        for (const auto& relative : relative_space) {
            Position2D rot_angle = relative.rotate(angle);
            auto pos = st + rot_angle;
            if (!is_available(pos)) {
                return make_pair(false, vector<Position2D>());
            }
            pos_list.push_back(pos);
        }
        return make_pair(true, pos_list);
    }

public:
    QubitPlane(int32_t width, int32_t height) : m_width(width), m_height(height) {
        field = vector<vector<PlaneBlock>>(height, vector<PlaneBlock>(width));
    }
    virtual bool is_available(Position2D pos) const {
        if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height) {
            return false;
        }
        auto block = get(pos);
        if (block.is_locked()) {
            return false;
        }
        return true;
    }
    virtual void step() {
        for (int32_t y = 0; y < m_height; ++y) {
            for (int32_t x = 0; x < m_width; ++x) {
                field.at(y).at(x).step();
            }
        }
    }
    virtual pair<bool, vector<Position2D>> find_path(Position2D st, PathConnectionType con_st, Position2D en, PathConnectionType con_en) const {
        return find_path(st, con_st, { make_pair(en, con_en) });
    }
    virtual pair<bool, vector<Position2D>> find_path(Position2D st, PathConnectionType con_st, vector<pair<Position2D, PathConnectionType>> en_list) const {
        auto search_map = create_search_map();
        search_map.set_status(st, NodeStatus::START, con_st);
        for (const auto& pair : en_list) {
            search_map.set_status(pair.first, NodeStatus::GOAL, pair.second);
        }
        auto result = search_map.bfs();
        return result;
    }

    virtual pair<bool, vector<Position2D>> find_space_around(Position2D st, vector<Position2D> relative_space) const {
        vector<RotationAngle> rotation_list = { RotationAngle::ROT_0, RotationAngle::ROT_90, RotationAngle::ROT_180, RotationAngle::ROT_270 };
        for (auto rotation : rotation_list) {
            auto res = find_space_rotate(st, relative_space, rotation);
            if (res.first) {
                return res;
            }
        }
        return make_pair(false, vector<Position2D>());
    }
    virtual pair<bool, vector<Position2D>> find_space_leftright(Position2D st, vector<Position2D> relative_space) const {
        vector<RotationAngle> rotation_list = { RotationAngle::ROT_0, RotationAngle::ROT_180};
        for (auto rotation : rotation_list) {
            auto res = find_space_rotate(st, relative_space, rotation);
            if (res.first) {
                return res;
            }
        }
        return make_pair(false, vector<Position2D>());
    }
    virtual void allocate_parent_block(Position2D position) {
        field[position.y][position.x].set_parent_block();
    }
    virtual void allocate_magic_block(Position2D position) {
        field[position.y][position.x].set_magic_block();
    }
    virtual void deallocate_parent_block(Position2D position) {
        field[position.y][position.x].unset_parent_block();
    }
    virtual void allocate_children_block(Position2D position, int32_t lifetime) {
        field[position.y][position.x].set_children_block(lifetime);
    }
    virtual string get_floor_string() const {
        stringstream ss;
        int count = 0;
        int vertical_size = 43;
        vector<string> line_list(vertical_size);
        while (count < m_height){ 
            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
                    if (field[y][x].is_locked()) {
                        if (field[y][x].get_is_parent()) {
                            if(field[y][x].get_is_magic())
                                ss << "M";
                            else
                                ss << "Q";
                        }
                        else {
                            ss << "*";
                        }
                    }
                    else {
                        ss << ".";
                    }
                }
                line_list[y%vertical_size] = line_list[y%vertical_size] + "      " + ss.str();
                count++;
                ss.str("");
                if (count >= m_height) break;
            }
        }
        // while (count < m_height){
        //     for (int y = count; y < vertical_size; ++y) {
        //         if (y >= m_height) break;
        //         for (int x = 0; x < m_width; ++x) {
        //             if (field[y][x].is_locked()) {
        //                 if (field[y][x].get_is_parent()) {
        //                     if(field[y][x].get_is_magic())
        //                         ss << "M";
        //                     else
        //                         ss << "Q";
        //                 }
        //                 else {
        //                     ss << "*";
        //                 }
        //             }
        //             else {
        //                 ss << ".";
        //             }
        //         }
        //         line_list[y%vertical_size] = line_list[y%vertical_size] + "      " + ss.str();
        //         count++;
        //         ss.str("");
        //         if (count == m_height){
        //             break;
        //         }
        //     }
        // }

        stringstream out;
        for (int i = 0; i < min(m_height, vertical_size); ++i){
            out << line_list[i] << std::endl;
        }

        out << std::endl;

        return out.str();
    }
};
