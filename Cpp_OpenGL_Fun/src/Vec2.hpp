template<typename T>
Vec2<T>::Vec2() : data{ 0.0f, 0.0f } {}

template<typename T>
Vec2<T>::Vec2(T _x, T _y) : data{ _x, _y } {}

template<typename T>
Vec2<T>& Vec2<T>::operator*=(const T& obj) {
	this->x *= obj;
	this->y *= obj;
	return *this;
}

template<typename T>
const T& Vec2<T>::operator[](size_t idx) const { return data[idx]; }

template<typename T>
T& Vec2<T>::operator[](size_t idx) { return data[idx]; }