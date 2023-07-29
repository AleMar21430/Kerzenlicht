#pragma once

#include "Math.h"

struct Camera {
	Vec3 position, forward_vec, up_vec, right_vec;

	double Fov;
	double sensor_size;
	uint32_t x_resolution, y_resolution;

	bool depth_of_field;
	double depth_of_field_focus_distance;
	double depth_of_field_f_stops;
	
	Camera();

	Vec3 rotateX(const Vec3& vec, double angle);
	Vec3 rotateY(const Vec3& vec, double angle);
	Vec3 rotateZ(const Vec3& vec, double angle);

	void f_rotate(const double& P_Pitch_X, const double& P_Yaw_Y, const double& P_Roll_Z);
	void f_moveForward(double P_Distance);
	void f_moveRight(double P_Distance);
	void f_moveUp(double P_Distance);
};