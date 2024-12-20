#pragma once

#include <tuple>
#include <algorithm>
#include <string>

using namespace std;

enum class RotationAngle {
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270
};

class Position3D {
public:
    const int x;
    const int y;
    const int z;
    Position3D(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    Position3D(const Position3D&) = default;
    virtual bool operator==(const Position3D& obj) const {
        return std::tie(x, y, z) == std::tie(obj.x, obj.y, obj.z);
    }
    virtual bool operator<(const Position3D& obj) const {
        return std::tie(x, y, z) < std::tie(obj.x, obj.y, obj.z);
    }
    virtual Position3D operator+(const Position3D& obj) const {
        return Position3D(x + obj.x, y + obj.y, z + obj.z);
    }
    virtual Position3D operator-(const Position3D& obj) const {
        return Position3D(x - obj.x, y - obj.y, z - obj.z);
    }
    virtual Position3D operator=(const Position3D& obj) const {
        return Position3D(obj.x, obj.y, obj.z);
    }
    virtual int length() const {
        return abs(x) + abs(y) + abs(z);
    }
    virtual Position3D rotate(RotationAngle angle) const {
        if (angle == RotationAngle::ROT_0) return Position3D(x, y, z);
        else if (angle == RotationAngle::ROT_90) return Position3D(-y, x, z);
        else if (angle == RotationAngle::ROT_180) return Position3D(-x, -y, z);
        else return Position3D(y, -x, z);
    }
    virtual string to_string() const {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")" + std::to_string(z) + ")";
    }
};