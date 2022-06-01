/**
 * Wrap a class method with a mutex so that it becomes a critical region
 */

#include <iostream>
#include <string>
#include <unistd.h>

/**
 * This class contains a bunch of methods that are critical regions, i.e., no
 * more than one thread should execute it at the same time. These methods should
 * be protected by mutexes.
 */
class ImportantClass {
protected:
    std::string m_ImportantData;
    u_int32_t m_ImportantValue;
public:
    ImportantClass(std::string data, u_int32_t value): m_ImportantData(data), m_ImportantValue(value) { }
    void performImportantOp1() {
        std::cout << "Performing op1: data: " << m_ImportantData << ", value: " << m_ImportantValue << "\n";
        sleep(6);
        std::cout << "Op1 was successfully performed\n";
    }
    void performImportantOp2() {
        std::cout << "Performing op2: data: " << m_ImportantData << ", value: " << m_ImportantValue << "\n";
        sleep(2);
        std::cout << "Op2 was successfully performed\n";
    }

    class Mutexer
};




int main(void) {
    std::cout << "Oi\n";

    ImportantClass c1 { "important data 1", 10 };
    ImportantClass c2 { "important data 2", 50 };

    c1.performImportantOp1();
    return 0;
}
