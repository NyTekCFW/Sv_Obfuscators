/*
 * MIT License
 * Copyright © 2026 NyTekCFW
 * See LICENSE file for details.
*/

#include "./PointerObfuscator.hpp"

#ifndef __SNC__
//? ============================================================
//? Based on std::chrono (non-SNC compiler)
//? ============================================================
# include <chrono>
# if defined(_MSC_VER)
   #include <intrin.h>
#endif

static inline uint64_t getTick(void)
{
    return (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

static inline uint64_t getReturnAddress()
{
	#if defined(_MSC_VER)
		return reinterpret_cast<uint64_t>(_ReturnAddress());

	#elif defined(__GNUC__) || defined(__clang__)
		return reinterpret_cast<uint64_t>(__builtin_return_address(0));

	#else
		return 0;
	#endif
}

# else
//? ============================================================
//? SNC compiler: private code
//? ============================================================

#include "../Core/System/scheduler.hpp"

static inline __attribute__((always_inline)) uint64_t getTick(void)
{
	return (svsc::scheduler::tick());//private library
}

static inline __attribute__((always_inline)) uint64_t getReturnAddress()
{
	return (reinterpret_cast<uint64_t>(__builtin_return_address()));
}
# endif

uintptr_t	PointerObfuscator::createKey(uintptr_t** tab) noexcept
{
	return (static_cast<uintptr_t>(((getTick() ^ reinterpret_cast<uint64_t>(tab) ^ getReturnAddress()) & ~uintptr_t(3)) + 1));
}