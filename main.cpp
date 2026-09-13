#include <iostream>
#include <string>
#include <raylib.h>

#include "point.hpp"
#include "shapes.hpp"
#include "constants.hpp"
#include "gjk.hpp"
#include "epa.hpp"

int main() {
    const int screen_width = constants::screen_width;
    const int screen_height = constants::screen_height;

    InitWindow(screen_width, screen_height, "basic window");

    //touching
    Shape* e1 = new Ellipse(Point(-10, 125), 20, 20);
    Shape* e2 = new Ellipse(Point(10, 100), 20, 20);

    std::vector<Simplex> s = gjk(e1, e2);

    std::vector<Point> ps1 = {Point(-30, -145), Point(10, -145), Point(10, -105), Point(-30, -105)};
    std::vector<Point> ps2 = {Point(-10, -170), Point(30, -170), Point(30, -130), Point(-10, -130)};
    
    Shape* p1 = new Polygon(ps1);
    Shape* p2 = new Polygon(ps2);

    std::vector<Simplex> s2 = gjk(p1, p2);

    Shape* p3 = new Polygon(Point(100, 100), 30, 5);
    Shape* p4 = new Polygon(Point(120, 75), 30, 8);

    std::vector<Simplex> s3 = gjk(p3, p4);
    Point p;
    for (Simplex s : s3) {
        p = epa(p3, p4, s);
    }

    p3->move(p);

    Shape* star = new Polygon(Point(-250, -125), 30, 5, 10);

    while (!WindowShouldClose()) {

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, BLACK);
            DrawLine(0, screen_height/2, screen_width, screen_height/2, BLACK);
            e1->draw_self();
            e2->draw_self();

            p1->draw_self();
            p2->draw_self();

            p3->draw_self();
            p4->draw_self();

            star->draw_self();

            for (Simplex s : s3) {
                s.draw_self();
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}