#include "point.hpp"
#include "shapes.hpp"

#include <cmath>
#include <iostream>
#include <raylib.h>

//shape class, kind of a default
Shape::Shape(Point c) : center(c) {}

Point Shape::get_center() {
    return Point(0, 0);
}
Point Shape::support(Point d) {
    return Point(0, 0);
}
void Shape::draw_self() {
    return;
}

//ellipse class
Ellipse::Ellipse(Point c, float h, float v) : Shape(c), h_rad(h), v_rad(v) {}
Ellipse::Ellipse(Point c, float r) : Ellipse(c, r, r) {}
Ellipse::Ellipse(Point c) : Ellipse(c, 50, 50) {}

Point Ellipse::get_center() {
    return center;
}

Point Ellipse::support(Point d) {
    float h_sqd = h_rad * h_rad;
    float v_sqd = v_rad * v_rad;

    float d_x = d.get_x() / h_rad;
    float d_y = d.get_y() / v_rad;

    float inside_sqrt = h_sqd * (d_x * d_x) + v_sqd * (d_y * d_y);
    float denom = std::sqrt(inside_sqrt);

    if (denom == 0) {
        return Point(h_rad, 0);
    } 

    return Point(h_sqd * d_x / denom, v_sqd * d_y / denom) + center;
}

void Ellipse::move(Point p) {
    center = center + p;
}

void Ellipse::draw_self() {
    DrawEllipse((int) center.get_draw_x(), center.get_draw_y(), h_rad, v_rad, RED);
}

Polygon::Polygon() : Shape(Point(0, 0)) {}
Polygon::Polygon(std::vector<Point> p) : 
    Shape(gen_center(p)), 
    points(gen_sorted_points(p, center)), 
    draw_points(gen_draw_points(points, center)),
    pieces({ this })
{}
Polygon::Polygon(std::vector<Point> p, bool is_pieces) :
    Shape(gen_center(p)),
    points(gen_sorted_points(p, center)), 
    draw_points(gen_draw_points(points, center))
{}
Polygon::Polygon(Point c, float r, float p) : Shape(c) {
    points = gen_sorted_points(c, r, p, 0);
    draw_points = gen_draw_points(points, center);
    pieces = { this };
}
Polygon::Polygon(Point c, float r, float p, float ir) : Shape(c) {
    points = gen_sorted_points(c, r, p, ir);
    draw_points = gen_draw_points(points, center);
    pieces = { this };
}

std::vector<Shape*> Polygon::get_pieces() {
    std::vector<Shape*> p = {};
    for (const auto& pg : pieces) {
        p.push_back(pg);
    }
    return p;
}

Point Polygon::gen_center(std::vector<Point> ps) {
    float x_total = 0;
    float y_total = 0;
    for (Point p : ps) {
        x_total += p.get_x();
        y_total += p.get_y();
    }
    return Point(x_total / ps.size(), y_total / ps.size());
}

Point Polygon::get_center() {
    return center;
}

Point Polygon::support(Point d) {
    int max = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i] * d > points[max] * d) {
            max = i;
        }
    }
    return points[max];
}

std::vector<Point> Polygon::gen_sorted_points(std::vector<Point> ps, Point c) {
    std::vector<float> angles = {};
    std::vector<Point> rps = ps;
    //get angles respective to center
    for (Point p : ps) {
        angles.push_back(std::atan2(p.get_y() - c.get_y(), p.get_x() - c.get_x()));
    }
    //sort angles and points
    for (int i = 0; i < angles.size(); i++) {
        int max = 0;
        for (int k = 0; k < angles.size() - i; k++) {
            if (angles[k] > angles[max]) {
                max = k;
            }
        }

        int target = angles.size() - (i + 1);

        if (max != target) {

            float temp_a = angles[target];
            angles[target] = angles[max];
            angles[max] = temp_a;

            Point temp_p = rps[target];
            rps[target] = rps[max];
            rps[max] = temp_p;
        }
    }
    return rps;
}

std::vector<Point> Polygon::gen_sorted_points(Point c, float r, float p, float ir) {
    float angle = (2 * M_PI) / p; //distance between each point in angles
    std::vector<Point> ps;
    float star_angle = (ir) ? angle / 2 : 0;
    for (int i = 0; i < p; i++) {
        float x_offset = r * std::cos(angle * i);
        float y_offset = r * std::sin(angle * i);
        ps.push_back(Point(c.get_x() + x_offset, c.get_y() + y_offset));
        if (star_angle) {
            x_offset = ir * std::cos(angle * i + star_angle);
            y_offset = ir * std::sin(angle * i + star_angle);
            ps.push_back(Point(c.get_x() + x_offset, c.get_y() + y_offset));
        }
    }
    return ps;
}

std::vector<Vector2> Polygon::gen_draw_points(std::vector<Point> sp, Point c) {
    std::vector<Point> dp = sp;

    dp.push_back(dp.front());
    dp.insert(dp.begin(), center);

    //convert to vector2
    std::vector<Vector2> dpv2 = {};
    for (Point p : dp) {
        dpv2.push_back((Vector2)p);
    }
    return dpv2;
}

void Polygon::move(Point p) {
    center = center + p;
    for (int i = 0; i < draw_points.size(); i++) {
        if (!(i == 0 || i == draw_points.size() - 1)) {
            points[i - 1] = points[i - 1] + p;
        }
        draw_points[i].x += p.get_x();
        draw_points[i].y -= p.get_y();
    }
}

std::vector<Vector2> Polygon::get_draw_points() {
    return draw_points;
}

void Polygon::draw_self() {
    for (Polygon* p : pieces) {
        DrawTriangleFan(p->get_draw_points().data(), p->get_draw_points().size(), RED);
    }
}

Simplex::Simplex(bool e) : exists(e) {}
Simplex::Simplex(std::vector<Point> p) {
    center = gen_center(p);
    points = p;
    draw_points = gen_draw_points(p, center);
    exists = true;
}

bool Simplex::get_exists() {
    return exists;
}

void Simplex::draw_self() {
    if (!exists) return;
    DrawTriangleFan(draw_points.data(), draw_points.size(), PURPLE);
}