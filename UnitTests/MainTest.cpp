//  Author:     The Mister M.A.
//  Created at: 27.02.21

#include "MainTest.h"

bool ConstructChip8(const char* file_path)
{
    try
    {
        Chip8* chip = new Chip8(file_path);
    }
    catch (const std::overflow_error &overflow_err_exception)
    {
        return false;
    }

    return true;
}

int main()
{
    if(
        //  This Should return `false`, as it should catch an overflow_error from the emulators constructor.
        !ConstructChip8(MEMORY_OVERFLOW_FILE) &&

        //  This Should return `true`, as it is normal sized file.
        ConstructChip8(NORMAL_MEMORY_FILE)
    )
    {
        printf("Success - :-)\n");
    }
    else
    {
        printf("Fail - :-(\n");
        exit(false);
    }
}
