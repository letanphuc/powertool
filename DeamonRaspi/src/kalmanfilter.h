#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double q; //process noise covariance
    double r; //measurement noise covariance
    double x; //value
    double p; //estimation error covariance
    double k; //kalman gain
} kalman_state;

kalman_state kalman_init(double q, double r, double p, double intial_value);
void kalman_update(kalman_state* state, double measurement);

#ifdef __cplusplus
extern "C" {
#endif

#endif // KALMANFILTER_H

