#pragma once

#include "point.hpp"

#include <vector>

class Shape {
    protected: 
        Point center;
    public:
        Shape(Point c);
        virtual ~Shape() = default;
        virtual Point get_center();
        virtual Point support(Point d);
        virtual std::vector<Shape*> get_pieces() {return {};};
        virtual void move(Point p) {return;};

        virtual void draw_self();
};

class Ellipse : public Shape {
    private:
        float h_rad;
        float v_rad;
    public:
        Ellipse(Point c, float h, float v);
        Ellipse(Point c, float r);
        Ellipse(Point c);
        Point get_center() override;
        Point support(Point d) override;
        void move(Point p) override;//moves object by given vector
        
        void draw_self() override;
};

class Polygon : public Shape {
    protected:
        std::vector<Point> points;
        std::vector<Vector2> draw_points;
        std::vector<Polygon*> pieces;
    public:
        Polygon(); //default constructor, so simplex can work
        Polygon(std::vector<Point> p);
        Polygon(std::vector<Point> p, bool is_pieces);
        Polygon(Point c, float r, float p);
        Polygon(Point c, float r, float p, float ir);
        Point gen_center(std::vector<Point> p);
        Point get_center() override;
        Point support(Point d) override;
        std::vector<Point> gen_sorted_points(std::vector<Point> p, Point c); //sorts user list of points
        std::vector<Point> gen_sorted_points(Point c, float r, float p, float ir); //used for default polygons
        std::vector<Shape*> get_pieces() override;
        std::vector<Point> get_points() {return points;};
        void move(Point p) override;

        std::vector<Vector2> gen_draw_points(std::vector<Point> sp, Point c);
        std::vector<Vector2> get_draw_points();
        virtual void draw_self() override;
};

class Simplex : public Polygon {
    private:
        bool exists;
    public:
        Simplex(bool e);
        Simplex(std::vector<Point> p);
        bool get_exists();

        void draw_self() override;
};