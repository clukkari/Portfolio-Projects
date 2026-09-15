/* 
Variable Music Box for MGMT's Kids
Aydin Ally
Connor Lukkari
*/

// Global Variables
const unsigned char MSG0[21] = "MGMT: KIDS ";
const unsigned char MSG1[21] = "Intro              ";
const unsigned char MSG2[21] = "Verse              ";
const unsigned char MSG3[21] = "Chorus             ";
const unsigned char MSG4[21] = "Instrumental       ";


const unsigned int A4 = 11363;
const unsigned int B4 = 10124;
const unsigned int Cs5 = 9019;
const unsigned int E5 = 7584;
const unsigned int Fs5 = 6757;
const unsigned int Gs5 = 6020;
const unsigned int Fs4 = 13514;
const unsigned int Gs4 = 12039;
const unsigned int D5 = 8513;

const unsigned int A3 = 22727;      
const unsigned int E4 = 15151;
const unsigned int B3 = 20247;
const unsigned int E3 = 30303;
const unsigned int Cs4 = 18038;
const unsigned int D4 = 17059;
const unsigned int Ds5 = 8005;

volatile unsigned int N0, DELAY, PLAY;
volatile unsigned int TEMPO_PCT = 100;

#include <pic18.h>
#include "lcd_portd.c"

unsigned int ADC_Read_AN0(void)
{
  unsigned int res;
  unsigned char i;
  ADCON0 = 0x01; // CHS=0, ADON=1
  for (i = 0; i < 6; i++); // small acquisition delay
  GODONE = 1;
  while (GODONE);
  res = ((unsigned int)ADRESH << 8) | ADRESL;
  return res & 0x03FF; // 10-bit result
}

// ---------- Tempo scaling ----------
unsigned int scale_delay_by_pot(unsigned int orig_ms)
{
  unsigned int adc = ADC_Read_AN0();
  unsigned long pct;

  if (adc <= 511) {
    pct = 50UL + ((unsigned long)adc * 50UL + 255UL) / 511UL;
  } else {
    unsigned int hi = adc - 512;
    pct = 100UL + ((unsigned long)hi * 100UL + 255UL) / 511UL;
  }

  TEMPO_PCT = (unsigned int)(10000UL / pct);

  unsigned long scaled = (unsigned long)orig_ms * pct / 100UL;
  if (scaled == 0) scaled = 1;
  return (unsigned int)scaled;
}

void UpdateTempoDisplay(void)
{
  unsigned int adc = ADC_Read_AN0();
  unsigned long pct;

  if (adc <= 511) {
    pct = 50UL + ((unsigned long)adc * 50UL + 255UL) / 511UL;
  } else {
    unsigned int hi = adc - 512;
    pct = 100UL + ((unsigned long)hi * 100UL + 255UL) / 511UL;
  }

  TEMPO_PCT = (unsigned int)(10000UL / pct);
}

void ShowTempo(void)
{
  LCD_Move(0,11);
  LCD_Out(TEMPO_PCT, 3, 0);
  LCD_Write('%');
}

// Interrupts
void interrupt IntServe(void)
{
  if (TMR0IF) {
    TMR0 = -N0 + 40;
    if (PLAY) {
      if (DELAY > 50) RC0 = !RC0;
      else RC0 = 0;
    } else RC0 = 0;
    TMR0IF = 0;
  }
  if (TMR2IF) {
    RC1 = !RC1;
    if (DELAY) DELAY -= 1;
    TMR2IF = 0;
  }
}

// ---------- Note player ----------
void PlayNote(unsigned int N, unsigned int D)
{
  unsigned int Dscaled;
  if (N == 0) {
    PLAY = 0;
    Dscaled = scale_delay_by_pot(D);
    DELAY = Dscaled;
    while (DELAY) {
      UpdateTempoDisplay();
      ShowTempo();
    }
  } else {
    PLAY = 1;
    N0 = N;
    Dscaled = scale_delay_by_pot(D);
    DELAY = Dscaled;
    while (DELAY) {
      UpdateTempoDisplay();
      ShowTempo();
    }
  }
}

// ---------- Songs ----------
void PlaySong_RB0(void)
{

//Intro

  PlayNote(A4,200); PlayNote(0,200);
  PlayNote(B4,200); PlayNote(0,200);
  PlayNote(Cs5,200); PlayNote(0,200);
  PlayNote(E5,200); PlayNote(0,200);

  PlayNote(Fs5,400); PlayNote(Gs5,200);
  PlayNote(Fs5,200); PlayNote(0,200);
  PlayNote(E5,200);  PlayNote(0,400);
  PlayNote(Cs5,1800);

  PlayNote(B4,1600);

  PlayNote(A4,200); PlayNote(0,200);
  PlayNote(B4,200); PlayNote(0,200);
  PlayNote(Cs5,200); PlayNote(0,200);
  PlayNote(B4,200); PlayNote(0,200);

  PlayNote(A4,400); PlayNote(B4,200);
  PlayNote(A4,200); PlayNote(0,200);
  PlayNote(Fs4,200); PlayNote(0,200);
  PlayNote(A4,1800);

  PlayNote(D5,200); PlayNote(Cs5,200);
  PlayNote(B4,200); PlayNote(A4,400);
  PlayNote(Cs5,200); PlayNote(A4,200);
  PlayNote(Gs4,200);
}

void PlaySong_RB1(void)
{

// Verse

//Frame 1
PlayNote(0, 800);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(A3, 400);


//Frame 2      
PlayNote(0, 600);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs5, 200);

 //Frame 3
PlayNote(E4, 800);
PlayNote(B3, 300);
PlayNote(A3, 100);
PlayNote(A3, 400);

//Frame 4
PlayNote(0, 1600);

//Frame 5
PlayNote(0, 400);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 400);

//Frame 6
PlayNote(A3, 400);
PlayNote(0, 1200);

//Frame 7
PlayNote(0, 400);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 400);

//Frame 8
PlayNote(E3, 400);
PlayNote(0, 1200);
}

void PlaySong_RB2(void)
{

//Chourus

//Frame 1
PlayNote(0, 600);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 600);

//Frame 2
PlayNote(0, 600);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);

//Frame 3
PlayNote(E4, 800);
PlayNote(B3, 300);
PlayNote(A3, 200);
PlayNote(0, 300);

//Frame 4
PlayNote(0, 1600);

//Frame 5
PlayNote(0, 600);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 200);
PlayNote(Cs4, 600);

//Frame 6
PlayNote(E4, 800);
PlayNote(B3, 300);
PlayNote(A3, 200);
PlayNote(0, 300);

//Frame 7
PlayNote(0, 200);
PlayNote(A3, 200);
PlayNote(E4, 400);
PlayNote(B3, 300);
PlayNote(A3, 200);
PlayNote(0, 300);

//Frame 8
PlayNote(0, 1600);
}

void PlaySong_RB3(void)
{

//instrumental

//Frame 1
PlayNote(Fs4, 100);
PlayNote(A4, 100);
PlayNote(D5,100);
PlayNote(E5, 100);
PlayNote(Fs5, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);

PlayNote(B4, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);

PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);

//Frame 2
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);
PlayNote(E4, 100);
PlayNote(Gs4, 100);
PlayNote(B4, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);
PlayNote(E4, 100);
PlayNote(Gs4, 100);

//Frame 3
PlayNote(A4, 100);
PlayNote(Fs4, 100);
PlayNote(A4, 100);
PlayNote(Cs5, 100);
PlayNote(Fs5, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);

//Frame 4
PlayNote(E4, 100);
PlayNote(Gs4, 100);
PlayNote(Gs4, 100);
PlayNote(B4, 100);
PlayNote(Fs4, 100);
PlayNote(Gs4, 100);
PlayNote(A4, 100);
PlayNote(Cs5, 100);
PlayNote(Gs4, 100);
PlayNote(A4, 100);
PlayNote(Cs5, 100);
PlayNote(Ds5, 100);
PlayNote(A4, 100);
PlayNote(B4, 100);
PlayNote(Ds5, 100);
PlayNote(E5, 100);

//Frame 5
PlayNote(Cs5, 100);
PlayNote(D5, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);
PlayNote(E4, 100);

//Frame6
PlayNote(B3, 100);
PlayNote(D4, 100);
PlayNote(Fs4, 100);
PlayNote(A4, 100);
PlayNote(B4, 100);
PlayNote(Cs5, 100);
PlayNote(D5, 100);
PlayNote(E5, 100);
PlayNote(Fs5, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);

//Frame 7
PlayNote(A4, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(E5, 100);
PlayNote(D5, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(Cs5, 100);
PlayNote(B4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(A4, 100);
PlayNote(Gs4, 100);
PlayNote(Fs4, 100);

//Frame 8
PlayNote(Gs4, 1600);
}

unsigned char DebouncePress(unsigned char mask)
{
  if (!(PORTB & mask)) return 0;
  Wait_ms(20);
  return (PORTB & mask) ? 1 : 0;
}

//Main
void main(void)
{
  unsigned char i;
  unsigned char prevButtons, curButtons;

  TRISA = 0; 
  TRISB = 0xFF; 
  TRISC = 0; 
  TRISD = 0; 
  TRISE = 0;
  ADCON1 = 0x0E;        
  ADCON2 = 0b10101010;  
  ADCON0 = 0x01;        
  TRISA |= 0x01;        

  LCD_Init();
  LCD_Move(0,0); for (i=0; i<20; i++) LCD_Write(MSG0[i]);

  // Timer0
  T0CS = 0; 
  T0CON = 0x88;
  TMR0ON = 1; 
  TMR0IE = 1; 
  TMR0IP = 1; 
  PEIE = 1;

  // Timer2
  T2CON = 0x4D; 
  PR2 = 249;
  TMR2ON = 1; 
  TMR2IE = 1; 
  TMR2IP = 1; 
  PEIE = 1;

  GIE = 1;

  prevButtons = PORTB & 0x0F;

  while (1) {
    curButtons = PORTB & 0x0F;

    if ((curButtons & 0x01) && !(prevButtons & 0x01)) {
		LCD_Move(1,0); for (i=0; i<20; i++) LCD_Write(MSG1[i]);
      if (DebouncePress(0x01)) PlaySong_RB0();
    }
    if ((curButtons & 0x02) && !(prevButtons & 0x02)) {
		LCD_Move(1,0); for (i=0; i<20; i++) LCD_Write(MSG2[i]);
      if (DebouncePress(0x02)) PlaySong_RB1();
    }
    if ((curButtons & 0x04) && !(prevButtons & 0x04)) {
		LCD_Move(1,0); for (i=0; i<20; i++) LCD_Write(MSG3[i]);
      if (DebouncePress(0x04)) PlaySong_RB2();
	}
	if ((curButtons & 0x08) && !(prevButtons & 0x08)) {
		LCD_Move(1,0); for (i=0; i<20; i++) LCD_Write(MSG4[i]);
      if (DebouncePress(0x08)) PlaySong_RB3();
    }

    prevButtons = curButtons;

    UpdateTempoDisplay();
    ShowTempo();

    Wait_ms(100);
  }
}
