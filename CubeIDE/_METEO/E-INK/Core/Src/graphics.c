/*
 *  Graphics
 *
 *  Created on: Sen 27, 2025
 *      Author: Artem Vorobyov <justdoit4yourfamily@gmail.com>
 */
#include "../Inc/SSD1680.h"
#include <stdlib.h>
#include <string.h>

void SSD1680_Redbar(SSD1680_HandleTypeDef *hepd, const uint8_t x, const uint8_t xmin, const uint8_t xmax, const uint8_t length) {
    uint8_t bar[length];
    if (x % 2) {
		for(uint8_t i = 2; i < (sizeof(bar)-2); i++) {
			bar[i] = (i <= x) ? 0xFF : (i % 2) ? 0x01 : 0x80;
		}
	} else {
		for(uint8_t i = 2; i < sizeof(bar); i++) {
			bar[i] = (i < x) ? 0xFF : (i % 2) ? 0x01 : 0x80;
		}
	}
	bar[0] = 0xFF;
	bar[1] = 0xFF;
	bar[sizeof(bar)-2] = 0xFF;
	bar[sizeof(bar)-1] = 0xFF;

    SSD1680_SetRegion(&hepd, 8, 148, 16, 100, NULL, bar);
}
