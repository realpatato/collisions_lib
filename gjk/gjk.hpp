#pragma once

#include "point.hpp"
#include "shapes.hpp"

Point triple_product(Point a, Point b, Point c);
Point get_simplex_point(Shape* A, Shape* B, Point d);
bool update_simplex(Shape* A, Shape* B, Point& a, Point& b, Point& c, int& smpx_size, Point& d);
std::vector<Simplex> gjk(Shape* A, Shape* B);