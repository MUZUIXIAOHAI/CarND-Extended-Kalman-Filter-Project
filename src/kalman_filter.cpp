#include "kalman_filter.h"
#include <iostream>

using std::cout;
using std::endl;
using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
   * TODO: predict the state
   */
  x_ = F_*x_;
  MatrixXd F_t = F_.transpose();
  P_ = F_*P_*F_t + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
   * TODO: update the state by using Kalman Filter equations
   */
  MatrixXd H_t = H_.transpose();
  MatrixXd Si =(H_*P_*H_t+R_).inverse();
  MatrixXd k_ = (P_*H_t)*Si;
  
  x_ = x_ + k_*(z - H_*x_);
  P_ = P_ - k_*H_*P_;
}


void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
   * TODO: update the state by using Extended Kalman Filter equations
   */
  float px = x_[0];
  float py = x_[1];
  float vx = x_[2];
  float vy = x_[3];
  
  VectorXd h_x;
  h_x = VectorXd(3);
  h_x <<  sqrt(px*px + py*py), atan(py/px), (px*vx+py*vy)/sqrt(px*px+py*py);
  
  VectorXd y = z - h_x;
  if(y[1] > 3.14/2){
    y[1] = y[1] - 3.14;
  }
  if(y[1] < -3.14/2){
    y[1] = y[1] + 3.14;
  }
  MatrixXd H_t = H_.transpose();
  MatrixXd Si = (H_*P_*H_t + R_).inverse();
  MatrixXd K = P_*H_t*Si;
  
  cout << "h_x = " << h_x << endl;
  cout << "y = " << y << endl;
  
  x_ = x_ + K*y;
  P_ = P_ - K*H_*P_;
}
