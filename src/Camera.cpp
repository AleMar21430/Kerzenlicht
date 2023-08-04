#include "Camera.h"

Camera::Camera() {
	position =    Vec3(0, 0, 0);
	rotation =    Vec3(0, 0, 180);
	right_vec =   Vec3(1, 0, 0);
	up_vec =      Vec3(0, 1, 0);
	forward_vec = Vec3(0, 0, 1);

	Fov = 60.0;
	sensor_size = 50;
	near_clip = 0.1;
	far_clip = 1000.0;
	x_resolution = 1800;
	y_resolution = 1100;

	camera_matrix = Matrix_4x4();
	view_matrix = Matrix_4x4();
	projection_matrix = Matrix_4x4();
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

void Camera::f_processMatrix() {
	Matrix_4x4 translation = Matrix_4x4({
		{ 1 , 0 , 0 , position.X },
		{ 0 , 1 , 0 , position.Y },
		{ 0 , 0 , 1 , position.Z },
		{ 0 , 0 , 0 , 1          }
		});

	double Yaw =   rotation.X * PI / 180.0;
	double Pitch = rotation.Y * PI / 180.0;
	double Roll =  rotation.Z * PI / 180.0;

	Matrix_4x4 yawMat = Matrix_4x4({
		{ cos(Yaw)  , 0 , sin(Yaw) , 0 },
		{ 0         , 1 , 0        , 0 },
		{ -sin(Yaw) , 0 , cos(Yaw) , 0 },
		{ 0         , 0 , 0        , 1 }
		});

	Matrix_4x4 pitchMat = Matrix_4x4({
		{ 1 , 0          , 0           , 0 },
		{ 0 , cos(Pitch) , -sin(Pitch) , 0 },
		{ 0 , sin(Pitch) , cos(Pitch)  , 0 },
		{ 0 , 0          , 0           , 1 }
		});

	Matrix_4x4 rollMat = Matrix_4x4({
		{ cos(Roll) , -sin(Roll) , 0 , 0 },
		{ sin(Roll) , cos(Roll)  , 0 , 0 },
		{ 0         , 0          , 1 , 0 },
		{ 0         , 0          , 0 , 1 }
		});

	camera_matrix = translation * (pitchMat * yawMat * rollMat);

	aspect_ratio = x_resolution / y_resolution;

	double t = tan((Fov * PI / 180) / 2) * near_clip;
	double r = t * aspect_ratio;

	projection_matrix = Matrix_4x4({
		{ near_clip / r , 0             , 0                                                , 0                                                   },
		{ 0             , near_clip / t , 0                                                , 0                                                   },
		{ 0             , 0             , - (far_clip + near_clip) / (far_clip - near_clip), - 2 * far_clip * near_clip / (far_clip - near_clip) },
		{ 0             , 0             , -1                                               , 0                                                   }
	});

	view_matrix = Matrix_4x4({
		{ x_resolution / 2.0 , 0                  , 0   , x_resolution / 2.0 },
		{ 0                  , y_resolution / 2.0 , 0   , y_resolution / 2.0 },
		{ 0                  , 0                  , 0.5 , 0.5                },
		{ 0                  , 0                  , 0   , 1                  }
	});
}