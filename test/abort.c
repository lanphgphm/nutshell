#include <stdio.h>
#include <cstdlib>

int main() {
    printf("Test: Triggering SIGABRT (Abort)\n");
    abort();
    return 0;
}