#ifndef SMATH_VEC3_HPP
#define SMATH_VEC3_HPP
#include <cmath>

namespace smath {
    template<typename T>
    class Vec3 {
	private:
		T v[3];
	public:
		/* Constructors */
		Vec3(): v{0, 0, 0} {}
		explicit Vec3(T value): v{value, value, value} {}
		Vec3(T x, T y, T z): v{x, y, z}{}
		Vec3(const Vec3& other) = default;
		Vec3(const Vec3&& other) noexcept = default;
		/* Data Getters */
		const T& x() const { return v[0]; }
		const T& y() const { return v[1]; }
		const T& z() const { return v[2]; }
		T* data(){return v;}
		const T* data() const {return v;}
		
		/* Methods */
		T dot(const Vec3& other) const {
			v[0]*other.v[0] + v[1]*other.v[1] + v[2]*other.v[2];
		}
		T normalized() const {
			return v / v.length();
		}
		T length() const {
			sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
		}
		/* Operators */
		T& operator[](int i) {
			return v[i];
		}
		const T& operator[](int i) const {
			return v[i];
		}
		Vec3& operator=(const Vec3& other) = default;
		Vec3& operator=(Vec3&& other) noexcept = default;
		friend Vec3 operator+(const Vec3& a, const Vec3& b);
		friend Vec3 operator-(const Vec3& a, const Vec3& b);
		friend Vec3 operator*(const Vec3& a, const Vec3& b);
		friend Vec3 operator*(const Vec3& a, const T& b);
		friend Vec3 operator*(const T& a, const Vec3& b);
		friend Vec3 operator/(const Vec3& a, const T& b);
		friend Vec3& operator+=(Vec3& a, const Vec3& b);
		friend Vec3& operator-=(Vec3& a, const Vec3& b);
		friend Vec3& operator*=(Vec3& a, const Vec3& b);
		friend bool operator==(const Vec3& a, const Vec3& b);
		friend bool operator!=(const Vec3& a, const Vec3& b);
    };
	// Addition Operation
    template<typename T>
	Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) {
		return Vec3(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
	}
	// Subtraction Operation
    template<typename T>
	Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b){
		return Vec3(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
	}
	// Cross Product Operation
    template<typename T>
	Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) {
		return Vec3(a[1]*b[2]-a[2]*b[1],
		            a[0]*b[2]-a[2]*b[0],
		            a[0]*b[1]-a[1]*b[0]);
	}
	// Scalar Operations
	template<typename T>
	Vec3<T> operator*(const Vec3<T>& a, const T& b) {
		return Vec3(a[0]*b, a[1]*b, a[2]*b);
	}
	template<typename T>
	Vec3<T> operator*(const T&a, const Vec3<T>& b) {
		return Vec3(a[0]*b, a[1]*b, a[2]*b);
	}
	template<typename T>
	Vec3<T> operator/(const T&a, const Vec3<T>& b) {
		return Vec3(a[0]/b, a[1]/b, a[2]/b);
	}
	// Assignment Operations
	template<typename T>
	Vec3<T>& operator+=(Vec3<T>& a, const Vec3<T>& b){
		a = a + b;
		return a;
	}
	template<typename T>
	Vec3<T>& operator-=(Vec3<T>& a, const Vec3<T>& b){
		a = a - b;
		return a;
	}
	template<typename T>
	Vec3<T>& operator*=(Vec3<T>& a, const Vec3<T>& b){
		a = a * b;
		return a;
	}
	// Boolean Operations
	template<typename T>
	bool operator==(const Vec3<T>& a, const Vec3<T>& b){
		return a[0]==b[0] && a[1]==b[1] && a[2]==b[2];
	}
	template<typename T>
	bool operator!=(const Vec3<T>& a, const Vec3<T>& b){
		return a[0]!=b[0] || a[1]!=b[1] || a[2]!=b[2];
	}
	//
    using vec3f = Vec3<float>;
    using vec3d = Vec3<double>;
}
#endif //SMATH_VEC3_HPP
