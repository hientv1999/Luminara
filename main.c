/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "include/sol64x64.h"
#include "include/ssd1306_i2c.h"

/* 
   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on display
   board
   GPIO PICO_DEFAULT_I2C_SCL_PIN (on Pico this is GP5 (pin 7)) -> SCL on
   display board
   3.3v (pin 36) -> VCC on display board
   GND (pin 38)  -> GND on display board
*/


int main() {
    stdio_init_all();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / SSD1306_i2d example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    // useful information for picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("SSD1306 OLED driver I2C example for the Raspberry Pi Pico"));

    printf("Luminara begins\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    const uint DEBUG_LED = PICO_DEFAULT_LED_PIN;
    gpio_init(DEBUG_LED);
    gpio_set_dir(DEBUG_LED, GPIO_OUT);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // If you don't do anything before initializing a display pi pico is too fast and starts sending
    // commands before the screen controller had time to set itself up, so we add an artificial delay for
    // ssd1306 to set itself up
    sleep_ms(250);
    // run through the complete initialization process
    gpio_put(DEBUG_LED, 0);
    while (!SSD1306_init()){
        gpio_put(DEBUG_LED, 1);
        sleep_ms(250);
        gpio_put(DEBUG_LED, 0);
        sleep_ms(250);
    }

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    clearDisplay();
    struct render_area area;
    createBitmapArea(&area, IMG_WIDTH, IMG_HEIGHT);
    char* text = "SOL";
    drawBitmap(sol64x64, &area, IMG_WIDTH, IMG_HEIGHT, 0);
    writeStringCenterInRestrictedArea(buf, IMG_WIDTH, 0, SSD1306_WIDTH, SSD1306_HEIGHT, text);
    while (true){
        sleep_ms(1000);
    }
#endif
    return 0;
}
