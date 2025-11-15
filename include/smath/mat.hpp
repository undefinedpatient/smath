#ifndef SMATH_MAT_HPP
#define SMATH_MAT_HPP

#include "smath/vec.hpp"
#include <cmath>
#include <concepts>
#include <ostream>
#include <stdexcept>
#include <string>

namespace smath{
	/*
	    M := row, N := column
    	Mat is a column major matrix
    	Meaning the layout is:
    	[r0,r0,r0,r1,r1,r1,r2,r2,r2]
  	*/
	template<unsigned int M, unsigned int N, class T>
	class Mat{
		private:
			T data[M*N];
		public:
		/* Constructors */
		Mat():data{}{}
		template<class... Us> requires ((sizeof...(Us)==M*N) && (std::convertible_to<Us, T> && ...))
		Mat(Us... args):data{static_cast<T>(args)...}{}
		Mat(const Mat& other) = default;
		Mat(Mat&& other) noexcept = default;
		Mat& operator=(const Mat<M, N, T>& other) = default;
		const Mat& operator=(const Mat<M, N, T>& other) const = default;
		/* Getters */
		T* operator[](unsigned int col_num) {
			if(col_num<0||col_num>N-1){
				throw std::out_of_range("Index out of bound");
			}
			return data[col_num*M];
		}
		const T* operator[](unsigned int col_num) const {
			if(col_num<0||col_num>N-1){
				throw std::out_of_range("Index out of bound");
			}
			return data[col_num*M];
		}
		std::string toString() const {
			std::string str = "";
			for (unsigned int m = 0; m<M;m++){
				str += "[";
				for (unsigned int n = 0; n<N;n++){
					str += std::to_string(n);
					if (n!=N-1)
						str += ",";
				}
				str += "]\n";
			}
			return str;
		}
		/* Operations */
		Vec<M, T>* toVectors() const {
			Vec<M, T> vec[M];
			for(int n=0; n<N; n++){
				T temp_data[M];
				for(int m=0; m<M; m++){
					temp_data[m] = :w
					
				}
				vec[n] = Vec<M, T>();
			}
		}
		friend Mat<M,N,T> operator+(const Mat<M,N,T>& a, const Mat<M,N,T>& b){
			Mat<M,N,T> result = Mat<M,N,T>();
			for(unsigned int m = 0; m<M; m++){
				for(unsigned int n = 0; n<N; n++){
					result.data[m+n] = a.data[m+n] + b.data[m+n];
				}
				return result;
			}
		}
		friend Mat<M,N,T> operator-(const Mat<M,N,T>& a, const Mat<M,N,T>& b){
			Mat<M,N,T> result = Mat<M,N,T>();
			for(unsigned int m = 0; m<M; m++){
				for(unsigned int n = 0; n<N; n++){
					result.data[m+n] = a.data[m+n] - b.data[m+n];
				}
				return result;
			}
		}
	};
}
#endif
