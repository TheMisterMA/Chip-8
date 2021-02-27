#include <Chip8.h>
#include <stdexcept>

const char* MEMORY_OVERFLOW_FILE    = "UnitTests/MemoryOverflowCode";
const char* NORMAL_MEMORY_FILE      = "UnitTests/NormalSizeCode";

//  This function tests the Overflow error handling when constructing a `Chip8` object
bool OverflowMemChip8Test(const char* file_path);
