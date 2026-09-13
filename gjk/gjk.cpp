#include "gjk.hpp"

#include <cmath>

Point triple_product(Point a, Point b, Point c) {
    Point result = b * (a * c) - a * (b * c); //dot products in parenthesis, vector multiplication, then point subtraction
    //prevent returns of 0, 0
    if (std::abs(result.get_x()) < 1e-5f && std::abs(result.get_y()) < 1e-5f) {
        return Point(a.get_y() * -1, a.get_x());
    }
    return result;
}

Point get_simplex_point(Shape* A, Shape* B, Point d) {
    return A->support(d) - B->support(d * -1);
}

bool update_simplex(Shape* A, Shape* B, Point& a, Point& b, Point& c, int& smpx_size, Point& d) {
    if (smpx_size == 1) {
        b = a;
        a = get_simplex_point(A, B, d);
        //sanity check
        if (a * d < 0) {
            return true;
        }
        smpx_size++;
        Point AB = b - a;
        d = triple_product(b - a, a * -1, b - a);
        return false;
    }

    if (smpx_size == 2) {
        c = b;
        b = a;
        a = get_simplex_point(A, B, d);
        //sanity check
        if (a * d < 0) {
            return true;
        }
        //origin checks
        //Region AB
        Point ab_perpendicular = triple_product(c - a, b - a, b - a);
        if (ab_perpendicular * (a * -1) >= 0) {
            d = ab_perpendicular;
            return false;
        }
        //Region AC
        Point ac_perpendicular = triple_product(b - a, c - a, c - a);
        if (ac_perpendicular * (a * -1) >= 0) {
            d = ac_perpendicular;
            b = c;
            return false;
        }
        smpx_size++;
        return true;
    }
    return true;
}

std::vector<Simplex> gjk(Shape* A, Shape* B) {
    std::vector<Simplex> simplexes = {};

    std::vector<Shape*> A_pieces = A->get_pieces();
    std::vector<Shape*> B_pieces = B->get_pieces();

    for (Shape* a_p : A_pieces) {
        for (Shape* b_p : B_pieces) {
            Point sp1 = get_simplex_point(a_p, b_p, (b_p->get_center() - a_p->get_center()));
            Point sp2, sp3;
            Point direction = sp1 * -1;
            int simplex_size = 1;

            bool found = update_simplex(a_p, b_p, sp1, sp2, sp3, simplex_size, direction);

            while (!found) {
                found = update_simplex(a_p, b_p, sp1, sp2, sp3, simplex_size, direction);
            }

            if (simplex_size < 3) {
                simplexes.push_back(Simplex(false));
            }

            //sort the direction
            float cross_product = (sp2.get_x() - sp1.get_x()) * (sp3.get_y() - sp1.get_y()) - ((sp2.get_y() - sp1.get_y()) * (sp3.get_x() - sp1.get_x()));
            if (cross_product < 0) {
                Point temp = sp2;
                sp2 = sp1;
                sp1 = temp;
            }

            simplexes.push_back(Simplex({sp1, sp2, sp3}));
        }
    }

    Point sp1 = get_simplex_point(A, B, (B->get_center() - A->get_center()));
    Point sp2, sp3;
    Point direction = sp1 * -1;
    int simplex_size = 1;

    bool found = update_simplex(A, B, sp1, sp2, sp3, simplex_size, direction);

    while (!found) {
        found = update_simplex(A, B, sp1, sp2, sp3, simplex_size, direction);
    }

    if (simplex_size < 3) {
        simplexes.push_back(Simplex(false));
    }

    //sort the direction
    float cross_product = (sp2.get_x() - sp1.get_x()) * (sp3.get_y() - sp1.get_y()) - ((sp2.get_y() - sp1.get_y()) * (sp3.get_x() - sp1.get_x()));
    if (cross_product < 0) {
        Point temp = sp2;
        sp2 = sp1;
        sp1 = temp;
    }

    simplexes.push_back(Simplex({sp1, sp2, sp3}));
    return simplexes;
}