//  Author:     The Mister M.A.
//  Created at: 17.02.21

#include "Chip8.h"
#include <stdexcept>
#include <chrono>
#include <cstring>

using namespace std;

/***
 *  Constructor:
 *      Setting all of the randomness seeds, setting the `Program Counter`to the
 *      starting address and set all of the fonts in the `Memory Buffer`.
 *
 *  @param file_path The file path of the Operations.
 ***/
Chip8::Chip8(const char *file_path) : //  Initializing randomness seed.
                                      randGen(std::chrono::system_clock::now().time_since_epoch().count()),
                                      randByte(std::uniform_int_distribution<uint8_t>(0, 255U)),

                                      //  Initializing `Program Counter` to the starting adresss.
                                      pc(Chip8Constants::START_ADDRESS)
{

    //  Loading the program into the emulated ROM
    this->LoadROM(file_path);

    //  Load fonts into memory.
    for (size_t i = Chip8Constants::FONTSET_START_ADDRESS; i < (Chip8Constants::FONT_HIGHT * Chip8Constants::AMOUNT_OF_CHARS); i++)
    {
        this->memory[i] = Chip8Constants::fontset[i - Chip8Constants::FONTSET_START_ADDRESS];
    }

    //  Set up function pointer table
    //  All the operations in which the first byte equals to 0, which there are 2 of.
    this->table[0x0] = &Chip8::Table0;
    this->table[0x1] = &Chip8::OP_1nnn;
    this->table[0x2] = &Chip8::OP_2nnn;
    this->table[0x3] = &Chip8::OP_3xkk;
    this->table[0x4] = &Chip8::OP_4xkk;
    this->table[0x5] = &Chip8::OP_5xy0;
    this->table[0x6] = &Chip8::OP_6xkk;
    this->table[0x7] = &Chip8::OP_7xkk;

    //  All the operations in which the first byte equals to 8, which there are 9 of..
    this->table[0x8] = &Chip8::Table8;
    this->table[0x9] = &Chip8::OP_9xy0;
    this->table[0xA] = &Chip8::OP_Annn;
    this->table[0xB] = &Chip8::OP_Bnnn;
    this->table[0xC] = &Chip8::OP_Cxkk;
    this->table[0xD] = &Chip8::OP_Dxyn;

    //  All the operations in which the first byte equals to E, which there are 2 of.
    this->table[0xE] = &Chip8::TableE;

    //  All the operations in which the first byte equals to F, which there are 9 of.
    this->table[0xF] = &Chip8::TableF;

    //  Operations starting with 0.
    this->table0[0x0] = &Chip8::OP_00E0;
    this->table0[0xE] = &Chip8::OP_00EE;

    //  Operations starting with 8.
    this->table8[0x0] = &Chip8::OP_8xy0;
    this->table8[0x1] = &Chip8::OP_8xy1;
    this->table8[0x2] = &Chip8::OP_8xy2;
    this->table8[0x3] = &Chip8::OP_8xy3;
    this->table8[0x4] = &Chip8::OP_8xy4;
    this->table8[0x5] = &Chip8::OP_8xy5;
    this->table8[0x6] = &Chip8::OP_8xy6;
    this->table8[0x7] = &Chip8::OP_8xy7;
    this->table8[0xE] = &Chip8::OP_8xyE;

    //  Operations starting with E.
    this->tableE[0x1] = &Chip8::OP_ExA1;
    this->tableE[0xE] = &Chip8::OP_Ex9E;

    //  OPerations starting with F.
    this->tableF[0x07] = &Chip8::OP_Fx07;
    this->tableF[0x0A] = &Chip8::OP_Fx0A;
    this->tableF[0x15] = &Chip8::OP_Fx15;
    this->tableF[0x18] = &Chip8::OP_Fx18;
    this->tableF[0x1E] = &Chip8::OP_Fx1E;
    this->tableF[0x29] = &Chip8::OP_Fx29;
    this->tableF[0x33] = &Chip8::OP_Fx33;
    this->tableF[0x55] = &Chip8::OP_Fx55;
    this->tableF[0x65] = &Chip8::OP_Fx65;
}

Chip8::~Chip8()
{
}

/***
 *  LoadROM:
 *      This function loades the binary value of the content in the file,
 *      to the emulated memory buffer.
 *
 * @param file_path The file containing the program this program will emulate.
 ***/
void Chip8::LoadROM(const char *file_path)
{
    //  Opening the file as a binary stream and set the file pointer at the end.
    std::ifstream code_file(file_path, std::ios::binary | std::ios::ate);

    if (code_file.is_open())
    {
        //  Get the current read position and assining it as the file size. Allocating a buffer to hold the contents of the file.
        std::streampos file_size = code_file.tellg();
        char *buffer = new char[file_size];

        //  Check if the memory size allocated for commands has enogh space for all the commands.
        if (file_size > (Chip8Constants::RAM_SIZE_IN_BYTES - Chip8Constants::START_ADDRESS))
        {
            printf("Aborting, `ROM` file is bigger than allocated memory buffer.\n");
            throw std::overflow_error("ROM is bigger than allocated memory buffer.");
        }

        //  Set the read position at the start of the file, fill the buffer and closing the file.
        code_file.seekg(0, std::ios::beg);
        code_file.read(buffer, file_size);
        code_file.close();

        //  Loading the buffer contents into the `Chip-8` memory, starting at 0x200.
        for (int unsigned i = 0; i < file_size; ++i)
        {
            memory[Chip8Constants::START_ADDRESS + i] = buffer[i];
        }

        //  Free the buffer
        delete[] buffer;
    }
}

//  Operation tables.
void Chip8::Table0() { (this->*table0[this->opcode & 0x000Fu])(); }

void Chip8::Table8() { (this->*table0[this->opcode & 0x000Fu])(); }

void Chip8::TableE() { (this->*table0[this->opcode & 0x000Fu])(); }

void Chip8::TableF() { (this->*table0[this->opcode & 0x00FFu])(); }

void Chip8::OP_NULL() {}

/***
 *  OP_00E0:
 *      CLS
 *      Clearing the `Video Buffer`.
 *      This means setting all of the values in the buffer to 0.
 ***/
void Chip8::OP_00E0()
{
    memset(this->video, 0, sizeof(this->video));
}

/***
 *  OP_00EE:
 *      RET
 *      Returns the `Program Pointer` value on the top of the stack into `Program Pointer`,
 *      hence returning back to the flow of the program.
 ***/
void Chip8::OP_00EE()
{
    --(this->sp);
    this->pc = this->stack[this->sp];
}

/***
 *  OP_2nnn:
 *      CALL subroutine_adress.
 *      Calls a subroutine, meaning it stores the current PC at the
 *      stack and jumps to the address of the subroutine.
 ***/
void Chip8::OP_2nnn()
{
    //  Storing the current Program Counter in the stack, and moving the Stack Pointer up.
    this->stack[this->sp] = this->pc;
    ++(this->sp);

    //  Calling the Jump command to jump to the needed address, ass it is done by the same calculation.
    this->OP_1nnn();
}

/********************
 *  JP Operations *
 ********************/
/***
 *  OP_1nnn:
 *      JP address.
 *      Jump to the address, i.e. moving the Program Counter/Pointer
 *      specified in the last 3 digits of the opcode, so no matter what is exact command,
 *      as long as the last 3 digits of the opcode are the address.
 ***/
void Chip8::OP_1nnn()
{
    //  Calculating the address to jump to.
    uint16_t new_address = ((this->opcode) & 0x0FFFu);

    //  Jumping to the new address.
    this->pc = new_address;
}

/***
 *  OP_Bnnn:
 *      JP V0, address.
 *      Jump to the address specified by the value of V0 and the last 3 digits of the opcode,
 *      i.e. moving the Program Counter/Pointer.
 ***/
void Chip8::OP_Bnnn()
{
    //  Calculating the address to jump to.
    uint16_t new_address = (this->registers[0x0u] + ((this->opcode) & 0x0FFFu));

    //  Jumping to the new address.
    this->pc = new_address;
}

/********************
 *  SE Operations *
 ********************/
/***
 *  OP_3xkk:
 *      SE Vx, byte.
 *      Skips the next command if Vx == byte,
 *      where X is the register value of the second word and byte equals the last byte.
 ***/
void Chip8::OP_3xkk()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t byte = (this->opcode & 0x00FFu);

    //  Moving the Program Pointer up with 2 to skip the next command.
    this->pc += ((this->registers[Vx] == byte) ? 2 : 0);
}

/***
 *  OP_5xy0:
 *      SE Vx,Vy.
 *      Skips the next command if Vx != Vy,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_5xy0()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  Moving the Program Pointer up with 2 to skip the next command.
    this->pc += ((this->registers[Vx] == this->registers[Vy]) ? 2 : 0);
}

/*********************
 *  SNE Operations *
 *********************/
/***
 *  OP_4xkk:
 *      SNE Vx, byte.
 *      Skips the next command if Vx != byte,
 *      where X is the register value of the second word and byte equals the last byte.
 ***/
void Chip8::OP_4xkk()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t byte = (this->opcode & 0x00FFu);

    //  Moving the Program Pointer up with 2 to skip the next command.
    this->pc += ((this->registers[Vx] != byte) ? 2 : 0);
}

/***
 *  OP_9xy0:
 *      SNE Vx, Vy.
 *      Skips the next command if Vx != Vy,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_9xy0()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  Moving the Program Pointer up with 2 to skip the next command.
    this->pc += ((this->registers[Vx] != this->registers[Vy]) ? 2 : 0);
}

/********************
 *  LD Operations *
 ********************/
/***
 *  OP_6xkk:
 *      LD Vx, byte.
 *      Sets the Vx register the value of byte,
 *      where X is the register value of the second word and byte equals the last byte.
 ***/
void Chip8::OP_6xkk()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t byte = (this->opcode & 0x00FFu);

    //  Appling the value of byte to Vx.
    this->registers[Vx] = byte;
}

/***
 *  OP_8xy0:
 *      LD Vx, Vy.
 *      Sets the value of Vy register into the Vx register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy0()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  Setting the register Vx as the register Vy.
    this->registers[Vx] = this->registers[Vy];
}

/***
 *  OP_Annn:
 *      LD I, nnn.
 *      Sets the value of index register as the specified address,
 *      where address is the last 3 words of the opcode.
 ***/
void Chip8::OP_Annn()
{
    //  Calculating values out of the opcode.
    uint16_t address = (this->opcode & 0x0FFFu);

    //  Setting the index as the specified address.
    this->index = address;
}

/***
 *  OP_Fx07:
 *      LD Vx, DT.
 *      Sets the Vx rgister as the value of the delay timer,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx07()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    this->registers[Vx] = this->delayTimer;
}

/***
 *  OP_Fx0A:
 *      LD Vx, Key.
 *      Waits for a key to be pressed, sets Vx the value of the key pressed.
 *      Inorder to continue to the same command each `Cycle`, it will reduce the PC if no key is pressed.
 *      X is the register value of the second word.
 ***/
void Chip8::OP_Fx0A()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    bool keySaved = false;

    //  Itterate over all the keys, and one more,
    //  if no key is pressed the PC is reduced by size of a opcode.
    for (size_t i = 0; i <= Chip8Constants::AMOUNT_OF_CHARS && !keySaved; i++)
    {
        if (i == Chip8Constants::AMOUNT_OF_CHARS)
        {
            this->pc -= 2;
        }
        else if (this->keypad[i])
        {
            this->registers[Vx] = i;
            keySaved = true;
        }
    }
}

/***
 *  OP_Fx15:
 *      LD DT, Vx.
 *      Sets the delay timer as the value of the Vx rgister,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx15()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    this->delayTimer = this->registers[Vx];
}

/***
 *  OP_Fx18:
 *      LD DT, Vx.
 *      Sets the sound timer as the value of the Vx rgister,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx18()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    this->soundTimer = this->registers[Vx];
}

/***
 *  OP_Fx29:
 *      LD F, Vx.
 *      Sets the index register the location of the digit stored in Vx register.
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx29()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t digit = this->registers[Vx];

    //  Each digit is 5 bytes long.
    this->index = Chip8Constants::FONTSET_START_ADDRESS + (5 * digit);
}

/***
 *  OP_Fx33:
 *      LD B, Vx.
 *      Sets the BCD representations of the value in Vx register,
 *      in the index, index + 1 and index + 2 locations.
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx33()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t value = this->registers[Vx];

    //  Ones-Place
    this->memory[this->index + 2] = value % 10;

    //  Tens-Place
    this->memory[this->index + 1] = value % 100;

    //  Hundreds-Place
    this->memory[this->index] = value % 1000;
}

/***
 *  LD  [I], Vx.
 *      Sets all the memory form index up to the value in Vx,
 *      as the values of in the registers Vx.
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx55()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    for (size_t i = 0; i < Vx; i++)
    {
        this->memory[this->index + i] = this->registers[i];
    }
}

/***
 *  LD  [I], Vx.
 *      Sets all the registers up to Vx,
 *      as the values of in the memory from index plus Vx.
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx55()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    for (size_t i = 0; i < Vx; i++)
    {
        this->registers[i] = this->memory[this->index + i];
    }
}

/****************************
 *  Addition Operations   *
 ****************************/
/***
 *  OP_7xkk:
 *      ADD Vx, byte.
 *      Adds the value of byte to the Vx register,
 *      where X is the register value of the second word and byte equals the last byte.
 ***/
void Chip8::OP_7xkk()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t byte = (this->opcode & 0x00FFu);

    //  Adding the value of byte to Vx.
    this->registers[Vx] += byte;
}

/***
 *  OP_8xy4:
 *      ADD Vx, Vy.
 *      Adds the value of Vy register to the Vx register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy4()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  Storing the sum in a bigger uint type to examine for a carry.
    uint16_t sum = this->registers[Vx] + this->registers[Vy];

    //  Raising the carry bit.
    this->registers[0xF] = (sum > UINT8_MAX);

    //  storing the Sum, only for the bits that can get into the type of the register, i.e. no carry.
    this->registers[Vx] = sum & 0xFFu;
}

/***
 *  OP_Fx1E:
 *      ADD index, Vx.
 *      Adds the value of Vx register to the index register,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Fx1E()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    //  Adding the value of byte to Vx.
    this->index += this->registers[Vx];
}

/********************************
 *  Substruction Operations   *
 ********************************/
/***
 *  OP_8xy5:
 *      SUB Vx, Vy.
 *      Subtracts the value of Vy from the Vx register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy5()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  If there is no borrow, i.e. Vx is greater than Vy.
    this->registers[0xF] = (this->registers[Vx] > this->registers[Vy]);

    //  Subtructing the Vy register out of Vx register.
    this->registers[Vx] -= this->registers[Vy];
}

/***
 *  OP_8xy7:
 *      SUBN Vx, Vy.
 *      Subtracts the value of Vy from the Vx register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy7()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  If there is no borrow, i.e. Vx is greater than Vy.
    this->registers[0xF] = (this->registers[Vy] > this->registers[Vx]);

    //  Subtructing the Vy register out of Vx register.
    this->registers[Vx] -= this->registers[Vy];
}

/****************************
 *  Logical Operations    *
 ****************************/
/***
 *  OP_8xy1:
 *      OR Vx, Vy.
 *      Sets the value of Vx register ORed with the Vy register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy1()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  ORing the register Vx as the register Vy.
    this->registers[Vx] |= this->registers[Vy];
}

/***
 *  OP_8xy2:
 *      AND Vx, Vy.
 *      Sets the value of Vx register ANDed with the Vy register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy2()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  ANDing the register Vx as the register Vy.
    this->registers[Vx] &= this->registers[Vy];
}

/***
 *  OP_8xy3:
 *      XOR Vx, Vy.
 *      Sets the value of Vx register XORed with the Vy register,
 *      where X is the register value of the second word and Y equals the third word.
 ***/
void Chip8::OP_8xy3()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);

    //  XORing the register Vx as the register Vy.
    this->registers[Vx] ^= this->registers[Vy];
}

/**************************
 *  Shifitng operations   *
 **************************/
/***
 *  OP_8xy6:
 *      SHR Vx, Vy.
 *      Shifts Vx register to the right, and stores at VF the Least Segnificant Bit(LSB),
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_8xy6()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    //  Saves the LSB.
    this->registers[0xF] = (this->registers[Vx] & 0x1u);

    //  Shifting to the right the Vx register.
    this->registers[Vx] >>= 1;
}

/***
 *  OP_8xyE:
 *      SHL Vx, Vy.
 *      Shifts Vx register to the left, and stores at VF the Most Segnificant Bit(MSB),
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_8xyE()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);

    //  Saves the MSB, 0x80 is 1000 0000 in binary, which means it needs a shift of 7 bits to get the MSB.
    this->registers[0xF] = (this->registers[Vx] & 0x80u) >> 0x7u;

    //  Shifting to the right the Vx register.
    this->registers[Vx] <<= 1;
}

/************************
 *  Random Operations   *
 ************************/
/***
 *  OP_Cxkk:
 *      RND Vx, byte.
 *      Randomizes a number and ANDs it with the given byte, which is stored at Vx register,
 *      where X is the register value of the second word and byte equals the last byte.
 ***/
void Chip8::OP_Cxkk()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t byte = (this->opcode & 0x00FFu);

    //  Generating the random value and ANDing it with the byte value.
    this->registers[Vx] = this->randByte(this->randGen) & byte;
}

/************************
 *  Display Operations  *
 ************************/
/***
 *  OP_Dxyn:
 *      DRW Vx, Vy, nibble.
 *      Vx is the x position on the screen of the pixel,
 *      Vy is the is the y position on the screen of the pixel.
 *      The pixel is where the sprite begins, which is each byte
 *      from the memory starting from the index.
 ***/
void Chip8::OP_Dxyn()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t Vy = ((this->opcode & 0x00F0u) >> 4u);
    uint8_t height = (this->opcode & 0x000Fu);

    //  Wrap the position inside the video buffer if it going beyond screen boundaries.
    uint8_t xPos = this->registers[Vx] % Chip8Constants::VIDEO_WIDTH;
    uint8_t yPos = this->registers[Vy] % Chip8Constants::VIDEO_HIGHT;

    //  The VF indicates a collision of the sprite with it self.
    this->registers[0xF] = 0;

    for (size_t row = 0; row < height; row++)
    {
        //  Loads the byte of the row in the memory starting from the index.
        uint8_t spriteByte = this->memory[this->index + row];

        //  Itterating over the byte, bit by bit.
        for (size_t column = 0; column < 8; column++)
        {
            //  Getting the needed bit, and getting the pointer to the screen pixel.
            uint8_t spritePixel = spriteByte & (0x80u >> column);
            uint32_t *screenPixel = &(this->video[(yPos + row) * Chip8Constants::VIDEO_WIDTH + (xPos + column)]);

            if (spritePixel)
            {
                //  Raising the VF register in the case of collision.
                this->registers[0xF] = (*screenPixel == 0xFFFFFFFF) ? 1 : 0;

                //  XORing the sprite pixel.
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

/********************
 *  Skip Operations *
 ********************/
/***
 *  OP_Ex9E:
 *      SKP Vx.
 *      Skips the next instruction if the key with the value of Vx is pressed,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_Ex9E()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t key = this->registers[Vx];

    //  Increasing Program Counter by 2, if the is pressed.
    this->pc += (this->keypad[key]) ? 2 : 0;
}

/***
 *  OP_ExA1:
 *      SKNP Vx.
 *      Skips the next instruction if the key with the value of Vx is not pressed,
 *      where X is the register value of the second word.
 ***/
void Chip8::OP_ExA1()
{
    //  Calculating values out of the opcode.
    uint8_t Vx = ((this->opcode & 0x0F00u) >> 8u);
    uint8_t key = this->registers[Vx];

    //  Increasing Program Counter by 2, if the is not pressed.
    this->pc += (!this->keypad[key]) ? 2 : 0;
}
