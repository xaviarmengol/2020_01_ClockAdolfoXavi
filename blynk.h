// Blynk WEB

//#include <BlynkSimpleEsp32.h>
#include <BlynkSimpleEsp32_SSL.h>

// Global application variables in GUI

int logValue;

BLYNK_READ(V0) { // Lidar Dist value
    Blynk.virtualWrite(V0, logValue);
}

