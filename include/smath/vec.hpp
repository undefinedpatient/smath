#ifndef SMATH_VEC_HPP
#define SMATH_VEC_HPP

#include <cmath>
#include <concepts>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace smath {
	template<unsigned int N, class T>
	requires (std::is_arithmetic<T>::value && N<32)
	class Vec{
	private:
		T data[N];
	public:
		/* Constructors */
		Vec():data{}{};
		template<class... Us> requires (sizeof...(Us)==N && (std::convertible_to<Us, T> && ...))
		Vec(Us... args):data{static_cast<T>(args)...}{}
		explicit Vec(const T* arr){
			for(unsigned int i = 0; i<N; i++){
				this->data[i] = arr[i];
			}
		}
		// Copy constructor
		Vec(const Vec& other) = default;
		// Move consturctor
		Vec(Vec&& other) noexcept = default;
		// Copy assignment
		Vec& operator=(const Vec<N, T>& other) = default;
		// Move assignment
		Vec& operator=(Vec<N, T>&& other) noexcept = default;

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
		std::string toString() const {
			std::string str = "Vec" + std::to_string(N) + "<";
			for (unsigned int i = 0; i < N; i++){
				str += std::to_string(data[i]);
				if (i!=N-1)
					str += ", ";
			}
			str += ">";
			return str;
		}
		/* Operations */
		T length() const {
			T len = 0;
			for (int i = 0; i<N; i++){
				len += data[i]*data[i];
			}
			return static_cast<T>(std::sqrt(len));
		}
		T dot(const Vec<N, T>& other) const {
			T result = 0;
			for (int i = 0; i<N; i++) {
				result+=(*this)[i]*other[i];
			}
			return result;
		}

		friend Vec<N,T> operator+(const Vec<N,T>& a, const Vec<N,T>& b){
			Vec<N,T> result = Vec<N,T>();
			for(unsigned int i = 0; i<N; i++){
				result[i] = a[i] + b[i];
			}
			return result;
		};
		friend Vec<N,T> operator-(const Vec<N,T>& a, const Vec<N,T>& b){
			Vec<N,T> result = Vec<N,T>();
			for(unsigned int i = 0; i<N; i++){
				result[i] = a[i] - b[i];
			}
			return result;
		}
		friend Vec<N,T> operator*(const Vec<N,T>& a, const Vec<N,T>& b){
    		Vec<N,T> result = Vec<N,T>();
    		for(unsigned int i = 0; i<N; i++){
    			result[i] = a[i]*b[(i+1)%N] - a[(i+1)%N]*b[i];
    		}
    		return result;
    	}
		friend Vec<N,T> operator*(const Vec<N,T>& a, const T& b){
    		Vec<N,T> result = Vec<N,T>();
    		for(unsigned int i = 0; i<N; i++){
    			result[i] = a[i]*b;
    		}
    		return result;
		}
		friend Vec<N,T> operator*(const T& a, const Vec<N, T>& b){
    		Vec<N,T> result = Vec<N,T>();
    		for(unsigned int i = 0; i<N; i++){
    			result[i] = a*b[i];
    		}
    		return result;
		}
		friend Vec<N,T> operator/(const Vec<N,T>& a, const T& b){
    		return a*(1/b);
		}
		friend Vec<N,T> operator%(const Vec<N,T>& a, const T& b){
    		Vec<N,T> result = Vec<N,T>();
    		for(unsigned int i = 0; i<N; i++){
    			result[i] = a[i]%b;
    		}
    		return result;
		}
		friend bool operator==(const Vec<N, T>& a, const Vec<N,T>& b){
			for(unsigned int i = 0; i<N; i++){
				if (a[i]!=b[i]) return false;
			}
			return true;
		}
		friend bool operator!=(const Vec<N, T>& a, const Vec<N, T>& b){
			return !(a==b);
		}
		friend std::ostream& operator<<(std::ostream& o, const Vec<N,T>& vec){
			o << "Vec" << N << "<";
			for (int i = 0; i<N; i++){
				o << vec[i];
				if (i!=N-1){
					o<<", ";
				}
			}
			o << ">";
			return o;
			
		}
	};

	using Vec3f = Vec<3, float>;
	using Vec3d = Vec<3, double>;
}
#endif //SMATH_VEC3_HPP
