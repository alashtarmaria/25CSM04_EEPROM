char ToUpper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    return c;
}

unsigned char EEPROM_Search_String(unsigned long start_addr, unsigned long end_addr, char *target_str) {
    unsigned long i;
    char temp_buf[32];
    unsigned int len = strlen(target_str);
    unsigned int j;
    unsigned char match;

    for (i = start_addr; i <= end_addr - len; i++) {
        EEPROM_Read_String(i, temp_buf, len);
        match = 1;

        for (j = 0; j < len; j++) {
            if (ToUpper(temp_buf[j]) != ToUpper(target_str[j])) {
                match = 0;
                break;
            }
        }

        if (match) {
            UART3_Write_Text("[ARA] Bulundu @ adres: ");
            LongToStr(i, temp_buf);
            UART3_Write_Text(temp_buf);
            UART3_Write_Text("\r\n");
            return 1;
        }
    }

    UART3_Write_Text("[ARA] String bulunamadi.\r\n");
    return 0;
}
