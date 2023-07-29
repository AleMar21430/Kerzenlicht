#include "Camera.h"

Camera::Camera() {
	position =    Vec3(0, -1, 0);
	right_vec =   Vec3(1, 0, 0);
	up_vec =      Vec3(0, 1, 0);
	forward_vec = Vec3(0, 0, 1);

	Fov = 50;
	sensor_size = 50;

	depth_of_field = false;
	depth_of_field_focus_distance = 10;
	depth_of_field_f_stops = 1.7;
}

Vec3 Camera::rotateX(const Vec3& vec, double angle) {
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);
	double newY = cosAngle * vec.Y - sinAngle * vec.Z;
	double newZ = sinAngle * vec.Y + cosAngle * vec.Z;
	return Vec3(vec.X, newY, newZ);
}
// Function to rotate a 3D vector around the Y-axis (yaw)
Vec3 Camera::rotateY(const Vec3& vec, double angle) {
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);
	double newX = cosAngle * vec.X + sinAngle * vec.Z;
	double newZ = -sinAngle * vec.X + cosAngle * vec.Z;
	return Vec3(newX, vec.Y, newZ);
}

// Function to rotate a 3D vector around the Z-axis (roll)
Vec3 Camera::rotateZ(const Vec3& vec, double angle) {
	double cosAngle = cos(angle);
	double sinAngle = sin(angle);
	double newX = cosAngle * vec.X - sinAngle * vec.Y;
	double newY = sinAngle * vec.X + cosAngle * vec.Y;
	return Vec3(newX, newY, vec.Z);
}

void Camera::f_rotate(const double& P_Pitch_X, const double& P_Yaw_Y, const double& P_Roll_Z) {
	forward_vec = rotateZ(forward_vec, P_Roll_Z);
	right_vec = rotateZ(right_vec, P_Roll_Z);
	up_vec = rotateZ(up_vec, P_Roll_Z);

	// Apply Y-axis (yaw) rotation
	forward_vec = rotateY(forward_vec, P_Yaw_Y);
	right_vec = rotateY(right_vec, P_Yaw_Y);
	up_vec = rotateY(up_vec, P_Yaw_Y);

	// Apply X-axis (pitch) rotation
	forward_vec = rotateX(forward_vec, P_Pitch_X);
	right_vec = rotateX(right_vec, P_Pitch_X);
	up_vec = rotateX(up_vec, P_Pitch_X);
}

void Camera::f_moveForward(double P_Distance) {
	position += forward_vec * P_Distance;
}

void Camera::f_moveRight(double P_Distance) {
	position += right_vec * P_Distance;
}

void Camera::f_moveUp(double P_Distance) {
	position += up_vec * P_Distance;
}
