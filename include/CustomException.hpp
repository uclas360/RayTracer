/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** customException base class
*/

#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception {
    public:
        CustomException(std::string message): _message_(message) {}
        const char *what() const noexcept override {
            return _message_.c_str();
        }
    private:
        std::string _message_;
};
