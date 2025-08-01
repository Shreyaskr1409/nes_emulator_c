#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../cartridge/cartridge.h"
#include <raylib.h>

typedef struct ppu2C02 {
    uint8_t tblName[2][1024]; // VRAM
    uint8_t tblPalette[32];

    // 1 half handles sprites and other half handles bg tiles in many cases
    // each half consists of 16*16 tiles where each tile is 8*8 pixels
    // each pixel takes up 2 bits (4 options for colors)
    // the data for these pixels are laid out in the memory in 2 8*8 planes
    // first plane holds the LSB and the other holds the MSB
    // All we need to access these planes is location of the pixel in tile,
    // then we can just lookup the LSB and add offset of 8 to find the MSB
    uint8_t tblPattern[2][4096];
    cartridge* cart;

    Color palScreen[0x40];

    Image* imgScreen;
    Image* imgNameTable[2];
    Image* imgPatternTable[2];

    Texture2D *texScreen;
    Texture2D *texNameTable[2];
    Texture2D *texPatternTable[2];

    // Changed from 256*240*4 to 256*256*4 because last 16 rows are present, but hidden
    uint8_t frameBuffer[256 * 256 * 4]; // 32-bit RGBA

    bool frame_complete;

    int16_t scanline;
    int16_t cycle;


    union {
		struct {
			uint8_t unused : 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_zero_hit : 1;
			uint8_t vertical_blank : 1;
		};
		uint8_t reg;
	} status;

	union {
		struct {
			uint8_t grayscale : 1;
			uint8_t render_background_left : 1;
			uint8_t render_sprites_left : 1;
			uint8_t render_background : 1;
			uint8_t render_sprites : 1;
			uint8_t enhance_red : 1;
			uint8_t enhance_green : 1;
			uint8_t enhance_blue : 1;
		};
		uint8_t reg;
	} mask;

	union PPUCTRL {
		struct {
			uint8_t nametable_x : 1;
			uint8_t nametable_y : 1;
			uint8_t increment_mode : 1;
			uint8_t pattern_sprite : 1;
			uint8_t pattern_background : 1;
			uint8_t sprite_size : 1;
			uint8_t slave_mode : 1; // unused
			uint8_t enable_nmi : 1;
		};
		uint8_t reg;
	} control;

    uint8_t address_latch; // tells whether we are reading from low bit or high bit
    uint8_t data_buffer; // when reading from ppu, it is delayed by 1 cycle,
                         // thus we are storing the data from a cycle before

	union loopy_register {
		struct {
			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};
		uint16_t reg;
	} loopy;

    union loopy_register vram_addr;
    union loopy_register tram_addr;
    uint8_t fine_x;

    bool nmi;

    uint8_t bg_next_tile_id;
    uint8_t bg_next_tile_attrib;
    uint8_t bg_next_tile_lsb;
    uint8_t bg_next_tile_msb;

} ppu2C02;

void PpuInit(ppu2C02* ppu);
void PpuDestroy(ppu2C02* ppu);

Texture2D PpuGetScreen(ppu2C02* ppu);
Texture2D PpuGetNameTable(ppu2C02* ppu, uint8_t i);
Texture2D PpuGetPatternTable(ppu2C02* ppu, uint8_t i, uint8_t palette);

Color GetColorFromPaletteRam(ppu2C02* ppu, uint8_t palette, uint8_t pixel);

void PpuDrawPixelScreen(ppu2C02* ppu, int x, int y, Color color);
void PpuUpdateScreenTexture(ppu2C02* ppu);

uint8_t PpuReadFromCpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToCpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

uint8_t PpuReadFromPpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToPpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

void PpuConnectCartridge(ppu2C02* ppu, cartridge *cart);
void PpuClock(ppu2C02*  ppu);
