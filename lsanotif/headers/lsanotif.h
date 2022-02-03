#pragma once

#include "pch.h"
#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define rKey L"SOFTWARE\\kindtime"

void DWORD getPtReg();
char* getIPReg(const wchar_t*);