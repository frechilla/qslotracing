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
#define     MIN_TICKS   45      // 48 ticks x 0.2us/tick = 9,6 us
#define     MAX_TICKS   60      // 65 ticks x 0.2us/tick = 13 us

#define     BIT_DELAY   5
#define     LOW_DELAY   5
#define     HIGH_DELAY  7

float const uSxTick = 0.2;      // 0.2 us per tick at 20MHz clock (one timer click for each 4 clock cycles)

// Global variables
int8        dataByte = 0;       // Data byte to be send through the serial port
int8        synced = 0;
int8        edge_high = 0;
int8        arrData[10];
int8        ndx = 0;

#INT_EXT
void extinterrupt()
{
    int16        val;

    val = get_timer1();
    if ((val > 5000) && (synced == 0))
    {
        disable_interrupts(INT_EXT);
        disable_interrupts(GLOBAL);
        synced = 1;
        edge_high = 1;
    }
    else if ((synced > 0) && (val > 50))
    {
        disable_interrupts(INT_EXT);
        disable_interrupts(GLOBAL);
        edge_high = 1;
    }

    if (synced > 0)
    {
         delay_us(9);
#asm
          // wait (32 cycles) 6,4us until mid of following bit pulse width
          // resultado esperado: distancia start bit: 12,2us, distancia a final de
          // bit, 3,8us

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

        arrData[ndx] = dataByte;

    }
}

void main()
{
    int8        act;
    int8        prev;
    int16       count;

    act = 0;
    prev = 0;
    count = 0;
    output_a(0);
    arrData[0] = 0;
    arrData[1] = 0;
    arrData[2] = 0;
    arrData[3] = 0;
    arrData[4] = 0;
    arrData[5] = 0;
    arrData[6] = 0;
    arrData[7] = 0;
    arrData[8] = 0;
    
    delay_ms(400);
    disable_interrupts(global);
    disable_interrupts(int_timer1);
    setup_adc_ports(NO_ANALOGS|VSS_VDD);
    setup_adc(ADC_OFF|ADC_TAD_MUL_0);
    setup_spi(FALSE);
    setup_wdt(WDT_OFF);
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
    setup_timer_2(T2_DISABLED,0,1);
    setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
    setup_comparator(NC_NC_NC_NC);
    setup_vref(FALSE);
    setup_low_volt_detect(FALSE);
    setup_oscillator(False);

    output_a(0);
    synced = 0;
    delay_ms(400);

    while(1)
    {
        // Sincro esperando primer start bit
        // Wait for rising edge
        ndx = 0;
        enable_interrupts(GLOBAL);
        enable_interrupts(INT_EXT);
        clear_interrupt(INT_EXT);
        ext_int_edge(L_TO_H);
        set_timer1(0);

        while (edge_high == 0);

        if (synced == 1)
        {
            ndx = 1;
            edge_high = 0;
            set_timer1(0);

            synced = 2;
        } // synced == 1
        else if (synced == 2)
        {
            ndx = 2;
            edge_high = 0;
            set_timer1(0);
            synced = 3;
        }
        else if (synced == 3)
        {
            ndx = 3;
            edge_high = 0;
            set_timer1(0);
            synced = 4;
        }
        else if (synced == 4)
        {
            ndx = 4;
            edge_high = 0;
            set_timer1(0);
            synced = 5;
        }
        else if (synced == 5)
        {
            ndx = 5;
            edge_high = 0;
            set_timer1(0);
            synced = 6;
        }
        else if (synced == 6)
        {
            ndx = 6;
            edge_high = 0;
            set_timer1(0);
            synced = 7;
        }
        else if (synced == 7)
        {
            ndx = 7;
            edge_high = 0;
            set_timer1(0);
            synced = 8;
        }
        else if (synced == 8)
        {
            ndx = 8;
            edge_high = 0;
            set_timer1(0);
            synced = 9;
        }
        else if (synced == 9)
        {
            ndx = 0;
            edge_high = 0;
            set_timer1(0);
            synced = 0;

            putc(arrData[0]);
            putc(arrData[1]);
            putc(arrData[2]);
            putc(arrData[3]);
            putc(arrData[4]);
            putc(arrData[5]);
            putc(arrData[6]);
            putc(arrData[7]);
            putc(arrData[8]);

            arrData[0] = 0;
            arrData[1] = 0;
            arrData[2] = 0;
            arrData[3] = 0;
            arrData[4] = 0;
            arrData[5] = 0;
            arrData[6] = 0;
            arrData[7] = 0;
            arrData[8] = 0;
        }
        else
        {
            ndx = 0;
            edge_high = 0;
            set_timer1(0);

            synced = 0;
        }


        edge_high = 0;
        set_timer1(0);
    } // while 1

    sleep();
}
