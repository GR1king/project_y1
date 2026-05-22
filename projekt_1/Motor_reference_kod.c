#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"
#include "pwm.h"
#include "vind.h"
#define EI 1
#define DI 0


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
						T1setPWMch2((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch3(0);
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
				vinkel = gpio_input_bit_get(GPIOB,GPIO_EVENT_PIN_5); //Tar in status från plattan som ska nuddas av metallstaven
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
				vinkel = gpio_input_bit_get(GPIOB,GPIO_EVENT_PIN_6); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_6); //Tar in status från plattan som ska nuddas av metallstaven
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
				vinkel = gpio_input_bit_get(GPIOB,GPIO_EVENT_PIN_7); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_7); //Tar in status från plattan som ska nuddas av metallstaven
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
				vinkel = gpio_input_bit_get(GPIOB,GPIO_EVENT_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
				} while (vinkel == 0);T1setPWMch2(0);
						T1setPWMch3(0);break; //checkar om den är förändrad dvs om pinnen är framme
				} else {
					do{
						T1setPWMch3((5 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
						T1setPWMch2(0);
						vinkel = gpio_input_bit_get(GPIOB,GPIO_PIN_8); //Tar in status från plattan som ska nuddas av metallstaven
						} while (vinkel == 0); //checkar om den är förändrad dvs om pinnen är framme
						T1setPWMch2(0);
						T1setPWMch3(0);
						} break;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	}

	
}


int main(){
	
	int onskatLage; //ser vilken vinkel snökanonen vill vrida sig mot
	uint32_t lage; //tittar checkar läget
	int toggle = 0;//bestämmer hur själva motorn ska snurra
	int grader = 0; //tar emot vinddatans tal
	int bGrader = 0; //en checkup för om motorn behöver vrida på sig
	int minne = 0;


	/*användbara funktioner för motorn*/
	//gpio_init(GPIOX, GPIO_MODE_OUT_X, GPIO_OSPEED_XMHZ, GPIO_PIN_X);
	//gpio_output_bit_get(GPIOX,X(nummer))
	//gpio_input_bit_get(GPIOX,X(nummer));
	//gpio_bit_set()
	//gpio_bit_reset(GPIOX,X(nummer));
	//rcu_periph_clock_enable(RCU_GPIOB);
	//T1setPWMmotorB();
	/*---------------------------------------------------------------*/

	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);

	//Pinben A6 och A7 är upptagna//

	/*inputs*/

	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_11); //Dessa initierar bara pinben för olika riktningarna //0 och 360 grader grader B11
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5); // A5 = 45 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8); ///B8 = 90 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_9); //B9 = 135 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5); //B5 = 180 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_6); //B4 = 225 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_7); //A8 = 270 grader
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8); //B12 = 315 grader

	
	T1powerUpInitPWM(0x0C); //gör A2 och A3 på mcun är PWM outputsen som man ändra på styrkan 0%-100%. Det är outputsen till själva h-bryggan som ska driva motorn
	

	do{
		T1setPWMch3((15 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
		T1setPWMch2(0);
		
		lage = gpio_input_bit_get(GPIOB,GPIO_PIN_11); //Tar in status från plattan som ska nuddas av metallstaven

	
	} while (lage == 0); //checkar om den är förändrad dvs om pinnen är där 

	T1setPWMch3(0);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
	T1setPWMch2(0);
			
	while(1){

		grader = 1;//Sätt att ta in vinddatans siffor;//

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

			if(grader == 360){
				onskatLage = 1;
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
	}
}

