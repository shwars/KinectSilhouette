#include <Arduino.h>

// Connections    
#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

#define MAXDISPLAYS 4

const bool transpose = true;

unsigned char ScreenBuffer[32*MAXDISPLAYS]; // Screen buffer memory
unsigned char* DisplayBuffer; // Buffer being displayed on screen
unsigned char* CurrentBuffer; // Buffer being manipulated by routines

unsigned char* GetBuf() { return ScreenBuffer; }

void InitLEDMatrix()
{
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);
  for (int i=0;i<32*MAXDISPLAYS;i++)
  {
    ScreenBuffer[i]=0;
  }
  SetDisplay(0);
}

void ClearDisplay()
{
    for(int i=0;i<32;i++) CurrentBuffer[i] = 0;  
}

void SetPixel(byte x, byte y, bool onoff)
{
  byte no,m;
  if (transpose)
  {
    no = (15-x)*2+y/8;
    m = getMask(7-y%8);
  }
  else
  {
    no = y*2+x/8;
    m = getMask(7-x%8);    
  }
  if (onoff) CurrentBuffer[no] |= m;
  else CurrentBuffer[no] &= ~m;
}

void Fill(byte x, byte y, byte w, byte h, bool onoff)
{
  for (byte i=0; i<w; i++)
     for (byte j=0;j<h; j++)
        SetPixel(x+i,y+j,onoff);
}

void BitLine(byte x, byte y, int w, unsigned int l)
{
   for (byte i=w;i>0;i--)
   {
      SetPixel(x+i-1,y,l%2);
      l>>=1;
   }
}

void DisplaySprite8(int x, int y, int w, int h, const byte* buf)
{
  for (byte i=0;i<h;i++)
  {
     BitLine(x,y+i,w,buf[i]);
  }
}

void DisplaySprite16(int x, int y, int w, int h, const unsigned int* buf)
{
  for (byte i=0;i<h;i++)
  {
     BitLine(x,y+i,w,buf[i]);
  }
}


byte getMask(byte n)
{
  byte res = 1;
  for (byte i=0;i<n;i++) res = res << 1;
  return res;
}

void Refresh()         
{
  unsigned char i;
  unsigned char db0, db1;
  for( i = 0 ; i < 16 ; i+=1 )
  {
    digitalWrite(LEDARRAY_G, HIGH);
    db0 = ~DisplayBuffer[2*i];    
    db1 = ~DisplayBuffer[2*i+1];
    Send(db1);
    Send(db0);
    digitalWrite(LEDARRAY_LAT, HIGH);   
    delayMicroseconds(1);
    digitalWrite(LEDARRAY_LAT, LOW);
    delayMicroseconds(1);
    Scan_Line(i);
    digitalWrite(LEDARRAY_G, LOW);
    delayMicroseconds(500);;
  } 
}

void Scan_Line( unsigned char m)
{ 
  switch(m)
  {
    case 0:     
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);          
      break;
    case 1:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 2:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 3:         
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 4:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 5:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    case 6:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 7:
      digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 8:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 9:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
      break;  
    case 10:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 11:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 12:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);    
      break;
    case 13:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
      break;
    case 14:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
      break;
    case 15:
      digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
      break;
    default : break;  
  }
}

void Send(unsigned char dat)
{
  unsigned char i;
  digitalWrite(LEDARRAY_CLK, LOW);
  delayMicroseconds(1);;  
  digitalWrite(LEDARRAY_LAT, LOW);
  delayMicroseconds(1);;
  for( i = 0 ; i < 8 ; i++ )
  {
    if( dat&0x01 )
    {
      digitalWrite(LEDARRAY_DI, HIGH);  
    }
    else
    {
      digitalWrite(LEDARRAY_DI, LOW);
    }
    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(LEDARRAY_CLK, LOW);
    delayMicroseconds(1);   
    dat >>= 1;  
  }     
}

void SetCurrentBuffer(byte no)
{
  CurrentBuffer = ScreenBuffer+no*32;
}

void SetDisplayBuffer(byte no)
{
  DisplayBuffer = ScreenBuffer+no*32;
}

void SetDisplay(byte no)
{
   SetCurrentBuffer(no);
   SetDisplayBuffer(no);
}

char scroll_dir = 0;
byte scroll_left = 0;
byte scroll_delay = 0;

void StartScroll(char dir)
{
    if (scroll_left==0 && scroll_dir==0)
    {
      scroll_left = 16;
      scroll_dir = dir;
    }
}

bool isScrolling()
{
  return !(scroll_left==0 && scroll_dir==0);
}

void LEDloop()
{
    Refresh();
    if (scroll_left!=0)
    {
      if (scroll_delay==0)
      {
         Serial.println(scroll_dir);
         scroll_left--;
         DisplayBuffer+=scroll_dir*2;
         scroll_delay=0;
      }
      else
      {
         scroll_delay--;
      }
    }
    else
    {
      scroll_dir = scroll_delay = 0;
    }
}

