#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"
#include "pwm.h"
#define EI 1
#define DI 0

void rotera(int pinMal){
	
	int vinkel = 5;

	do{
		T1setPWMch1((30 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
		T1setPWMch2(0);
		vinkel = gpio_input_bit_get(GPIOB,pinMal); //Tar in status från plattan som ska nuddas av metallstaven
	} while (vinkel != 0); //checkar om den är förändrad dvs om pinnen är där 

}


int main(){
	
	int onskatLage; //ser vilken vinkel snökanonen vill vrida sig mot
	uint32_t lage; //tittar checkar läget
	int toggle = 0;//bestämmer hur själva motorn ska snurra
	int grader = 0; //tar emot vinddatans tal
	int bGrader = 0; //en checkup för om motorn behöver vrida på sig


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

	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_1); //Dessa initerar bara pinben för olika riktningarna
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_5); 
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_8); //möjligtvis för att ta emot vinddata behöver hjälp
	/*outputs*/

	T1powerUpInitPWM(0x6); //gör A1 och A2 på mcun är PWM outputsen som man ändra på styrkan 0%-100%. Det är outputsen till själva h-bryggan som ska driva motorn
	

	do{
		T1setPWMch1((30 / 100.0) * 16000);  // Den sätter in movement till ch1 alltså A1 och sätter ch2 alltså A2 till 0 så den snurrar åt en riktning
		T1setPWMch2(0);
		lage = gpio_input_bit_get(GPIOB,1); //Tar in status från plattan som ska nuddas av metallstaven
	} while (lage != 0); //checkar om den är förändrad dvs om pinnen är där 
			
	while(1){

		grader = 1/*Sätt att ta in vinddatans siffor*/;

		if(grader != bGrader){

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

			//här är en switch för vilken platta den ska rotera till (siffrorna som skickas in är bara placeholders för nu)

			switch(onskatLage){

				case 1: rotera(1);break;
				case 2: rotera(2);break;
				case 3: rotera(3);break;
				case 4: rotera(4);break;
				case 5: rotera(5);break;
				case 6: rotera(6);break;
				case 7: rotera(7);break;
				case 8: rotera(8);break;


			}
		}
	}
}

