/* 
    Author: Maciej Cieslik
    
    Class, which handles saving logs to file.
*/

#pragma once


#include <string>
#include "Log.h"


class FileLogger {
private:
    std::string file_path_;
public:
    FileLogger(const std::string& path);
    FileLogger(const FileLogger& other);
    FileLogger& operator=(const FileLogger&) = delete;

    const std::string& getFilePath() const;
    
    void saveLog(const Log& log) const;
    void saveMessage(const std::string& message) const;
};
