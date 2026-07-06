/*
 * MIT License
 * Copyright © 2026 NyTekCFW
 * See LICENSE file for details.
*/

#pragma once

#ifndef POINTEROBFUSCATOR_HPP
# define POINTEROBFUSCATOR_HPP

# ifndef __SNC__
#  include <cstdint>
#  include <cstddef>
# else
#  include <sys/sys_types.h>
# endif


namespace PointerObfuscator
{
	uintptr_t			createKey(uintptr_t** tab) noexcept;
	uintptr_t**			update(uintptr_t** tab, uintptr_t& ckey, size_t elementsCount) noexcept;

	#ifndef __SNC__
		[[gnu::always_inline]] inline
	# else
		inline __attribute__((always_inline))
	#endif
	uintptr_t**	tab(uintptr_t** tab, const uintptr_t key) noexcept
	{
		return ((tab == nullptr) ? (nullptr) : reinterpret_cast<uintptr_t**>(reinterpret_cast<uintptr_t>(tab) ^ key));
	}

	#ifndef __SNC__
		[[gnu::always_inline]] inline
	# else
		inline __attribute__((always_inline))
	#endif
	uintptr_t*	data(uintptr_t* ptr, const uintptr_t key) noexcept
	{
		return ((ptr == nullptr) ? (nullptr) : reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(ptr) ^ key));
	}

	template <bool decodeTableAddress>
	#ifndef __SNC__
		[[gnu::always_inline]] inline
	# else
		inline __attribute__((always_inline))
	#endif
	//? decodeTableAddress == true  : table address is obfuscated.
	//? decodeTableAddress == false : only the entries are obfuscated.
	uintptr_t*	getDecodedData(uintptr_t** table, size_t index, uintptr_t key, size_t elementsCount) noexcept
	{
		if (!table || index >= elementsCount)
			return (nullptr);
		if (decodeTableAddress)
			table = PointerObfuscator::tab(table, key);
		return (PointerObfuscator::data(table[index], key));
	}

	//? decodeTableAddress == true  : table address is obfuscated.
	//? decodeTableAddress == false : only the entries are obfuscated.
	template <bool decodeTableAddress>
	bool	copyDecodedTable(uintptr_t** dest, uintptr_t** src, uintptr_t key, size_t elementsCount) noexcept
	{
		if (!dest || !src)
			return false;

		if (decodeTableAddress)
			src = PointerObfuscator::tab(src, key);
		for (size_t i = 0; i < elementsCount; ++i)
			dest[i] = PointerObfuscator::data(src[i], key);
		return true;
	}
};

#endif