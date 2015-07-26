#ifndef EMULATOR_HPP_INCLUDED
#define EMULATOR_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Chip8.hpp"
#include <SDL/SDL.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class Emulator {
    public:
                                Emulator            ();
        virtual                 ~Emulator           ();
        void                    loadFile            ( string );
        void                    start               ();
        void                    launchGraphics      ();

    private:
        void                    render              ( bool* );
        void                    handleKeys          ( SDL_Event );
        Chip8*                  mChip;
        bool                    mRunning;
        string                  mFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // EMULATOR_HPP_INCLUDED
