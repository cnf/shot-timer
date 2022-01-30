#include "peakdetection.h"
#include <Arduino.h>
#include <PDM.h>

#define BUFFER_SIZE 256
#define LAG_SIZE 30

// buffer to read samples into, each sample is 16-bits
short sampleBuffer[BUFFER_SIZE + LAG_SIZE];

// number of samples read
volatile int samplesRead;

// Peak Detection
PeakDetection PD(LAG_SIZE, 3.0, 0.0);

//
byte g_sample_window = 50;

// PDM Callback
void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(&sampleBuffer[LAG_SIZE], bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}

void setup() {
  memset(sampleBuffer, 0, sizeof(sampleBuffer));
  samplesRead = 0;

  Serial.begin(115200);
  while (!Serial)
    yield();

  // configure the data receive callback
  PDM.onReceive(onPDMdata);

  PDM.setBufferSize(BUFFER_SIZE);

  // optionally set the gain, defaults to 20
  // PDM.setGain(0);

  // initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1)
      yield();
  }

  Serial.println("starting...");
}

void loop() {
  // wait for samples to be read
  if (samplesRead) {
    PD.process(sampleBuffer, samplesRead + LAG_SIZE);

    if (PD.get_signal_count() > 1) {
      Serial.println("peaks detected: " + String(PD.get_signal_count()));
      float *signals = PD.get_signals();
      for (int i = 0; i < PD.get_signal_count(); i++) {
        Serial.println(">peak: " + String(signals[i]));
      }
    }

    // print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
      // if (sampleBuffer[i] >= 10000) {
      //   Serial.println("PANG!");
      // }
      //
      // if (sampleBuffer[i] >= 500) {
      //   digitalWrite(LEDR, LOW);
      //   digitalWrite(LEDG, HIGH);
      //   digitalWrite(LEDB, HIGH);
      // }
      // // check if the sound value is higher than 250 and lower than 500
      // if (sampleBuffer[i] >= 250 && sampleBuffer[i] < 500) {
      //   digitalWrite(LEDB, LOW);
      //   digitalWrite(LEDR, HIGH);
      //   digitalWrite(LEDG, HIGH);
      // }
      // // check if the sound value is higher than 0 and lower than 250
      // if (sampleBuffer[i] >= 0 && sampleBuffer[i] < 250) {
      //   digitalWrite(LEDG, LOW);
      //   digitalWrite(LEDR, HIGH);
      //   digitalWrite(LEDB, HIGH);
      // }
      //
      // Serial.print(">pdm:");
      // Serial.println(sampleBuffer[i]);
    }

    // copy LAG_SIZE samples to the beginning of the buffer.
    memcpy(sampleBuffer, &sampleBuffer[samplesRead], LAG_SIZE * sizeof(sampleBuffer[0]));
    // clear the read count
    samplesRead = 0;
  }
}
