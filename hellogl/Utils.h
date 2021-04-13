#pragma once

#include <iostream>
#include <glm/ext.hpp>

template <typename T, size_t N>
static std::ostream& operator<<(std::ostream& o, const glm::vec<N, T>& v)
{
	o << "( ";
	for (int i = 0; i < v.length(); i++) {
		o << v[i] << " ";
	}
	return o << ")";
}

template <typename T, size_t M, size_t N>
static std::ostream& operator<<(std::ostream& o, const glm::mat<M, N, T>& m)
{
	
	o << "[";
	for (int i = 0; i < m.length(); i++) {
		o << m[i] << " ";
	}
	return o << "]";
}