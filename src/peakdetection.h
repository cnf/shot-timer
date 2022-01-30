#ifndef PEAKDETECTION_H
#define PEAKDETECTION_H

class PeakDetection {
public:
  PeakDetection(int lag, float threshold, float influence);
  ~PeakDetection();
  void process(short *sample_buffer, int sample_count);
  // int *get_signals();
  // int get_signal_count();

private:
  int lag;
  float threshold;
  float influence;
  float avgFilter[0];
  float stdFilter[0];
  float signals[0];
  // int signal_count;
  short mean(short data[], int len);
  short stddev(short data[], int len);
};

// float stddev(float data[], int len);
// float mean(float data[], int len);
// void thresholding(float y[], int signals[], int lag, float threshold, float influence);
#endif