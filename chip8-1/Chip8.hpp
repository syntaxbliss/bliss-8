#ifndef CHIP8_HPP_INCLUDED
#define CHIP8_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <iostream>
#include "Commons.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Chip8 {
    public:
                                Chip8               ();
        virtual                 ~Chip8              ();
        void                    loadFile            ( string );
        uint16_t                fetch               ();
        void                    execute             ( uint16_t );
        bool*                   getDisplayBuffer    () const;
        void                    setKeyPressed       ( int );
        void                    setKeyReleased      ( int );
        void                    updateTimers        ();

    private:
        uint8_t                 readMemory          ( uint16_t ) const;
        void                    writeMemory         ( uint16_t, uint8_t );
        void                    stackPush           ( uint16_t );
        uint16_t                stackPop            ();
        uint8_t*                mMemory;
        uint8_t*                mReg;
        uint16_t*               mStack;
        uint16_t                mI;
        Pair                    mPC;
        uint16_t*               mSP;
        uint8_t                 mDelayTimer;
        uint8_t                 mSoundTimer;
        bool*                   mDisplay;
        bool*                   mKeys;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // CHIP8_HPP_INCLUDED
