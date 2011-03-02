#include <18F2420.h>
#device adc=8
//#device ICD=TRUE

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES WDT128                	//Watch Dog Timer uses 1:128 Postscale
#FUSES HS                    	//High speed Osc (> 4mhz)
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BORV20                	//Brownout reset at 2.0V
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOCPD                 	//No EE protection
#FUSES STVREN                	//Stack full/underflow will cause reset
#FUSES NODEBUG               	//No Debug mode for ICD
#FUSES LVP                   	//Low Voltage Programming on B3(PIC16) or B5(PIC18)
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOWRTD                	//Data EEPROM not write protected
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES PBADEN                	//PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                	//configuration not registers write protected
#FUSES NOWRTB                	//Boot block not write protected
#FUSES NOEBTR                	//Memory not protected from table reads
#FUSES NOEBTRB               	//Boot block not protected from table reads
#FUSES NOCPB                 	//No Boot Block code protection
#FUSES LPT1OSC               	//Timer1 configured for low-power operation
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES NOXINST               	//Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(clock=20000000)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

// Type definitions
#define     TRISA       0xF92
#define     LATA        0xF89
#define     PORTA       0xF80
#define     TRISB       0xF93
#define     LATB        0xF8A
#define     PORTB       0xF81

// Constants definitions
#define     MIN_TICKS   60      // 66 ticks x 0.2us/tick = 12 us
#define     MAX_TICKS   70      // 70 ticks x 0.2us/tick = 14 us

#define     BIT_DELAY   5
#define     LOW_DELAY   5
#define     HIGH_DELAY  7

float const uSxTick = 0.2;      // 0.2 us per tick at 20MHz clock (one timer click for each 4 clock cycles)

// Global variables
int1        flagINT = 0;        // Expected level for external interrupt
int8        dataByte = 0;       // Data byte to be send through the serial port
int8        dataByteOffset = 1; // Data byte offset for the current bit
int1        flagStartBit = 0;   // Flag to show the presence of a start bit
int16       ticks = 0;          // Temp variable for Timer1 value storing
int         bitCount;       // Bit counter

// For message composing
int8        arrMsg[50];
int8        arrRecv[50];
int         m_nIndex;
int         m_nLastCmd;
int8        buff_serial[50];
int         buff_index;

int testing;


#int_EXT
void extern_int()
{
    if (flagINT == 0)
    {
        // Waiting for falling edge. Start tick counts
        ext_int_edge(0, L_TO_H);
        set_timer1(0);
        flagINT = 1;
    }
    else if (flagINT == 1)
    {
        // Waiting for rising edge
        ticks = get_timer1();
        flagINT = 0;
        // Candidate for start bit
        flagStartBit = 1;
    }
}

// Timer2 interrupt handler
//
#int_TIMER2
TIMER2_isr()
{
   if (testing == 0)
   {
       output_a(0);
       testing = 1;
   }
   else
   {
       output_a(255);
       testing = 0;
   }
}

#int_TIMER1
timer1_isr()
{
   if (testing == 0)
   {
       output_a(0);
       testing = 1;
   }
   else
   {
       output_a(255);
       testing = 0;
   }
}

#int_RDA
RDA_isr()
{
}

// This function performs all the necessary initialization steps
//
void Init()
{
    // Configure external interrupt edge -> falling edge
    ext_int_edge(0, H_TO_L);
    flagINT = 0;
    flagStartBit = 0;

    // Initialization of command processing variables
    m_nLastCmd          = 0;
    memset(arrMsg, 0, 15);
    memset(arrRecv, 0, 15);

    m_nIndex = 0;

    buff_index = 0;

    // Interrupts enabling
    enable_interrupts(INT_RDA);
    enable_interrupts(INT_EXT);
    enable_interrupts(global);
}


void main()
{
    int8 act;
    int8 prev;
    int8 data;
    int8 bit;
    int16 val;
    int8 delayval;
    char str[5];

    // Initialization
    delay_ms(400);
    disable_interrupts(global);
    disable_interrupts(int_timer1);
    disable_interrupts(int_timer2);
    disable_interrupts(int_ext);
    setup_adc_ports(NO_ANALOGS|VSS_VDD);
    setup_adc(ADC_OFF|ADC_TAD_MUL_0);
    setup_spi(FALSE);
    setup_wdt(WDT_OFF);
    setup_timer_0(RTCC_INTERNAL);
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
    setup_timer_2(T2_DIV_BY_1,2,1);
    setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    setup_low_volt_detect(FALSE);
    setup_oscillator(False);
    delay_ms(400);

    // Main loop
    output_a(0);

    // delay cycles 41 -> 8,8 us
    //              40 -> 8,4 us
    // delay_us 8      -> 8,4 us

    // Variables initialization
    act = 0;
    prev = 0;
    data = 0;
    bit = 0;
    
    while(1)
    {
        act = input(PIN_B0);
        if ((act == 0) && (prev == 1))
        {
            set_timer1(0);
        }
        else if ((act == 1) && (prev == 0))
        {
            val = get_timer1();
            if ((val <= MAX_TICKS) && (val >= MIN_TICKS))
            {
            // una vez ajustado el offset de inicio de lectura, comprobar
            // los tiempos de lectura de cada bit

#asm
          // esperar 5,8us
          movlw 0x09
          movwf 0
sync_0:   decfsz 0, 1
          bra sync_0
          nop

          // subir primer flanco de lectura
chkbit0:  movlw 0xFF
          movwf LATA    // output_a(255)

          // check bit 0
          btfss PORTB, 0
          bra bit_0_0
          // se ha leido 1, esperar 6us, contando con el chequeo, esperar 27 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          movlw 0x0A
          movwf 0
w_0_1:    decfsz 0,1
          bra w_0_1
          nop
          nop
          bra chkbit1

bit_0_0:  // se ha leido 0, esperar 10us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_0_0:    decfsz 0,1
          bra w_0_0

          // bajar segundo flanco de lectura
chkbit1:  clrf LATA    // output_a(0);

          // check bit 1
          btfss PORTB, 0
          bra bit_1_0
          // se ha leido 1, esperar 6us, contando con el chequeo, esperar 28 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(0) y el
          // salto al siguiente bit

          movlw 0x0A
          movwf 0
w_1_1:    decfsz 0,1
          bra w_1_1
          nop
          nop
          
          nop
          bra chkbit2

bit_1_0:  // se ha leido 0, esperar 10us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 45 ciclos
          movlw 0x0F
          movwf 0
w_1_0:    decfsz 0,1
          bra w_1_0
          nop

          // subir tercer flanco de lectura
chkbit2:  movlw 0xFF
          movwf LATA    // output_a(255)

          // check bit 0
          btfss PORTB, 0
          bra bit_2_0
          // se ha leido 1, esperar 6us, contando con el chequeo, esperar 27 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          movlw 0x0A
          movwf 0
w_2_1:    decfsz 0,1
          bra w_2_1
          nop
          nop
          bra chkbit3

bit_2_0:  // se ha leido 0, esperar 10us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 44 ciclos
          movlw 0x0F
          movwf 0
w_2_0:    decfsz 0,1
          bra w_2_0

          // bajar cuarto flanco de lectura
chkbit3:  clrf LATA    // output_a(0);

          // check bit 1
          btfss PORTB, 0
          bra bit_3_0
          // se ha leido 1, esperar 6us, contando con el chequeo, esperar 28 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(0) y el
          // salto al siguiente bit

          movlw 0x0A
          movwf 0
w_3_1:    decfsz 0,1
          bra w_3_1
          nop
          nop
          
          nop
          bra chkbit4

bit_3_0:  // se ha leido 0, esperar 10us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 45 ciclos
          movlw 0x0F
          movwf 0
w_3_0:    decfsz 0,1
          bra w_3_0
          nop

chkbit4:


#endasm
            }
        }
        prev = act;
    }  // Infinite loop
}
