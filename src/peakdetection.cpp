#include "peakdetection.h"
#include <Arduino.h>

PeakDetection::PeakDetection(int lag, float threshold, float influence) {
  // memset(signals, 0, sizeof(int) * SAMPLE_LENGTH);
  // memset(avgFilter, 0, sizeof(int) * SAMPLE_LENGTH);
  // memset(stdFilter, 0, sizeof(int) * SAMPLE_LENGTH);
  this->lag = lag;
  this->threshold = threshold;
  this->influence = influence;
}

PeakDetection::~PeakDetection() {}

void PeakDetection::process(short *sample_buffer, int sample_count) {

  short filteredY[sample_count];
  memcpy(filteredY, sample_buffer, sizeof(float) * sample_count); // sizeof(float)?
  float avgFilter[sample_count];
  float stdFilter[sample_count];

  avgFilter[lag - 1] = mean(sample_buffer, lag);
  stdFilter[lag - 1] = stddev(sample_buffer, lag);

  for (int i = lag; i < sample_count; i++) {
    if (fabsf(sample_buffer[i] - avgFilter[i - 1]) > threshold * stdFilter[i - 1]) {
      if (sample_buffer[i] > avgFilter[i - 1]) {
        signals[i] = 1;
      } else {
        signals[i] = -1;
      }
      filteredY[i] = influence * sample_buffer[i] + (1 - influence) * filteredY[i - 1];
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