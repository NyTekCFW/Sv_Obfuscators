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
	static constexpr uint64_t fnv1a(const char* s, uint64_t h = 1469598103934665603ULL)
	{
		return ((*s) ? (fnv1a(s + 1, (h ^ static_cast<uint8_t>(*s)) * 1099511628211ULL)) : (h));
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
	template<int... I> struct IndexList {};

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
	template <int N> struct MakeIndex
	{
		using type = typename Append<typename MakeIndex<N - 1>::type, N - 1>::type;
	};

	template <> struct MakeIndex<0>
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
	
	inline constexpr uint8_t mix(uint64_t k, int i, int u = 0)
	{
		return (uint8_t((((u == 0) ? (mix((k ^ (k >> 33)) * 0xff51afd7ed558ccdULL, i, 1)) : (k ^ (k >> 33))) + uint64_t(i) * 1315423911u) & 0xFF));
	}

	static constexpr char xorChar(const char c, uint64_t k, int i)
	{
		return (char(uint8_t(c) ^ mix(k, i)));
	}

	static constexpr char getKeyStream(uint64_t k, int i)
	{
		return (mix(k, i));
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


	template<typename IndexList, bool isHeavy> class ObfString;

	template<int... Index, bool isHeavy>
	class ObfString<IndexList<Index...>, isHeavy>
	{
	private:
		volatile bool		_locked;
		volatile uint64_t	_key;
		volatile char		_data[sizeof...(Index) + 1];
		volatile char		_ks[sizeof...(Index) + 1];
	public:
		//? ========================================================
		//? Compile-time constructor
		//?
		//? - xor the string literal at compile-time
		//? - Derives a unique key using BUILD_KEY + salt
		//? ========================================================
		inline constexpr ObfString(const char* s, uint64_t salt): _locked(true), _key(salt), _data{ xorChar(s[Index], salt, Index)... }, _ks{ getKeyStream(salt, Index)... } {}

		//? ========================================================
		//? unlock the string (unxor)
		//?
		//? Safe to call multiple times.
		//? ========================================================
		inline volatile char* unlock(void)
		{
			if (_locked)
			{
				if (isHeavy)
				{
					int dummy[] = { (_data[Index] ^= (((Index & 3) == ((_key >> 5) & 3)) ? svxor::mix(_key, Index) : _ks[Index]), 0)... };
					(void)dummy;
				}
				else
				{
					for (int i = 0; i < sizeof...(Index); ++i)
					{
						_data[i] ^= _ks[i];
					}
				}
				_data[sizeof...(Index)] = '\0';
				_locked = false;
			}
			return (_data);
		}

		//? ========================================================
		//? Explicitly re-xor the string
		//?
		//? Safe to call multiple times.
		//? ========================================================
		inline volatile char* lock(void)
		{
			if (!_locked)
			{
				if (isHeavy)
				{
					int dummy[] = { (_data[Index] ^= (((Index & 3) == ((_key >> 5) & 3)) ? svxor::mix(_key, Index) : _ks[Index]), 0)... };
					(void)dummy;
				}
				else
				{
					for (int i = 0; i < sizeof...(Index); ++i)
						_data[i] ^= _ks[i];
				}
				_data[sizeof...(Index)] = '\0';
				_locked = true;
			}
			return _data;
		}

		//? ========================================================
		//? alias for unxor()
		//? ========================================================
		inline const char* c_str(void)
		{
			return (const_cast<const char*>(this->unlock()));
		}
		//? ========================================================
		//? Destructor
		//?
		//? Clears buffer and key to reduce memory residue.
		// ========================================================
		~ObfString(void)
		{
			_key = 0;
			for (size_t i = 0; i < sizeof...(Index); ++i)
			{
				_data[i] = 0;
				_ks[i] = 0;
			}
			_data[sizeof...(Index)] = 0;
			_ks[sizeof...(Index)] = 0;
		}
	};
}

//! ============================================================
//! MACROS
//! ============================================================

//? ============================================================
//? RAII object (stack-based, safe lifetime)
//? ============================================================
# define OBF(s, heavy) (svxor::ObfString<svxor::MakeIndex<sizeof(s) - 1>::type, heavy>(s, svxor::BUILD_KEY ^ (uint64_t(__COUNTER__) * 0x9E3779B97F4A7C15ULL)))
# define OBF_H(s) OBF(s, true)
# define OBF_L(s) OBF(s, false)

//? ============================================================
//? One-shot helper (INTENTIONALLY UB lifetime)
//?
//? Intended for immediate use only (printf, comparisons, etc).
//? ============================================================
# define OBF_ONCE(s, heavy) OBF(s, heavy).c_str()
# define OBF_HONCE(s) OBF_ONCE(s, true)
# define OBF_LONCE(s) OBF_ONCE(s, false)

#endif