#pragma once

typedef unsigned int uint;

/**
@brief		squares the passed parameter
@param a	value to square
@return		squared value
*/
template<typename T>
inline T sq(T a) {
	return a * a;
}

/**
@brief		cube parameter
@param a	value to cube
@return		cubed value
*/
template<typename T>
inline T cb(T a) {
	return a * a * a;
}