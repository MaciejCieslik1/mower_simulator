/* 
    Author: Maciej Cieslik
    
    Implements Log class.
*/

#include "../include/Log.h"

using namespace std;


Log::Log(const u_int64_t& time, const string& message) : time_(time), message_(message) {}


Log::Log(const Log& other) : time_(other.getTime()), message_(other.getMessage()) {}


Log& Log::operator=(const Log& other) {
    if (this != &other) {
        time_ = other.getTime();
        message_ = other.getMessage();
    }
    return *this;
}


u_int64_t Log::getTime() const {
    return time_;
}


string Log::getMessage() const {
    return message_;
}


bool Log::operator==(const Log& other) const {
    return this->time_ == other.getTime() && this->message_ == other.getMessage();
}


bool Log::operator!=(const Log& other) const {
    return !((*this) == other);
}
