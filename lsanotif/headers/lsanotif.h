#pragma once

#include "pch.h"
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <NTSecAPI.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define rKey L"SOFTWARE\\kindtime"

DWORD getPtReg();
char* getIPReg(const wchar_t*);