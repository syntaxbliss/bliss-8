#ifndef COMMONS_HPP_INCLUDED
#define COMMONS_HPP_INCLUDED

////////////////////////////////////////////////////////////////////////////////////////////////////

#define MEMORY_SIZE     0x1000
#define ROMS_DIR        "/home/sosella/Desarrollo/Emuladores/Chip8/Roms/"
#define SCREEN_WIDTH    64
#define SCREEN_HEIGHT   32
#define SCREEN_SCALE    8
#define PIXEL_ON        0xff000000
#define PIXEL_OFF       0xffffffff

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union ___PAIR {
    uint16_t    w;
    struct {
        uint8_t l;
        uint8_t h;
    };
} Pair;

////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY_1   SDLK_1
#define KEY_2   SDLK_2
#define KEY_3   SDLK_3
#define KEY_C   SDLK_4

#define KEY_4   SDLK_q
#define KEY_5   SDLK_w
#define KEY_6   SDLK_e
#define KEY_D   SDLK_r

#define KEY_7   SDLK_a
#define KEY_8   SDLK_s
#define KEY_9   SDLK_d
#define KEY_E   SDLK_f

#define KEY_A   SDLK_z
#define KEY_0   SDLK_x
#define KEY_B   SDLK_c
#define KEY_F   SDLK_v

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // COMMONS_HPP_INCLUDED
