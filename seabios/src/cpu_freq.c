#include "util.h"
#include "output.h"

/*
PLL Freq = 25 * NS / (MS * 2^RS)
CPU Freq = PLL / (CPU_DIV + 2)
DRAM Freq = PLL / (2 * (DRAM_DIV + 1))

0xB6: NS
0xB7: [6] - DRAM_DIV, [5:4] - CPU_DIV, [3:2] - RS, [1:0] - MS
0xBB: [7] - PLL2M, [6] - PLL1M, [5:4] - PCI_Mode, [3:0] - PCI_DIV
0xBC: [5] - DIS_SPIbp, [4] - DIS_D3GT, [3] - DIS_D3WL, [2:0] - PLL_1_IPSEL
0xBD: [7:4] - Checksum*
0xBE: BOARD_ID (Low)
0xBF: [3:0] - BOARD_ID (High)

* Checksum:

Example:

 50/125/100
PLL:  25 * 60 = 1500. 1500 / (3 * 2^1) = 1500 / (3 * 2) = 250
CPU:  250 / (3 + 2) = 50
DRAM: 250 / (2 * (0 + 1)) = 125
CHK:  0x3C + 0x37 + 0x23 + 0x02 = 0x98. 0x09 + 0x08 = 0x11. "0x1F"

//  60/150/100
// PLL:  25 * 72 = 1800. 1800 / (3 * 2^1) = 1800 / (3 * 2) = 300
// CPU:  300 / (3 + 2) = 60
// DRAM: 300 / (2 * (0 + 1)) = 150
// CHK:  0x48 + 0x37 + 0x23 + 0x02 = 0xA4. 0x0A + 0x04 = 0x0E. "0xEF"

100/200/100
PLL:  25 * 64 = 1600. 1600 / (2 * 2^1) = 1600 / (2 * 2) = 400
CPU:  400 / (2 + 2) = 100
DRAM: 400 / (2 * (0 + 1)) = 200
CHK:  0x40 + 0x26 + 0x23 + 0x02 = 0x8B. 0x08 + 0x0B = 0x13. "0x3F"

200/200/100
PLL:  25 * 48 = 1200. 1200 / (3 * 2^0) = 1200 / (3 * 1) = 400
CPU:  400 / (0 + 2) = 200
DRAM: 400 / (2 * (0 + 1)) = 200
CHK:  0x30 + 0x03 + 0x23 + 0x02 = 0x58. 0x05 + 0x08 = 0x0D. "0xDF"

300/300/100
PLL:  25 * 72 = 1800. 1800 / (3 * 2^0) = 1800 / (3 * 1) = 600
CPU:  600 / (0 + 2) = 300
DRAM: 600 / (2 * (0 + 1)) = 300
CHK:  0x48 + 0x03 + 0x23 + 0x02 = 0x70. 0x07 + 0x00 = 0x07. "0x7F"

400/400/100
PLL:  25 * 128 = 3200. 3200 / (2 * 2^0) = 3200 / (2 * 1) = 1600
CPU:  1600 / (2 + 2) = 400
DRAM: 1600 / (2 * (1 + 1)) = 1600 / 4 = 400
CHK:  0x80 + 0x62 + 0x23 + 0x02 = 0x107. 0x01 + 0x00 + 0x07 = 0x08. "0x8F"

466/350/100
PLL:  25 * 168 = 4200. 4200 / (3 * 2^0) = 4200 / (3 * 1) = 1400
CPU:  1400 / (1 + 2) = 466
DRAM: 1400 / (2 * (1 + 1)) = 1400 / 4 = 350
CHK:  0xA8 + 0x53 + 0x23 + 0x02 = 0x120. 0x01 + 0x02 + 0x00 = 0x03. "0x3F"

500/375/100
PLL:  25 * 120 = 3000. 3000 / (2 * 2^0) = 3000 / (2 * 1) = 1500
CPU:  1500 / (1 + 2) = 500
DRAM: 1500 / (2 * (1 + 1)) = 1500 / 4 = 375
CHK:  0x78 + 0x52 + 0x23 + 0x02 = 0xEF. 0x0E + 0x0F = 0x1D. "0xDF"

// 500/375/125 (PCI_DIV=, PCI_Mode=0)
// PLL:  25 * 120 = 3000. 3000 / (2 * 2^0) = 3000 / (2 * 1) = 1500
// CPU:  1500 / (1 + 2) = 500
// DRAM: 1500 / (2 * (1 + 1)) = 1500 / 4 = 375
// CHK:  0x78 + 0x52 + 0x04 + 0x02 = 0xD1. 0x0D + 0x00 = 0x0D. "0xDF"
*/

const int num_cpu_freqs = 7;
int cpu_freqs[] = {60, 100, 200, 300, 400, 466, 500};

const u8 clock_array[7][6] = {
  //{0x3C, 0x37, 0x23, 0x02, 0x1F, 0x07},  //  50/125/100
  {0x48, 0x37, 0x23, 0x02, 0xEF, 0x07},  //  60/150/100
  {0x40, 0x26, 0x23, 0x02, 0x3F, 0x07},  // 100/200/100
  {0x30, 0x03, 0x23, 0x02, 0xDF, 0x07},  // 200/200/100
  {0x48, 0x03, 0x23, 0x02, 0x7F, 0x07},  // 300/300/100
  {0x80, 0x62, 0x23, 0x02, 0x8F, 0x07},  // 400/400/100
  {0xA8, 0x53, 0x23, 0x02, 0x3F, 0x07},  // 466/350/100
  {0x78, 0x52, 0x23, 0x02, 0xDF, 0x07},  // 500/375/100
  //{0x78, 0x52, 0x04, 0x02, 0xDF, 0x07},  // 500/375/125
};

u32 get_current_cpu_freq(void) {
    u32 strapreg2 = nbsb_read32(vx86ex_nb, 0x64);
    u32 ddiv = (strapreg2 >> 14) & 0x01L;
    u32 cdiv = (strapreg2 >> 12) & 0x03L;
    u32 cms  = (strapreg2 >> 8) & 0x03L;
    u32 cns  = strapreg2 & 0xFFL;
    int crs  = (int)((strapreg2 >> 10) & 0x03L);
    dprintf(1, "NS       = %d\n", (unsigned char)cns);
    dprintf(1, "MS       = %d\n", (unsigned char)cms);
    dprintf(1, "RS       = %d\n", crs);
    dprintf(1, "CPU_DIV  = %d\n", (unsigned char)cdiv);
    dprintf(1, "DRAM_DIV = %d\n", (unsigned char)ddiv);
    return (25L * cns) / (cms * (1L << crs) * (cdiv + 2L));
}

static void set_values(int cpu_clock_index, int cache_enabled, int boot_tune_enabled) {
    int i;
    u8 crossbar[spi_page_size];
    u8 cpu_clock[spi_page_size];

    if (get_spi_flash_info() == 0) {
        dprintf(1, "Aborting.");
        return;
    }
    dprintf(1, "Reading crossbar page.\n");
    for (i = 0; i < spi_page_size; i++) {
        crossbar[i] = spi_flash_read_byte(spi_crossbar_offset + i);
    }

    dprintf(1, "Reading cpu clock page.\n");
    for (i = 0; i < spi_page_size; i++) {
        cpu_clock[i] = spi_flash_read_byte(spi_cpu_clock_offset + i);
    }

    if (cpu_clock_index != -1) {
        dprintf(1, "Modifying cpu clock page.\n");
        cpu_clock[0xB6] = clock_array[cpu_clock_index][0];
        cpu_clock[0xB7] = clock_array[cpu_clock_index][1];
        cpu_clock[0xBB] = clock_array[cpu_clock_index][2];
        cpu_clock[0xBC] = clock_array[cpu_clock_index][3];
        cpu_clock[0xBD] = clock_array[cpu_clock_index][4];
        cpu_clock[0xBF] = clock_array[cpu_clock_index][5];
    }
    if (cache_enabled != -1) {
        cpu_clock[0xC0] = (u8)cache_enabled;
    }
    if (boot_tune_enabled != -1) {
        cpu_clock[0xC1] = (u8)boot_tune_enabled;
    }

    dprintf(1, "Erasing sector.\n");
    spi_flash_erase_sector(spi_sector_offset);

    dprintf(1, "Writing back crossbar page.\n");
    for (i = 0; i < spi_page_size; i++) {
        spi_flash_write_byte(spi_crossbar_offset + i, crossbar[i]);
    }

    dprintf(1, "Writing back cpu clock page.\n");
    for (i = 0; i < spi_page_size; i++) {
        spi_flash_write_byte(spi_cpu_clock_offset + i, cpu_clock[i]);
    }
}

void set_cpu_freq(int index) {
    set_values(index, -1, -1);
}

int get_cpu_cache_enabled(void) {
    u8 enabled = spi_flash_read_byte(spi_cpu_clock_offset + 0xC0);
    return (int)enabled;
}

void set_cpu_cache_enabled(int enabled) {
    set_values(-1, enabled, -1);
}

int get_boot_tune_enabled(void) {
    u8 enabled = spi_flash_read_byte(spi_cpu_clock_offset + 0xC1);
    return (int)enabled;
}

void set_boot_tune_enabled(int enabled) {
    set_values(-1, -1, enabled);
}