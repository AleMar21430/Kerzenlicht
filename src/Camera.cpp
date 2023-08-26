#include "Camera.h"

Camera::Camera() {
	position =    Vec3(0, 0, 0);
	rotation =    Vec3(0, 0, 0);
	right_vec =   Vec3(1, 0, 0);
	up_vec =      Vec3(0, 1, 0);
	forward_vec = Vec3(0, 0, 1);

	fov = 60;
	sensor_size = 50.0;
	near_clip = 0.1;
	far_clip = 1000.0;
	x_resolution = 2048;
	y_resolution = 1200;
	orthogonal = false;
	aspect_ratio = double(x_resolution) / double(y_resolution);

	camera_matrix = Matrix_4x4();
	viewport_matrix = Matrix_4x4();
	projection_matrix = Matrix_4x4();
}

void Camera::f_moveForward(const double& P_Distance) {
	position += forward_vec * P_Distance;
}

void Camera::f_moveRight(const double& P_Distance) {
	position += right_vec * P_Distance;
}

void Camera::f_moveUp(const double& P_Distance) {
	position += up_vec * P_Distance;
}

void Camera::f_processMatrix() {
	const Matrix_4x4 translation = Matrix_4x4({
		{ 1 , 0 , 0 , position.X },
		{ 0 , 1 , 0 , position.Y },
		{ 0 , 0 , 1 , position.Z },
		{ 0 , 0 , 0 , 1          }
	});

	const double Yaw =   rotation.X * RAD;
	const double Pitch = rotation.Y * RAD;
	const double Roll =  rotation.Z * RAD;

	const Matrix_4x4 yawMat = Matrix_4x4({
		{ cos(Yaw)  , 0 , sin(Yaw) , 0 },
		{ 0         , 1 , 0        , 0 },
		{ -sin(Yaw) , 0 , cos(Yaw) , 0 },
		{ 0         , 0 , 0        , 1 }
	});

	const Matrix_4x4 pitchMat = Matrix_4x4({
		{ 1 , 0          , 0           , 0 },
		{ 0 , cos(Pitch) , -sin(Pitch) , 0 },
		{ 0 , sin(Pitch) , cos(Pitch)  , 0 },
		{ 0 , 0          , 0           , 1 }
	});

	const Matrix_4x4 rollMat = Matrix_4x4({
		{ cos(Roll) , -sin(Roll) , 0 , 0 },
		{ sin(Roll) , cos(Roll)  , 0 , 0 },
		{ 0         , 0          , 1 , 0 },
		{ 0         , 0          , 0 , 1 }
	});

	camera_matrix = translation * (pitchMat * yawMat * rollMat);
	/*right_vec.normalize();
	up_vec.normalize();
	forward_vec.normalize();
	camera_matrix = Matrix_4x4({
		{ right_vec.X , up_vec.X , forward_vec.X , position.X },
		{ right_vec.Y , up_vec.Y , forward_vec.Y , position.Y },
		{ right_vec.Z , up_vec.Z , forward_vec.Z , position.Z },
		{ 0           , 0        , 0             , 1          }
	});*/

	aspect_ratio = double(x_resolution) / double(y_resolution);

	const double top = tan((fov * RAD) / 2.0) * near_clip;
	const double right = top * aspect_ratio;

	if (!orthogonal) {
		projection_matrix = Matrix_4x4({
			{ near_clip / right , 0               , 0                                                , 0                                                      },
			{ 0                 , near_clip / top , 0                                                , 0                                                      },
			{ 0                 , 0               , -(far_clip + near_clip) / (far_clip - near_clip) , -(2.0 * far_clip * near_clip) / (far_clip - near_clip) },
			{ 0                 , 0               , -1                                               , 0                                                      }
		});
	}
	else {
		projection_matrix = Matrix_4x4();
	}

	viewport_matrix = Matrix_4x4({
		{ double(x_resolution) / 2.0 , 0                          , 0   , double(x_resolution) / 2.0 },
		{ 0                          , double(y_resolution) / 2.0 , 0   , double(y_resolution) / 2.0 },
		{ 0                          , 0                          , 0.5 , 0.5                        },
		{ 0                          , 0                          , 0   , 1                          }
	});
}