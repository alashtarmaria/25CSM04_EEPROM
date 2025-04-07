#define LOG_BASE_ADDR   0x001000
#define LOG_ENTRY_SIZE  32
unsigned int log_index = 0;

void EEPROM_Log_Write(char *log_str) {
    unsigned long log_addr = LOG_BASE_ADDR + (log_index * LOG_ENTRY_SIZE);
    EEPROM_Write_String(log_addr, log_str);
    log_index++;

    UART3_Write_Text("[LOG] Kayit eklendi: ");
    UART3_Write_Text(log_str);
    UART3_Write_Text("\r\n");
}
