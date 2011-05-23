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

// At 20MHz, each oscillator tick is 0,05us
// Each instruction cycle (4 oscillator cicles) is 0,2us

// Constants definitions
#define     MIN_TICKS   55      // 48 ticks x 0.2us/tick = 9,6 us
#define     MAX_TICKS   57      // 65 ticks x 0.2us/tick = 13 us

#define     BIT_DELAY   5
#define     LOW_DELAY   5
#define     HIGH_DELAY  7

float const uSxTick = 0.2;      // 0.2 us per tick at 20MHz clock (one timer click for each 4 clock cycles)

// Global variables
int8        dataByte = 0;       // Data byte to be send through the serial port

// For message composing
int8        arrRecv[5];

#int_RDA
RDA_isr()
{
}

void main()
{
    int8 act;
    int8 prev;
    int16 val;
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
    setup_timer_2(T3_DISABLED,0,1);
    setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    setup_low_volt_detect(FALSE);
    setup_oscillator(False);
    
    // Wait for stabilisation
    delay_ms(400);

    // Initialize PORTA output
    output_a(0);

    // Variables initialization
    act      = 0;
    prev     = 0;
    dataByte = 0;

    // Main loop
    while(1)
    {
        // Get current pin value
        act = input(PIN_B0);
        if ((act == 1) && (prev == 0))
        {
            // Reset timer value
            set_timer1(0);
#asm
          movlw 0xFF
          movwf LATA    // output_a(255)
#endasm
        }
        else if ((act == 0) && (prev == 1))
        {
            // Get current timer value
            val = get_timer1();

            // Check pulse width
            if ((val <= MAX_TICKS) && (val >= MIN_TICKS))
            {

            // resultados esperados: los flancos de subida o bajada tienen que estar
            // a 3.8us del inicio del bit. La espera en nivel alto será de 7,6us y a
            // nivel bajo 10us
#asm
          clrf LATA    // output_a(0);
          // wait (59 cycles) 11,8us until mid of following bit pulse width
          // resultado esperado: distancia start bit: 11,8us, distancia a final de
          // bit, 3,8us
          movlw 0x13
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
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_0_1:    decfsz 0,1
          bra w_0_1
          nop
          // nop replaced by bit set

          // Set bit 0
          bsf dataByte, 7
          bra chkbit1

bit_0_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_0_0:    decfsz 0,1
          bra w_0_0

          // bajar segundo flanco de lectura
chkbit1:  clrf LATA    // output_a(0);
          nop

          // check bit 1
          btfss PORTB, 0
          bra bit_1_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_1_1:    decfsz 0,1
          bra w_1_1
          nop
          // nop replaced by bit set

          // Set bit 1
          bsf dataByte, 6
          bra chkbit2

bit_1_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_1_0:    decfsz 0,1
          bra w_1_0

          // subir tercer flanco de lectura
chkbit2:  movlw 0xFF
          movwf LATA    // output_a(255)

          // check bit 0
          btfss PORTB, 0
          bra bit_2_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_2_1:    decfsz 0,1
          bra w_2_1
          nop
          // nop replaced by bit set

          // Set bit 2
          bsf dataByte, 5
          bra chkbit3

bit_2_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_2_0:    decfsz 0,1
          bra w_2_0

          // bajar cuarto flanco de lectura
chkbit3:  clrf LATA    // output_a(0);
          nop

          // check bit 1
          btfss PORTB, 0
          bra bit_3_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_3_1:    decfsz 0,1
          bra w_3_1
          nop
          // nop replaced by bit set

          // Set bit 3
          bsf dataByte, 4
          bra chkbit4

bit_3_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_3_0:    decfsz 0,1
          bra w_3_0

          // subir quinto flanco de lectura
chkbit4:  movlw 0xFF
          movwf LATA    // output_a(255)

          // check bit 0
          btfss PORTB, 0
          bra bit_4_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_4_1:    decfsz 0,1
          bra w_4_1
          nop
          // nop replaced by bit set

          // Set bit 4
          bsf dataByte, 3
          bra chkbit5

bit_4_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_4_0:    decfsz 0,1
          bra w_4_0

          // bajar sexto flanco de lectura
chkbit5:  clrf LATA    // output_a(0);
          nop

          // check bit 1
          btfss PORTB, 0
          bra bit_5_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_5_1:    decfsz 0,1
          bra w_5_1
          nop
          // nop replaced by bit set

          // Set bit 5
          bsf dataByte, 2
          bra chkbit6

bit_5_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_5_0:    decfsz 0,1
          bra w_5_0

          // subir septimo flanco de lectura
chkbit6:  movlw 0xFF
          movwf LATA    // output_a(255)

          // check bit 0
          btfss PORTB, 0
          bra bit_6_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_6_1:    decfsz 0,1
          bra w_6_1
          nop
          // nop replaced by bit set

          // Set bit 6
          bsf dataByte, 1
          bra chkbit7

bit_6_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_6_0:    decfsz 0,1
          bra w_6_0

          // bajar octavo flanco de lectura
chkbit7:  clrf LATA    // output_a(0);
          nop

          // check bit 1
          btfss PORTB, 0
          bra bit_7_0
          // se ha leido 1, esperar 7,6us, contando con el chequeo, esperar 38 ciclos
          // en la cuenta de la espera se tiene en cuenta el output_a(255) y el
          // salto al siguiente bit
          // nos quedamos pasados 3.8us en el siguiente bit
          movlw 0x0A
          movwf 0
w_7_1:    decfsz 0,1
          bra w_7_1
          nop
          // nop replaced by bit set

          // Set bit 7
          bsf dataByte, 0
          bra stop

bit_7_0:  // se ha leido 0, esperar 10,4us, contando con los ciclos gastados
          // anteriormente en saltos y chequeos, quedan por esperar 46 ciclos
          movlw 0x0F
          movwf 0
w_7_0:    decfsz 0,1
          bra w_7_0

stop:
#endasm
                 //putc(dataByte);
                 dataByte = 0xAA;
                 putc(dataByte);
                 dataByte = 0;
            }
            else
            {
            #asm
                clrf LATA    // output_a(0);
            #endasm
            }
        }
        prev = act;
    }  // Infinite loop
}
