//  Author:     The Mister M.A.
//  Created at: 27.02.21

#include "MainTest.h"

/**
 *  OverflowMemChip8Test:
 *      This function tests the work of the constructions, handling the exceptions the `Constructor` should throw.
 *
 * @param file_path The file path to the emulated code to be checked.
 *
 * @return          Boolean value if the Constructor threw an overflow.
 */
bool OverflowMemChip8Test(const char* file_path)
{
    //  Try to build the Chip8 object with the given file.
    try
    {
        Chip8* chip = new Chip8(file_path);
    }
    //  Catches the overflow error if the Chip8 throws one, if it does this function return false.
    catch (const std::overflow_error &overflow_err_exception)
    {
        return false;
    }

    //  If everything went well it returns true.
    return true;
}

int main()
{
    if(
        //  This Should return `false`, as it should catch an overflow_error from the emulators constructor.
        !OverflowMemChip8Test(MEMORY_OVERFLOW_FILE) &&

        //  This Should return `true`, as it is normal sized file.
        OverflowMemChip8Test(NORMAL_MEMORY_FILE)
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
