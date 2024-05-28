/*
 * Px4.c
 *
 *  Created on: Dec 22, 2023
 *      Author: User
 */
#include "Px4.h"
#include "/home/sangam/Desktop/jd/apps/examples/sensor_fusion/EKF.h"
#include "math.h"

float phiHat_deg_ = 0.0f;
float thetaHat_deg_ = 0.0f;

float phiHat_rad_ = 0.0f;
float thetaHat_rad_ = 0.0f;

float KALMAN_P_INIT = 0.1f;
float KALMAN_Q = 0.001f;
float KALMAN_R = 0.011f;

void IMU_ReadAccelerometerComplete(Px4 *imu,struct mpu6050_imu_msg mpu6500) {

	/* Pre-compute accelerometer conversion constant (raw to m/s^2) */
	imu->accConversion = 1 / 16384.0f;

	/* Form signed 16-bit integers */
	// int16_t accX = (int16_t) imu_acc_data.x;//((imu->accRxBuf[0] << 8) | imu->accRxBuf[1]);
	// int16_t accY = (int16_t) imu_acc_data.y;//((imu->accRxBuf[2] << 8) | imu->accRxBuf[3]);
	// int16_t accZ = (int16_t) imu_acc_data.z;//((imu->accRxBuf[4] << 8) | imu->accRxBuf[5]);

	/* Convert to m/s^2 */
	imu->acc_mps2[0] = imu->accConversion * mpu6500.acc_x;
	imu->acc_mps2[1] = imu->accConversion * mpu6500.acc_y;
	imu->acc_mps2[2] = imu->accConversion * mpu6500.acc_z;

	/* Filter accelerometer data */
	RCFilter_Update(&lpfAcc[0], imu->acc_mps2[0]);
	RCFilter_Update(&lpfAcc[1], imu->acc_mps2[1]);
	RCFilter_Update(&lpfAcc[2], imu->acc_mps2[2]);

	//Filtered accelerometer measurement
	float ax_mps2 = lpfAcc[0].out[0];
	float ay_mps2 = lpfAcc[1].out[0];
	float az_mps2 = lpfAcc[2].out[0];

	/*Calculate roll (phi) and pitch(theta) angle estimates using filtered accelerometer readings*/
	phiHat_deg_ = atanf(ay_mps2 / az_mps2) * RAD_TO_DEG;
	thetaHat_deg_ = asinf(ax_mps2 / G_MPS2) * RAD_TO_DEG;

}

void IMU_ReadGyroscopeComplete(Px4 *imu, struct mpu6050_imu_msg mpu6500) {

	/* Pre-compute gyroscope conversion constant (raw to rad/s) */
	imu->gyrConversion = 0.01745329251f / 131.0f; /* Datasheet page 39 */

	/* Form signed 16-bit integers */
	// int16_t gyrX = (int16_t) raw_imu.gyro_x;//((imu->gyrRxBuf[0] << 8) | imu->gyrRxBuf[1]);
	// int16_t gyrY = (int16_t) raw_imu.gyro_y;//((imu->gyrRxBuf[2] << 8) | imu->gyrRxBuf[3]);
	// int16_t gyrZ = (int16_t) raw_imu.gyro_z;//((imu->gyrRxBuf[4] << 8) | imu->gyrRxBuf[5]);

	/* Convert to deg/s */
	imu->gyr_rps[0] = imu->gyrConversion * mpu6500.gyro_x;
	imu->gyr_rps[1] = imu->gyrConversion * mpu6500.gyro_y;
	imu->gyr_rps[2] = imu->gyrConversion * mpu6500.gyro_z;

	/* Filter gyroscope data */
	RCFilter_Update(&lpfGyr[0], imu->gyr_rps[0]);
	RCFilter_Update(&lpfGyr[1], imu->gyr_rps[1]);
	RCFilter_Update(&lpfGyr[2], imu->gyr_rps[2]);

	//Filtered accelerometer measurement
	float p_rps = lpfGyr[0].out[0];
	float q_rps = lpfGyr[1].out[0];
	float r_rps = lpfGyr[2].out[0];

	//Transform body rates to Euler rates to get estimate of roll and pitch angles
	float phiDot_rps = p_rps
			+ tanf(thetaHat_rad_)
					* (sinf(phiHat_rad_) * q_rps + cosf(phiHat_rad_) * r_rps);
	float thetaDot_rps = cosf(phiHat_rad_) * q_rps - sinf(phiHat_rad_) * r_rps;

	//Integrate Euler rates to get estimate of roll and pitch angles
	phiHat_rad_ = (phiHat_rad_ + (SAMPLE_TIME_MS_USB_ / 1000.0f) * phiDot_rps)
			* RAD_TO_DEG;
	thetaHat_rad_ = (thetaHat_rad_
			+ (SAMPLE_TIME_MS_USB_ / 1000.0F) * thetaDot_rps) * RAD_TO_DEG;

}

void Complementary_Filter(Px4 *imu) {

	/* Filter accelerometer data */
	RCFilter_Update(&lpfAcc[0], imu->acc_mps2[0]);
	RCFilter_Update(&lpfAcc[1], imu->acc_mps2[1]);
	RCFilter_Update(&lpfAcc[2], imu->acc_mps2[2]);

	//Filtered accelerometer measurement
	float ax_mps2 = lpfAcc[0].out[0];
	float ay_mps2 = lpfAcc[1].out[0];
	float az_mps2 = lpfAcc[2].out[0];

	/*Calculate roll (phi) and pitch(theta) angle estimates using filtered accelerometer readings*/
	float phiHat_acc_rad = atanf(ay_mps2 / az_mps2);
	float thetaHat_acc_rad = asinf(ax_mps2 / G_MPS2);

	/* Filter gyroscope data */
	RCFilter_Update(&lpfGyr[0], imu->gyr_rps[0]);
	RCFilter_Update(&lpfGyr[1], imu->gyr_rps[1]);
	RCFilter_Update(&lpfGyr[2], imu->gyr_rps[2]);

	//Filtered accelerometer measurement
	float p_rps = lpfGyr[0].out[0];
	float q_rps = lpfGyr[1].out[0];
	float r_rps = lpfGyr[2].out[0];

	//Transform body rates to Euler rates to get estimate of roll and pitch angles
	float phiDot_rps = p_rps
			+ tanf(thetaHat_rad_)
					* (sinf(phiHat_rad_) * q_rps + cosf(phiHat_rad_) * r_rps);
	float thetaDot_rps = cosf(phiHat_rad_) * q_rps - sinf(phiHat_rad_) * r_rps;

	//Combining Accel and Gyro data for complementary filter
	phiHat_rad_ = (
	COMP_FILT_ALPHA * phiHat_acc_rad
			+ (1.0f - COMP_FILT_ALPHA)
					* (phiHat_rad_
							+ (SAMPLE_TIME_MS_USB_ / 1000.0f) * phiDot_rps))
			* RAD_TO_DEG;
	thetaHat_rad_ = (
	COMP_FILT_ALPHA * thetaHat_acc_rad
			+ (1.0f - COMP_FILT_ALPHA)
					* (thetaHat_rad_
							+ (SAMPLE_TIME_MS_USB_ / 1000.0f) * thetaDot_rps))
			* RAD_TO_DEG;
	printf("Phi Hat Radian: %d \t", phiHat_rad_);
	print("Theta Hat Radian: %d\n", thetaHat_rad_)		

}

void EKF_AccelGyro(Px4 *imu, EKF *ekf) {
	float KalmanQ[2] = { KALMAN_Q, KALMAN_Q };
	float KalmanR[3] = { KALMAN_R, KALMAN_R, KALMAN_R };
	float Kalman_P_Init[2] = { KALMAN_P_INIT, KALMAN_P_INIT };

	RCFilter_Update(&lpfAcc[0], imu->acc_mps2[0]);
	RCFilter_Update(&lpfAcc[1], imu->acc_mps2[1]);
	RCFilter_Update(&lpfAcc[2], imu->acc_mps2[2]);

	//Filtered accelerometer measurement
	float ax_mps2 = lpfAcc[0].out[0];
	float ay_mps2 = lpfAcc[1].out[0];
	float az_mps2 = lpfAcc[2].out[0];

	/* Filter gyroscope data */
	RCFilter_Update(&lpfGyr[0], imu->gyr_rps[0]);
	RCFilter_Update(&lpfGyr[1], imu->gyr_rps[1]);
	RCFilter_Update(&lpfGyr[2], imu->gyr_rps[2]);

	//Filtered accelerometer measurement
	float p_rps = lpfGyr[0].out[0];
	float q_rps = lpfGyr[1].out[0];
	float r_rps = lpfGyr[2].out[0];

	//Remapping axis data of Accel and Gyro
	ax_mps2 = -ay_mps2;
	ay_mps2 = ax_mps2;
	az_mps2 = -az_mps2;

	p_rps = -q_rps;
	q_rps = p_rps;
	r_rps = -r_rps;

	//Initialize kalman filter
	EKF_Init(&ekf, Kalman_P_Init, KalmanQ, KalmanR);

	//Prediction step using filtered gyro data
	EKF_Predict(&ekf, p_rps, q_rps, r_rps, 0.001f * KALMAN_PREDICT_PERIOD_MS);

	//Update step using Accel data
	EKF_Update(&ekf, ax_mps2, ay_mps2, az_mps2);

}

