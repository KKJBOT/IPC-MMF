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
#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

typedef struct{
	double	size;
	BYTE	data[218770];		//�̹���
} IMAGE_DATA;

IMAGE_DATA* img_smdat;

HANDLE hMemoryMap = NULL;
LPBYTE pMemoryMap = NULL; // LPBYTE�� unsigned char�� ��������,8bit
TCHAR TEST_SM[] = TEXT("MyFileMappingObject");

void main() {

	hMemoryMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,	// read/write access
		FALSE,					// do not inherit the name
		TEST_SM);				// ���� ���ϸ��� �̸� - Uique �ؾ��Ѵ�.

	if (!hMemoryMap) {
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		//return FALSE;
	}

	pMemoryMap = (LPBYTE)MapViewOfFile(
		hMemoryMap,				// ���ϸ��� �ڵ�
		FILE_MAP_ALL_ACCESS,    // �׼��� ��� - ����� ����
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ 
		0,						// �޸� ���۹��������� �̰ݵ� ���� 32��Ʈ
		0);						// ����� �޸� ����� ũ�� - 0�̸� ������ ��ü �޸�

	if (!pMemoryMap)
	{
		CloseHandle(hMemoryMap);
		printf("COULD NOT OPEN THE SHARED MEMORY\n");
		//return FALSE;
	}
	printf("TEST1 SHARED MEMORY IS CREATED\n");
	
	img_smdat = (IMAGE_DATA*)pMemoryMap;

	// ���� �޸� �Ҵ� : ��Ȯ�� ������ ũ�⸸ŭ�� Matrix �����͸� ����� ����
	BYTE* data_buf;
	data_buf = (BYTE*)malloc(sizeof(BYTE) * img_smdat->size);

	for (int i = 0; i < img_smdat->size; i++)
		data_buf[i] = img_smdat->data[i];

	// Byte array to Matrix to Image ��ȯ, imshow�� �̹��� Ȯ��
	Mat image = Mat(img_smdat->size,1,CV_8U, data_buf);
	Mat pic = imdecode(image, 1);
	imshow("Pic", pic);
	waitKey(0);

	UnmapViewOfFile(pMemoryMap);
	CloseHandle(hMemoryMap);
	//return 0;
}

#endif

