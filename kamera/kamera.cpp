#include <windows.h>
#include <iostream>
#include <cstdio>
#include "vfw.h"
#include <string>
#include <time.h>
#pragma comment(lib, "Vfw32.lib")


void displayCameraDrivers();
void takePicture(HWND);
void recordVideo(HWND);
void menu(HWND);

typedef struct CameraDrivers
{
	char driverName[100];
	char driverVersion[100];
	int driverIndex;
};

int main()
{
	HINSTANCE hAviCap = LoadLibrary((LPCTSTR)"avicap32.dll");

	HWND hWndC = capCreateCaptureWindowA(
		("My Capture Window"),       // window name if pop-up 
		WS_CHILD | WS_VISIBLE,       // window style 
		0, 0, 200, 150,              // window position and dimensions
		GetDesktopWindow(),
		(int)0 /* child ID */);

	if (capDriverConnect(hWndC, 0)) {
		menu(hWndC);
	}
	else {
		std::cout << "Check camera?" << std::endl;
		std::cout << "=======================" << std::endl;
	}
	displayCameraDrivers();
	DestroyWindow(hWndC);
	return 0;
}

void displayCameraDrivers()
{
	//wypisywanie listy sterowników do obs³ugi kamer w komputerze
	CameraDrivers listOfCameraDrivers[10];
	int number = 0;
	for (auto i = 0; i < 10; i++) {
		if (capGetDriverDescriptionA(
			i,
			(LPSTR)listOfCameraDrivers[i].driverName,
			sizeof(listOfCameraDrivers[i].driverName),
			(LPSTR)listOfCameraDrivers[i].driverVersion,
			sizeof(listOfCameraDrivers[i].driverVersion)
		)) {
			listOfCameraDrivers[i].driverIndex = number++;
			// Append name to list of installed capture drivers
			// and then let the user select a driver to use.
		}
	}

	for (int i = 0; i < 10; i++) {
		if (listOfCameraDrivers[i].driverIndex > 10 || listOfCameraDrivers[i].driverIndex < 0)
			continue;
		std::cout << "=======================" << std::endl;
		std::cout << "Camera driver name: " << listOfCameraDrivers[i].driverName << std::endl;
		std::cout << "Driver version : " << listOfCameraDrivers[i].driverVersion << std::endl;
		std::cout << "Index of camera driver: " << listOfCameraDrivers[i].driverIndex << std::endl;
		std::cout << "=======================" << std::endl;
	}
}

void takePicture(HWND hWndC)
{
	capFileSaveDIB(hWndC, L"shot.jpg");
	// the screenshot is in the current 
	// directory, usually the same 
	// where the exe is created by 
	// your compiler 
	std::cout << "=======================" << std::endl;
	std::cout << "Photo saved as shot.jpg!" << std::endl;
	std::cout << "=======================" << std::endl;
}

void recordVideo(HWND hWndC)
{
	CAPTUREPARMS captureParams;
	//connect to Webcam
	SendMessage(hWndC, WM_CAP_DRIVER_CONNECT, 0, 0);
	//Set the frame rate for Webcam
	SendMessage(hWndC, WM_CAP_SET_PREVIEWRATE, 66, 0);
	//Set the preview flag to true 
	SendMessage(hWndC, WM_CAP_SET_PREVIEW, 1, 0);
	//Set this if you want to control the video compression for the output video
	SendMessage(hWndC, WM_CAP_DLG_VIDEOCOMPRESSION, 0, 0);

	SendMessage(hWndC, WM_CAP_GRAB_FRAME_NOSTOP, 0, 0);
	//set name of the recorded video
	SendMessageA(hWndC, WM_CAP_FILE_SET_CAPTURE_FILEA, 0, (LPARAM)"film.avi");
	//set size of video (at least it should do it, but it doesnt, so you never know how long your video will be)
	SendMessage(hWndC, WM_CAP_SET_SEQUENCE_SETUP, (1024L * 1024L * 10), (LPARAM)&captureParams);
	std::cout << "Recording..." << std::endl;
	//start recording the video
	SendMessage(hWndC, WM_CAP_SEQUENCE, 0, 0);
	//stop recording the video
	SendMessage(hWndC, WM_CAP_STOP, 0, 0);
	//disconnect from WebCam
	SendMessage(hWndC, WM_CAP_DRIVER_DISCONNECT, 0, 0);

	std::cout << "=======================" << std::endl;
	std::cout << "Video saved as film.avi!" << std::endl;
	std::cout << "=======================" << std::endl;
}

void menu(HWND hWndC)
{
	int choice = 0;
	std::cout << "Menu " << std::endl;
	std::cout << "1. Take a photo " << std::endl;
	std::cout << "2. Record video " << std::endl;
	std::cout << "3. Display installed camera drivers " << std::endl;
	std::cout << "Your choice: ";
	do {
		std::cin >> choice;
	} while (choice < 1 || choice > 3 );
	std::cout << std::endl; 

	switch (choice)
	{
	case 1: takePicture(hWndC);
		break;
	case 2: recordVideo(hWndC);
		break;
	case 3: displayCameraDrivers();
		break;
	default:
		break;
	}
	Sleep(2000);
	system("cls");
	menu(hWndC);
}
