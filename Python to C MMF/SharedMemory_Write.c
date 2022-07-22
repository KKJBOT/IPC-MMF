

#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <tchar.h>

#define UNIT_PI                 (3.14)
#define RUNTIME (double)        (300.0)
#define FREQ    (double)        (20.0)
#define N_data  (unsigned_int)  (RUNTIME*FREQ)

double    iniTime = 0.0;
double    curTime = 0.0;
double    delTime = 0.0;
double    simTime = 0.0;
double    simTime_gps = 0.0;
double    Ts = 1 / FREQ;

double    data1 = 0.0;
double    data2 = 0.0;

int	      num = 0;

int  simCnt = 0;

typedef struct {
	double       data_1;
	double       data_2;
} TEST_DATA;

TCHAR TEST_SM[] = TEXT("test data");

double GetWindowTime(void) {
	LARGE_INTEGER   liCount, liFreq;
	QueryPerformanceCounter(&liCount); // �ð� �Լ� �и� ������ ������ ������ �����ϴ�
	QueryPerformanceFrequency(&liFreq); // ������/[sec]
	return((liCount.QuadPart / ((double)(liFreq.QuadPart))) * 1000.0);
};
int main()
{
	HANDLE dMemoryMap = NULL;
	LPBYTE qMemoryMap = NULL; // LPBYTE�� unsigned char�� ��������

	dMemoryMap = CreateFileMapping(
		INVALID_HANDLE_VALUE, // ���� ���� �ڵ�, �ʱ⿡ 0xffffffff�� �����Ѵ�.
		NULL,				// ���� �Ӽ�
		PAGE_READWRITE,     // �а�/���� �Ӽ�
		0,					// 64��Ʈ ��巹���� ����Ѵ�. ���� 32��Ʈ - �޸��� ũ��
		sizeof(TEST_DATA),   // ���� 32��Ʈ - ���⼱LPBYTE Ÿ��.
		TEST_SM);            // ���� ���ϸ��� �̸� - Uique �ؾ��Ѵ�.

	if (!dMemoryMap) {
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
		return FALSE;
	}

	qMemoryMap = (BYTE*)MapViewOfFile(
		dMemoryMap,				// ���ϸ��� �ڵ�
		FILE_MAP_ALL_ACCESS,    // �׼��� ��� - ����� ����
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ 
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ
		0);						// ����� �޸� ����� ũ�� - 0�̸� ������ ��ü �޸�

	if (!qMemoryMap)
	{
		CloseHandle(dMemoryMap);
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
		return FALSE;
	}

	TEST_DATA* test_smdat = (TEST_DATA*)qMemoryMap;

	num = num + 1;
	test_smdat->data_1 = 50;
	test_smdat->data_2 = 100.0;

	printf("%d: %f %f\n", num, test_smdat->data_1,test_smdat->data_2);

	getchar();

	if (qMemoryMap)
		UnmapViewOfFile(qMemoryMap);
	if (dMemoryMap)
		CloseHandle(dMemoryMap);

	return 0;
}
