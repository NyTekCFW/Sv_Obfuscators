/*
 * MIT License
 * Copyright © 2026 NyTekCFW
 * See LICENSE file for details.
*/

#pragma once

#ifndef SVXOR_HPP
# define SVXOR_HPP

# include <cstdint>
# include <cstddef>

namespace svxor
{
	//? ============================================================
	//? Compile-time FNV-1a hash
	//?
	//? Used to derive keys from string literals and build metadata.
	//? This runs entirely at compile-time for string literals.
	//? ============================================================
	constexpr uint64_t fnv1a(const char* s, uint64_t h = 1469598103934665603ULL)
	{
		return ((*s) ? (fnv1a(s + 1, (h ^ uint8_t(*s)) * 1099511628211ULL)) : (h));
	}

	//? ============================================================
	//? Global build-specific key based on DATE TIME FILE
	//?
	//? Changes every compilation to break binary diffing
	//? and prevent static key reuse across builds.
	//? ============================================================
	constexpr uint64_t BUILD_KEY = fnv1a(__DATE__ __TIME__ __FILE__);

	//? ============================================================
	//? C++11 index sequence implementation
	//?
	//? Used to expand character indices at compile-time
	//? (port in C++11 of std::index_sequence in C++14+).
	//? ============================================================
	template<int... I> struct IndexList
	{
	};

	//? ============================================================
	//? Append an index to an IndexList
	//? ============================================================
	template<typename L, int R> struct Append;
	template<int... L, int R>
	struct Append<IndexList<L...>, R>
	{
		using type = IndexList<L..., R>;
	};

	//? ============================================================
	//? Generate IndexList<0, 1, 2, ..., N-1>
	//? ============================================================
	template<int N> struct MakeIndex
	{
		using type = typename Append<typename MakeIndex<N - 1>::type, N - 1>::type;
	};

	template<> struct MakeIndex<0>
	{
		using type = IndexList<>;
	};

	//? ============================================================
	//? Key mixing function
	//?
	//? Derives a pseudo-random byte from:
	//?  - the per-string key
	//?  - the character index
	//?
	//? Heavy enough to avoid trivial XOR patterns,
	//? cheap enough for runtime use.
	//? Umix bypass assignation restricted in C++11
	//? ============================================================
	
	constexpr uint64_t	umix(uint64_t k, int u)
	{
		return (u == 0 ? umix((k ^ (k >> 33)) * 0xff51afd7ed558ccdULL, 1) : (k ^ (k >> 33)));
	}

	constexpr uint8_t mix(uint64_t k, int i)
	{
		return (uint8_t((umix(k, 0) + uint64_t(i) * 1315423911u) & 0xFF));
	}


	//? ============================================================
	//? Compile-time xor of a single character
	//?
	//? XORs the character with its derived key byte.
	//? ============================================================
	constexpr char xorChar(char c, uint64_t k, int i)
	{
		return (char(uint8_t(c) ^ mix(k, i)));
	}

	//? ============================================================
	//? Obfuscated string object (RAII)
	//?
	//? Stores an xored string in-place.
	//? unxor and re-xor are explicit and controlled.
	//?
	//? HEAVY mode:
	//?  - forces template expansion per character
	//?  - produces noisier assembly
	//?
	//? LIGHT mode:
	//?  - simple loop
	//?  - smaller and faster
	//? ============================================================
	template<bool HEAVY, typename> class ObfString;
	template<bool HEAVY, int... I>
	class ObfString<HEAVY, IndexList<I...>>
	{
	public:
		//? ========================================================
		//? Compile-time constructor
		//?
		//? - xor the string literal at compile-time
		//? - Derives a unique key using BUILD_KEY + salt
		//? ========================================================
		constexpr ObfString(const char* s, uint64_t salt) : key(fnv1a(s, BUILD_KEY^ salt)), data{ xorChar(s[I], key, I)..., 0 }, locked(true)
		{
		}

		//? ========================================================
		//? unlock the string (unxor)
		//?
		//? Safe to call multiple times.
		//? ========================================================
		const char* unlock(void)
		{
			if (this->locked)
			{
				this->xor_buffer();
				this->locked = false;
			}
			return (this->data);
		}

		//? ========================================================
		//? Explicitly re-xor the string
		//?
		//? Safe to call multiple times.
		//? ========================================================
		void lock(void)
		{
			if (!this->locked)
			{
				this->xor_buffer();
				this->locked = true;
			}
		}

		//? ========================================================
		//? alias for unxor()
		//? ========================================================
		const char* c_str(void)
		{
			return (this->unlock());
		}

		//? ========================================================
		//? Destructor
		//?
		//? Clears buffer and key to reduce memory residue.
		// ========================================================
		~ObfString(void)
		{
			for (size_t i = 0; i < sizeof...(I); ++i)
			{
				this->data[i] = 0;
			}
			this->key = 0;
		}
	private:
		uint64_t	key;					//! Per-string derived key
		char		data[sizeof...(I) + 1];	//! string buffer (+ null terminator)
		bool		locked;					//! Tracks current state to avoid double-xor

		//? ========================================================
		//? XOR the buffer in-place (xor <-> unxor)
		//?
		//? Same operation used for unxor and xor.
		//? ========================================================
		inline void xor_buffer(void)
		{
			if (HEAVY)						//! Unrolled, index-based expansion
			{
				int dummy[] = { (this->data[I] ^= mix(this->key, I), 0)... };
				(void)dummy;
				return;
			}
			else
			{
				for (size_t i = 0; i < sizeof...(I); ++i)
				{
					this->data[i] ^= mix(this->key, int(i));
				}
			}
		}
	};
}

//! ============================================================
//! MACROS
//! ============================================================

//? ============================================================
//? RAII object (stack-based, safe lifetime)
//? ============================================================
# define OBF(s, heavy) svxor::ObfString<heavy, svxor::MakeIndex<sizeof(s)-1>::type>(s, __COUNTER__)
# define OBF_H(s) OBF(s, true)
# define OBF_L(s) OBF(s, false)

//? ============================================================
//? One-shot helper (INTENTIONALLY UB lifetime)
//?
//? Intended for immediate use only (printf, comparisons, etc).
//? ============================================================
# define OBF_ONCE(s, heavy) OBF(s, heavy).c_str()
# define OBF_HONCE(s) OBF_H(s).c_str()
# define OBF_LONCE(s) OBF_L(s).c_str()

#endif