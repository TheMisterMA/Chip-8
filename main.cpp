#include "Chip8.h"
#include "PlatformInterface.h"

#include <chrono>
#include <iostream>

void printVideo(uint32_t *video, size_t sizeOfVideo);

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Usage: %s <Scale> <Delay> <ROM>\n", argv[0]);
		abort();
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const *romFileName = argv[3];

	PlatformInterface platform("Chip8 - Emulator",
							   Chip8Constants::VIDEO_WIDTH * videoScale,
							   Chip8Constants::VIDEO_HEIGHT * videoScale,
							   Chip8Constants::VIDEO_WIDTH,
							   Chip8Constants::VIDEO_HEIGHT);

	Chip8 chip8(romFileName);

	int videoPitch = sizeof(chip8.video[0]) * Chip8Constants::VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.Cycle();

			platform.Update(chip8.video, videoPitch);
		}
	}
}
