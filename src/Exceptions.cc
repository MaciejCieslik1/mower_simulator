/* 
    Author: Maciej Cieslik
    
    Exception classes implemenetation.
*/

#include "../include/Exceptions.h"


using namespace std;


MoveOutsideLawnError::MoveOutsideLawnError(const string& message)
    : msg(message) {}


const char* MoveOutsideLawnError::what() const noexcept {
    return msg.c_str();
}
