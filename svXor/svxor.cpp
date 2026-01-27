/*
 * MIT License
 * Copyright © 2026 NyTekCFW
 * See LICENSE file for details.
*/

#include <iostream>
#include <chrono>
#include <cstdio>
#include <stdexcept>
#include "./svxor.hpp"

// =========================
// TEST
// =========================

int main()
{
	try
	{
		std::cout << "===== ONE SHOT TEST =====\n";
		std::cout << OBF_ONCE("Hello world", true) << "\n";
		std::cout << OBF_ONCE("Light string", false) << "\n";

		std::cout << "\n===== RAII OBJECT BASIC TEST =====\n";
		auto obj = OBF("RAII string", true);

		const char* p1 = obj.c_str();
		std::cout << "Decrypt #1: " << p1 << "\n";

		obj.lock();
		std::cout << "After recrypt (garbage expected): " << p1 << "\n";

		const char* p2 = obj.c_str();
		std::cout << "Decrypt #2: " << p2 << "\n";

		// pointer stability
		if (p1 != p2)
		std::cout << "[WARN] pointer changed (unexpected)\n";

		std::cout << "\n===== MULTI DECRYPT TEST =====\n";
		auto md = OBF("MultiDecrypt", true);
		for (int i = 0; i < 5; ++i)
		std::cout << "Decrypt #" << i << ": " << md.c_str() << "\n";

		std::cout << "\n===== MULTI RECRYPT TEST =====\n";
		auto mr = OBF("MultiRecrypt", true);
		const char* pr = mr.c_str();
		std::cout << "Initial: " << pr << "\n";

		for (int i = 0; i < 3; ++i)
		{
			mr.lock();
			std::cout << "Recrypt #" << i << ": " << pr << "\n";
		}

		std::cout << "Decrypt again: " << mr.c_str() << "\n";

		std::cout << "\n===== MIXED OBJECT TEST =====\n";
		auto a = OBF("First heavy", true);
		auto b = OBF("Second light", false);

		std::cout << a.c_str() << "\n";
		std::cout << b.c_str() << "\n";

		a.lock();
		b.lock();

		std::cout << a.c_str() << "\n";
		std::cout << b.c_str() << "\n";

		std::cout << "\n===== ONE-SHOT UB TEST (EXPECTED) =====\n";
		const char* ub;
		{
			ub = OBF_ONCE("UB string", true);
		}
		// UB by design
		std::cout << "UB read (garbage or crash acceptable): <" << ub << ">\n";

		std::cout << "\n===== MIXED PRINTF TEST =====\n";
		printf("%s | %s | %s\n", OBF_ONCE("One", true), OBF_ONCE("Two", false), OBF_ONCE("Three", true));

		std::cout << "\n===== LOOP STABILITY TEST =====\n";
		{
			auto t = OBF("LoopTest", (0 & 1));
			std::cout << "[" << 0 << "] " << t.c_str() << "\n";
			t.lock();

			auto x = OBF("LoopTest", (1 & 1));
			std::cout << "[" << 1 << "] " << x.c_str() << "\n";
			x.lock();

			auto y = OBF("LoopTest", (2 & 1));
			std::cout << "[" << 2 << "] " << y.c_str() << "\n";
			y.lock();

			auto z = OBF("LoopTest", (3 & 1));
			std::cout << "[" << 3 << "] " << z.c_str() << "\n";
			z.lock();

			auto e = OBF("LoopTest", (4 & 1));
			std::cout << "[" << 4 << "] " << e.c_str() << "\n";
			e.lock();
		}

		std::cout << "\n===== PERFORMANCE MICRO TEST =====\n";
		constexpr int iters = 1'000'000;
		volatile size_t sink = 0;

		auto bench = [&](const char* name, auto fn)
		{
			auto start = std::chrono::high_resolution_clock::now();
			fn();
			auto end = std::chrono::high_resolution_clock::now();
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << name << ": " << ms << " ms\n";
		};

		bench("One-shot heavy", [&]
		{
			for (int i = 0; i < iters; ++i)
				sink += OBF_ONCE("Bench", true)[0];
		});

		bench("One-shot light", [&]
		{
			for (int i = 0; i < iters; ++i)
				sink += OBF_ONCE("Bench", false)[0];
		});

		bench("RAII heavy", [&]
		{
			for (int i = 0; i < iters; ++i)
			{
				auto o = OBF("Bench", true);
					sink += o.c_str()[0];
				o.lock();
			}
		});
		std::cout << "\nSink (ignore): " << sink << "\n";
		std::cout << "\n===== DONE =====\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "std::exception caught: " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cerr << "unknown exception caught\n";
		return 2;
	}
	return 0;
}