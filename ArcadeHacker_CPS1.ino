// CPS1 Desuicider 1.07 by arcadehacker.blogspot.com
/*
 *
 * Shield ISP Header Pinout
 * -----------------------------
 *   (2) (4) (6)    1- MISO; 2- Vcc;   3- SCK
 *   (1) (3) (5)    4- MOSI; 5- Reset; 6- GND
 *
 * Shield Extra I/O Pinout
 * ---------------------------
 *  (13) (12) (11) (3) (2) (1) (0)
 *
 */

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//  CPS1 C Board interface pins
#define PIN61       2   //Clock #2.   Connect programmer pin 3 (LCD Extra I/O Pin# 2) to CNB pin 61
#define PIN62       3   //Data.       Connect programmer pin 4 (LCD Extra I/O Pin# 3) to CNB pin 62
#define PIN42       11  //Clock #1.   Connect programmer pin 5 (LCD Extra I/O Pin# 11) to CNA pin 19
#define PIN44       12  //Setup.      Connect programmer pin 6 (LCD Extra I/O Pin# 12) to CNA pin 20


int lcd_key       = 0;
int adc_key_in    = 0;
int adc_key_prev  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// delay amount
int time = 25;

unsigned char bytes[][18] = {

{ 0x00, 0x08, 0x00, 0xC0, 0x2E, 0xC1, 0x60, 0x54, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x98, 0x00, 0x40 },  // 01
{ 0x08, 0x0A, 0x00, 0x88, 0x24, 0xC1, 0x30, 0x50, 0x00, 0x00, 0x02, 0xA0, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x40 },  // 02
{ 0x00, 0x10, 0x00, 0x98, 0x28, 0xC1, 0x50, 0x58, 0x00, 0x00, 0x02, 0xE0, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x40 },  // 03
{ 0x10, 0x0A, 0x00, 0xA8, 0x32, 0xC1, 0x40, 0x60, 0x00, 0x00, 0x02, 0x60, 0x00, 0x00, 0x00, 0xB8, 0x00, 0x40 },  // 04
{ 0x14, 0x0E, 0x00, 0xC8, 0x31, 0x41, 0x70, 0x58, 0x00, 0x00, 0x02, 0xA0, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x40 },  // 05
{ 0x04, 0x13, 0x20, 0xC0, 0x2F, 0x41, 0x60, 0x54, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x98, 0x00, 0xC1 },  // 11
{ 0x08, 0x0A, 0x00, 0x88, 0x24, 0xC1, 0x30, 0x50, 0x00, 0x00, 0x02, 0xA0, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x41 },  // 12
{ 0x0C, 0x06, 0xE0, 0xB0, 0x2A, 0x81, 0x40, 0x4C, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x88, 0x01, 0x41 },  // 13
{ 0x10, 0x15, 0xE0, 0x70, 0x1B, 0x00, 0xC0, 0x2C, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x48, 0x00, 0xC1 },  // 14
{ 0x14, 0x04, 0xE0, 0x30, 0x0B, 0x40, 0x40, 0x0C, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x08, 0x00, 0x81 },  // 15
{ 0x1B, 0xEC, 0x1F, 0x0F, 0xC4, 0xFE, 0x3F, 0x90, 0x00, 0x00, 0x7C, 0xA0, 0x00, 0x1F, 0x1F, 0x37, 0xFF, 0x01 },  // 16
{ 0x1C, 0x10, 0x80, 0x28, 0x0C, 0x40, 0x70, 0x20, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x50, 0x00, 0xC1 },  // 17
{ 0x20, 0x0D, 0x00, 0x48, 0x14, 0x40, 0xB0, 0x30, 0x00, 0x00, 0x01, 0xA0, 0x00, 0x00, 0x00, 0x70, 0x01, 0x01 },  // 18
{ 0x00, 0x0B, 0x21, 0xC0, 0xAE, 0xC7, 0x60, 0x54, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x98, 0x00, 0x40 }   // 21

};

char* GameList[] = 
  { 
  "CPS-B-01        ",
  "CPS-B-02        ",
  "CPS-B-03        ",
  "CPS-B-04        ",
  "CPS-B-05        ",
  "CPS-B-11        ",
  "CPS-B-12        ",
  "CPS-B-13        ",
  "CPS-B-14        ",
  "CPS-B-15        ",
  "CPS-B-16        ",
  "CPS-B-17        ",
  "CPS-B-18        ",
  "CPS-B-21        "
  };
  
unsigned char mask = 1; // Bit 
unsigned char bits[8];
int c = -1;

void setup() {
  

  pinMode(PIN42, OUTPUT); pinMode(PIN44, OUTPUT);
  pinMode(PIN61, OUTPUT); pinMode(PIN62, OUTPUT); 

 
   digitalWrite(PIN42, LOW);
   digitalWrite(PIN44, LOW);

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("CPS1 Desuicide");
  lcd.setCursor(0,1);
  lcd.print("By ArcadeHacker");
  delay(2500);
  lcd.setCursor(0,0);
  lcd.print("Select a game:  ");
  lcd.setCursor(0,1);
  lcd.print(" --press down-- ");
  
}

void PIN42CLK()
{
    digitalWrite(PIN42, HIGH); delay(time);
    digitalWrite(PIN42, LOW); delay(time);
}

void PIN61CLK()
{
    digitalWrite(PIN61, HIGH); delay(time);
    digitalWrite(PIN61, LOW); delay(time);
}

void program_unlock()
{
    digitalWrite(PIN44, HIGH);
    delay(time);
    delay(time);
    delay(time);
    PIN42CLK();
    PIN42CLK();
    PIN42CLK();
    PIN42CLK();  
 
    digitalWrite(PIN62, HIGH);
    PIN61CLK();
    digitalWrite(PIN62, LOW);
    PIN61CLK();
    digitalWrite(PIN62, HIGH);
    PIN61CLK();
    digitalWrite(PIN62, LOW);
    PIN61CLK();
    digitalWrite(PIN62, HIGH);
    PIN61CLK();
    PIN61CLK();
    
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    PIN61CLK();
    
   
} 

void ProgramCPS1(int prg)
                {
                int i;
                int test;
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Unlocking...    ");                  

               
                program_unlock();      
                

   lcd.setCursor(0,0);   
   lcd.print("Programming     ");
   lcd.setCursor(0,1);
   lcd.print("CPS1...         ");    

                               
                for(i=0; i<18; i++)        
                  {      
                    for (int b = 7; b > -1; b--) {  
               
                     bits[b] = (bytes[prg][i] & (mask << b)) != 0;
                     printf("%d",bits[b]);
       
                     if (bits[b] == 1) {
                      digitalWrite(PIN62, HIGH);}
                      else { 
                      digitalWrite(PIN62, LOW);}                  
                      PIN61CLK();  
                     }
                  }
                  
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("Done!           ");                   
 delay(1000);       

//pinMode(PIN42, INPUT); pinMode(PIN44, INPUT);
//pinMode(PIN61, INPUT); pinMode(PIN62, INPUT); 

while (analogRead (0) != 638) { 
                 lcd.setCursor(0,0); lcd.print("--disconnect and ");   lcd.setCursor(0,1); lcd.print("insert pcb--");  }
                }

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);   
  delay(5);
  int k = (analogRead(0) - adc_key_in); 
  if (5 < abs(k)) return btnNONE;  

  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 195)  return btnUP; 
  if (adc_key_in < 380)  return btnDOWN; 
  if (adc_key_in < 555)  return btnLEFT; 
  if (adc_key_in < 790)  return btnSELECT;   
  return btnNONE;  
}                

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{

  adc_key_prev = lcd_key ;       // Looking for changes
  lcd_key = read_LCD_buttons();  // read the buttons

  if (adc_key_prev != lcd_key)
  {

    lcd.setCursor(0,1);            // move to the begining of the second line
  
    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
    case btnRIGHT:
      {
        break;
      }
    case btnLEFT:
      {
        break;
      }
    case btnUP:
      {
       if (c !=0)
        {
          if (c > 0) {c--;} else {c++;}
        }
      
       lcd.print(GameList[c]);
       break;
      }
    case btnDOWN:
      {
     if (c != sizeof(GameList)/sizeof(GameList[0])-1) {c++;}
     lcd.print(GameList[c]);
        break;
      }
    case btnSELECT:
      {
       if (c != -1) 
         {
           ProgramCPS1(c);                
           lcd.setCursor(0,0);
           lcd.print("Select a game:  ");
           lcd.setCursor(0,1);
           lcd.print(GameList[c]);
         }
        break;
      }
    case btnNONE:
      {
        break;
      }
    }
  }
}




