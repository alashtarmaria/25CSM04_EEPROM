// === EEPROM Komutlari ===
#define EEPROM_CMD_WREN   0x06
#define EEPROM_CMD_WRITE  0x02
#define EEPROM_CMD_READ   0x03

#define EEPROM_CS        LATF.F13
#define EEPROM_CS_TRIS   TRISF.F13

// === Write Enable ===
void EEPROM_WriteEnable() {
    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_WREN);
    EEPROM_CS = 1;
    Delay_ms(1);
}

// === EEPROM'a string yaz ===
void EEPROM_Write_String(unsigned long address, char *str) {
    unsigned int i = 0;
    EEPROM_WriteEnable();
    EEPROM_CS = 0;

    SPI1_Write(EEPROM_CMD_WRITE);
    SPI1_Write((address >> 16) & 0xFF);
    SPI1_Write((address >> 8) & 0xFF);
    SPI1_Write(address & 0xFF);

    while (str[i] != 0) {
        SPI1_Write(str[i]);
        i++;
    }

    EEPROM_CS = 1;
    Delay_ms(10);  // Yazma tamamlanmasi için
}

// === EEPROM'dan string oku ===
void EEPROM_Read_String(unsigned long address, char *buffer, unsigned int len) {
    unsigned int i;
    EEPROM_CS = 0;

    SPI1_Write(EEPROM_CMD_READ);
    SPI1_Write((address >> 16) & 0xFF);
    SPI1_Write((address >> 8) & 0xFF);
    SPI1_Write(address & 0xFF);

    for (i = 0; i < len; i++) {
        buffer[i] = SPI1_Read(0xFF);
    }

    buffer[len] = '\0'; // Null sonlandirici
    EEPROM_CS = 1;
}

// === EEPROM sayfa yaz (max 256 byte) ===
void EEPROM_Write_Page(unsigned long address, char *dta, unsigned int len) {
    unsigned int i;

    if (len > 256) len = 256; // Sayfa limiti

    EEPROM_WriteEnable();
    EEPROM_CS = 0;

    SPI1_Write(EEPROM_CMD_WRITE);
    SPI1_Write((address >> 16) & 0xFF);
    SPI1_Write((address >> 8) & 0xFF);
    SPI1_Write(address & 0xFF);

    for (i = 0; i < len; i++) {
        SPI1_Write(dta[i]);
    }

    EEPROM_CS = 1;
    Delay_ms(10);
}

// === EEPROM blok dump (terminalde göster) ===
void EEPROM_Dump_Block(unsigned long address, unsigned int len) {
    unsigned int i;
    char txt[10];

    EEPROM_CS = 0;
    SPI1_Write(EEPROM_CMD_READ);
    SPI1_Write((address >> 16) & 0xFF);
    SPI1_Write((address >> 8) & 0xFF);
    SPI1_Write(address & 0xFF);

    UART3_Write_Text("EEPROM BLOCK DUMP:\r\n");

    for (i = 0; i < len; i++) {
        unsigned char val = SPI1_Read(0xFF);
        ByteToHex(val, txt);
        UART3_Write_Text(txt);
        UART3_Write_Text(" ");
        if ((i + 1) % 16 == 0) UART3_Write_Text("\r\n");
    }

    EEPROM_CS = 1;
    UART3_Write_Text("\r\n");
}


// === SPI PPS AYARI ===
void SPI1_PPS_Init() {
    PPS_Mapping(96, _OUTPUT, _SCK1OUT);  // RF0
    PPS_Mapping(97, _OUTPUT, _SDO1);     // RF1
    PPS_Mapping(98, _INPUT,  _SDI1);     // RF2

    TRISF.F0 = 0; // CLK
    TRISF.F1 = 0; // MOSI
    TRISF.F2 = 1; // MISO
    EEPROM_CS_TRIS = 0;
    EEPROM_CS = 1;
}

// === SPI BASLAT ===
void SPI1_Initialize_Master() {
    SPI1_Init();
}

// === UART3 BASLAT ===
void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT,  _U3RX);     // RX = RP69
    PPS_Mapping(68, _OUTPUT, _U3TX);     // TX = RP68
    UART3_Init(baudrate);
    Delay_ms(100);
}
/*
void main() {
    char buffer[32];
    char page_data[256];
    unsigned int i;

    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI1_PPS_Init();
    SPI1_Initialize_Master();
    UART3_Init_Config(9600);

    UART3_Write_Text("\n\nEEPROM TEST BASLIYOR \r\n");

    // === 1. STRING YAZMA VE OKUMA ===
    UART3_Write_Text("\r\n[HELLO EEPROM yaziliyor...]\r\n");
    EEPROM_Write_String(0x000010, "HELLO EEPROM");

    UART3_Write_Text("[0x000010 adresinden dump aliniyor...]\r\n");
    EEPROM_Dump_Block(0x000010, 16);

    // === 2. 256 BYTE PAGE YAZMA TESTI ===
    UART3_Write_Text("\r\n[256 byte sayfa verisi hazirlaniyor...]\r\n");
    for (i = 0; i < 256; i++) {
        page_data[i] = i;  // 0x00 - 0xFF
    }

    UART3_Write_Text("[0x000100 adresine 256 byte sayfa yaziliyor...]\r\n");
    EEPROM_Write_Page(0x000100, page_data, 256);

    UART3_Write_Text("[0x000100 adresinden ilk 32 byte dump...]\r\n");
    EEPROM_Dump_Block(0x000100, 32);

    while (1);
} */


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
unsigned char EEPROM_Search_String(unsigned long start_addr, unsigned long end_addr, char *target_str) {
    unsigned long i;
    char temp_buf[32];
    unsigned int len = strlen(target_str);

    for (i = start_addr; i <= end_addr - len; i++) {
        EEPROM_Read_String(i, temp_buf, len);

        if (strncmp(temp_buf, target_str, len) == 0) {
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
void EEPROM_Show_Menu() {
    UART3_Write_Text("\r\n=== EEPROM MENU ===\r\n");
    UART3_Write_Text("w: Write 'HELLO EEPROM' to 0x000010\r\n");
    UART3_Write_Text("r: Read string from 0x000010\r\n");
    UART3_Write_Text("d: Dump 16 bytes from 0x000010\r\n");
    UART3_Write_Text("p: Write 256-byte page to 0x000100\r\n");
    UART3_Write_Text("x: Dump 32 bytes from 0x000100\r\n");
    UART3_Write_Text("c: Clear 16 bytes at 0x000010\r\n");
    UART3_Write_Text("l: Add log entry (SICAKLIK...)\r\n");
    UART3_Write_Text("s: Search 'nem' in 0x001000–0x002000\r\n");
    UART3_Write_Text("h: Show this help menu\r\n");
    UART3_Write_Text("====================\r\n");
}

void main() {
    char buffer[32];
    char page_data[256];
    char komut;
    unsigned int i;

    PLLFBD = 70;
    CLKDIV = 0x0000;

    SPI1_PPS_Init();
    SPI1_Initialize_Master();
    UART3_Init_Config(9600);

    UART3_Write_Text(">>> EEPROM UART MENU SISTEMI <<<\r\n");
    EEPROM_Show_Menu();

    while (1) {
        if (UART3_Data_Ready()) {
            komut = UART3_Read();

            switch (komut) {
                case 'w':
                    EEPROM_Write_String(0x000010, "HELLO EEPROM");
                    UART3_Write_Text("[w] Veri yazildi.\r\n");
                    break;

                case 'r':
                    EEPROM_Read_String(0x000010, buffer, 12);
                    UART3_Write_Text("[r] Okunan: ");
                    UART3_Write_Text(buffer);
                    UART3_Write_Text("\r\n");
                    break;

                case 'd':
                    UART3_Write_Text("[d] Dump @ 0x000010\r\n");
                    EEPROM_Dump_Block(0x000010, 16);
                    break;

                case 'p':
                    for (i = 0; i < 256; i++) page_data[i] = i;
                    EEPROM_Write_Page(0x000100, page_data, 256);
                    UART3_Write_Text("[p] Sayfa yazildi.\r\n");
                    break;

                case 'x':
                    UART3_Write_Text("[x] Dump @ 0x000100\r\n");
                    EEPROM_Dump_Block(0x000100, 32);
                    break;

                case 'c':
                    EEPROM_Clear_Block(0x000010, 16);
                    UART3_Write_Text("[c] 16 byte silindi @ 0x000010\r\n");
                    break;

                case 'l':
                    EEPROM_Log_Write("SICAKLIK: 24.5C");
                    break;

                case 's':
                    EEPROM_Search_String(0x001000, 0x002000, "nem");
                    break;

                case 'h':
                    EEPROM_Show_Menu();
                    break;

                default:
                    UART3_Write_Text("Bilinmeyen komut! Yardim icin 'h'\r\n");
                    break;
            }
        }
    }
}
