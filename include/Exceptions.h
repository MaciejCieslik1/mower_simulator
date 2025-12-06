#pragma once

#include <exception>
#include <string>


class MoveOutsideLawnError : public std::exception {
private:
    std::string msg;
public:
    explicit MoveOutsideLawnError(const std::string& message);

    const char* what() const noexcept override;
};
