#include <stm32f031x6.h>
#include "display.h"
#include "musical_notes.h"
#include "sound.h"

#define startx 50
#define starty 120
#define MAX_ATTEMPTS 3
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void moveDefender();
int isColliding(uint16_t playerX, uint16_t playerY,uint16_t defenderX,uint16_t defenderY);
void displayStats();
void clearStats();
void resetGame();
void greenOn();
void greenOff();
void redOn();
void redOff();
void playGoalSound();

volatile uint32_t milliseconds;

const uint16_t football_player1[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2047,2047,2047,2047,0,0,0,0,0,0,0,0,0,0,0,2047,2047,2047,2047,2047,2047,0,0,0,0,0,0,0,0,0,2047,50712,50712,50712,50712,50712,50712,2047,0,0,0,0,0,0,0,0,2047,50712,0,50712,50712,0,50712,2047,0,0,0,0,0,0,0,0,0,50712,50712,50712,50712,50712,50712,0,0,0,0,0,0,0,0,0,0,50712,50712,65535,65535,50712,50712,0,0,0,0,0,0,0,0,0,0,0,50712,50712,50712,50712,0,0,0,0,0,0,0,0,0,0,1040,1040,1040,1040,1040,1040,1040,1040,0,0,0,0,0,0,0,0,1040,1040,1040,1040,1040,1040,1040,1040,0,0,0,0,0,0,0,0,1040,0,1040,1040,1040,1040,0,1040,0,0,0,0,0,0,0,0,1040,0,1040,1040,1040,1040,0,1040,0,0,0,0,0,0,0,0,0,0,1040,1040,1040,1040,0,0,0,0,50712,50712,0,0,0,0,0,0,1040,0,0,1040,0,0,0,50712,50712,50712,50712,0,0,0,0,0,50712,0,0,50712,0,0,0,50712,50712,50712,50712,0,0,0,0,50712,50712,0,0,50712,50712,0,0,0,50712,50712,0,0,
};
const uint16_t deco2[]= 
    {
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
    };
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,57344,8456,8456,8456,57344,264,0,0,0,0,0,0,0,0,8192,8456,8456,57344,264,264,57344,8456,57344,8192,0,0,0,0,0,0,8192,264,264,264,264,264,264,264,264,57344,0,0,0,0,0,0,57344,57608,264,264,57608,264,264,264,57608,57344,0,0,0,0,0,0,57344,57608,57608,264,264,264,57608,264,57344,0,0,0,0,0,264,57608,264,57608,264,264,264,264,264,57608,264,264,57608,264,0,0,57344,49416,49416,49416,49416,49416,49416,49416,49416,49416,49416,49416,264,264,0,0,57344,264,264,49416,49416,49416,49416,49416,49416,49416,264,264,264,264,0,0,57344,57608,57608,264,49416,49416,49416,49416,49416,49416,264,264,57608,264,0,0,264,57608,57608,264,49416,49416,49416,49416,49416,49416,264,57608,57608,264,0,0,0,0,49152,49416,49416,49416,49416,49416,49416,49416,264,57344,0,0,0,0,0,0,49152,49416,49416,264,57608,57608,264,49416,264,57344,0,0,0,0,0,57344,57608,264,264,57608,57344,57344,57608,264,57608,57608,57608,57608,57608,0,57344,57608,264,264,264,57608,57344,57344,57608,264,264,264,264,264,264,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t FootballGoal[]=
{
    792,50473,18762,35674,11363,60250,52058,43866,43866,43866,43866,43866,43866,43866,43866,43866,52058,43866,43866,43866,52058,60250,60250,43602,34617,58136,16648,0,0,0,44395,20612,29596,46245,13477,5020,45716,53908,45980,5549,30389,38581,46509,13477,53908,12948,45716,37788,5285,54172,29596,21404,54172,13477,53908,28011,1585,264,0,0,53380,44659,11098,2114,42281,50209,25633,25633,58401,9513,25897,25897,17705,58401,17441,58136,33825,25897,34089,50209,1057,58136,1321,18762,28275,3699,42809,33032,0,0,53115,20083,60250,18762,50737,17705,50209,50209,1321,58401,42017,42017,50209,58401,17441,25368,9249,50473,34353,50473,33825,33825,42281,35146,20083,36203,18233,24840,0,0,20347,28275,11363,35146,58929,17705,58401,42281,42545,10041,10041,10041,26425,42809,58929,42281,1585,26425,18498,59193,34617,59193,10570,35939,4228,3699,18233,24840,0,0,61307,28011,59458,1321,8720,16648,24840,16912,8984,33560,8984,57872,17176,58136,33560,25104,25104,57872,57872,49680,792,9249,42281,43338,52851,11891,51001,33032,0,0,36731,28275,19555,35146,1849,42281,34089,26161,1849,58929,17969,1585,50737,51001,34617,26161,34353,26425,59193,26425,10041,51001,2378,35939,36996,3963,2114,33032,0,0,61043,11891,3435,52058,10834,26690,18233,10041,18233,42545,9777,17969,1849,51001,26425,50737,34617,59458,2378,51001,50737,50737,34617,27482,61572,28804,2378,49416,0,0,12155,19555,51266,26161,9513,25633,41752,17176,17176,792,792,25368,1057,17441,49944,792,17441,50473,58665,33825,8984,8984,58136,26690,20347,4228,18762,57608,0,0,61043,19819,59986,18762,2114,18233,34353,50737,18233,26425,26425,42809,26690,59458,26690,51001,34882,51530,2642,59458,26425,1849,59193,27747,21140,61836,2642,528,0,0,12155,27747,51266,26161,1321,9249,17176,1057,1321,58401,25633,17441,42017,42017,58136,8984,49944,50209,58401,58136,57872,25368,34089,19290,61836,37524,19290,57872,0,0,61043,28011,19290,43338,10306,58929,1585,34353,42809,51001,42809,26425,18233,26161,58401,9249,17705,26425,18498,42809,42545,34353,10306,60515,29332,5020,52323,9513,32768,0,36467,19819,19290,35146,42809,17969,25897,1585,10041,34617,51001,51001,26425,26161,58401,25633,58665,59193,51266,26690,34617,42809,43074,60250,61307,44923,10834,58136,40960,40960,36467,36203,60250,2642,34617,1321,25368,58136,17705,34089,34089,42281,50473,42281,9513,58401,25897,17969,1585,34089,1321,42281,34353,2642,28539,4492,60515,58929,57872,41224,44659,61043,52851,19819,19026,1849,25897,1585,34617,51001,26425,1849,58929,42545,9777,42281,50473,17969,9777,42281,9513,42281,42545,35410,44923,20612,35674,58665,33560,49944,44659,11363,10306,50209,16912,8456,8456,16912,33560,41752,792,25104,16912,16912,49416,264,24840,16912,25104,57608,16648,33032,41488,26425,11891,45188,27747,58929,25633,50209,
};

const uint16_t defender[] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63488,63488,63488,63488,0,0,0,0,0,0,0,0,0,0,0,63488,63488,63488,63488,63488,63488,0,0,0,0,0,0,0,0,0,63488,50712,50712,50712,50712,50712,50712,63488,0,0,0,0,0,0,0,0,63488,50712,0,50712,50712,0,50712,63488,0,0,0,0,0,0,0,0,0,50712,50712,50712,50712,50712,50712,0,0,0,0,0,0,0,0,0,0,50712,50712,65535,65535,50712,50712,0,0,0,0,0,0,0,0,0,0,0,50712,50712,50712,50712,0,0,0,0,0,0,0,0,0,0,63488,63488,63488,63488,63488,63488,63488,63488,0,0,0,0,0,0,0,0,63488,63488,63488,63488,63488,63488,63488,63488,0,0,0,0,0,0,0,0,63488,0,63488,63488,63488,63488,0,63488,0,0,0,0,0,0,0,0,63488,0,63488,63488,63488,63488,0,63488,0,0,0,0,0,0,0,0,0,0,63488,63488,63488,63488,0,0,0,0,0,0,0,0,0,0,0,0,63488,0,0,63488,0,0,0,0,0,0,0,0,0,0,0,0,50712,0,0,50712,0,0,0,0,0,0,0,0,0,0,0,50712,50712,0,0,50712,50712,0,0,0,0,0,0,0,
};


uint16_t defenderX = 60;
uint16_t defenderY = 60;
int defenderDirection = 1;

int goalCount = 0;
int missCount = 0;


int main()
{
    int hinverted = 0;
    int vinverted = 0;
    int toggle = 0;
    int hmoved = 0;
    int vmoved = 0;
    uint16_t x = 50;
    uint16_t y = 120;
    uint16_t oldx = x;
    uint16_t oldy = y;
    x = startx;
    y = starty;
    initClock();
    initSysTick();
    setupIO();
    putImage(50,10,30,16,FootballGoal,0,0);
    putImage(60,60,16,16,defender,0,0);

	//prints main menu until a button is pressed
    while((GPIOB->IDR & (1 << 4)) != 0 &&
          (GPIOB->IDR & (1 << 5)) != 0 &&
          (GPIOA->IDR & (1 << 11)) != 0 &&
          (GPIOA->IDR & (1 << 8)) != 0)
            {
                
				printText("How to play",0,30,RGBToWord(0xff,0xff,0),0);
    			printText("Use the buttons to",0,40,RGBToWord(0xff,0xff,0),0);
    			printText("Move the player",0,50,RGBToWord(0xff,0xff,0),0);
    			printText("Skill the defender",0,75,RGBToWord(0xff,0xff,0),0);
    			printText("And score a goal",0,85,RGBToWord(0xff,0xff,0),0);
    			printText("Press the down",0,95,RGBToWord(0xff,0xff,0),0);
				printText("button to start",0,105,RGBToWord(0xff,0xff,0),0);
				printText("V",50,115,RGBToWord(0xff,0xff,0),0);

            }
    
    fillRectangle(0, 40, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 30, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 50, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 75, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 85, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 95, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 105, 140, 100, RGBToWord(0 ,0 ,0));
	fillRectangle(0, 115, 140, 100, RGBToWord(0 ,0 ,0));
    while(1)
    {
        hmoved = vmoved = 0;
        hinverted = vinverted = 0;
        if ((GPIOB->IDR & (1 << 4))==0) // right pressed
        {                   
            if (x < 110)
            {
                x = x + 2;
                hmoved = 2;
                hinverted=0;
            }                       
        }
        if ((GPIOB->IDR & (1 << 5))==0) // left pressed
        {           
            
            if (x > 10)
            {
                x = x - 2;
                hmoved = 2;
                hinverted=1;
            }           
        }
        if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
        {
            if (y < 140)
            {
                y = y + 2;          
                vmoved = 2;
                vinverted = 0;
            }
        }
        if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
        {           
            if (y > 16)
            {
                y = y - 2;
                vmoved = 2;
                vinverted = 1;
            }
        }
        if ((vmoved) || (hmoved))
        {
            // only redraw if there has been some movement (reduces flicker)
            fillRectangle(oldx,oldy,16,16,0);
            oldx = x;
            oldy = y;               
            if (hmoved)
            {
                if (toggle)
                    putImage(x,y,16,16,football_player1,hinverted,0);
                else
                    putImage(x,y,16,16,football_player1,hinverted,0);
                
                toggle = toggle ^ 1;
            }
            else
            {
                putImage(x,y,16,16,football_player1,0,vinverted);
            }
            // Now check for an overlap by checking to see if ANY of the 4 corners of goal are within the target area
            if (isInside(50,10,16,16,x,y) || isInside(50,10,16,16,x+12,y) || isInside(50,10,16,16,x,y+16) || isInside(50,10,16,16,x+12,y+16) )
            {
				//prints goal if player collides with the goal
                printTextX2("goal!", 30, 40, RGBToWord(0xff,0xff,0), 0);
				playGoalSound();
				greenOn();


                delay(250);
				greenOff();

                fillRectangle(30, 40, 55, 16, RGBToWord(0, 0, 0));


                
                goalCount++;  // Increment goals counter
                displayStats(); // Update the stats display

                delay(500);

                    
              

                fillRectangle(x, y, 16, 16, 0);

                // Reset player to starting position
                x = 50;
                y = 120;
                oldx = x;
                oldy = y;

                // Draw player at the starting position
                putImage(x, y, 16, 16, football_player1, 0, 0);

                
                
            }
			//function to update miss counter if the player collides with the defender 
            if(isColliding(x,y,defenderX,defenderY))
            {   
				
				redOn();
				delay(250);
				redOff();
                if (missCount < MAX_ATTEMPTS) {
                    missCount++; // Increment misses counter
                    displayStats(); // Update the stats display
                    delay(500); // Short delay to show updated score
                }

                if (missCount == MAX_ATTEMPTS) {
					printText("Game Over!", 30, 40, RGBToWord(0xff,0xff,0), 0);
					printText("Try Again", 30, 50, RGBToWord(0xff,0xff,0), 0);
					
					delay(1000);
					fillRectangle(30, 50, 120, 100, RGBToWord(0 ,0 ,0));
					fillRectangle(30, 40, 120, 100, RGBToWord(0 ,0 ,0));

                    resetGame(); // Reset game after max attempts
                    }                


                fillRectangle(x,y,16,16,0);
                x = 50;
                y = 120;
                oldx = x;
                oldy = y;

                putImage(x,y,16,16,football_player1,0,0);
            }
        }

        moveDefender();

        delay(50);
    }
    return 0;
}
void initSysTick(void)
{
    SysTick->LOAD = 48000;
    SysTick->CTRL = 7;
    SysTick->VAL = 10;
    __asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
    milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while((RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
                
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
    uint32_t end_time = dly + milliseconds;
    while(milliseconds != end_time)
        __asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
    Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
    Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
    /*
    */
    uint32_t mode_value = Port->MODER;
    Mode = Mode << (2 * BitNumber);
    mode_value = mode_value & ~(3u << (BitNumber * 2));
    mode_value = mode_value | Mode;
    Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
    // checks to see if point px,py is within the rectange defined by x,y,w,h
    uint16_t x2,y2;
    x2 = x1+w;
    y2 = y1+h;
    int rvalue = 0;
    if ( (px >= x1) && (px <= x2))
    {
        // ok, x constraint met
        if ( (py >= y1) && (py <= y2))
            rvalue = 1;
    }
    return rvalue;
}

void setupIO()
{
    RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
    display_begin();
    pinMode(GPIOB,4,0);
    pinMode(GPIOB,5,0);
    pinMode(GPIOA,8,0);
    pinMode(GPIOA,11,0);
	pinMode(GPIOA,0,1);
	pinMode(GPIOA,1,1);
    enablePullUp(GPIOB,4);
    enablePullUp(GPIOB,5);
    enablePullUp(GPIOA,11);
    enablePullUp(GPIOA,8);
}

void moveDefender()
{
    uint16_t x1;
    uint16_t x2;
    
    //clears screen
    fillRectangle(defenderX,defenderY,16,16,0);

    defenderX += defenderDirection;

    //edge positions off the screen
    x1 = 110;
    x2 = 10;

    //if the defenders position in greater than or equal to the right edge of the screen
    if(defenderX >= x1)
    {
        //the defender moves left across to the other side of the screen
        defenderDirection = -3;
    }

    //if the defenders position in greater than or equal to the left edge of the screen
    else if(defenderX <=x2)
    {
        //the defender moves left across to the other side of the screen
        defenderDirection = 3;
    }
    
    //puts defender image onto the screen
    putImage(defenderX,defenderY,16,16,defender,0,0);

}

int isColliding(uint16_t playerX, uint16_t playerY,uint16_t defenderX,uint16_t defenderY)
{
    uint16_t height = 16;
    uint16_t width = 16;
	
	//if player is inside defender zone
    if(isInside(defenderX,defenderY,width,height,playerX,playerY) ||
       isInside(defenderX,defenderY,width,height,playerX + width,playerY) ||
       isInside(defenderX,defenderY,width,height,playerX,playerY + height) ||
       isInside(defenderX,defenderY,width,height,playerX + width,playerY + height))
    {
		
        return 1;
    }
    return 0;
}

void displayStats() {
    // Clear the display area for the stats to avoid overlapping text
    clearStats();

    // Display Goals count
    char goalText[20];
    sprintf(goalText,"G:%d",goalCount);
    printText(goalText, 5, 15, RGBToWord(0xff, 0xff, 0xff), 0); // White color
    
    // Display Misses count
    char missText[20];
    sprintf(missText, "M:%d", missCount);
    printText(missText, 5,5, RGBToWord(0xff, 0xff, 0xff), 0); // White color
}

// Function to clear the stats display area
void clearStats() {
    // Clear the display area (assuming black background)
    fillRectangle(50, 50, 160, 10, RGBToWord(0, 0, 0)); // Clears area for stats
}

void resetGame() {
    goalCount = 0; // Reset goals counter
    missCount = 0; // Reset misses counter
    clearStats();  // Clear the display area
    displayStats(); // Display updated stats
}
//turns the green LED on 
void greenOn()
{
	GPIOA->ODR |= (1 << 0);
}
//turns the green LED off
void greenOff()
{
	GPIOA->ODR &=~ (1 << 0);
}
//turns the red LED on
void redOn()
{
	GPIOA->ODR |= (1 << 1);
}
//turns the red LED off
void redOff()
{
	GPIOA->ODR &=~ (1 << 1);
}
void playGoalSound() {
    // Play a series of higher-pitched notes for a goal (C5, E5, G5, C6)
    playNote(C5); // C5 = 523 Hz
    for (int i = 0; i < 1000000; i++) {} 
    
    playNote(E5); // E5 = 659 Hz
    for (int i = 0; i < 1000000; i++) {}
    
    playNote(G5); // G5 = 784 Hz
    for (int i = 0; i < 1000000; i++) {}
    
    playNote(C6); // C6 = 1047 Hz
    for (int i = 0; i < 1000000; i++) {}
}