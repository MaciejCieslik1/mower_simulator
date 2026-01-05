/* 
    Author: Maciej Cieslik
    
    Implements Logger class.
*/

#include <queue>
#include "../include/Logger.h"

using namespace std;


Logger::Logger() : logs_(queue<Log>()) {}


const queue<Log>& Logger::getLogs() const {
    return std::move(logs_);
}


void Logger::push(Log&& log) {
    logs_.push(std::move(log));
}


void Logger::clear() {
    logs_ = queue<Log>();
}
