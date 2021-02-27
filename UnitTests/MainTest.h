#include <Chip8.h>
#include <stdexcept>

const char* MEMORY_OVERFLOW_FILE    = "UnitTests/MemoryOverflowCode";
const char* NORMAL_MEMORY_FILE      = "UnitTests/NormalSizeCode";

bool ConstructChip8(const char* file_path);
