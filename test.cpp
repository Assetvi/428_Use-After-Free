#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>

// A class for tracking runtime statistics
class RuntimeMonitor {
private:
    std::unordered_map<std::string, std::chrono::duration<double>> timings;

public:
    void start(const std::string& funcName) {
        timings[funcName] = std::chrono::steady_clock::now().time_since_epoch();
    }

    void end(const std::string& funcName) {
        auto endTime = std::chrono::steady_clock::now().time_since_epoch();
        timings[funcName] = endTime - timings[funcName];
    }

    void log() {
        std::cout << "Function Runtime Statistics:\n";
        for (const auto& [funcName, duration] : timings) {
            std::cout << " - " << funcName << ": " << duration.count() << " seconds\n";
        }
    }
};

// Global instance of the monitor
RuntimeMonitor runtimeMonitor;

// Macros for instrumentation
#define START_MONITOR(funcName) runtimeMonitor.start(funcName)
#define END_MONITOR(funcName) runtimeMonitor.end(funcName)

// Example function to instrument
void exampleFunction() {
    START_MONITOR(__FUNCTION__); // Start monitoring this function
    for (volatile int i = 0; i < 1000000; ++i); // Simulated workload
    END_MONITOR(__FUNCTION__); // End monitoring this function
}

int main() {
    exampleFunction(); // Run the instrumented function

    // Log runtime statistics
    runtimeMonitor.log();

    return 0;
}
