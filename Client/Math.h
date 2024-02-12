#pragma once
class GameObject;

namespace Math
{
	void Normalize(float* fX, float* fY);

	float Rotate(GameObject* gGameObject, float vLocalPositionX, float vLocalPositionY);

	bool IsInsideInterval(float v, float vMin, float vMax);
};