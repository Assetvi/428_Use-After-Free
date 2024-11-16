#include "test.h"

int main() {
    exampleFunction(); // Run the instrumented function

    // Log runtime statistics
    runtimeMonitor.log();
    
    return 0;
}