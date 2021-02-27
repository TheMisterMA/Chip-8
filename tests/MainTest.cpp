//  Author:     The Mister M.A.
//  Created at: 27.02.21

#include "MainTest.h"

bool ConstructChip8(const char *file_path)
{
    try
    {
        Chip8 *chip = new Chip8(file_path);
    }
    catch (const std::overflow_error &overflow_err_exception)
    {
        return false;
    }

    return true;
}

int main()
{
}
