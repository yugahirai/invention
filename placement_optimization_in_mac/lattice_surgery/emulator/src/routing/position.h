#pragma once

#include <tuple>
#include <algorithm>

enum class RotationAngle {
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270
};

class Position2D {
public:
    const int x;
    const int y;
    Position2D(int _x, int _y) : x(_x), y(_y) {}
    Position2D(const Position2D&) = default;
    virtual bool operator==(const Position2D& obj) const {
        return std::tie(x, y) == std::tie(obj.x, obj.y);
    }
    virtual bool operator<(const Position2D& obj) const {
        return std::tie(x, y) < std::tie(obj.x, obj.y);
    }
    virtual Position2D operator+(const Position2D& obj) const {
        return Position2D(x + obj.x, y + obj.y);
    }
    virtual Position2D operator-(const Position2D& obj) const {
        return Position2D(x - obj.x, y - obj.y);
    }
    virtual Position2D operator=(const Position2D& obj) const {
        return Position2D(obj.x, obj.y);
    }
    virtual int length() const {
        return abs(x) + abs(y);
    }
    virtual Position2D rotate(RotationAngle angle) const {
        if (angle == RotationAngle::ROT_0) return Position2D(x, y);
        else if (angle == RotationAngle::ROT_90) return Position2D(-y, x);
        else if (angle == RotationAngle::ROT_180) return Position2D(-x, -y);
        else return Position2D(y, -x);
    }
    virtual string to_string() const {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
};



