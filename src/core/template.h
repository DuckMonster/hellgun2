#pragma once

template<typename T> struct Remove_Reference { typedef T type; };
template<typename T> struct Remove_Reference<T&> { typedef T type; };
template<typename T> struct Remove_Reference<T&&> { typedef T type; };

template<typename T>
typename Remove_Reference<T>::type&& move(T&& val)
{
	typedef typename Remove_Reference<T>::type Move_Type;
	return (Move_Type&&)val;
}

template<typename T>
void swap(T& a, T& b)
{
	T temp = b;
	b = a;
	a = temp;
}

template<typename T>
struct Optional
{
public:
	void unset() { _is_set = false; }
	bool is_set() const { return _is_set; }

	T& value() { return _value; }
	const T& value() const { return _value; }

	Optional& operator=(const T& val)
	{
		_is_set = true;
		_value = val;
		return *this;
	}
	bool operator==(const T& other)
	{
		if (!_is_set)
			return false;

		return _value == other;
	}
	bool operator!=(const T& other) { return !(*this == other); }

	T& operator*() { return _value; }
	const T& operator*() const { return _value; }

private:
	bool _is_set;
	T _value;
};