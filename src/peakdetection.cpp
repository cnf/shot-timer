#include "peakdetection.h"
#include <Arduino.h>

PeakDetection::PeakDetection(int lag, float threshold, float influence) {
  memset(signals, 0, sizeof(int) * SAMPLE_LENGTH);
  // memset(avgFilter, 0, sizeof(int) * SAMPLE_LENGTH);
  // memset(stdFilter, 0, sizeof(int) * SAMPLE_LENGTH);
  this->lag = lag;
  this->threshold = threshold;
  this->influence = influence;
}

PeakDetection::~PeakDetection() {}

void PeakDetection::process(short *sample_buffer, int sample_count) {
  float fsamples[sample_count];
  // std::copy(sample_buffer, sample_buffer + sample_count, fsamples);
  for (int i = 0; i < sample_count; i++) {
    fsamples[i] = (float)sample_buffer[i];
  }

  float filteredY[sample_count];
  memcpy(filteredY, fsamples, sizeof(float) * lag); // sizeof(float)?
  float avgFilter[sample_count];
  float stdFilter[sample_count];

  avgFilter[lag - 1] = mean(fsamples, lag);
  stdFilter[lag - 1] = stddev(fsamples, lag);

  for (int i = lag; i < sample_count; i++) {
    Serial.print(">pdm: ");
    Serial.println(sample_buffer[i]);
    Serial.print(">sample: ");
    Serial.println(fsamples[i]);
    Serial.print(">lala: ");
    Serial.println((float)sample_buffer[i]);
    Serial.println(">check: " + String(fabsf(fsamples[i] - avgFilter[i - 1])));
    Serial.println(">treshold: " + String(threshold * stdFilter[i - 1]));
    if (fabsf(fsamples[i] - avgFilter[i - 1]) > threshold * stdFilter[i - 1]) {
      if (fsamples[i] > avgFilter[i - 1]) {
        signals[i] = 1;
      } else {
        signals[i] = 1; // TODO: normally -1
      }
      filteredY[i] = influence * fsamples[i] + (1 - influence) * filteredY[i - 1];
    } else {
      signals[i] = 0;
      filteredY[i] = fsamples[i];
    }
    avgFilter[i] = mean(filteredY + i - lag, lag);
    stdFilter[i] = stddev(filteredY + i - lag, lag);

    // Serial.println(">mean: " + String(avgFilter[i]));
    // Serial.println(">std: " + String(stdFilter[i]));
    Serial.println(">peak: " + String(signals[i]));
  }
}

float *PeakDetection::get_signals() {
  float rsignals[SAMPLE_LENGTH];
  memcpy(signals, rsignals, sizeof(float) * SAMPLE_LENGTH);
  memset(signals, 0, sizeof(int) * SAMPLE_LENGTH);

  return rsignals;
}

int PeakDetection::get_signal_count() {
  int count = 0;
  for (int i = 0; i < SAMPLE_LENGTH; i++) {
    if (signals[i] != 0) {
      count++;
    }
  }
  return count;
}

float PeakDetection::mean(float data[], int len) {
  float sum = 0.0, mean = 0.0;

  int i;
  for (i = 0; i < len; ++i) {
    sum += data[i];
  }

  mean = sum / len;
  return mean;
}

float PeakDetection::stddev(float data[], int len) {
  float the_mean = mean(data, len);
  float standardDeviation = 0.0;

  int i;
  for (i = 0; i < len; ++i) {
    standardDeviation += pow(data[i] - the_mean, 2);
  }

  return sqrt(standardDeviation / len);
}

/*
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SAMPLE_LENGTH 1000

// float stddev(float data[], int len);
// float mean(float data[], int len);
// void thresholding(float y[], int signals[], int lag, float threshold,
//                   float influence);

void thresholding(float y[], int signals[], int lag, float threshold,
                  float influence) {
  memset(signals, 0, sizeof(int) * SAMPLE_LENGTH);
  float filteredY[SAMPLE_LENGTH];
  memcpy(filteredY, y, sizeof(float) * SAMPLE_LENGTH);
  float avgFilter[SAMPLE_LENGTH];
  float stdFilter[SAMPLE_LENGTH];

  avgFilter[lag - 1] = mean(y, lag);
  stdFilter[lag - 1] = stddev(y, lag);

  for (int i = lag; i < SAMPLE_LENGTH; i++) {
    if (fabsf(y[i] - avgFilter[i - 1]) > threshold * stdFilter[i - 1]) {
      if (y[i] > avgFilter[i - 1]) {
        signals[i] = 1;
      } else {
        signals[i] = -1;
      }
      filteredY[i] = influence * y[i] + (1 - influence) * filteredY[i - 1];
    } else {
      signals[i] = 0;
    }
    avgFilter[i] = mean(filteredY + i - lag, lag);
    stdFilter[i] = stddev(filteredY + i - lag, lag);
  }
}

float mean(float data[], int len) {
  float sum = 0.0, mean = 0.0;

  int i;
  for (i = 0; i < len; ++i) {
    sum += data[i];
  }

  mean = sum / len;
  return mean;
}

float stddev(float data[], int len) {
  float the_mean = mean(data, len);
  float standardDeviation = 0.0;

  int i;
  for (i = 0; i < len; ++i) {
    standardDeviation += pow(data[i] - the_mean, 2);
  }

  return sqrt(standardDeviation / len);
}

int main() {
  printf("Hello, World!\n");

  //   thresholding(y, signal, lag, threshold, influence);

  return 0;
}
*/