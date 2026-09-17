#include "point.hpp"
#include "constants.hpp"

#include <cmath>
#include <raylib.h>

const int half_screen_width = constants::screen_width / 2;
const int half_screen_height = constants::screen_height / 2;

Point triple_product(Point a, Point b, Point c) {
    Point result = b * (a * c) - a * (b * c); //dot products in parenthesis, vector multiplication, then point subtraction
    //prevent returns of 0, 0
    if (std::abs(result.get_x()) < 1e-5f && std::abs(result.get_y()) < 1e-5f) {
        return Point(a.get_y() * -1, a.get_x());
    }
    return result;
}

Point::Point(float x, float y) : 
    x(x), y(y), 
    draw_x(x + half_screen_width), draw_y(half_screen_height - y) 
{}
Point::Point() : x(0), y(0), draw_x(half_screen_width), draw_y(half_screen_height) {}

Point Point::operator+(const Point& other) {
    return Point(this-> x + other.x, this-> y + other.y);
}

Point Point::operator-(const Point& other) {
    return Point(this-> x - other.x, this-> y - other.y);
}

Point Point::operator*(const float& other) {
    return Point(this-> x * other, this-> y * other);
}

float Point::operator*(const Point& other) {
    return (this-> x * other.x) + (this-> y * other.y);
}

Point::operator Vector2() const {
    return Vector2{draw_x, draw_y};
}

float Point::get_x() {
    return x;
}

void Point::mod_x(float nx) {
    x += nx;
    draw_x = x + half_screen_width;
}

float Point::get_y() {
    return y;
}

void Point::mod_y(float ny) {
    y += ny;
    draw_y = half_screen_height - y;
}

float Point::get_draw_x() {
    return draw_x;
}

float Point::get_draw_y() {
    return draw_y;
}

void Point::draw_self() {
    DrawCircle((int) draw_x, (int) draw_y, 5, PURPLE);
}