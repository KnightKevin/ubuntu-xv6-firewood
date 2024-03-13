#define UART0 0x10000000L
#define WriteReg(reg, v) (*((unsigned char *)(UART0+reg)) = (v))
#define IER 1
#define LCR 3
#define LSB 0
#define MSB 1
#define FCR 2
#define THX 0
void main() {
    // IER
    WriteReg(1, 0);

    // LCR
    WriteReg(LCR, 1<<7);

    // LSB
    WriteReg(LSB, 3);

    // MSB
    WriteReg(MSB, 00);

    // LCR
    WriteReg(LCR, 3);

    // FCR
    WriteReg(FCR, 6);

    // IER
    WriteReg(IER, 3);

    WriteReg(THX, 97);
    WriteReg(THX, 97);
    WriteReg(THX, 97);
    
}