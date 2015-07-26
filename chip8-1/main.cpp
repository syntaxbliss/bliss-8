#include <stdio.h>
#include "Emulator.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    Emulator* emu = new Emulator();
    //emu->loadFile( string(ROMS_DIR) + "INVADERS" );
    //emu->loadFile( string(ROMS_DIR) + "MAZE" );
    //emu->loadFile( string(ROMS_DIR) + "TETRIS" );
    emu->loadFile( string(ROMS_DIR) + "VBRIX" );
    //emu->loadFile( string(ROMS_DIR) + "WORM3" );
    emu->start();
    delete emu;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
