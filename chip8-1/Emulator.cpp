#include "Emulator.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

Emulator::Emulator() {
    mChip = new Chip8();
    mRunning = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Emulator::~Emulator() {
    delete mChip;
    SDL_Quit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Emulator::loadFile( string path ) {
    mChip->loadFile( path );
    mFile = path;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Emulator::start() {
    launchGraphics();
    mRunning = true;
    SDL_Event e;

    int fps = 60;
    int maxOpcodes = 400;
    float interval = 1000 / fps;
    uint32_t timer = SDL_GetTicks();
    
    while( mRunning ) {
        while( SDL_PollEvent(&e) ) handleKeys( e );

        uint32_t current = SDL_GetTicks();
        if( timer + interval < current ) {
            for( int i = 0; i < maxOpcodes / fps; i++ ) {
                mChip->updateTimers();
                mChip->execute( mChip->fetch() );
            }
            timer = current;
            render( mChip->getDisplayBuffer() );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Emulator::handleKeys( SDL_Event e ) {
    switch( e.type ) {
        case SDL_QUIT: {
            mRunning = false;
        } break;

        case SDL_KEYDOWN: {
            int k = -1;
            if     ( e.key.keysym.sym == SDLK_ESCAPE ) mRunning = false;
            else if( e.key.keysym.sym == KEY_0 ) k = 0;
            else if( e.key.keysym.sym == KEY_1 ) k = 1;
            else if( e.key.keysym.sym == KEY_2 ) k = 2;
            else if( e.key.keysym.sym == KEY_3 ) k = 3;
            else if( e.key.keysym.sym == KEY_4 ) k = 4;
            else if( e.key.keysym.sym == KEY_5 ) k = 5;
            else if( e.key.keysym.sym == KEY_6 ) k = 6;
            else if( e.key.keysym.sym == KEY_7 ) k = 7;
            else if( e.key.keysym.sym == KEY_8 ) k = 8;
            else if( e.key.keysym.sym == KEY_9 ) k = 9;
            else if( e.key.keysym.sym == KEY_A ) k = 0xa;
            else if( e.key.keysym.sym == KEY_B ) k = 0xb;
            else if( e.key.keysym.sym == KEY_C ) k = 0xc;
            else if( e.key.keysym.sym == KEY_D ) k = 0xd;
            else if( e.key.keysym.sym == KEY_E ) k = 0xe;
            else if( e.key.keysym.sym == KEY_F ) k = 0xf;
            if( k != -1 ) mChip->setKeyPressed( k );
        } break;
        
        case SDL_KEYUP: {
            int k = -1;
            if     ( e.key.keysym.sym == SDLK_ESCAPE ) mRunning = false;
            else if( e.key.keysym.sym == KEY_0 ) k = 0;
            else if( e.key.keysym.sym == KEY_1 ) k = 1;
            else if( e.key.keysym.sym == KEY_2 ) k = 2;
            else if( e.key.keysym.sym == KEY_3 ) k = 3;
            else if( e.key.keysym.sym == KEY_4 ) k = 4;
            else if( e.key.keysym.sym == KEY_5 ) k = 5;
            else if( e.key.keysym.sym == KEY_6 ) k = 6;
            else if( e.key.keysym.sym == KEY_7 ) k = 7;
            else if( e.key.keysym.sym == KEY_8 ) k = 8;
            else if( e.key.keysym.sym == KEY_9 ) k = 9;
            else if( e.key.keysym.sym == KEY_A ) k = 0xa;
            else if( e.key.keysym.sym == KEY_B ) k = 0xb;
            else if( e.key.keysym.sym == KEY_C ) k = 0xc;
            else if( e.key.keysym.sym == KEY_D ) k = 0xd;
            else if( e.key.keysym.sym == KEY_E ) k = 0xe;
            else if( e.key.keysym.sym == KEY_F ) k = 0xf;
            if( k != -1 ) mChip->setKeyReleased( k );
        } break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Emulator::render( bool* display ) {
    uint8_t screen[ SCREEN_HEIGHT ][ SCREEN_WIDTH ][ 3 ];
    for( int y = 0; y < SCREEN_HEIGHT; y++ ) {
        for( int x = 0; x < SCREEN_WIDTH; x++ ) {
            uint8_t color = display[ y * SCREEN_WIDTH + x ] ? 0 : 255;
            screen[ y ][ x ][ 0 ] = color;
            screen[ y ][ x ][ 1 ] = color;
            screen[ y ][ x ][ 2 ] = color;
        }
    }
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glRasterPos2i( -1, 1 );
    glPixelZoom( 1 * SCREEN_SCALE, -1 * SCREEN_SCALE );
    glDrawPixels( SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, screen );
    SDL_GL_SwapBuffers() ;
    glFlush();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Emulator::launchGraphics() {
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_SetVideoMode( SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, 8, SDL_OPENGL );
    SDL_WM_SetCaption( string("bliss-8" + mFile).c_str(), NULL ); 
    glViewport( 0, 0, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0 );
    glClearColor( 0, 0, 0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
    glShadeModel( GL_FLAT );
    glEnable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glDisable( GL_DITHER) ;
    glDisable( GL_BLEND );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
