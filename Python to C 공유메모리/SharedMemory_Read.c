#ifndef SM_SERVER_
#define SM_SERVER_

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <tchar.h>

#define RUNTIME                       (double)             (100.0)      //[sec]
#define FREQ                          (double)             (20.0)      //[Hz]
#define N_data                        (unsigned int)       (RUNTIME*FREQ)

double iniTime = 0.0;
double simTime = 0.0;
double curTime = 0.0;
double delTime = 0.0;
double Ts = 1 / FREQ;
int    simcnt = 0;

typedef struct
{
	double data_1;
	double data_2;
} TEST_DATA;

TEST_DATA* test_smdat;

double data_1 = 0.0;
double data_2 = 0.0;

HANDLE hMemoryMap = NULL;
LPBYTE pMemoryMap = NULL; // LPBYTE�� unsigned char�� ��������
TCHAR TEST_SM[] = TEXT("MyFileMappingObject");

double GetWindowTime();
void CheckTime();

void main() {
	GetWindowTime();

	hMemoryMap = OpenFileMapping(
		FILE_MAP_READ,	  // read/write access
		FALSE,	          // do not inherit the name
		TEST_SM);          // ���� ���ϸ��� �̸� - Uique �ؾ��Ѵ�.

	if (!hMemoryMap) {
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return FALSE;
	}

	pMemoryMap = (LPVOID*)MapViewOfFile(
		hMemoryMap,				// ���ϸ��� �ڵ�
		FILE_MAP_READ,    // �׼��� ��� - ����� ����
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ 
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ
		sizeof(TEST_DATA));						// ����� �޸� ����� ũ�� - 0�̸� ������ ��ü �޸�

	if (!pMemoryMap)
	{
		CloseHandle(hMemoryMap);
		printf("COULD NOT OPEN THE SHARED MEMORY\n");
		return FALSE;
	}
	printf("TEST1 SHARED MEMORY IS CREATED\n");

	test_smdat = (TEST_DATA*)pMemoryMap;

	iniTime = GetWindowTime() * 0.001;
	do
	{
		data_1 = test_smdat->data_1;
		data_2 = test_smdat->data_2;
		printf("%f %f\n", data_1, data_2);

		CheckTime();
	} while (simTime < RUNTIME);

	UnmapViewOfFile(pMemoryMap);
	CloseHandle(hMemoryMap);
	return 0;
}

double GetWindowTime(void) {
	LARGE_INTEGER   liCount, liFreq;
	QueryPerformanceCounter(&liCount); // �ð� �Լ� �и� ������ ������ ������ �����ϴ�
	QueryPerformanceFrequency(&liFreq); // ������/[sec]
	return((liCount.QuadPart / ((double)(liFreq.QuadPart))) * 1000.0);
};

void CheckTime()
{

	while (1) {
		curTime = GetWindowTime() * 0.001;   // [ms]
		delTime = curTime - iniTime - simTime;

		if (delTime >= Ts) {
			break;
		}
	}
	simTime = ((double)simcnt + 1.0) * Ts;
	simcnt = simcnt + 1;
}

#endif