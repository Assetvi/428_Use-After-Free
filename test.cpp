#include "test.h"
#include <iostream>

// A class for tracking runtime statistics
void RuntimeMonitor::start(const std::string& funcName) {
    timings[funcName] = std::chrono::steady_clock::now().time_since_epoch();
}

void RuntimeMonitor::end(const std::string& funcName) {
    auto endTime = std::chrono::steady_clock::now().time_since_epoch();
    timings[funcName] = endTime - timings[funcName];
}

void RuntimeMonitor::log() {
    std::cout << "Function Runtime Statistics:\n";
    for (const auto& [funcName, duration] : timings) {
        std::cout << " - " << funcName << ": " << duration.count() << " seconds\n";
    }
}

// Global instance of the monitor
RuntimeMonitor runtimeMonitor;

// Macros for instrumentation
#define START_MONITOR(funcName) runtimeMonitor.start(funcName)
#define END_MONITOR(funcName) runtimeMonitor.end(funcName)

// Example function to instrument
void exampleFunction() {
    runtimeMonitor.start(__FUNCTION__); // Start monitoring this function
    for (volatile int i = 0; i < 1000000; ++i); // Simulated workload
    runtimeMonitor.end(__FUNCTION__); // End monitoring this function
}