#include <iostream>

#include "../include/Gomoku.hpp"

int main(int ac, char **argv)
{
    (void) ac;
    (void) argv;

    try {
        gmk::Gomoku gomoku;
        gomoku.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return (EXIT_SUCCESS);
}
