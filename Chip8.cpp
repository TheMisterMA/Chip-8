//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include "Chip8.h"

Chip8::Chip8(const char* file_path)
{
    //  Initializing `Program Counter` to the starting adresss.
    this->pc = this->START_ADDRESS;

    //  Loading the program into the emulated ROM
    this->LoadROM(file_path);
}

Chip8::~Chip8()
{
}

void Chip8::LoadROM(const char* file_path)
{
    //  Opening the file as a binary stream and set the file pointer at the end.
    std::ifstream code_file(file_path, std::ios::binary | std::ios::ate);

    if(code_file.is_open())
    {
        //  Get the current read position and assining it as the file size. Allocating a buffer to hold the contents of the file.
        std::streampos file_size = code_file.tellg();
        char* buffer = new char[file_size];

        //  Check if the memory size allocated for commands has enogh space for all the commands.
        if(file_size > (this->RAM_SIZE_IN_BYTES - this->START_ADDRESS))
        {
            printf("Aborting, `ROM` file is bigger than allocated memory buffer.\n");
            throw std::overflow_error("ROM is bigger than allocated memory buffer.");
        }

        //  Set the read position at the start of the file, fill the buffer and closing the file.
        code_file.seekg(0, std::ios::beg);
        code_file.read(buffer, file_size);
        code_file.close();

        //  Loading the buffer contents into the `Chip-8` memory, starting at 0x200.
        for(int unsigned i = 0; i < file_size; ++i)
        {
            memory[this->START_ADDRESS + i] = buffer[i];
        }

        //  Free the buffer
        delete[] buffer;
    }
}
