/* 
    Author: Maciej Cieslik
    
    Collects logs from StateSimulation in a queue. Each log represents error, for example: attempt of mowing
    outside the lawn. Log constists of simulation time when it occured and message.
*/

#pragma once
#include <string>
#include <queue>
#include "Log.h"

class Logger {
private:
    std::queue<Log> logs_;
public:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    const std::queue<Log>& getLogs() const;

    void push(Log&& newLog);
    void clear();
};
