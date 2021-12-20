#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <utility>
#include <sstream>

#include "./patricia-tree.hpp"


int main(void) {
    std::cout << "Add new command!\n";
    PatriciaTree<Node<std::string, StringKeySpec>> pt;

    std::string command = "command";
    std::string key;

    while(command != "exit") {
        getline(std::cin, key);
        pt.insertNode(key, command);
        std::cout << pt << "\n";
    }
    return 0;
}

