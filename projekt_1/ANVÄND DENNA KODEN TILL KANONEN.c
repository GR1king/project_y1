#include "gd32vf103.h"   /* MCU library */
#include "sht31.h"       /* our sensor functions */
#include "drivers.h"     /* timer functions */
#include "lcd.h"         /* LCD functions */
#include "vind.h"        /* vind sensor functions */
#include "led.h"        /* led functions */
#include "pwm.h"
#include "math.h"

#define EXTRA 10        //exktra vinkel för safezone
#define SPEED 5         // procent av hastigheten motorn ska ha

void rotera(int pinMal,int seMinne,int gradMal);

/* Wait for a number of milliseconds */
static void delay_ms(int ms);


int main(void) {
    uint16_t temp_raw;   /* raw temperature from sensor */
    uint16_t hum_raw;    /* raw humidity from sensor */
    int temp_c;          /* temperature in Celsius */
    int hum_pct;         /* humidity in percent */
    int ret_sht35;       /* return value from sensor function */
    int temp_abs;        /* used if temperature is negative */
    float wet_bulb;      /* calculated wet bulb temperature */

    uint16_t vind_raw;   /* raw wind direction from sensor */
    uint16_t new_average = 0; /* new average wind direction */
    int vind_deg;        /* wind direction in degrees */
    int ret_vind;             /* return value from vind sensor function */
    float angles[10];       /* array to hold recent wind direction readings for averaging */
    int index = 0;         /* index for storing readings in the angles array */
    int average_flag = 0;  /* flag to indicate when we have enough readings for averaging */
    int onskatLage; //ser vilken vinkel snökanonen vill vrida sig mot
	uint32_t lage; //tittar checkar läget
	int toggle = 0;//bestämmer hur själva motorn ska snurra
	int grader = 0; //tar emot vinddatans tal
	int bGrader = 0; //en checkup för om motorn behöver vrida på sig
	int minne = 0;

    
    t5omsi();   /* start Timer5 as a 1 ms timer */

    /* start LCD */
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);

    /* visa direkt att programmet lever */
    //LCD_ShowStr(0, 0, (u8 *)"Starting...", YELLOW, OPAQUE);
    LCD_Wait_On_Queue();
    delay_ms(500);

    /* start I2C */
    sht31_init_setup();
    vind_init_setup();
    led_init();

    
    /*Initsera Motor*/
    rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);

    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_11); //Dessa initierar bara pinben för olika riktningarna //0 och 360 grader grader B11
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5); // A5 = 45 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8); ///B8 = 90 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_9); //B9 = 135 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5); //B5 = 180 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_4); //B4 = 225 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_12); //A8 = 270 grader
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8); //B12 = 315 grader

	
	T1powerUpInitPWM(0x0C); //gör A2 och A3 på mcun är PWM outputsen som man ändra på styrkan 0%-100%. Det är outputsen till själva h-bryggan som ska driva motorn
    do{
		T1setPWMch3((15 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
		T1setPWMch2(0);
        lage = gpio_input_bit_get(GPIOB,GPIO_PIN_11); //Tar in status från plattan som ska nuddas av metallstaven
        } while (lage == 0); //checkar om den är förändrad dvs om pinnen är där 


        /* main loop */

    while (1) {
        /* ask the sensor for one reading */
        ret_sht35 = sht31_read(&temp_raw, &hum_raw);
        ret_vind = vind_read_raw(&vind_raw);
        /* clear the screen before drawing new text */
        LCD_Clear(BLACK);
        /* show fixed labels */
        LCD_ShowStr(0, 0, (u8 *)"Temp:", WHITE, OPAQUE);
        LCD_ShowStr(0, 15, (u8 *)"Humidity:", WHITE, OPAQUE);
        LCD_ShowStr(0, 30, (u8 *)"Wet bulb:", WHITE, OPAQUE);
        LCD_ShowStr(0, 45, (u8 *)"Wind Direction:", WHITE, OPAQUE);

        if (ret_sht35 == 0) {
            /* change raw values into real values */
            sht31_convert(temp_raw, hum_raw, &temp_c, &hum_pct);
            wet_bulb = sht31_wet_bulb((float)temp_c, (float)hum_pct);

            if(wet_bulb < 30)            //LED stängs av om wetbulb är mindre än -2.8
            {
                led_on();
            }
            else
            {
                led_off();
            }

            /* show temperature */
            if (temp_c < 0) {
                temp_abs = -temp_c;                            /* make it positive */
                LCD_ShowChar(45, 0, '-', OPAQUE, CYAN);      /* show minus sign */
                LCD_ShowNum(53, 0, (u16)temp_abs, 3, CYAN);  /* show number */
            } else {
                LCD_ShowNum(45, 0, (u16)temp_c, 3, CYAN);    /* show positive number */
            }
            LCD_ShowChar(80, 0, 'C', OPAQUE, CYAN);          /* show C */

            /* show humidity */
            LCD_ShowNum(75, 15, (u16)hum_pct, 3, CYAN);       /* show humidity number */
            LCD_ShowChar(102, 15, '%', OPAQUE, CYAN);  
            
            /* Wet bulb */
            LCD_ShowNum1(75, 30, wet_bulb, 4, GREEN);
            LCD_ShowChar(115, 30, 'C', OPAQUE, GREEN);/* show % */



            /* show if temperature is high, low, or normal */
            if (temp_c > temp_high) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too high", RED, OPAQUE);
            } else if (temp_c < temp_low) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too low", BLUE, OPAQUE);
            } else {
                LCD_ShowStr(0, 45, (u8 *)"Temp normal", GREEN, OPAQUE);
            }
        } else {
            /* if reading failed, show error */
            LCD_ShowStr(0, 45, (u8 *)"Error:", RED, OPAQUE);
            
        }
        if (ret_vind == 0) {
            vind_read_degrees(vind_raw, &vind_deg);
            LCD_ShowNum(0, 60, vind_deg, 3, CYAN);

        } else {
            LCD_ShowStr(120, 60, (u8 *)"Error", RED, OPAQUE);
        }



        LCD_Wait_On_Queue();   /* make sure LCD finishes drawing */




   //////////////////////////////////motor logik/////////////////////////////////////////////

    grader = vind_deg;

		if(grader != bGrader){
			minne = bGrader;
			bGrader = grader;

			//denna sektionen tar reda på vilket det onskadelaget är

			if(grader>=0 && grader<45){
				onskatLage = 1;
			} 

			if(grader>=45 && grader<90){
				onskatLage = 2;
			} 

			if(grader>=90 && grader<135){
				onskatLage = 3;
			} 

			if(grader>=135 && grader<180){
				onskatLage = 4;
			} 

			if(grader>=180 && grader<225){
				onskatLage = 5;
			} 

			if(grader>=225 && grader<270){
				onskatLage = 6;
			} 

			if(grader>=270 && grader<315){
				onskatLage = 7;
			} 

			if(grader>=315 && grader<360){
				onskatLage = 8;
			}



			//här är en switch för vilken platta den ska rotera till (siffrorna som skickas in är bara en placeholder för)

			switch(onskatLage){

				case 1: rotera(1,minne,grader);break;
				case 2: rotera(2,minne,grader);break;
				case 3: rotera(3,minne,grader);break;
				case 4: rotera(4,minne,grader);break;
				case 5: rotera(5,minne,grader);break;
				case 6: rotera(6,minne,grader);break;
				case 7: rotera(7,minne,grader);break;
				case 8: rotera(8,minne,grader);break;


			}
		}
delay_ms(250);
////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

}

//FUNKTIONER SOM ANVÄNDS

void rotera(int pinMal,int seMinne,int gradMal){
	
	int vinkel = 5;

	switch(pinMal){

		case 1:
/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_11); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_11); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0); //checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		case 2:
		/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOA,GPIO_PIN_5); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOA,GPIO_PIN_5); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0); T1setPWMch2(0);
						T1setPWMch3(0);//checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	case 3:

	/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0); //checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	case 4: 	/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_9); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_9); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0); //checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	case 5:

		/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_5); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_5); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0); T1setPWMch2(0);
						T1setPWMch3(0);//checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	case 6:

			/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_4); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_4); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0); //checkar om den är förändrad dvs om pinnen är framme
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	case 7:

			/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_12); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_12); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0); //checkar om den är förändrad dvs om pinnen är framme
						T1setPWMch2(0);
						T1setPWMch3(0);
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	case 8:

			/////////////////////////////////////////////////////////////////////////////////////7//*0 grader*//
		if(gradMal<seMinne){
			do{
				T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
				T1setPWMch3(0);
				vinkel = gpio_input_bit_get(GPIOA,GPIO_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOA,GPIO_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0); //checkar om den är förändrad dvs om pinnen är framme
						T1setPWMch2(0);
						T1setPWMch3(0);
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	}	
}

static void delay_ms(int ms) {
    while (ms > 0) {
        while (!t5expq()) {
            /* stay here until 1 ms has passed */
        }
        ms--;
    }
}
