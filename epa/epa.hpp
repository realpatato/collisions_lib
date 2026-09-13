#pragma once

#include "point.hpp"
#include "shapes.hpp"

#include <vector>

float line_seg_distance_sqrd(Point a, Point b); //to origin
Point epa(Shape* A, Shape* B, Simplex S);