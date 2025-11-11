#ifndef SMATH_VEC_HPP
#define SMATH_VEC_HPP



namespace smath {
	template<unsigned long N, typename T>
	class Vec{
	private:
		T data[N];
	public:
		/* Constructors */
		Vec():data{}{};
		template<typename... Args> requires (sizeof...(Args)==N)
		Vec(Args... args):data{static_cast<T>(args)...}{}
		Vec(const Vec& other) = default;
		Vec(const Vec&& other) noexcept = default;
		/* Getters */
	};
	
}
#endif //SMATH_VEC3_HPP
