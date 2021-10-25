/**
 * @file test-template-1.cpp
 * @author Giovanni Curiel
 * @brief Test for template
 * @version 0.1
 * @date 2021-10-23
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <list>

struct Logger {
    bool isEnabled;
    template<typename T>
    Logger & operator<<(const T &obj) {
        if (isEnabled) {
            std::cout << obj;
        }
        return *this;
    }
};

Logger logger({true});

bool isEven(u_int32_t n) { return n % 2 == 1;}

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum ProjectionSide {
    NO_PROJECTION,
    TO_THE_LEFT,
    TO_THE_RIGHT
};

struct Edge {
    Point p1;
    Point p2;
    bool isWithinRange(const Point & p) const {
        return ((p.x >= p1.x && p.x <= p2.x) || ( p.x >= p2.x && p.x <= p1.x)) &&
        ((p.y >= p1.y && p.y <= p2.y) || (p.y >= p2.y && p.y <= p1.y));
    }

    bool isWithinXRange(const Point & p) const {
        return (p.x > p1.x && p.x <= p2.x) ||( p.x > p2.x && p.x <= p1.x);
    }

    bool isWithinYRange(const Point & p) const {
        return (p.y > p1.y && p.y <= p2.y) || (p.y > p2.y && p.y <= p1.y);
    }
};
std::ostream & operator<<(std::ostream & os, const Point & p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::ostream & operator<<(std::ostream & os, const Edge & e) {
    os << "Edge: " << e.p1 << " - " << e.p2;
    return os;
};

struct FirstOrderFunction {
    double m;
    double a;
    bool isVertical;
    FirstOrderFunction(const Edge & e) {
        isVertical = e.p1.x == e.p2.x;
        if (!isVertical) {
            m = (e.p2.y - e.p1.y) / double (e.p2.x - e.p1.x);
            a =  e.p1.y - m * e.p1.x;
        } else {
            m = 0;
            a = e.p1.x;
        }
    }
    double f(int x) const {
        return a + m * x;
    }
    bool isToTheLeft(const Point & p) const {
        return (isVertical ? p.x > a : m > 0 ? f(p.x) > p.y : f(p.x) < p.y);
    }
    bool isToTheRight(const Point & p) const {
        return (isVertical ? p.x < a : m > 0 ? f(p.x) < p.y : f(p.x) > p.y);
    }
    bool contains(const Point & p) const {
        return (isVertical ? (a == p.x ): f(p.x) == p.y);
    }

};

struct Vertex : public Point {
    Vertex(int x, int y) : Point({x, y}) {}
    Vertex(const Point & p) : Point(p) {}
};

bool hasProjectionOnEdge(const Point & point, const Edge & edge, ProjectionSide side) {
    auto edgeFunction = FirstOrderFunction(edge);
    switch (side) {
        case NO_PROJECTION:
            return !edge.isWithinYRange(point);
        case TO_THE_LEFT:
            return edge.isWithinYRange(point) && edgeFunction.isToTheLeft(point);
        case TO_THE_RIGHT:
            return edge.isWithinYRange(point) && edgeFunction.isToTheRight(point);
    }
    return false;
}

bool isOnEdge(const Point & point, const Edge & edge) {
    FirstOrderFunction edgeFunction(edge);
    return (edge.p1 == point || edge.p2 == point || (edge.isWithinRange(point) && edgeFunction.contains(point)));
}

struct Polygon {
    const std::list<Edge> edges;

    Polygon(const std::list<Vertex> & p) :
        edges(buildEdgesFrom(p)
    ) {}

    static std::list<Edge> buildEdgesFrom(const std::list<Vertex> & vertices) {
        std::list<Edge> edges;
        edges.clear();
        Vertex const * firstVertex = nullptr;
        Vertex const * secondVertex = nullptr;
        for (auto & vertex: vertices) {
            firstVertex = &vertex;
            if (secondVertex) {
                edges.push_back({*secondVertex, *firstVertex});
            }
            secondVertex = firstVertex;
        }
        // Last edge
        edges.push_back((Edge){vertices.back(), vertices.front()});
        return edges;
    }

    bool contains(const Point & point) const {
        u_int32_t cross = 0;
        for (auto & edge: edges) {
            if (isOnEdge(point, edge)) {
                // Point is on the edge, no need to keep checking
                return true;
            }
            auto state = hasProjectionOnEdge(point, edge, TO_THE_LEFT);
            if (state) {
                cross++;
            }
        }
        return isEven(cross);
    }
};


int main(void) {
    logger.isEnabled = false;
    Polygon polygon = Polygon (
        {
            {10, 4},
            {20, 20},
            {5, 25},
            {20, 26},
            {20, 37},
            {25, 27},
            {37, 25},
            {10, 15},
            {30, 4}
        }
    );
    for (int y = 0; y < 50; y++) {
        std::cout << ">";
        for (int x = 0; x < 50; x++) {
            if (polygon.contains({x, y})) {
                std::cout << "*";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "<" << std::endl;
    }
    // Point probe({10, 31});
    // auto result = polygon.contains(probe);
    // std::cout << "Point " << probe << " is " << (result ? "" : "not ") << "inside the polygon" << std::endl;
    return 0;
}
