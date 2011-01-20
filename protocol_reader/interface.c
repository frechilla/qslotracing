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


void main()
{
    int8    val;
    
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

    while(1)
    {
        val = input(PIN_B0);
        if (val == 0)
        {
            output_a(0);
        }
        else if (val == 1)
        {
            output_a(255);
        }
    }
}
