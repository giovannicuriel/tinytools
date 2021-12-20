#include <iostream>
#include <utility>
#include <list>
#include <map>

#include "./patricia-tree.hpp"

typedef int16_t Coordinate;
typedef int16_t BombCount;

struct Dimension {
    size_t x;
    size_t y;
};

std::ostream& operator<<(std::ostream& out, const Dimension& p) {
    out << "(" << p.x << "-" << p.y << ")";
    return out;
}

struct Point {
    Coordinate x;
    Coordinate y;

    bool operator==(const Point& other) { return x == other.x && y == other.y; }
    Point operator+(const Point& other) {
        return Point {
            Coordinate(x + other.x),
            Coordinate(y + other.y)
            };
    }
};
bool operator<(const Point lhs, const Point rhs) { return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y)); }

std::ostream& operator<<(std::ostream& out, const Point& p) {
    out << "[" << p.x << "," << p.y << "]";
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::map<Point, BombCount>& result) {
    for(const auto& [point, value]: result) {
        out << point << ": " << value << "\n";
    }
    return out;
}


class Field {
protected:
    Dimension m_Dimension;
    std::list<Point> m_Bombs;
    BombCount countBombs(const Point p) const;
public:
    Field();

    void generate(Dimension d, size_t nBombs);
    std::map<Point, BombCount> probe(const Point p) const;
    std::map<Point, BombCount> sweep() const;
    void print();
};


Field::Field() {
}

void Field::generate(Dimension d, size_t nBombs) {
    m_Dimension = d;
    m_Bombs.clear();
    for (size_t i = 0; i < nBombs; i++) {
        m_Bombs.push_back(Point {
            Coordinate(rand() % d.x),
            Coordinate(rand() % d.y)
        });
    }
}

std::map<Point, BombCount> Field::probe(const Point p) const {
    std::map<Point, BombCount> result;
    Point surroundings[9];
    for (auto bomb: m_Bombs) {
        if (p == bomb) {
            result[p] = -1;
            return result;
        }
    }
    size_t counter = 0;
    // Is there a way to build a python-generator like based on Point?
    for (Coordinate x = -1; x <= 1; x++) {
        for (Coordinate y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) { continue; }
            surroundings[counter] = Point { x, y } + p;
            result[surroundings[counter]] = countBombs(surroundings[counter]);
            counter++;
        }
    }
    return result;
}

BombCount Field::countBombs(const Point p) const {
    BombCount result = 0;
    for (Coordinate x = -1; x <= 1; x++) {
        for (Coordinate y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) { continue; }
            Point probePoint = Point { x, y } + p;
            for (auto b: m_Bombs) {
                if (b == probePoint) {
                    result++;
                }
            }
        }
    }
    return result;
}


std::map<Point, int16_t> Field::sweep() const {
    std::map<Point, int16_t> result;
    for (auto b: m_Bombs) {
        result[b] = -1;
    }

    for (Coordinate x = 0; x < m_Dimension.x; x++) {
        for (Coordinate y = 0; y < m_Dimension.y; y++) {
            Point p = { x, y };
            if (result.contains(p)) { continue; }
            result[p] = countBombs(p);
        }
    }
    return result;
}

void Field::print() {
    std::cout << "Dimensions: " << m_Dimension << "\n";
    std::cout << "Bombs are: \n";
    for (auto b: m_Bombs) {
        std::cout << "   " << b << "\n";
    }
}

class Cli {
protected:
    typedef void(Cli::*CliCommand)();
    PatriciaTree<Node<CliCommand, StringKeySpec>> pt;

    Field m_Field;
    void printHelp();
    void createMinefield();
    void printMinefield();
    void probeMinefield();
    void sweepMinefield();
public:
    Cli(Field& f);

    void run();
};


Cli::Cli(Field& f) : m_Field(f) {
    pt.insertNode(std::string("print help"), &Cli::printHelp);
    pt.insertNode(std::string("minefield create"), &Cli::createMinefield);
    pt.insertNode(std::string("minefield print"), &Cli::printMinefield);
    pt.insertNode(std::string("probe"), &Cli::probeMinefield);
    pt.insertNode(std::string("sweep"), &Cli::sweepMinefield);
}

void Cli::run() {
    std::string command;
    printHelp();
    m_Field.generate(Dimension { 10, 10}, 4);
    m_Field.print();
    while (command != "exit") {
        getline(std::cin, command);
        if (command == "") { continue; }
        auto node = pt.findNode(command, false);
        if (node != nullptr) {
            auto fn = node->getData();
            (this->*fn)();
        } else {
            std::cout << "Command not found\n";
        }
    }
}
void Cli::printHelp() {
    std::cout << "This is the help menu\n";
    std::cout << this->pt;
}
void  Cli::createMinefield() {
    std::cout << "Generating minefield...\n";
    m_Field.generate(Dimension { 10, 10}, 4);
    std::cout << "ok.\n";
}
void Cli::printMinefield() {
    std::cout << "Minefield is: \n";
    m_Field.print();
    std::cout << "ok.\n";
}
void Cli::probeMinefield() {
    Point p;
    std::cout << "Probing minefield: \n";
    std::cout << "X: ";
    std::cin >> p.x;
    std::cout << "Y: ";
    std::cin >> p.y;
    auto result = m_Field.probe(p);
    std::cout << "Result: \n";
    std::cout << result;
    std::cout << "Minefield is: \n";
    m_Field.print();
    std::cout << "ok.\n";
}

void Cli::sweepMinefield() {
    auto result = m_Field.sweep();
    for (int16_t x = 0; x < 10; x++) {
        for (int16_t y = 0; y < 10; y++) {
            auto nBomb = result[Point { x, y }];
            std::cout << "[" << (nBomb < 0 || nBomb > 10 ? "" : " ") << nBomb << "]";
        }
        std::cout << "\n";
    }
}

int main(void) {
    Field minefield;
    Cli cli { minefield };
    cli.run();
    return 0;
}