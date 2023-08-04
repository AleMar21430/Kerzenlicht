#pragma once

#include "Math.h"

struct Camera {
	Vec3 position, rotation, forward_vec, up_vec, right_vec;
	Matrix_4x4 camera_matrix, view_matrix, projection_matrix;

	double fov, aspect_ratio;
	double sensor_size, near_clip, far_clip;
	uint32_t x_resolution, y_resolution;

	bool orthogonal;
	
	Camera();

	void f_moveForward(double P_Distance);
	void f_moveRight(double P_Distance);
	void f_moveUp(double P_Distance);

	void f_processMatrix();
};