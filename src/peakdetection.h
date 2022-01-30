#ifndef _PEAKDETECTION_H_INCLUDED
#define _PEAKDETECTION_H_INCLUDED

#define SAMPLE_LENGTH 1000

class PeakDetection {
public:
  PeakDetection(int lag, float threshold, float influence);
  ~PeakDetection();
  void process(short *sample_buffer, int sample_count);
  float *get_signals();
  int get_signal_count();

private:
  int lag;
  float threshold;
  float influence;
  float avgFilter[0];
  float stdFilter[0];
  float signals[SAMPLE_LENGTH];
  // int signal_count;
  float mean(float data[], int len);
  float stddev(float data[], int len);
};

// float stddev(float data[], int len);
// float mean(float data[], int len);
// void thresholding(float y[], int signals[], int lag, float threshold, float influence);
#endif