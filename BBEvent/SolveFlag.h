#ifndef BB_HANDLE_FLAG_H
#define BB_HANDLE_FLAG_H

namespace BB
{
	//! It it possible for solverflagseSolveFlags to evolve into priority-like eSolvePriorityFlag or 

	enum class eSolveFlags : char
	{
		NONE = 0x00,
		SPECIFIC = 0x01,// hex for 0000 0001
		THREADED = 0x02,// hex for 0000 0010
		ANY = SPECIFIC | THREADED
	};
	using T = std::underlying_type_t <eSolveFlags>;

	//AND &
	inline eSolveFlags operator & (eSolveFlags lhs, eSolveFlags rhs)
	{
		return (eSolveFlags)(static_cast<T>(lhs) & static_cast<T>(rhs));
	}
	inline eSolveFlags& operator &= (eSolveFlags& lhs, eSolveFlags rhs)
	{
		lhs = (eSolveFlags)(static_cast<T>(lhs) & static_cast<T>(rhs));
		return lhs;
	}

	//OR |
	inline eSolveFlags operator | (eSolveFlags lhs, eSolveFlags rhs)
	{
		return (eSolveFlags)(static_cast<T>(lhs) | static_cast<T>(rhs));
	}
	inline eSolveFlags& operator |= (eSolveFlags& lhs, eSolveFlags rhs)
	{
		lhs = (eSolveFlags)(static_cast<T>(lhs) | static_cast<T>(rhs));
		return lhs;
	}
	//XOR ^
	inline eSolveFlags operator ^ (eSolveFlags lhs, eSolveFlags rhs)
	{
		return (eSolveFlags)(static_cast<T>(lhs) ^ static_cast<T>(rhs));
	}
	inline eSolveFlags& operator ^= (eSolveFlags& lhs, eSolveFlags rhs)
	{
		lhs = (eSolveFlags)(static_cast<T>(lhs) ^ static_cast<T>(rhs));
		return lhs;
	}
}

#endif