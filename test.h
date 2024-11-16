#ifndef TEST_H
#define TEST_H

#include <string>
#include <chrono>
#include <unordered_map>

class RuntimeMonitor {
public:
    void start(const std::string& funcName);
    void end(const std::string& funcName);
    void log();

private:
    std::unordered_map<std::string, std::chrono::duration<double>> timings;
};

extern RuntimeMonitor runtimeMonitor;

void exampleFunction();

#endif // TEST_H