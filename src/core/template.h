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