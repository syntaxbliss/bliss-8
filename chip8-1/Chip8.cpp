#include "Chip8.hpp"
#include "Commons.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

Chip8::Chip8() {
    mMemory = ( uint8_t* )malloc( sizeof(uint8_t) * MEMORY_SIZE );
    memset( mMemory, 0, sizeof(uint8_t) * MEMORY_SIZE );

    mReg = ( uint8_t* )malloc( sizeof(uint8_t) * 16 );
    memset( mReg, 0, sizeof(uint8_t) * 16 );

    mStack = ( uint16_t* )malloc( sizeof(uint16_t) * 16 );
    memset( mStack, 0, sizeof(uint16_t) * 16 );
    
    mDisplay = ( bool* )malloc( sizeof(bool) * SCREEN_WIDTH * SCREEN_HEIGHT );
    memset( mDisplay, false, sizeof(bool) * SCREEN_WIDTH * SCREEN_HEIGHT );
    
    mKeys = ( bool* )malloc( sizeof(bool) * 16 );
    memset( mKeys, false, sizeof(bool) * 16 );
    
    mI = 0;
    mDelayTimer = 0;
    mSoundTimer = 0;
    mPC.w = 0x200;
    mSP = mStack;
    
    // fontset
    const uint8_t fontset[ 16 * 5 ] = {
        0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
        0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
        0x90, 0x90, 0xf0, 0x10, 0x10, // 4
        0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
        0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
        0xf0, 0x10, 0x20, 0x40, 0x40, // 7
        0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
        0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
        0xf0, 0x90, 0xf0, 0x90, 0x90, // a
        0xe0, 0x90, 0xe0, 0x90, 0xe0, // b
        0xf0, 0x80, 0x80, 0x80, 0xf0, // c
        0xe0, 0x90, 0x90, 0x90, 0xe0, // d
        0xf0, 0x80, 0xf0, 0x80, 0xf0, // e
        0xf0, 0x80, 0xf0, 0x80, 0x80, // f
    };
    memcpy( mMemory, fontset, sizeof(uint8_t) * 16 * 5 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Chip8::~Chip8() {
    free( mMemory );
    free( mReg );
    free( mStack );
    free( mDisplay );
    free( mKeys );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::loadFile( string path ) {
    FILE* fp = fopen( path.c_str(), "rb" );
    if( ! fp ) {
        printf("\nerror: no se pudo abrir el archivo < %s >", path.c_str() );
        exit( 1 );
    }

    fseek( fp, 0, 2 );
    int length = ftell( fp );
    rewind( fp );
    fread( &mMemory[0x200], sizeof(uint8_t), length, fp );

    fclose( fp );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Chip8::readMemory( uint16_t address ) const {
    return mMemory[ address ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::writeMemory( uint16_t address, uint8_t value ) {
    mMemory[ address ] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t Chip8::fetch() {
    uint8_t h = readMemory( mPC.w++ );
    uint8_t l = readMemory( mPC.w++ );
    
    return (uint16_t)( (h << 8) | l );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::stackPush( uint16_t value ) {
    ( *mSP ) = value;
    mSP++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t Chip8::stackPop() {
    mSP--;
    return ( *mSP );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::execute( uint16_t opcode ) {
    //printf("\n%04X: %04X", (mPC.w - 2) & 0xffff, opcode );

    switch( (opcode & 0xf000) >> 12 ) {
        case 0x0: { // 0???
            switch( opcode & 0x0fff ) {
                case 0xe0: { // 00E0
                    memset( mDisplay, false, sizeof(bool) * SCREEN_WIDTH * SCREEN_HEIGHT );
                } break;
                
                case 0x00ee: { // 00EE
                    mPC.w = stackPop();
                } break;
            }
        } break;

        case 0x1: { // 1NNN
            mPC.w = ( opcode & 0x0fff );
        } break;

        case 0x2: { // 2NNNN
            stackPush( mPC.w );
            mPC.w = ( opcode & 0x0fff );
        } break;
        
        case 0x3: { // 3XNN
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t nn = ( opcode & 0x00ff );
            if( mReg[x] == nn ) mPC.w += 2;
        } break;

        case 0x4: { // 4XNN
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t nn = ( opcode & 0x00ff );
            if( mReg[x] != nn ) mPC.w += 2;
        } break;
        
        case 0x5: { // 5XY0
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t y = ( opcode & 0x00f0 ) >> 4;
            if( mReg[x] == mReg[y] ) mPC.w += 2;
        } break;

        case 0x6: { // 6XNN
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t nn = ( opcode & 0x00ff );
            mReg[ x ] = nn;
        } break;
        
        case 0x7: { // 7XNN
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t nn = ( opcode & 0x00ff );
            mReg[ x ] += nn;
        } break;
        
        case 0x8: { // 8XY?
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t y = ( opcode & 0x00f0 ) >> 4;

            switch( opcode & 0xf ) {
                case 0x0: { // 8XY0
                    mReg[ x ] = mReg[ y ];
                } break;
                
                case 0x1: { // 8XY1
                    mReg[ x ] |= mReg[ y ];
                } break;

                case 0x2: { // 8XY2
                    mReg[ x ] &= mReg[ y ];
                } break;
                
                case 0x3: { // 8XY3
                    mReg[ x ] ^= mReg[ y ];
                } break;
                
                case 0x4: { // 8XY4
                    int16_t r = mReg[ x ] + mReg[ y ];
                    if( r > 0xff ) mReg[ 0xf ] = 1; else mReg[ 0xf ] = 0;
                    mReg[ x ] = r;
                } break;
                
                case 0x5: { // 8XY5
                    int16_t r = mReg[ x ] - mReg[ y ];
                    if( r < 0 ) mReg[ 0xf ] = 0; else mReg[ 0xf ] = 1;
                    mReg[ x ] = r;
                } break;
                
                case 0x6: { // 8XY6
                    mReg[ 0xf ] = mReg[ x ] & 0x1;
                    mReg[ x ] >>= 1;
                } break;
                
                case 0x7: { // 8XY7
                    int16_t r = mReg[ y ] - mReg[ x ];
                    if( r < 0 ) mReg[ 0xf ] = 0; else mReg[ 0xf ] = 1;
                    mReg[ x ] = r;
                } break;
                
                case 0xe: { // 8XYE
                    mReg[ 0xf ] = ( mReg[x] >> 7 ) & 0x1;
                    mReg[ x ] <<= 1;
                } break;
                
                default: { // opcode invalido
                    printf(" 8XY?");
                    exit( 1 );
                } break;
            }
        } break;
        
        case 0x9: { // 9XY0
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t y = ( opcode & 0x00f0 ) >> 4;
            if( mReg[x] != mReg[y] ) mPC.w += 2;
        } break;
        
        case 0xa: { // ANNN
            mI = ( opcode & 0x0fff );
        } break;
        
        case 0xc: { // CXNN
            uint8_t x = ( opcode & 0x0f00 ) >> 8;
            uint8_t nn = ( opcode & 0x00ff );
            mReg[ x ] = ( rand() & nn );
        } break;
        
        case 0xd: { // DXYN
            uint8_t x = mReg[( opcode & 0x0f00 ) >> 8];
            uint8_t y = mReg[( opcode & 0x00f0 ) >> 4];
            uint8_t n = ( opcode & 0x000f );
            mReg[ 0xf ] = 0;
            for( uint16_t i = 0; i < n; i++ ) {
                uint8_t data = readMemory( mI + i );
                for( int pixel = 7; pixel >= 0; pixel-- ) {
                    uint8_t mask = ( 1 << pixel );
                    bool draw = ( (data & mask) == mask );
                    if( draw ) {
                        bool* current = &mDisplay[ (y + i) * SCREEN_WIDTH + x + 7 - pixel ];
                        if( *current ) mReg[ 0xf ] = 1; // pixel collision
                        *current ^= true;
                    }
                }
            }
        } break;
        
        case 0xe: { // EX??
            uint8_t x = ( opcode & 0x0f00 ) >> 8;

            switch( opcode & 0x00ff ) {
                case 0x9e: { // EX9E
                    if( mKeys[mReg[x]] ) mPC.w += 2;
                } break;
                
                case 0xa1: { // EXA1
                    if( ! mKeys[mReg[x]] ) mPC.w += 2;
                } break;
                
                default: { // opcode invalido
                    printf(" EX??");
                    exit( 1 );
                } break;
            }
        } break;
        
        case 0xf: { // FX??
            uint8_t x = ( opcode & 0x0f00 ) >> 8;

            switch( opcode & 0x00ff ) {
                case 0x07: { // FX07
                    mReg[ x ] = mDelayTimer;
                } break;
                
                case 0x0a: { // FX0A
                    int key = -1;
                    for( int i = 0; i < 16; i++ ) {
                        if( mKeys[i] ) {
                            key = i;
                            break;
                        }
                    }
                    if( key != -1 ) mReg[ x ] = key; else mPC.w -= 2;
                } break;
                
                case 0x15: { // FX15
                    mDelayTimer = mReg[ x ];
                } break;
                
                case 0x18: { // FX18
                    mSoundTimer = mReg[ x ];
                } break;

                case 0x1e: { // FX1E
                    mI += mReg[ x ];
                } break;
                
                case 0x29: { // FX29
                    mI = mReg[ x ] * 5;
                } break;
                
                case 0x33: { // FX33
                    uint8_t value = mReg[ x ];
                    writeMemory( mI, value / 100 );
                    writeMemory( mI + 1, (value / 100) % 10 );
                    writeMemory( mI + 2, value % 10 );
                } break;
                
                case 0x55: { // FX55
                    for( uint16_t i = 0; i <= x; i++ ) writeMemory( mI + i, mReg[i] );
                } break;
                
                case 0x65: { // FX65
                    for( uint16_t i = 0; i <= x; i++ ) mReg[ i ] = readMemory( mI + i );
                } break;
                
                default: { // opcode invalido
                    printf(" FX??");
                    exit( 1 );
                } break;
            }
        } break;
        
        default: { // opcode invalido
            printf(" ????");
            exit( 1 );
        } break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool* Chip8::getDisplayBuffer() const {
    return mDisplay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::setKeyPressed( int key ) {
    mKeys[ key ] = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::setKeyReleased( int key ) {
    mKeys[ key ] = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chip8::updateTimers() {
    if( mDelayTimer > 0 ) mDelayTimer--;
    if( mSoundTimer > 0 ) mSoundTimer--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
