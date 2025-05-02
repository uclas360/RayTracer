/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** main
*/

#include <exception>
#include <iostream>

#include "ArgsManager.hpp"
#include "RaytracerCore.hpp"
#include "Utils.hpp"

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "-h") {
        std::cout << HELP_MESSAGE << std::endl;
        return 0;
    }
    try {
        ArgManager argManager(argv);
        RaytracerCore core(argManager.getArguments());

        core.compute();
    } catch (const std::exception &exp) {
        std::cerr << exp.what() << std::endl << HELP_MESSAGE << std::endl;
        return 84;
    }
    return 0;
}
