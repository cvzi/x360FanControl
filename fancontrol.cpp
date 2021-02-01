#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <xinput.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "XInput.lib")

DWORD userIndex = 0;

XINPUT_STATE xinputState;
XINPUT_VIBRATION xinputVibration = { 0, 0 };

void setSpeed(int left, int right) {
	xinputVibration.wLeftMotorSpeed = left;
	xinputVibration.wRightMotorSpeed = right;
	XInputSetState(userIndex, &xinputVibration);
}

void resetSpeed() {
	setSpeed(0, 0);
}

void printMenu(int speedLeft, int speedRight) {
	system("CLS");
	std::cout << "Buttons:" << std::endl;
	std::cout << "[DPad Left/Right] Speed left" << std::endl;
	std::cout << "[DPad Down/Up] Speed right" << std::endl;
	std::cout << "[BACK] Exit" << std::endl << std::endl;
	std::cout << "Left 0-16: " << speedLeft << std::endl;
	std::cout << "Right 0-16: " << speedRight << std::endl;
}


int main(int argc, char* argv[]) {
	bool run = true;
	bool invalid = true;
	int speedLeft = 0;
	int speedRight = 0;
	while (run) {
		XInputGetState(userIndex, &xinputState);

		if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			invalid = true;
			speedRight -= 1;
		}
		if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			invalid = true;
			speedRight += 1;
		}
		if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			invalid = true;
			speedLeft -= 1;
		}
		if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			invalid = true;
			speedLeft += 1;
		}

		if (xinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
			resetSpeed();
			invalid = true;
			run = false;
		}

		if (invalid) {
			speedLeft = max(0, min(speedLeft, 0x10));
			speedRight = max(0, min(speedRight, 0x10));
			printMenu(speedLeft, speedRight);
			invalid = false;
		}
		setSpeed(speedLeft * 0xFFF, speedRight * 0xFFF);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}
