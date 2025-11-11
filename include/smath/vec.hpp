#ifndef SMATH_VEC_HPP
#define SMATH_VEC_HPP

#include <cmath>
#include <concepts>
#include <stdexcept>

namespace smath {
	template<unsigned int N, class T>
	class Vec{
	private:
		T data[N];
	public:
		/* Constructors */
		Vec():data{}{};
		template<typename... Us> requires (sizeof...(Us)==N && (std::convertible_to<Us, T> && ...))
		Vec(Us... args):data{static_cast<T>(args)...}{}
		Vec(const Vec& other) = default;
		Vec(const Vec&& other) noexcept = default;
		Vec& operator=(const Vec<N, T>& other) = default;
		Vec& operator=(const Vec<N, T>&& other) noexcept = default;
		/* Getters */
		T& operator[](int i){
			if (i<0||i>N-1)
				throw std::out_of_range("Index out of bound");
			return data[i];
		}
		const T& operator[](int i) const {
			if (i<0||i>N-1)
				throw std::out_of_range("Index out of bound");
			return data[i];
		}
		/* Operations */
		T length() const {
			T len = 0;
			for (int i = 0; i<N; i++){
				len += data[i]*data[i];
			}
			return static_cast<T>(std::sqrt(len));
		}
		Vec<N,T> dot(const Vec<N, T>& other) const {
			T result = 0;
			for (int i = 0; i<N; i++) {
				result+=(*this)[i]*other[i];
			}
			return result;
		}
		friend Vec<N,T> operator+(const Vec<N,T>& a, const Vec<N,T>& b);
		friend Vec<N,T> operator-(const Vec<N,T>& a, const Vec<N,T>& b);
		friend Vec<N,T> operator*(const Vec<N,T>& a, const Vec<N,T>& b);
		friend Vec<N,T> operator*(const Vec<N,T>& a, const T& b);
		friend Vec<N,T> operator*(const T& a, const Vec<N, T>& b);
		friend Vec<N,T> operator/(const Vec<N,T>& a, const T& b);
		friend Vec<N,T> operator%(const Vec<N,T>& a, const T& b);
	};
	template<unsigned int N, class T>
	Vec<N,T> operator+(const Vec<N,T>& a, const Vec<N,T>& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a[i] + b[i];
		}
		return result;
	}
	template<unsigned int N, class T>
	Vec<N,T> operator-(const Vec<N,T>& a, const Vec<N,T>& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a[i] - b[i];
		}
		return result;
	}
	template<unsigned int N, class T>
	Vec<N,T> operator*(const Vec<N,T>& a, const Vec<N,T>& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a[i]*b[(i+1)%N] - a[(i+1)%N]*b[i];
		}
		return result;
	}
	template<unsigned int N, class T>
	Vec<N,T> operator*(const Vec<N,T>& a, const T& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a[i]*b;
		}
		return result;
	}
	template<unsigned int N, class T>
	Vec<N,T> operator*(const T& a, const Vec<N,T>& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a*b[i];
		}
		return result;
	}
	template<unsigned int N, class T>
	Vec<N,T> operator/(const Vec<N,T>& a, const T& b){
		return a*(1/b);
	}
	template<unsigned int N, class T>
	Vec<N,T> operator%(const Vec<N,T>& a, const T& b){
		Vec<N,T> result = Vec<N,T>();
		for(unsigned int i = 0; i<N; i++){
			result[i] = a[i]%b;
		}
		return result;
	}
}
#endif //SMATH_VEC3_HPP
