#include "PulsePosition.h"

#define SERIAL_BAUD 115200
#define PPM_PIN 23
#define MAX_CHANNELS 16 // Number of PPM channels to output, max 16
#define SERIAL_TIMEOUT 500  // ms before triggering failsafe if no serial data

// Default output if serial is lost or failsafe is triggered
int failsafeChannels[MAX_CHANNELS] = {
  1500, //  CH1: Roll
  1500, //  CH2: Pitch
  1000, //  CH3: Throttle
  1500, //  CH4: Yaw
  1000, //  CH5: AUX 1 (should be mapped to ARM on ELRS)
  1500, //  CH6: AUX 2
  1500, //  CH7: AUX 3
  1500, //  CH8: AUX 4
  1500, //  CH9: AUX 5
  1500, // CH10: AUX 6
  1500, // CH11: AUX 7
  1500, // CH12: AUX 8
  1500, // CH13: AUX 9
  1500, // CH14: AUX 10
  1500, // CH15: AUX 11
  1500  // CH16: AUX 12
};

PulsePositionOutput ppmOut;

int currentChannels[MAX_CHANNELS];
bool manualFailsafe = false;
unsigned long lastSerialTime = 0;

void setup() {
  ppmOut.begin(PPM_PIN);
  Serial.begin(SERIAL_BAUD);

  // Initialize current channels to neutral values
  for (int i = 0; i < MAX_CHANNELS; i++) {
    currentChannels[i] = 1500;
    ppmOut.write(i + 1, currentChannels[i]);
  }

  lastSerialTime = millis();
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    lastSerialTime = millis();

    if (line.startsWith("CH:")) {
      line.remove(0, 3);

      // Parse up to MAX_CHANNELS values
      for (int i = 0; i < MAX_CHANNELS; i++) {
        if (line.length() == 0) break;
        int idx = line.indexOf(',');
        if (idx == -1) idx = line.length();
        currentChannels[i] = line.substring(0, idx).toInt();
        line.remove(0, idx + 1);
      }

      // Write to PPM immediately
      for (int i = 0; i < MAX_CHANNELS; i++) {
        ppmOut.write(i + 1, currentChannels[i]);
      }
    }
    else if (line == "FS") {
      manualFailsafe = true;
      Serial.println("Failsafe ENABLED manually");
    }
    else if (line == "RST") {
      manualFailsafe = false;
      Serial.println("Manual failsafe DISABLED");
    }
  }

  // Determine failsafe status
  bool serialTimeout = (millis() - lastSerialTime > SERIAL_TIMEOUT);
  bool failsafeActive = manualFailsafe || serialTimeout;

  // Failsafe if needed
  if (failsafeActive) {
    for (int i = 0; i < MAX_CHANNELS; i++) {
      ppmOut.write(i + 1, failsafeChannels[i]);
    }
  }
}
