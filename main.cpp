#include <iostream>
#include <string>
#include <raylib.h>

#include "constants.hpp"
#include "collision_objects.hpp"

int main() {
    const int screen_width = constants::screen_width;
    const int screen_height = constants::screen_height;

    InitWindow(screen_width, screen_height, "basic window");

    //Ellipses
    c_o::add_shape(new Ellipse(Point(-10, 125), 20, 20));
    c_o::add_shape(new Ellipse(Point(10, 100), 20, 20));

    std::vector<Point> ps1 = {Point(-30, -145), Point(10, -145), Point(10, -105), Point(-30, -105)};
    std::vector<Point> ps2 = {Point(-10, -170), Point(30, -170), Point(30, -130), Point(-10, -130)};

    c_o::add_shape(new Polygon(ps1));
    c_o::add_shape(new Polygon(ps2));

    c_o::add_shape(new Polygon(Point(100, 100), 30, 5));
    c_o::add_shape(new Polygon(Point(120, 75), 30, 8));

    Shape* star = new Polygon(Point(-250, -125), 30, 5, 10);

    while (!WindowShouldClose()) {

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, BLACK);
            DrawLine(0, screen_height/2, screen_width, screen_height/2, BLACK);
            c_o::collide();
            c_o::draw();

            star->draw_self();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}