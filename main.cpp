#include <iostream>
#include <cstring>
#include "CommandParser.h"

CommandParser ticketSystem;

int main() {
    freopen("testdata/basic_3/5.in", "r", stdin);
    freopen("ans.txt", "w", stdout);
    std::string commandLine, result;
    while (std::getline(std::cin, commandLine)) {
        result = ticketSystem.run(commandLine);
        if (result.length()) {
            std::cout<<result<<'\n';
            if (result == "bye") break;
        }
    }
    return 0;
}