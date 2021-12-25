#include <iostream>
#include <cstdarg>

int fn(int a) {
    std::cout << "> a: " << a << " END \n";
    return 0;
}
template<typename... Others>
int fn(int a, Others... others) {
    std::cout << "> a: " << a << "\n";
    fn(others...);
    return 0;
}


int main(void) {
    fn(3, 2, 5, 4, 1);
    return 0;
}