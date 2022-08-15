#pragma once
#include "Include.h"
struct Vector2 {
	float x, y;
	Vector2 operator+(Vector2 V) {
		return { x + V.x, y + V.y };
	}
	Vector2 operator-(Vector2 V) {
		return { x - V.x, y - V.y };
	}
	Vector2 operator/(Vector2 V) {
		return { x / V.x, y / V.y };
	}
	Vector2 operator*(Vector2 V) {
		return { x * V.x, y * V.y };
	}

	float Distance(Vector2 pos) {
		return sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
	}
};

struct Vector3 {
	float x, y, z;
	Vector3 operator+(Vector3 V) {
		return { x + V.x, y + V.y,z + V.z };
	}
	Vector3 operator-(Vector3 V) {
		return { x - V.x, y - V.y,z - V.z };
	}
	Vector3 operator/(Vector3 V) {
		return { x / V.x, y / V.y,z / V.z };
	}
	Vector3 operator*(Vector3 V) {
		return { x * V.x, y * V.y,z * V.z };
	}

	float Distance(Vector3 pos) {
		return sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2) + pow(pos.z - z, 2));
	}
};

struct Vector4 {
	float x, y, z, w;
	Vector4 operator+(Vector4 V) {
		return { x + V.x, y + V.y,z + V.z,w + V.w };
	}
	Vector4 operator-(Vector4 V) {
		return { x - V.x, y - V.y,z - V.z,w - V.w };
	}
	Vector4 operator/(Vector4 V) {
		return { x / V.x, y / V.y,z / V.z,w /V.w };
	}
	Vector4 operator*(Vector4 V) {
		return { x * V.x, y * V.y,z * V.z,w * V.w };
	}

	float Distance(Vector4 pos) {
		return sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2) + pow(pos.z - z, 2) + pow(pos.w - w, 2));
	}
};
