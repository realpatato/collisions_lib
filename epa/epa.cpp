#include "epa.hpp"
#include <cmath>

Point closest_point_on_segment(Point a, Point b) {
    Point line_ab = b - a;
    float ab_len_sqrd = line_ab * line_ab;

    if (ab_len_sqrd < 0.0001f) {
        return a;
    }

    //enforces considering only the line segment
    float t = (a * -1) * line_ab / ab_len_sqrd;
    if (t <= 0) {
        return a;
    }
    if (t >= 1) {
        return b;
    }

    return a + line_ab * t;
}

float line_seg_distance_sqrd(Point a, Point b) {
    Point cp = closest_point_on_segment(a, b);
    return cp * cp;
}

Point epa(Shape* A, Shape* B, Simplex& S) {
    if (!S.get_exists()) { //not colliding
        return Point(0, 0);
    }
    std::vector<Point> points = S.get_points();
    Point line, direction;
    float numerator, denominator;
    std::vector<int> closest_indexes;
    for (int iteration = 0; iteration < 32; iteration++) {
        closest_indexes = {(int)points.size() - 1, 0};
        float closest_dist = line_seg_distance_sqrd(points[closest_indexes[0]], points[closest_indexes[1]]); //start with base line
        for (int i = 0; i < points.size() - 1; i++) {
            float dist = line_seg_distance_sqrd(points[i], points[i + 1]);
            if (dist < closest_dist - 1e-7f) {
                closest_indexes = {i, i + 1};
                closest_dist = dist;
            }
        }
        line = points[closest_indexes[1]] - points[closest_indexes[0]];

        denominator = std::sqrt((line.get_x() * line.get_x()) + (line.get_y() * line.get_y()));
        if (denominator <= 1e-4f) {
            break;
        }

        direction = Point(-line.get_y() / denominator, -line.get_x() / denominator);
        Point support = A->support(direction) - B->support(direction * -1);

        Point a = points[closest_indexes[0]];
        Point b = points[closest_indexes[1]];
        numerator = b.get_x() * a.get_y() - b.get_y() * a.get_x();

        //check if support will expand polytope
        if (support * direction - a * direction >= -1e-6f) { //buffer for small errors
            bool dupe = false;
            for (Point& p : points) {
                Point diff = p - support;
                if ((diff * diff) <= 1e-6f) {
                    dupe = true;
                    break;
                }
            }

            if (dupe) {
                break;
            }

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
    Point cp = closest_point_on_segment(points[closest_indexes[0]], points[closest_indexes[1]]);
    float depth = std::sqrt(cp * cp);
    return direction * depth;
}