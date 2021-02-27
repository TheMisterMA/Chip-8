//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include "Chip8.h"

Chip8::Chip8(char const *filePath)
{
    //  Initializing `Program Counter` to the starting adresss.
    this->pc = this->START_ADDRESS;
}

Chip8::~Chip8()
{
}
