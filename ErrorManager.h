#pragma once
#include <string>
#include <iostream>
#include <exception>

class GameException : public std::exception {
private:
    std::string msg;

public:
    explicit GameException(const std::string& message) : msg(message) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class ErrorManager {
public:
    static void logError(const std::string& message) {
        std::cerr << "\n[ERROR]: " << message << std::endl;
    }
};