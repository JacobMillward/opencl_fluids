#include "Camera.h"

void  Camera::UpdateCamera(float  msec) {
	pitch -= (Mouse::ButtonHeld(MOUSE_RIGHT) ? Mouse::GetRelativePosition().y : 0);
	yaw -= (Mouse::ButtonHeld(MOUSE_RIGHT) ? Mouse::GetRelativePosition().x : 0);

	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	if (Keyboard::KeyDown(KEY_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, -0.1f) * msec;
	}
	if (Keyboard::KeyDown(KEY_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(0.0f, 0.0f, -0.1f) * msec;
	}
	if (Keyboard::KeyDown(KEY_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(-0.1f, 0.0f, 0.0f) * msec;
	}
	if (Keyboard::KeyDown(KEY_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) *
			Vector3(-0.1f, 0.0f, 0.0f) * msec;
	}
	if (Keyboard::KeyDown(KEY_SPACE)) {
		position.y += 0.1f * msec;
	}
	if (Keyboard::KeyDown(KEY_SHIFT)) {
		position.y -= 0.1f * msec;
	}
}

Matrix4  Camera::BuildViewMatrix() {
	return    Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};