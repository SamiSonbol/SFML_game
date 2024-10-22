#pragma once
#include <iostream>
#include <cmath>
#include <vector>


class point {

public:

	double x, y, z, w;

	point(double x, double y, double z, double w = 1) {

		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

	};

	void dehomogenize() {

		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w = 1;

	};

};

class vec2 {

public:

	double x, y;

	vec2(const double& x = 0.0f, const double& y = 0.0f) : x(x), y(y) {};

	double magnitude() const {

		return std::sqrt(this->x * this->x + this->y * this->y);

	};

	double dot(const vec2& vec) {

		return this->x * vec.x + this->y * vec.y;

	};

	vec2 operator-(const vec2& vec) const {

		return vec2(this->x - vec.x, this->y - vec.y);

	};
	void operator-=(const vec2& vec) {

		this->x -= vec.x;
		this->y -= vec.y;
	
	};

	vec2 operator+(const vec2& vec) const {

		return vec2(this->x + vec.x, this->y + vec.y);

	};
	void operator+=(const vec2& vec) {

		this->x += vec.x;
		this->y += vec.y;
		
	};

	vec2 operator*(const vec2& vec) const {

		return vec2(this->x * vec.x, this->y * vec.y);

	};
	vec2 operator*(const double& n) const {

		return vec2(this->x * n, this->y * n);

	};
	void operator*=(const vec2& vec) {

		this->x *= vec.x;
		this->y *= vec.y;
	
	};
	void operator*=(const double& n) {

		this->x *= n;
		this->y *= n;
	
	};

	vec2 operator/(const vec2& vec) const {

		return vec2(this->x / vec.x, this->y / vec.y);

	};
	vec2 operator/(const double& n) const {

		return vec2(this->x / n, this->y / n);

	};
	void operator/=(const vec2& vec) {

		this->x /= vec.x;
		this->y /= vec.y;
		
	};
	void operator/=(const double& n) {

		this->x /= n;
		this->y /= n;
		
	};

	bool operator==(const vec2& vec) const {

		if (this->x == vec.x && this->y == vec.y) {

			return true;

		};

		return false;

	};
	bool operator!=(const vec2& vec) const {

		if (this->x != vec.x && this->y != vec.y) {

			return true;

		};

		return false;

	};

	vec2 normalize() const {

		double n = magnitude();

		if (n != 0.0f) {
		
			return vec2(this->x, this->y) / n;

		};

	};

	std::vector<double> to_GL() {

		return std::vector<double> { this->x, this->y };

	};


};

class vec3 {

public:

	double x, y, z;

	vec3(const double& x = 0.0f, const double& y = 0.0f, const double& z = 1.0f) : x(x), y(y), z(z) {};

	vec3(const vec2& vec, const double& z) : x(vec.x), y(vec.y), z(z) {};//override

	double magnitude() const{

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

	};

	double dot(const vec3& vec) const {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z;

	};

	vec3 cross(const vec3& vec) const {

		return vec3(this->y * vec.z - this->z * vec.y,
			this->z * vec.x - this->x * vec.z,
			this->x * vec.y - this->y * vec.x);

	};

	vec3 operator-(const vec3& vec) const {

		return vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);

	};
	void operator-=(const vec3& vec) {

		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;

	};

	vec3 operator+(const vec3& vec) const {

		return vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);

	};
	void operator+=(const vec3& vec) {

		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;

	};

	vec3 operator*(const vec3& vec) const {

		return vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z);

	};
	vec3 operator*(const double& n) const {

		return vec3(this->x * n, this->y * n, this->z * n);

	};
	void operator*=(const vec3& vec) {

		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;

	};
	void operator*=(const double& n) {

		this->x *= n;
		this->y *= n;
		this->z *= n;

	};

	vec3 operator/(const vec3& vec) const {

		return vec3(this->x / vec.x, this->y / vec.y, this->z / vec.z);

	};
	vec3 operator/(const double& n) const {

		return vec3(this->x / n, this->y / n, this->z / n);

	};
	void operator/=(const vec3& vec) {

		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;

	};
	void operator/=(const double& n) {

		this->x /= n;
		this->y /= n;
		this->z /= n;

	};

	bool operator==(const vec3& vec) const {

		return this->x == vec.x && this->y == vec.y && this->z == vec.z;

	};

	bool operator!=(const vec3& vec) const {

		return !(*this == vec);

	};

	vec3 normalize() const {

		double n = magnitude();

		if (n != 0.0f) {

			return vec3(this->x, this->y, this->z) / n;

		};

	};

	std::vector<double> to_GL() {

		return std::vector<double> { this->x, this->y, this->z };

	};

};

class vec4 {

public:

	double x, y, z, w;
	vec4(const double& x = 0.0f, const double& y = 0.0f, const double& z = 0.0f, const double& w = 1.0f) : x(x), y(y), z(z), w(w) {};

	vec4(const vec3& vec, const double& w = 1.0f) : x(vec.x), y(vec.y), z(vec.z), w(w) {};//override

	double magnitude() const {

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);

	};

	double dot(const vec4& vec) const {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;

	};

	vec4 operator-(const vec4& vec) const {

		return vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);

	};
	void operator-=(const vec4& vec) {

		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		this->w -= vec.w;

	};

	vec4 operator+(const vec4& vec) const {

		return vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);

	};
	void operator+=(const vec4& vec) {

		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		this->w += vec.w;

	};

	vec4 operator*(const vec4& vec) const {

		return vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);

	};
	vec4 operator*(const double& n) const {

		return vec4(this->x * n, this->y * n, this->z * n, this->w * n);

	};
	void operator*=(const vec4& vec) {

		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		this->w *= vec.w;

	};
	void operator*=(const double& n) {

		this->x *= n;
		this->y *= n;
		this->z *= n;
		this->w *= n;

	};

	vec4 operator/(const vec4& vec) const {

		return vec4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w);

	};
	vec4 operator/(const double& n) const {

		return vec4(this->x / n, this->y / n, this->z / n, this->w / n);

	};
	void operator/=(const vec4& vec) {

		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;
		this->w /= vec.w;

	};
	void operator/=(const double& n) {

		this->x /= n;
		this->y /= n;
		this->z /= n;
		this->w /= n;

	};

	bool operator==(const vec4& vec) const {

		if (this->x == vec.x && this->y == vec.y && this->z == vec.z && this->w == vec.w) {

			return true;

		};

		return false;

	};
	bool operator!=(const vec4& vec) const {

		if (this->x != vec.x && this->y != vec.y && this->z != vec.z && this->w != vec.w) {

			return true;

		};

		return false;

	};

	vec4 normalize() const {

		double n = magnitude();

		if (n != 0.0f) {

			return vec4(this->x, this->y, this->z, this->w) / n;

		};

	};

	std::vector<double> to_GL() {

		return std::vector<double> { this->x, this->y, this->z, this->w };

	};

};

class mat3 {

public:

    double a11, a12, a13;
    double a21, a22, a23;
    double a31, a32, a33;

	mat3(

		double a11 = 1.0f, double a12 = 0.0f, double a13 = 0.0f,
		double a21 = 0.0f, double a22 = 1.0f, double a23 = 0.0f,
		double a31 = 0.0f, double a32 = 0.0f, double a33 = 1.0f) : 
		
		a11(a11), a12(a12), a13(a13),
	    a21(a21), a22(a22), a23(a23),
	    a31(a31), a32(a32), a33(a33) {

	};

	mat3 transpose() const {

		return mat3(

			this->a11, this->a21, this->a31,
			this->a12, this->a22, this->a32,
			this->a13, this->a23, this->a33

		);

	};

	mat3 operator*(const mat3& mat) const {

		return mat3(

			this->a11 * mat.a11 + this->a12 * mat.a21 + this->a13 * mat.a31, this->a11 * mat.a12 + this->a12 * mat.a22 + this->a13 * mat.a32, this->a11 * mat.a13 + this->a12 * mat.a23 + this->a13 * mat.a33,
			this->a21 * mat.a11 + this->a22 * mat.a21 + this->a23 * mat.a31, this->a21 * mat.a12 + this->a22 * mat.a22 + this->a23 * mat.a32, this->a21 * mat.a13 + this->a22 * mat.a23 + this->a23 * mat.a33,
			this->a31 * mat.a11 + this->a32 * mat.a21 + this->a33 * mat.a31, this->a31 * mat.a12 + this->a32 * mat.a22 + this->a33 * mat.a32, this->a31 * mat.a13 + this->a32 * mat.a23 + this->a33 * mat.a33

		);

	};

	mat3 operator*(const double& n) const {

		return mat3(

			this->a11 * n, this->a12 * n, this->a13 * n,
			this->a21 * n, this->a22 * n, this->a23 * n,
			this->a31 * n, this->a32 * n, this->a33 * n

		);

	};

	vec3 operator*(const vec3& vec) const {

		return vec3(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z

		);

	};

	std::vector<double> to_GL() const {

		return {

			this->a11, this->a21, this->a31,
			this->a12, this->a22, this->a32,
			this->a13, this->a23, this->a33

		};

	}

};

class mat4 {

public:

    double a11, a12, a13, a14;
    double a21, a22, a23, a24;
    double a31, a32, a33, a34;
    double a41, a42, a43, a44;

	mat4(

		double a11 = 1.0f, double a12 = 0.0f, double a13 = 0.0f, double a14 = 0.0f,
		double a21 = 0.0f, double a22 = 1.0f, double a23 = 0.0f, double a24 = 0.0f,
		double a31 = 0.0f, double a32 = 0.0f, double a33 = 1.0f, double a34 = 0.0f,
		double a41 = 0.0f, double a42 = 0.0f, double a43 = 0.0f, double a44 = 1.0f) :

		a11(a11), a12(a12), a13(a13), a14(a14),
		a21(a21), a22(a22), a23(a23), a24(a24),
		a31(a31), a32(a32), a33(a33), a34(a34),
		a41(a41), a42(a42), a43(a43), a44(a44) {
		
	};

	mat4 transpose() const {

		return mat4(

			this->a11, this->a21, this->a31, this->a41,
			this->a12, this->a22, this->a32, this->a42,
			this->a13, this->a23, this->a33, this->a43,
			this->a14, this->a24, this->a34, this->a44

		);

	};

	mat4 operator*(const mat4& mat) const {

		return mat4(

			this->a11 * mat.a11 + this->a12 * mat.a21 + this->a13 * mat.a31 + this->a14 * mat.a41,
			this->a11 * mat.a12 + this->a12 * mat.a22 + this->a13 * mat.a32 + this->a14 * mat.a42,
			this->a11 * mat.a13 + this->a12 * mat.a23 + this->a13 * mat.a33 + this->a14 * mat.a43,
			this->a11 * mat.a14 + this->a12 * mat.a24 + this->a13 * mat.a34 + this->a14 * mat.a44,

			this->a21 * mat.a11 + this->a22 * mat.a21 + this->a23 * mat.a31 + this->a24 * mat.a41,
			this->a21 * mat.a12 + this->a22 * mat.a22 + this->a23 * mat.a32 + this->a24 * mat.a42,
			this->a21 * mat.a13 + this->a22 * mat.a23 + this->a23 * mat.a33 + this->a24 * mat.a43,
			this->a21 * mat.a14 + this->a22 * mat.a24 + this->a23 * mat.a34 + this->a24 * mat.a44,

			this->a31 * mat.a11 + this->a32 * mat.a21 + this->a33 * mat.a31 + this->a34 * mat.a41,
			this->a31 * mat.a12 + this->a32 * mat.a22 + this->a33 * mat.a32 + this->a34 * mat.a42,
			this->a31 * mat.a13 + this->a32 * mat.a23 + this->a33 * mat.a33 + this->a34 * mat.a43,
			this->a31 * mat.a14 + this->a32 * mat.a24 + this->a33 * mat.a34 + this->a34 * mat.a44,

			this->a41 * mat.a11 + this->a42 * mat.a21 + this->a43 * mat.a31 + this->a44 * mat.a41,
			this->a41 * mat.a12 + this->a42 * mat.a22 + this->a43 * mat.a32 + this->a44 * mat.a42,
			this->a41 * mat.a13 + this->a42 * mat.a23 + this->a43 * mat.a33 + this->a44 * mat.a43,
			this->a41 * mat.a14 + this->a42 * mat.a24 + this->a43 * mat.a34 + this->a44 * mat.a44

		);
	};

	mat4 operator*(const double& n) const {

		return mat4(

			this->a11 * n, this->a12 * n, this->a13 * n, this->a14 * n,
			this->a21 * n, this->a22 * n, this->a23 * n, this->a24 * n,
			this->a31 * n, this->a32 * n, this->a33 * n, this->a34 * n,
			this->a41 * n, this->a42 * n, this->a43 * n, this->a44 * n

		);

	};

	vec4 operator*(const vec4& vec) const {

		return vec4(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z + this->a14 * vec.w,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z + this->a24 * vec.w,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z + this->a34 * vec.w,
			this->a41 * vec.x + this->a42 * vec.y + this->a43 * vec.z + this->a44 * vec.w

		);

	};

	mat4 inverse() const {

		double det = a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42))
			- a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41))
			+ a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41))
			- a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));

		if (det == 0) {

			std::cerr << "Matrix is singular and cannot be inverted." << std::endl;
			return mat4(); // Return identity or some error indicator

		};

		double inv_det = 1.0f / det;

		return mat4(

			inv_det * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42)),
			inv_det * -(a12 * (a33 * a44 - a34 * a43) - a13 * (a32 * a44 - a34 * a42) + a14 * (a32 * a43 - a33 * a42)),
			inv_det * (a12 * (a23 * a44 - a24 * a43) - a13 * (a22 * a44 - a24 * a42) + a14 * (a22 * a43 - a23 * a42)),
			inv_det * -(a12 * (a23 * a34 - a24 * a33) - a13 * (a22 * a34 - a24 * a32) + a14 * (a22 * a33 - a23 * a32)),

			inv_det * -(a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41)),
			inv_det * (a11 * (a33 * a44 - a34 * a43) - a13 * (a31 * a44 - a34 * a41) + a14 * (a31 * a43 - a33 * a41)),
			inv_det * -(a11 * (a23 * a44 - a24 * a43) - a13 * (a21 * a44 - a24 * a41) + a14 * (a21 * a43 - a23 * a41)),
			inv_det * (a11 * (a23 * a34 - a24 * a33) - a13 * (a21 * a34 - a24 * a31) + a14 * (a21 * a33 - a23 * a31)),

			inv_det * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41)),
			inv_det * -(a11 * (a32 * a44 - a34 * a42) - a12 * (a31 * a44 - a34 * a41) + a14 * (a31 * a42 - a32 * a41)),
			inv_det * (a11 * (a22 * a44 - a24 * a42) - a12 * (a21 * a44 - a24 * a41) + a14 * (a21 * a42 - a22 * a41)),
			inv_det * -(a11 * (a22 * a34 - a24 * a32) - a12 * (a21 * a34 - a24 * a31) + a14 * (a21 * a32 - a22 * a31)),

			inv_det * -(a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41)),
			inv_det * (a11 * (a32 * a43 - a33 * a42) - a12 * (a31 * a43 - a33 * a41) + a13 * (a31 * a42 - a32 * a41)),
			inv_det * -(a11 * (a22 * a43 - a23 * a42) - a12 * (a21 * a43 - a23 * a41) + a13 * (a21 * a42 - a22 * a41)),
			inv_det * (a11 * (a22 * a33 - a23 * a32) - a12 * (a21 * a33 - a23 * a31) + a13 * (a21 * a32 - a22 * a31))

		);
	}

	std::vector<double> to_GL() const {

		return {

			this->a11, this->a21, this->a31, this->a41,
			this->a12, this->a22, this->a32, this->a42,
			this->a13, this->a23, this->a33, this->a43,
			this->a14, this->a24, this->a34, this->a44

		};

	};

};

static double to_radians(const double& degree) {

	return degree * 3.14159265359f / 180.0f;

};

//2D
static mat3 create_translation_matrix(const double& x, const double& y) {

	return mat3(

		1, 0, x,
		0, 1, y,
		0, 0, 1

	);

};

static mat3 create_translation_matrix(const vec2& translation_vector) {//@override

	return mat3(

		1, 0, translation_vector.x,
		0, 1, translation_vector.y,
		0, 0, 1

	);

};

static mat3 create_scale_matrix(const double& x, const double& y) {

	return mat3(

		x, 0, 0,
		0, y, 0,
		0, 0, 1

	);

};

static mat3 create_scale_matrix(const vec2& scale_vector) {//@override

	return mat3(

		scale_vector.x, 0, 0,
		0, scale_vector.y, 0,
		0, 0, 1

	);

};

static mat3 create_rotation_matrix(const double& angle) {

	double alpha = to_radians(angle);

	return mat3(

		std::cos(alpha), -std::sin(alpha), 0,
		std::sin(alpha), std::cos(alpha), 0,
		0, 0, 1

	);

};

static mat3 create_model_transformation_matrix(const double& Tx, const double& Ty, const double& Sx, const double& Sy, const double& alpha) {

	return create_rotation_matrix(alpha) * create_scale_matrix(Sx, Sy) * create_translation_matrix(Tx, Ty);

};

static mat3 create_model_transformation_matrix(const vec2& T, const vec2& S, const double& alpha) {//@override

	return create_rotation_matrix(alpha) * create_scale_matrix(S) * create_translation_matrix(T);

};

static mat3 create_model_transformation_matrix(const mat3& T, const mat3& S, const mat3& R) {//@override

	return R * S * T;

};

//3D
static mat4 create_translation_matrix(const double& x, const double& y, const double& z) {

	return mat4(

		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1

	);

};

static mat4 create_translation_matrix(const vec3& translation_vector) {//@override

	return mat4(

		1, 0, 0, translation_vector.x,
		0, 1, 0, translation_vector.y,
		0, 0, 1, translation_vector.z,
		0, 0, 0, 1

	);

};

static mat4 create_scale_matrix(const double& x, const double& y, const double& z) {

	return mat4(

		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1

	);

};

static mat4 create_scale_matrix(const vec3& scale_vector) {//@override

	return mat4(

		scale_vector.x, 0, 0, 0,
		0, scale_vector.y, 0, 0,
		0, 0, scale_vector.z, 0,
		0, 0, 0, 1

	);

};

static mat4 create_X_rotation_matrix(const double& angle) {

	double alpha = to_radians(angle);

	return mat4(

		1, 0, 0, 0,
		0, std::cos(alpha), std::sin(alpha), 0,
		0, -std::sin(alpha), std::cos(alpha), 0,
		0, 0, 0, 1

	);

};

static mat4 create_Y_rotation_matrix(const double& angle) {

	double alpha = to_radians(angle);

	return mat4(

		std::cos(alpha), 0, -std::sin(alpha), 0,
		0, 1, 0, 0,
		std::sin(alpha), 0, std::cos(alpha), 0,
		0, 0, 0, 1

	);

};

static mat4 create_Z_rotation_matrix(const double& angle) {

	double alpha = to_radians(angle);

	return mat4(

		std::cos(alpha), std::sin(alpha), 0, 0,
		-std::sin(alpha), std::cos(alpha), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1

	);

};

static mat4 create_rotation_matrix(const double& alpha, const double& theta, const double& delta) {

	return create_X_rotation_matrix(alpha) * create_Y_rotation_matrix(theta) * create_Z_rotation_matrix(delta);

};

static mat4 create_model_transformation_matrix(const vec3& T, const vec3& S, const vec3& R) {

	return create_rotation_matrix(R.x, R.y, R.z) * create_scale_matrix(S) * create_translation_matrix(T);

};

static mat4 create_model_transformation_matrix(const mat4& T, const mat4& S, const mat4& R) {//@override

	return R * S * T;

};

//3D SCREEN TRANSFORMATION
static mat4 create_view_matrix(const vec3& r, const vec3& u, const vec3& d, const vec3& camera) {//also known as lookAtMatrix

    //here am basically taking r as X-axis, u as Y-axis, and d as Z-axis and using them to find the actual right, up, direction vectors
	vec3 direction = vec3(0, 0, 0) - camera;
	direction.normalize();

	vec3 right = u.cross(direction);
	right.normalize();

	vec3 up = direction.cross(right);

	return mat4(

		right.x, right.y, right.z, -right.dot(camera),
		up.x, up.y, up.z, -up.dot(camera),
		-direction.x, -direction.y, -direction.z, direction.dot(camera),
		0, 0, 0, 1

	);

};

static mat4 create_parallel_projection_matrix(const double& fov, const double& screen_width, const double& screen_height, const double& near, const double& far) {//also known as orthographic projection matrix

	double view_angle = to_radians(fov);
	double aspect_ratio = screen_width / screen_height;
	double alpha = tan(view_angle / 2.0f);
	double top = near * alpha;
	double bottom = -top;
	double right = near * aspect_ratio * alpha;
	double left = -right;

	return mat4(

		2 / (right - left), 0, 0, -(right + left) / (right - left),
		0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
		0, 0, -2 / (far - near), -(far + near) / (far - near),
		0, 0, 0, 1

	);

};

static mat4 create_frustum_projection_matrix(const double& fov, const double& screen_width, const double& screen_height, const double& near, const double& far) {

	double view_angle = to_radians(fov);
	double aspect_ratio = screen_width / screen_height;
	double alpha = tan(view_angle / 2.0f);
	double top = near * alpha;
	double bottom = -top;
	double right = near * aspect_ratio * alpha;
	double left = -right;

	mat4 frustum = mat4(

		(2 * near) / (right - left), 0, (right + left) / (right - left), 0,
		0, (2 * near) / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, (-far - near) / (far - near), (-2 * far * near) / (far - near),
		0, 0, -1, 0

	);

	return frustum;

};

//in openGL we dont require a view port matrix since the API transforms our coordinates automatically to NDC, we just need to use the function glViewport() at start of program initialization
static mat4 create_view_port_matrix(const double& fov, const double& screen_width, const double& screen_height, const double& near, const double& far) {

	double view_angle = to_radians(fov);
	double aspect_ratio = screen_width / screen_height;
	double alpha = tan(view_angle / 2.0f);
	double top = near * alpha;
	double bottom = -top;
	double right = near * aspect_ratio * alpha;
	double left = -right;

	return mat4(

		screen_width / 2, 0, 0, left + (screen_width / 2),
		0, screen_height / 2, 0, bottom + (screen_height / 2),
		0, 0, (far - near) / 2, (far + near) / 2,
		0, 0, 0, 1

	);

};

static mat4 create_model_view_matrix(const mat4& model_transformation_matrix, const mat4& look_at_matrix) {

	return look_at_matrix * model_transformation_matrix;

};

static mat4 create_model_view_projection_matrix(const mat4& model_transformation_matrix, const mat4& look_at_matrix, const mat4& frustum_matrix) {

	return frustum_matrix * create_model_view_matrix(model_transformation_matrix, look_at_matrix);

};

static double get_area_of_circle(const double& radius) {

	return radius * radius * 3.14159265359f;

};

static double get_determinant(const vec2& A, const vec2& B) {

	double a = A.x * B.y;
	double b = A.y * B.x;

	return a - b;

};

static double get_area_of_triangle(const vec2& a, const vec2& b, const vec2& c) {

	vec2 ab = b - a;
	vec2 ac = c - a;

	return 0.5 * get_determinant(ab, ac);

};

static double get_area_of_2D_surface(const std::vector<vec3>& positions, const vec3& center) {

	double total_area = 0.0f;
	for (int i = 0; i < positions.size(); ++i) {

		int next_index = (i + 1) % positions.size();

		vec2 A(positions[i].x, positions[i].y);
		vec2 B(center.x, center.y);
		vec2 C(positions[next_index].x, positions[next_index].y);

		total_area += get_area_of_triangle(A, B, C);

	};

	return total_area;

};

//openGL calculates these automatically
static std::vector<double> get_barycentric_coordinates(const vec3& point, const vec3& A, const vec3& B, const vec3& C) {

	vec2 p = vec2(point.x, point.y);
	vec2 a = vec2(A.x, A.y);
	vec2 b = vec2(B.x, B.y);
	vec2 c = vec2(C.x, C.y);

	std::vector<vec2> main_triangle = { a, b, c };
	std::vector<vec2> beta_triangle = { p, c, a };
	std::vector<vec2> alpha_triangle = { p, b, c };
	std::vector<vec2> gamma_triangle = { p, a, b };

	double area_main = get_area_of_triangle(main_triangle[0], main_triangle[1], main_triangle[2]);
	double area_alpha = get_area_of_triangle(alpha_triangle[0], alpha_triangle[1], alpha_triangle[2]);
	double area_beta = get_area_of_triangle(beta_triangle[0], beta_triangle[1], beta_triangle[2]);
	double area_gamma = get_area_of_triangle(gamma_triangle[0], gamma_triangle[1], gamma_triangle[2]);

	double alpha = area_alpha / area_main;
	double beta = area_beta / area_main;
	double gamma = area_gamma / area_main;

	return { alpha, beta, gamma };

};

static double clamp(const double& value, const double& min, const double& max) {

	if (value > max) {

		return max;

	}
	else if (value < min) {

		return min;

	}
	else {

		return value;

	};

};