const unsigned int XTree[] = {
  B00000000*256+B00000000,
  B00000001*256+B10000000,
  B00000011*256+B11000000,
  B00000001*256+B10000000,
  B00000111*256+B11100000,
  B00000001*256+B10000000,
  B00001111*256+B11110000,
  B00000001*256+B10000000,
  B00011111*256+B11111000,
  B00111111*256+B11111100,
  B00000001*256+B10000000,
  B01111111*256+B11111110,
  B00000001*256+B10000000,
  B11111111*256+B11111111,
  B00000001*256+B10000000,
  B00000001*256+B10000000
};

void DrawXMas()
{
  DisplaySprite16(0,0,16,16,XTree);
}

#define NOSNOWF 10

byte snow_x[NOSNOWF], snow_y[NOSNOWF], snow_ctr[NOSNOWF], snow_spd[NOSNOWF];

void setupPics()
{
  for (byte i=0;i<NOSNOWF;i++)
  {
    snow_x[i]=random(0,16);
    snow_y[i]=random(0,16);
    snow_spd[i]=snow_ctr[i]=random(2,15);
  }
}

byte ensure16(byte x)
{
  if (x>15) return 15;
  if (x<0) return 0;
  return x;
}

void loopPics()
{
  SetCurrentBuffer(1);
  DrawXMas();
  for(byte i=0;i<NOSNOWF;i++)
  {
     SetPixel(snow_x[i],snow_y[i],true);
     if (0==snow_ctr[i]--)
     {
        snow_ctr[i]=snow_spd[i];
        snow_x[i]=ensure16(snow_x[i]+random(-1,2));
        if (++snow_y[i]==16)
        {
          snow_y[i] = 0;
        }
     }
  }
}

const unsigned int HRSL[] = {
  256*B1000111+B01110100,
  256*B1000100+B01001110,
  256*B1000110+B01100100,
  256*B1000100+B01000100,
  256*B1110111+B01000110
};

void DrawLeft()
{
  DisplaySprite16(1,9,15,5,HRSL);
}

// Fireworks 
char coord[14],spd[14],spdv[14],spdc[14];

void resetFW()
{
  for (byte i=0;i<14;i++)
  {
    coord[i]=9;
    spd[i]=1;
    spdv[i]=spdc[i]=random(2,50);
  }
}

void loopFW()
{
  SetCurrentBuffer(0);
  for (byte i=0;i<14;i++)
  {
    if (0==spdc[i]--)
    {
      spdc[i]=spdv[i];
      SetPixel(i+1,coord[i],spd[i]>0);
      coord[i]+=spd[i];
      if (coord[i]>13 || coord[i]<10) spd[i]=-spd[i];
    }
  }
}

