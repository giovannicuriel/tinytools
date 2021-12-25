#include <iostream>

struct Data {
    int x;
};

int main(void) {
    double v = 10.9;
    Data d { x: v };
    std::cout << "Data: " << d.x << "\n";
    return 0;
}