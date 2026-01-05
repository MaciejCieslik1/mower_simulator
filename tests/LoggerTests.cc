#include <gtest/gtest.h>
#include <string>
#include "../include/Logger.h"

using namespace std;


TEST(ConstructorAndGetter, constructorAndGetter) {
    Logger logger = Logger();

    const queue<Log>& result_queue = logger.getLogs();

    EXPECT_EQ(queue<Log>(), result_queue);
}


TEST(push, logs) {
    Logger logger = Logger();
    Log log = Log(10, "Hello");

    logger.push(std::move(log));
    const queue<Log>& result_logs = logger.getLogs();
    int result_size = 1;

    EXPECT_EQ(result_size, result_logs.size());
}


TEST(push, emptyLogs) {
    Logger logger = Logger();
    int result_size = 0;

    const queue<Log>& result_logs = logger.getLogs();

    EXPECT_EQ(result_size, result_logs.size());
}


TEST(clear, clear) {
    Logger logger = Logger();
    Log log = Log(10, "Hello");

    logger.push(std::move(log));
    logger.clear();
    const queue<Log>& result_logs = logger.getLogs();
    int result_size = 0;

    EXPECT_EQ(result_size, result_logs.size());
}


TEST(clear, clearEmptyQueue) {
    Logger logger = Logger();

    logger.clear();
    const queue<Log>& result_logs = logger.getLogs();
    int result_size = 0;

    EXPECT_EQ(result_size, result_logs.size());
}
