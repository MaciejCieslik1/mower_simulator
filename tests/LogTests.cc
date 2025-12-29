#include <gtest/gtest.h>
#include <string>
#include "../include/Log.h"

using namespace std;


TEST(ConstructorAndGetters, constructorAndGetters) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);

    u_int64_t result_time = log.getTime();
    string result_message = log.getMessage();

    EXPECT_EQ(time, result_time);
    EXPECT_EQ(message, result_message);
}


TEST(OperatorEquals, equals) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);
    Log log2 = Log(time, message);

    bool result = log == log2;
    EXPECT_TRUE(result);
}


TEST(OperatorEquals, notEqualsTime) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);
    Log log2 = Log(time + 1, message);

    bool result = log == log2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsMessage) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);
    Log log2 = Log(time + 1, message + "f");

    bool result = log == log2;
    EXPECT_FALSE(result);
}


TEST(OperatorNotEquals, notEquals) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);
    Log log2 = Log(time + 1, message);

    bool result = log != log2;
    EXPECT_TRUE(result);
}


TEST(OperatorNotEquals, equals) {
    u_int64_t time = 20;
    string message = "Hello";
    Log log = Log(time, message);
    Log log2 = Log(time, message);

    bool result = log != log2;
    EXPECT_FALSE(result);
}
