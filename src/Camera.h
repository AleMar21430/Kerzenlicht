#pragma once

#include "Math.h"

struct Camera {
	Vec3 position, rotation, forward_vec, up_vec, right_vec;
	Matrix_4x4 camera_matrix, viewport_matrix, projection_matrix;

	double fov, aspect_ratio;
	double sensor_size, near_clip, far_clip;
	uint32_t x_resolution, y_resolution;

	bool orthogonal;
	
	Camera();

	void f_moveForward(const double& P_Distance);
	void f_moveRight(const double& P_Distance);
	void f_moveUp(const double& P_Distance);

	void f_processMatrix();
};