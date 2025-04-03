void EEPROM_Clear_Block(unsigned long address, unsigned int len) {
    unsigned int i;

    EEPROM_WriteEnable();
    EEPROM_CS = 0;

    SPI1_Write(0x02); // WRITE komutu
    SPI1_Write((address >> 16) & 0xFF);
    SPI1_Write((address >> 8) & 0xFF);
    SPI1_Write(address & 0xFF);

    for (i = 0; i < len; i++) {
        SPI1_Write(0xFF);  // Silme = 0xFF yazmak
    }

    EEPROM_CS = 1;
    Delay_ms(10);
}
