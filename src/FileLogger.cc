/* 
    Author: Maciej Cieslik
    
    FileLogger class implementation.
*/


#include <fstream>
#include "FileLogger.h"


using namespace std;


FileLogger::FileLogger(const string& file_path) : file_path_(file_path) {}


FileLogger::FileLogger(const FileLogger& other) : file_path_(other.getFilePath()) {}


const string& FileLogger::getFilePath() const {
    return file_path_;
}


void FileLogger::saveLog(const Log& log) const {
    // Save log to the logs file

    ofstream file(file_path_, ios::app);
    if (file.is_open()) {
        file << "Time: "<< log.getTime() << ": " << log.getMessage() << endl;
    }
    file.close();
}


void FileLogger::saveMessage(const string& message) const {
    // Save message to the logs file

    ofstream file(file_path_, ios::app);

    if (file.is_open()) {
        file << message << endl;
    }
    file.close();
}
