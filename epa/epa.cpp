#include "epa.hpp"
#include <cmath>

float line_seg_distance_sqrd(Point a, Point b) {
    float numerator = b.get_x() * a.get_y() - b.get_y() * a.get_x();
    float y_change_sqrd = (b.get_y() - a.get_y()) * (b.get_y() - a.get_y());
    float x_change_sqrd = (b.get_x() - a.get_x()) * (b.get_x() - a.get_x());
    float denominator = y_change_sqrd + x_change_sqrd;
    return numerator * numerator / denominator; //avoids sqrt and abs
}

Point epa(Shape* A, Shape* B, Simplex S) {
    if (!S.get_exists()) { //not colliding
        return Point(0, 0);
    }
    std::vector<Point> points = S.get_points();
    Point line;
    float numerator, denominator;
    for (int iteration = 0; iteration < 64; iteration++) {
        std::vector<int> closest_indexes = {(int)points.size() - 1, 0};
        float closest_dist = line_seg_distance_sqrd(points[closest_indexes[0]], points[closest_indexes[1]]); //start with base line
        for (int i = 0; i < points.size() - 1; i++) {
            float dist = line_seg_distance_sqrd(points[i], points[i + 1]);
            if (dist < closest_dist) {
                closest_indexes = {i, i + 1};
                closest_dist = dist;
            }
        }
        line = points[closest_indexes[1]] - points[closest_indexes[0]];

        denominator = (line.get_x() * line.get_x()) + (line.get_y() * line.get_y());
        float line_len = std::sqrt(denominator);

        Point direction = Point(-line.get_y() / line_len, line.get_x() / line_len);
        Point support = A->support(direction) - B->support(direction * -1);

        Point a = points[closest_indexes[0]];
        Point b = points[closest_indexes[1]];
        numerator = b.get_x() * a.get_y() - b.get_y() * a.get_x();

        //check if support will expand polytope
        if (support * direction - points[closest_indexes[0]] * direction > 0.0001f) { //buffer for small errors
            //expand
            if (closest_indexes[1] == 0) {
                points.push_back(support);
            } else {
                points.insert(points.begin() + closest_indexes[1], support);
            }
        } else {
            //exit
            break;
        }
    }
    numerator = std::abs(numerator);
    return Point(-line.get_y() * (numerator / denominator), line.get_x() * (numerator / denominator));
}