/* 
    Author: Maciej Cieslik
    
    Each log represents error, for example: attempt of mowing outside the lawn. 
    Log constists of simulation time when it occured and message.
*/

#pragma once
#include <string>

class Log {
private:
    u_int64_t time_;
    std::string message_;
public:
    Log(const u_int64_t& time, const std::string& message);
    Log(const Log&) = delete;
    Log(Log&&) = default;
    Log& operator=(Log&&) = default;
    Log& operator=(const Log&) = delete;
    bool operator==(const Log& other) const;
    bool operator!=(const Log& other) const;

    u_int64_t getTime() const;
    std::string getMessage() const;
};