/*
 * MIT License
 * Copyright © 2026 NyTekCFW
 * See LICENSE file for details.
*/

#include <iostream>
#include <chrono>
#include <cstdio>
#include <stdexcept>
#include "./PointerObfuscator.hpp"

static uintptr_t	g_AllocKey = 0;
static uintptr_t	g_staticKey = 0;
static uintptr_t**	tabLoc = nullptr;
static uintptr_t*	g_data[4] =
{
	reinterpret_cast<uintptr_t*>(0xDEADC0DE),
	reinterpret_cast<uintptr_t*>(0xDEADBEEF),
	reinterpret_cast<uintptr_t*>(0xBAADF00D),
	reinterpret_cast<uintptr_t*>(0xBADDCAFE),
};

void allocatedEx(void)
{
	size_t		i = 0;
	uintptr_t	key = 0;
	uintptr_t**	tab = new uintptr_t*[4];
	uintptr_t**	d = tab;
	
	if (tab == nullptr)
		return;
	std::cout << "Allocated Memory At:" << tab << std::endl;
	//? Generate a key
	key = PointerObfuscator::createKey(tab);
	//? Assign data into tab (ptr value must be read and stored instead of directly set)
	tab[0] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xDEADC0DE), key);
	tab[1] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xDEADBEEF), key);
	tab[2] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xBAADF00D), key);
	tab[3] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xBADDCAFE), key);
	//Define and set
	g_AllocKey = key;
	tab = PointerObfuscator::tab(tab, key);
	std::cout << "[allocatedEx] Tab Address: " << PointerObfuscator::tab(tab, g_AllocKey) << " <- Base / Obfuscated -> " << tab << std::endl;
	while (i < 4)
	{
		auto dec = PointerObfuscator::tab(tab, g_AllocKey);

		std::cout << "[allocatedEx] Data: " << dec[i]
				<< " <- Obfuscated / Base -> "
				<< PointerObfuscator::data(dec[i], g_AllocKey)
				<< std::endl;

		++i;
	}
	std::cout << "Update Key:" << std::endl;
	tab = PointerObfuscator::update<true>(tab, g_AllocKey, 4);
	std::cout << "[allocatedEx] Tab Address: " << PointerObfuscator::tab(tab, g_AllocKey) << " <- Base / Obfuscated -> " << tab << std::endl;
	i = 0;
	while (i < 4)
	{
		auto dec = PointerObfuscator::tab(tab, g_AllocKey);

		std::cout << "[allocatedEx] Data: " << dec[i]
				<< " <- Obfuscated / Base -> "
				<< PointerObfuscator::data(dec[i], g_AllocKey)
				<< std::endl;

		++i;
	}
	uintptr_t* cpyTab[4] = { nullptr };

	if (PointerObfuscator::copyDecodedTable<true>(cpyTab, tab, g_AllocKey, 4))
	{
		for (size_t i = 0; i < 4; ++i)
		{
			std::cout << "[allocatedEx] Data value: "
					<< cpyTab[i]
					<< std::endl;
		}
	}
	std::cout << "[allocatedEx] Data at 0: " << PointerObfuscator::getDecodedData<true>(tab, 0, g_AllocKey, 4) << std::endl;
	delete[](d);
}

void staticEx(void)
{
	size_t				i = 0;
	uintptr_t			key = 0;

	std::cout << "static tab At:" << g_data << std::endl;
	//? Generate a key
	key = PointerObfuscator::createKey(g_data);
	//? Assign data into tab (ptr value must be read and stored instead of directly set)
	g_data[0] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xDEADC0DE), key);
	g_data[1] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xDEADBEEF), key);
	g_data[2] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xBAADF00D), key);
	g_data[3] = PointerObfuscator::data(reinterpret_cast<uintptr_t*>(0xBADDCAFE), key);
	//Define and set
	g_staticKey = key;
	tabLoc = PointerObfuscator::tab(g_data, key);//! Obfuscate only the contents and not the address of g_data since its static
	std::cout << "[StaticEx] Tab Address: " << PointerObfuscator::tab(tabLoc, g_staticKey) << " <- Base / Obfuscated -> " << tabLoc << std::endl;
	while (i < 4)
	{
		auto dec = PointerObfuscator::tab(tabLoc, g_staticKey);

		std::cout << "[StaticEx] Data: " << dec[i]
				<< " <- Obfuscated / Base -> "
				<< PointerObfuscator::data(dec[i], g_staticKey)
				<< std::endl;

		++i;
	}
	std::cout << "Update Key:" << std::endl;
	tabLoc = PointerObfuscator::update<false>(g_data, g_staticKey, 4);
	std::cout << "[StaticEx] Tab Address: " << PointerObfuscator::tab(tabLoc, g_staticKey) << " <- Base / Obfuscated -> " << tabLoc << std::endl;
	i = 0;
	while (i < 4)
	{
		auto dec = PointerObfuscator::tab(tabLoc, g_staticKey);

		std::cout << "[StaticEx] Data: " << dec[i]
				<< " <- Obfuscated / Base -> "
				<< PointerObfuscator::data(dec[i], g_staticKey)
				<< std::endl;

		++i;
	}
	uintptr_t* cpyTab[4] = { nullptr };

	if (PointerObfuscator::copyDecodedTable<false>(cpyTab, g_data, g_staticKey, 4))
	{
		for (size_t i = 0; i < 4; ++i)
		{
			std::cout << "[StaticEx] Data value: "
					<< cpyTab[i]
					<< std::endl;
		}
	}
	std::cout << "[StaticEx] Data at 0: " << PointerObfuscator::getDecodedData<false>(g_data, 0, g_staticKey, 4) << std::endl;
}

int main(void)
{
    std::cout << "Pointer Obfuscator Sample:" << std::endl;
	std::cout << "Allocated Memory:" << std::endl;
	allocatedEx();
	std::cout << "Static Memory:" << std::endl;
	staticEx();
    return (0);
}