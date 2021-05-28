//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include <stdlib.h>
#include <fstream>
#include <random>

namespace Chip8Constants
{
    constexpr uint8_t BYTE_SIZE = 8;

    constexpr uint8_t BASE_REG_AMOUNT = 16;
    constexpr uint16_t RAM_SIZE_IN_BYTES = 4096;
    constexpr uint8_t STACK_LEVELS = 16;
    constexpr uint8_t VIDEO_WIDTH = 64;
    constexpr uint8_t VIDEO_HEIGHT = 32;

    //  The starting address, which the `Program Counter` would be initialize to
    constexpr uint16_t START_ADDRESS = 0x200;

    //  Characters Font height and the number of each character.
    constexpr uint8_t AMOUNT_OF_CHARS = 16;
    constexpr uint8_t FONT_HEIGHT = 5;
    constexpr uint16_t FONTSET_START_ADDRESS = 0x50;

    constexpr uint8_t fontset[AMOUNT_OF_CHARS * FONT_HEIGHT] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
};

class Chip8
{
private:
    uint8_t registers[Chip8Constants::BASE_REG_AMOUNT]{};
    uint8_t memory[Chip8Constants::RAM_SIZE_IN_BYTES]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[Chip8Constants::STACK_LEVELS]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint16_t opcode;

    //  Randomness seeds
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    //  Typedefing the operation tables.
    typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1]{&Chip8::OP_NULL};
	Chip8Func table0[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func table8[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func tableE[0xE + 1]{&Chip8::OP_NULL};
	Chip8Func tableF[0x65 + 1]{&Chip8::OP_NULL};

public:
    uint8_t keypad[Chip8Constants::AMOUNT_OF_CHARS]{};
    uint32_t video[Chip8Constants::VIDEO_WIDTH * Chip8Constants::VIDEO_HEIGHT]{};

    Chip8(const char *file_path);
    ~Chip8();

    //  Loads the binary values in the file to the Memory attribute.
    void LoadROM(const char *file_path);

    //  Fetches, Decodes and Excutes code on cycles, the simulation cycle.
    void Cycle();

    //  Operation tables.
    void Table0();
    void Table8();
    void TableE();
    void TableF();
    void OP_NULL();

    /****************
     *  Operations  *
     ****************/
    //  CLS:                Clears the video buffer.
    void OP_00E0();

    //  RET:                Return of a value from a sublrutine.
    void OP_00EE();

    //  CALL subroutine:    Calls a subroutine.
    void OP_2nnn();

    /********************
     *  JP Operations   *
     ********************/
    //  JP addr:            Jump to the address in the last 3 digits of the opcode.
    void OP_1nnn();

    //  JP V0, addr:        Jumps to the location specified by adding the value of V0 and the address in the last 3 digits of the opcode.
    void OP_Bnnn();

    /********************
     *  SE Operations   *
     ********************/
    //  SE Vx, byte:        Skips next instruction if Vx == byte.
    void OP_3xkk();

    //  SE Vx, Vy:          Skips next instruction if Vx == Vy.
    void OP_5xy0();

    /********************
     *  SNE Operations  *
     ********************/
    //  SNE Vx, byte:       Skips next instruction if Vx != byte.
    void OP_4xkk();

    //  SNE Vx, Vy:         Skips next instruction if Vx != Vy.
    void OP_9xy0();

    /********************
     *  LD Operations   *
     ********************/
    //  LD Vx, byte:        Sets the value of Vx as byte, i.e. Vx = kk.
    void OP_6xkk();

    //  LD Vx, Vy:          Sets the value of Vx as Vy.
    void OP_8xy0();

    //  LD I, nnn:          Sets the index as the specified address.
    void OP_Annn();

    //  LD Vx, DT:          Sets the Vx rgister as the value of the delay timer
    void OP_Fx07();

    //  LD Vx, key:         Waits for a key to be pressed, sets Vx the value of the key pressed.
    void OP_Fx0A();

    //  LD DT, Vx:          Sets the delay timer the value in Vx register.
    void OP_Fx15();

    //  LD ST, Vx:          Sets the sound timer the value in Vx register.
    void OP_Fx18();

    //  LD F, Vx:           Sets the index register the location of the digit stored in Vx register.
    void OP_Fx29();

    //  LD B, Vx:           Sets the BCD representations of the value in Vx register, in the index, index + 1 and index + 2 locations.
    void OP_Fx33();

    //  LD [I], Vx:         Sets all the memory form index up to the value in Vx, as the values of in the registers Vx.
    void OP_Fx55();

    //  LD Vx, [I]:         Sets all the registers up to the value in Vx, as the values of in the memory from index plus Vx.
    void OP_Fx65();

    /************************
     *  Addition Operations *
     ************************/
    //  ADD Vx, byte:       Adds the value of byte to Vx, i.e. Vx += kk.
    void OP_7xkk();

    //  ADD Vx, Vy:         Adds Vx and Vy and stores it in Vx, VF is the carry, i.e. Vx += Vy, VF = (sum > 255u).
    void OP_8xy4();

    //  ADD index, Vx:      Adds to the index the value of Vx, i.e. index += Vx.
    void OP_Fx1E();

    /****************************
     *  Substruction Operations *
     ****************************/
    //  SUB Vx, Vy:         Subtructs Vx and Vy and stores it in Vx, VF is not Borrow, i.e. Vx -= Vy, VF = (Vx > Vy).
    void OP_8xy5();

    //  SUBN Vx, Vy:        Substructs Vx and Vy and stores it in Vx, VF is borrow, i.e. Vx -= Vy, VF = (Vy > Vx).
    void OP_8xy7();

    /************************
     *  Logical Operations  *
     ************************/
    //  OR Vx, Vy:          Sets VX as the outcome of Vx OR Vy, i.e. Vx |= Vy.
    void OP_8xy1();

    //  AND Vx, Vy:         Ands Vx and Vy and stores it in Vx, i.e. Vx &= Vy.
    void OP_8xy2();

    //  XOR Vx, Vy:         Xors Vx and Vy and stores it in Vx, i.e. Vx ^= Vy.
    void OP_8xy3();

    /************************
     *  Shifitng operations *
     ************************/
    //  SHR Vx:             Shifts Vx by one bit to the right, i.e. Vx >>= 1, VF stores the Least Segnificant Bit(LSB).
    void OP_8xy6();

    //  SHL Vx:             Shifts Vx by one bit to the left, i.e. Vx >>= 1, VF stores the Most Segnificant Bit(MSB).
    void OP_8xyE();

    /************************
     *  Random Operations   *
     ************************/
    //  RND Vx, byte:       Sets Vx as random byte and the value given in the opcode.
    void OP_Cxkk();

    /************************
     *  Display Operations  *
     ************************/
    //  DRW Vx, Vy, nibble: Displays n-byte sprite starting at the memory location (Vx, Vy), VF indicates a collision.
    void OP_Dxyn();

    /********************
     *  Skip Operations *
     ********************/
    //  SKP Vx:             Skips the next instruction if the key with the value of Vx is pressed.
    void OP_Ex9E();

    //  SKNP Vx:             Skips the next instruction if the key with the value of Vx is pressed.
    void OP_ExA1();
};
