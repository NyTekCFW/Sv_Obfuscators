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

// int main()
// {
// 	try
// 	{
// 		std::cout << "===== ONE SHOT TEST =====\n";
// 		std::cout << OBF_ONCE("Hello world", true) << "\n";
// 		std::cout << OBF_ONCE("Light string", false) << "\n";

// 		std::cout << "\n===== RAII OBJECT BASIC TEST =====\n";
// 		auto obj = OBF("RAII string", true);

// 		const char* p1 = obj.c_str();
// 		std::cout << "Decrypt #1: " << p1 << "\n";

// 		obj.lock();
// 		std::cout << "After recrypt (garbage expected): " << p1 << "\n";

// 		const char* p2 = obj.c_str();
// 		std::cout << "Decrypt #2: " << p2 << "\n";

// 		// pointer stability
// 		if (p1 != p2)
// 		std::cout << "[WARN] pointer changed (unexpected)\n";

// 		std::cout << "\n===== MULTI DECRYPT TEST =====\n";
// 		auto md = OBF("MultiDecrypt", true);
// 		for (int i = 0; i < 5; ++i)
// 		std::cout << "Decrypt #" << i << ": " << md.c_str() << "\n";

// 		std::cout << "\n===== MULTI RECRYPT TEST =====\n";
// 		auto mr = OBF("MultiRecrypt", true);
// 		const char* pr = mr.c_str();
// 		std::cout << "Initial: " << pr << "\n";

// 		for (int i = 0; i < 3; ++i)
// 		{
// 			mr.lock();
// 			std::cout << "Recrypt #" << i << ": " << pr << "\n";
// 		}

// 		std::cout << "Decrypt again: " << mr.c_str() << "\n";

// 		std::cout << "\n===== MIXED OBJECT TEST =====\n";
// 		auto a = OBF("First heavy", true);
// 		auto b = OBF("Second light", false);

// 		std::cout << a.c_str() << "\n";
// 		std::cout << b.c_str() << "\n";

// 		a.lock();
// 		b.lock();

// 		std::cout << a.c_str() << "\n";
// 		std::cout << b.c_str() << "\n";

// 		std::cout << "\n===== ONE-SHOT UB TEST (EXPECTED) =====\n";
// 		const char* ub;
// 		{
// 			ub = OBF_ONCE("UB string", true);
// 		}
// 		// UB by design
// 		std::cout << "UB read (garbage or crash acceptable): <" << ub << ">\n";

// 		std::cout << "\n===== MIXED PRINTF TEST =====\n";
// 		printf("%s | %s | %s\n", OBF_ONCE("One", true), OBF_ONCE("Two", false), OBF_ONCE("Three", true));

// 		std::cout << "\n===== LOOP STABILITY TEST =====\n";
// 		{
// 			auto t = OBF("LoopTest", (0 & 1));
// 			std::cout << "[" << 0 << "] " << t.c_str() << "\n";
// 			t.lock();

// 			auto x = OBF("LoopTest", (1 & 1));
// 			std::cout << "[" << 1 << "] " << x.c_str() << "\n";
// 			x.lock();

// 			auto y = OBF("LoopTest", (2 & 1));
// 			std::cout << "[" << 2 << "] " << y.c_str() << "\n";
// 			y.lock();

// 			auto z = OBF("LoopTest", (3 & 1));
// 			std::cout << "[" << 3 << "] " << z.c_str() << "\n";
// 			z.lock();

// 			auto e = OBF("LoopTest", (4 & 1));
// 			std::cout << "[" << 4 << "] " << e.c_str() << "\n";
// 			e.lock();
// 		}

// 		std::cout << "\n===== PERFORMANCE MICRO TEST =====\n";
// 		constexpr int iters = 1'000'000;
// 		volatile size_t sink = 0;

// 		auto bench = [&](const char* name, auto fn)
// 		{
// 			auto start = std::chrono::high_resolution_clock::now();
// 			fn();
// 			auto end = std::chrono::high_resolution_clock::now();
// 			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
// 			std::cout << name << ": " << ms << " ms\n";
// 		};

// 		bench("One-shot heavy", [&]
// 		{
// 			for (int i = 0; i < iters; ++i)
// 				sink += OBF_ONCE("Bench", true)[0];
// 		});

// 		bench("One-shot light", [&]
// 		{
// 			for (int i = 0; i < iters; ++i)
// 				sink += OBF_ONCE("Bench", false)[0];
// 		});

// 		bench("RAII heavy", [&]
// 		{
// 			for (int i = 0; i < iters; ++i)
// 			{
// 				auto o = OBF("Bench", true);
// 					sink += o.c_str()[0];
// 				o.lock();
// 			}
// 		});
// 		std::cout << "\nSink (ignore): " << sink << "\n";
// 		std::cout << "\n===== DONE =====\n";
// 	}
// 	catch (const std::exception& e)
// 	{
// 		std::cerr << "std::exception caught: " << e.what() << "\n";
// 		return 1;
// 	}
// 	catch (...)
// 	{
// 		std::cerr << "unknown exception caught\n";
// 		return 2;
// 	}
// 	return 0;
// }

void LightObf(void)
{
	std::cout << "LightObf Function:" << std::endl;

	auto x = OBF_L("Light Obfuscation!");
	std::cout << OBF_LONCE("Current: ") << x.c_str() << std::endl;
}

void HeavyObf(void)
{
	std::cout << "HeavyObf Function:" << std::endl;

	auto x = OBF_H("Heavy Obfuscation!");
	std::cout << OBF_HONCE("Current: ") << x.c_str() << std::endl;
}


OBF_STL(sys_net, "sys_net")
OBF_STL(cellHttp, "cellHttp")
OBF_STL(cellHttpUtil, "cellHttpUtil")
OBF_STL(cellSsl, "cellSsl")
OBF_STL(cellHttps, "cellHttps")
OBF_STL(libvdec, "libvdec")
OBF_STL(cellAdec, "cellAdec")
OBF_STL(cellDmux, "cellDmux")
OBF_STL(cellVpost, "cellVpost")
OBF_STL(cellRtc, "cellRtc")
OBF_STL(cellSpurs, "cellSpurs")
OBF_STL(cellOvis, "cellOvis")
OBF_STL(cellSheap, "cellSheap")
OBF_STL(cellSync, "cellSync")
OBF_STL(sys_fs, "sys_fs")
OBF_STL(cellJpgDec, "cellJpgDec")
OBF_STL(cellGcmSys, "cellGcmSys")
OBF_STL(cellAudio, "cellAudio")
OBF_STL(cellPamf, "cellPamf")
OBF_STL(cellAtrac, "cellAtrac")
OBF_STL(cellNetCtl, "cellNetCtl")
OBF_STL(cellSysutil, "cellSysutil")
OBF_STL(sceNp, "sceNp")
OBF_STL(sys_io, "sys_io")
OBF_STL(cellPngDec, "cellPngDec")
OBF_STL(cellFont, "cellFont")
OBF_STL(cellFontFT, "cellFontFT")
OBF_STL(cell_FreeType2, "cell_FreeType2")
OBF_STL(cellUsbd, "cellUsbd")
OBF_STL(cellSail, "cellSail")
OBF_STL(cellL10n, "cellL10n")
OBF_STL(cellResc, "cellResc")
OBF_STL(cellDaisy, "cellDaisy")
OBF_STL(cellKey2char, "cellKey2char")
OBF_STL(cellMic, "cellMic")
OBF_STL(cellCamera, "cellCamera")
OBF_STL(cellVdecMpeg2, "cellVdecMpeg2")
OBF_STL(cellVdecAvc, "cellVdecAvc")
OBF_STL(cellAdecLpcm, "cellAdecLpcm")
OBF_STL(cellAdecAc3, "cellAdecAc3")
OBF_STL(cellAdecAtx, "cellAdecAtx")
OBF_STL(cellAdecAt3, "cellAdecAt3")
OBF_STL(cellDmuxPamf, "cellDmuxPamf")
OBF_STL(sys_lv2dbg, "sys_lv2dbg")
OBF_STL(cellSysutilAvcExt, "cellSysutilAvcExt")
OBF_STL(cellUsbPspcm, "cellUsbPspcm")
OBF_STL(cellSysutilAvconfExt, "cellSysutilAvconfExt")
OBF_STL(cellUserInfo, "cellUserInfo")
OBF_STL(cellSaveData, "cellSaveData")
OBF_STL(cellSubDisplay, "cellSubDisplay")
OBF_STL(cellRec, "cellRec")
OBF_STL(cellVideoExportUtility, "cellVideoExportUtility")
OBF_STL(cellGameExec, "cellGameExec")
OBF_STL(sceNp2, "sceNp2")
OBF_STL(cellSysutilAp, "cellSysutilAp")
OBF_STL(sceNpClans, "sceNpClans")
OBF_STL(cellOskExtUtility, "cellOskExtUtility")
OBF_STL(cellVdecDivx, "cellVdecDivx")
OBF_STL(cellJpgEnc, "cellJpgEnc")
OBF_STL(cellGame, "cellGame")
OBF_STL(cellBGDLUtility, "cellBGDLUtility")
OBF_STL(cellCelpEnc, "cellCelpEnc")
OBF_STL(cellGifDec, "cellGifDec")
OBF_STL(cellAdecCelp, "cellAdecCelp")
OBF_STL(cellAdecM2bc, "cellAdecM2bc")
OBF_STL(cellAdecM4aac, "cellAdecM4aac")
OBF_STL(cellAdecMp3, "cellAdecMp3")
OBF_STL(cellImeJpUtility, "cellImeJpUtility")
OBF_STL(cellMusicUtility, "cellMusicUtility")
OBF_STL(cellPhotoUtility, "cellPhotoUtility")
OBF_STL(cellPrintUtility, "cellPrintUtility")
OBF_STL(cellPhotoImportUtil, "cellPhotoImportUtil")
OBF_STL(cellMusicExportUtility, "cellMusicExportUtility")
OBF_STL(cellPhotoDecodeUtil, "cellPhotoDecodeUtil")
OBF_STL(cellSearchUtility, "cellSearchUtility")
OBF_STL(cellSysutilAvc2, "cellSysutilAvc2")
OBF_STL(cellSailRec, "cellSailRec")
OBF_STL(sceNpTrophy, "sceNpTrophy")
OBF_STL(cellSysutilNpEula, "cellSysutilNpEula")
OBF_STL(cellAdecAt3multi, "cellAdecAt3multi")
OBF_STL(cellAtracMulti, "cellAtracMulti")

#ifndef __SNC__
const char* staticObfx(int id)
{
	switch (id)
    {
        case 0x0000: return obf_sys_net().c_str();
        case 0x0001: return obf_cellHttp().c_str();
        case 0x0002: return obf_cellHttpUtil().c_str();
        case 0x0003: return obf_cellSsl().c_str();
        case 0x0004: return obf_cellHttps().c_str();
        case 0x0005: return obf_libvdec().c_str();
        case 0x0006: return obf_cellAdec().c_str();
        case 0x0007: return obf_cellDmux().c_str();
        case 0x0008: return obf_cellVpost().c_str();
        case 0x0009: return obf_cellRtc().c_str();
        case 0x000a: return obf_cellSpurs().c_str();
        case 0x000b: return obf_cellOvis().c_str();
        case 0x000c: return obf_cellSheap().c_str();
        case 0x000d: return obf_cellSync().c_str();
        case 0x000e: return obf_sys_fs().c_str();
        case 0x000f: return obf_cellJpgDec().c_str();
        case 0x0010: return obf_cellGcmSys().c_str();
        case 0x0011: return obf_cellAudio().c_str();
        case 0x0012: return obf_cellPamf().c_str();
        case 0x0013: return obf_cellAtrac().c_str();
        case 0x0014: return obf_cellNetCtl().c_str();
        case 0x0015: return obf_cellSysutil().c_str();
        case 0x0016: return obf_sceNp().c_str();
        case 0x0017: return obf_sys_io().c_str();
        case 0x0018: return obf_cellPngDec().c_str();
        case 0x0019: return obf_cellFont().c_str();
        case 0x001a: return obf_cellFontFT().c_str();
        case 0x001b: return obf_cell_FreeType2().c_str();
        case 0x001c: return obf_cellUsbd().c_str();
        case 0x001d: return obf_cellSail().c_str();
        case 0x001e: return obf_cellL10n().c_str();
        case 0x001f: return obf_cellResc().c_str();
        case 0x0020: return obf_cellDaisy().c_str();
        case 0x0021: return obf_cellKey2char().c_str();
        case 0x0022: return obf_cellMic().c_str();
        case 0x0023: return obf_cellCamera().c_str();
        case 0x0024: return obf_cellVdecMpeg2().c_str();
        case 0x0025: return obf_cellVdecAvc().c_str();
        case 0x0026: return obf_cellAdecLpcm().c_str();
        case 0x0027: return obf_cellAdecAc3().c_str();
        case 0x0028: return obf_cellAdecAtx().c_str();
        case 0x0029: return obf_cellAdecAt3().c_str();
        case 0x002a: return obf_cellDmuxPamf().c_str();
        case 0x002e: return obf_sys_lv2dbg().c_str();
        case 0x002f: return obf_cellSysutilAvcExt().c_str();
        case 0x0030: return obf_cellUsbPspcm().c_str();
        case 0x0031: return obf_cellSysutilAvconfExt().c_str();
        case 0x0032: return obf_cellUserInfo().c_str();
        case 0x0033: return obf_cellSaveData().c_str();
        case 0x0034: return obf_cellSubDisplay().c_str();
        case 0x0035: return obf_cellRec().c_str();
        case 0x0036: return obf_cellVideoExportUtility().c_str();
        case 0x0037: return obf_cellGameExec().c_str();
        case 0x0038: return obf_sceNp2().c_str();
        case 0x0039: return obf_cellSysutilAp().c_str();
        case 0x003a: return obf_sceNpClans().c_str();
        case 0x003b: return obf_cellOskExtUtility().c_str();
        case 0x003c: return obf_cellVdecDivx().c_str();
        case 0x003d: return obf_cellJpgEnc().c_str();
        case 0x003e: return obf_cellGame().c_str();
        case 0x003f: return obf_cellBGDLUtility().c_str();
        case 0xf00a: return obf_cellCelpEnc().c_str();
        case 0xf010: return obf_cellGifDec().c_str();
        case 0xf019: return obf_cellAdecCelp().c_str();
        case 0xf01b: return obf_cellAdecM2bc().c_str();
        case 0xf01d: return obf_cellAdecM4aac().c_str();
        case 0xf01e: return obf_cellAdecMp3().c_str();
        case 0xf023: return obf_cellImeJpUtility().c_str();
        case 0xf028: return obf_cellMusicUtility().c_str();
        case 0xf029: return obf_cellPhotoUtility().c_str();
        case 0xf02a: return obf_cellPrintUtility().c_str();
        case 0xf02b: return obf_cellPhotoImportUtil().c_str();
        case 0xf02c: return obf_cellMusicExportUtility().c_str();
        case 0xf02e: return obf_cellPhotoDecodeUtil().c_str();
        case 0xf02f: return obf_cellSearchUtility().c_str();
        case 0xf030: return obf_cellSysutilAvc2().c_str();
        case 0xf034: return obf_cellSailRec().c_str();
        case 0xf035: return obf_sceNpTrophy().c_str();
        case 0xf044: return obf_cellSysutilNpEula().c_str();
        case 0xf053: return obf_cellAdecAt3multi().c_str();
        case 0xf054: return obf_cellAtracMulti().c_str();
        default: return nullptr;
    }
}
#else

const char* staticObfx(int id)
{
	switch (id)
    {
        case 0x0000: return obf_sys_net();
        case 0x0001: return obf_cellHttp();
        case 0x0002: return obf_cellHttpUtil();
        case 0x0003: return obf_cellSsl();
        case 0x0004: return obf_cellHttps();
        case 0x0005: return obf_libvdec();
        case 0x0006: return obf_cellAdec();
        case 0x0007: return obf_cellDmux();
        case 0x0008: return obf_cellVpost();
        case 0x0009: return obf_cellRtc();
        case 0x000a: return obf_cellSpurs();
        case 0x000b: return obf_cellOvis();
        case 0x000c: return obf_cellSheap();
        case 0x000d: return obf_cellSync();
        case 0x000e: return obf_sys_fs();
        case 0x000f: return obf_cellJpgDec();
        case 0x0010: return obf_cellGcmSys();
        case 0x0011: return obf_cellAudio();
        case 0x0012: return obf_cellPamf();
        case 0x0013: return obf_cellAtrac();
        case 0x0014: return obf_cellNetCtl();
        case 0x0015: return obf_cellSysutil();
        case 0x0016: return obf_sceNp();
        case 0x0017: return obf_sys_io();
        case 0x0018: return obf_cellPngDec();
        case 0x0019: return obf_cellFont();
        case 0x001a: return obf_cellFontFT();
        case 0x001b: return obf_cell_FreeType2();
        case 0x001c: return obf_cellUsbd();
        case 0x001d: return obf_cellSail();
        case 0x001e: return obf_cellL10n();
        case 0x001f: return obf_cellResc();
        case 0x0020: return obf_cellDaisy();
        case 0x0021: return obf_cellKey2char();
        case 0x0022: return obf_cellMic();
        case 0x0023: return obf_cellCamera();
        case 0x0024: return obf_cellVdecMpeg2();
        case 0x0025: return obf_cellVdecAvc();
        case 0x0026: return obf_cellAdecLpcm();
        case 0x0027: return obf_cellAdecAc3();
        case 0x0028: return obf_cellAdecAtx();
        case 0x0029: return obf_cellAdecAt3();
        case 0x002a: return obf_cellDmuxPamf();
        case 0x002e: return obf_sys_lv2dbg();
        case 0x002f: return obf_cellSysutilAvcExt();
        case 0x0030: return obf_cellUsbPspcm();
        case 0x0031: return obf_cellSysutilAvconfExt();
        case 0x0032: return obf_cellUserInfo();
        case 0x0033: return obf_cellSaveData();
        case 0x0034: return obf_cellSubDisplay();
        case 0x0035: return obf_cellRec();
        case 0x0036: return obf_cellVideoExportUtility();
        case 0x0037: return obf_cellGameExec();
        case 0x0038: return obf_sceNp2();
        case 0x0039: return obf_cellSysutilAp();
        case 0x003a: return obf_sceNpClans();
        case 0x003b: return obf_cellOskExtUtility();
        case 0x003c: return obf_cellVdecDivx();
        case 0x003d: return obf_cellJpgEnc();
        case 0x003e: return obf_cellGame();
        case 0x003f: return obf_cellBGDLUtility();
        case 0xf00a: return obf_cellCelpEnc();
        case 0xf010: return obf_cellGifDec();
        case 0xf019: return obf_cellAdecCelp();
        case 0xf01b: return obf_cellAdecM2bc();
        case 0xf01d: return obf_cellAdecM4aac();
        case 0xf01e: return obf_cellAdecMp3();
        case 0xf023: return obf_cellImeJpUtility();
        case 0xf028: return obf_cellMusicUtility();
        case 0xf029: return obf_cellPhotoUtility();
        case 0xf02a: return obf_cellPrintUtility();
        case 0xf02b: return obf_cellPhotoImportUtil();
        case 0xf02c: return obf_cellMusicExportUtility();
        case 0xf02e: return obf_cellPhotoDecodeUtil();
        case 0xf02f: return obf_cellSearchUtility();
        case 0xf030: return obf_cellSysutilAvc2();
        case 0xf034: return obf_cellSailRec();
        case 0xf035: return obf_sceNpTrophy();
        case 0xf044: return obf_cellSysutilNpEula();
        case 0xf053: return obf_cellAdecAt3multi();
        case 0xf054: return obf_cellAtracMulti();
        default: return nullptr;
    }
}

#endif


void staticObf()
{
	std::cout << "StaticObf Function:" << std::endl;
	for (int i = 0; i < 0xf055; ++i)
	{
		const char * p = staticObfx(i);
		if (!p)
			continue;
		std::cout << p << std::endl;
	}
}

int main()
{
	std::cout << "Main Function:" << std::endl;
	
	LightObf();
	HeavyObf();
	staticObf();
	return (0);
}