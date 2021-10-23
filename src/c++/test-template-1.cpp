/**
 * @file test-template-1.cpp
 * @author Giovanni Curiel (giovannicuriel@gmail.com)
 * @brief Test for template
 * @version 0.1
 * @date 2021-10-23
 *
 * @copyright Copyright (c) 2021
 */

#include <iostream>
#include <list>

bool isEven(u_int32_t n) { return n % 2 == 1;}

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum ProjectionSide {
    TO_THE_LEFT,
    TO_THE_RIGHT,
    NO_PROJECTION
};

struct Edge {
    Point p1;
    Point p2;
    bool isWithinYRange(const Point & p) const {
        return (p.y >= p1.y && p.y <= p2.y) || (p.y <= p1.y && p.y >= p2.y);
    }
};

struct FirstOrderFunction {
    double m;
    double a;
    FirstOrderFunction(const Edge & e) {
        m = (e.p2.y - e.p1.y) / double (e.p2.x - e.p1.x);
        a =  e.p1.y - m * e.p1.x;
    }
    double f(int x) const {
        return a + m * x;
    }
    bool isAbove(const Point & p) const {
        return f(p.x) >= p.y;
    }
    bool isBelow(const Point & p) const {
        return f(p.x) <= p.y;
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
            return edge.isWithinYRange(point) && edgeFunction.isAbove(point);
        case TO_THE_RIGHT:
            return edge.isWithinYRange(point) && edgeFunction.isBelow(point);
    }
    return false;
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
            if (hasProjectionOnEdge(point, edge, TO_THE_LEFT)) {
                cross++;
            }
        }
        return isEven(cross);
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

int main(void) {
    Polygon polygon = Polygon (
        {
            {1, 1},
            {11, 3},
            {14, 14},
            {3, 11}
        }
    );
    std::list<Point> points = {
        { 0, 0 },
        { 11, 3 },
        { 2, 6 },
        { 2, 0 },
        { 5, 0 },
        { 13, 0 },
        { 15, 0 },
        { 0,  2 },
        { 2,  2 },
        { 5,  2 },
        { 10, 2 },
        { 13, 2 },
        { 15, 2 },
        { 0,  5 },
        { 2,  5 },
        { 5,  5 },
        { 13, 5 },
        { 15, 5 },
        { 0,  13 },
        { 2,  13 },
        { 5,  13 },
        { 13, 13 },
        { 15, 13 },
        { 0,  15 },
        { 2,  15 },
        { 5,  15 },
        { 13, 15 },
        { 15, 15 },
    };
    for (auto & point: points) {
        auto state = (polygon.contains(point) ? "" : "not ");
        std::cout << "Point " << point << " is " << state << "inside the polygon" << std::endl;
    }
    return 0;
}
