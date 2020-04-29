/* 
 * Copyright (c) 2020 Arm
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include <stdio.h>
#include <algorithm>

#if COMPONENT_SD
#include "SDBlockDevice.h"
#else
#error "Run this only on SD card"
#endif 

SDBlockDevice bd(
        MBED_CONF_SD_SPI_MOSI,
        MBED_CONF_SD_SPI_MISO,
        MBED_CONF_SD_SPI_CLK,
        MBED_CONF_SD_SPI_CS);

/*
  We want to full format SD card. To achieve this, we erase the entire SD card by
  writing 1. Note, this is aggressive method and should not be run often on a targat, rather
  run this when there is an issue with SD card
 */
int main() 
{
    int err = bd.init();
    MBED_ASSERT(!err);
    bd_size_t size = bd.size();
    bd_size_t erase_size = bd.get_erase_size();
    bd_size_t count = size / erase_size;
    uint8_t *ones = (uint8_t*)malloc(erase_size);
    memset(ones, 0xff, erase_size);
    for (bd_size_t i = 0; i < count; i++) {
        err = bd.erase(i * erase_size, erase_size);
        MBED_ASSERT(!err);
        err = bd.program(ones, i * erase_size, erase_size);
        MBED_ASSERT(!err);
        fflush(stdout);
    }
    err = bd.deinit();
    MBED_ASSERT(!err);
}
