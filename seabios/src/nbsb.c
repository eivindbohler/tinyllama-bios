#include "x86.h"
#include "util.h"

#define PCI_GET_CF8(bus, dev, fun)  (0x80000000UL + ((u32)(bus) << 16) + ((u32)(dev) << 11) + ((u32)(fun) << 8))

const u8 vx86ex_nb = 0;
const u8 vx86ex_sb = 7;

static u32 pci_in32(u32 addr) {
    outl(addr, 0x0CF8);
    return inl(0x0CFC);
}

static void pci_out32(u32 addr, u32 value) {
    outl(addr, 0x0CF8);
    outl(value, 0x0CFC);
}

//static u16 pci_in16(u32 addr) {
//    int i = (int)(addr & 0x03L) << 3; // * 8
//    outl(addr & 0xFFFFFFFCL, 0x0CF8);
//    return (u16)(inl(0x0CFC) >> i);
//}

//static void pci_out16(u32 addr, u16 value) {
//    int i = (int)(addr & 0x03L) << 3; // * 8
//    outl(addr & 0xFFFFFFFCL, 0x0CF8);
//    outl((inl(0x0CFC) & ~(0xFFFFL << i)) | ((u32)value << i), 0x0CFC);
//}

static u8 pci_in8(u32 addr) {
    int i = (int)(addr & 0x03L) << 3; // * 8
    outl(addr & 0xFFFFFFFCL, 0x0CF8);
    return (u8)(inl(0x0CFC) >> i);
}

static void pci_out8(u32 addr, u8 value) {
    int i = (int)(addr & 0x03L) << 3; // * 8
    outl(addr & 0xFFFFFFFCL, 0x0CF8);
    outl((inl(0x0CFC) & ~(0xFFL << i)) | ((u32)value << i), 0x0CFC);
}

u32 nbsb_read32(u8 nbsb, u8 offset) {
    return pci_in32(PCI_GET_CF8(0, nbsb, 0) + (u32)offset);
}

void nbsb_write32(u8 nbsb, u8 offset, u32 value) {
   pci_out32(PCI_GET_CF8(0, nbsb, 0) + (u32)offset, value);
}

//static u16 nbsb_read16(u8 nbsb, u8 offset) {
//    return pci_in16(PCI_GET_CF8(0, nbsb, 0) + (u32)offset);
//}

//static void nbsb_write16(u8 nbsb, u8 offset, u16 value) {
//    pci_out16(PCI_GET_CF8(0, nbsb, 0) + (u32)offset, value);
//}

u8 nbsb_read8(u8 nbsb, u8 offset) {
    return pci_in8(PCI_GET_CF8(0, nbsb, 0) + (u32)offset);
}

void nbsb_write8(u8 nbsb, u8 offset, u8 value) {
    pci_out8(PCI_GET_CF8(0, nbsb, 0) + (u32)offset, value);
}


