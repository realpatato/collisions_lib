#include "collision_objects.hpp"
#include "gjk.hpp"
#include "epa.hpp"

namespace c_o {
    std::vector<Shape*> objects;

    void add_shape(Shape* s) {
        objects.push_back(s);
    }

    void collide() {
        for (int i = 0; i < objects.size() - 1; i++) {
            for (int k = i + 1; k < objects.size(); k++) {
                std::vector<Simplex> simplexes = gjk(objects[i], objects[k]);
                Point displacement = Point(0, 0);
                for (Simplex& s : simplexes) {
                    displacement = displacement + epa(objects[i], objects[k], s);
                }
                displacement = Point(displacement.get_x() / 2, displacement.get_y() / 2);
                objects[i]->move(displacement);
                objects[k]->move(displacement * -1);
            }
        }
    }

    void draw() {
        for (Shape* s : objects) {
            s->draw_self();
        }
    }
}