# 1 "main.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files (x86)/Microchip/MPLABX/v5.35/packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "main.c" 2

# 1 "./keypad.h" 1


 unsigned int kpRead(void);
    char kpReadKey(void);
 void kpDebounce(void);
 void kpInit(void);
# 2 "main.c" 2

# 1 "./config.h" 1
# 26 "./config.h"
#pragma config OSC=HS
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOREN = OFF
#pragma config BORV = 46
#pragma config WDT=OFF
#pragma config WDTPS = 1
#pragma config MCLRE=ON
#pragma config LPT1OSC = OFF
#pragma config PBADEN = ON
#pragma config CCP2MX = PORTC
#pragma config STVREN = OFF
#pragma config LVP=OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF

#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF

#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF
#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF

#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF
# 3 "main.c" 2

# 1 "./lcd.h" 1


  void lcdCommand(char value);
  void lcdChar(char value);
  void lcdString(char msg[]);
  void lcdNumber(int value);
  void lcdNumberUnit(int value);
  void lcdPosition(int line, int col);
  void lcdInit(void);
# 4 "main.c" 2

# 1 "./rgb.h" 1
# 20 "./rgb.h"
 void rgbColor(int led);
 void turnOn(int led);
 void turnOff(int led);
 void rgbInit(void);
# 5 "main.c" 2

# 1 "./ssd.h" 1
# 22 "./ssd.h"
 void ssdDigit(char val, char pos);
 void ssdUpdate(void);
 void ssdInit(void);
# 6 "main.c" 2

# 1 "./adc.h" 1
# 22 "./adc.h"
 void adcInit(void);
 int adcRead(unsigned int channel);
# 7 "main.c" 2

# 1 "./pwm.h" 1
# 23 "./pwm.h"
 void pwmSet(unsigned char porcento);
 void pwmFrequency(unsigned int freq);
 void pwmInit(void);
# 8 "main.c" 2


void configuraPSI(void);
void comandos(void);
void instrucaoLCD(void);
void infoIndividual(void);
void infoPress(void);
void infoTemp(void);
void infoGeral(void);
void ssdHeat(void);
void teste(void);

unsigned char tecla,conf=1,prt=1;
char num = 0,p[4]={30, 28, 20, 45},pC[4]={32, 32, 32, 32}, buff,
        tp[4]={60,70,72,97},b2=0,block=0,pG,tpG,tblock;
int pot,pot1=0,tol;

void main(void) {

    rgbInit();
    ssdInit();
    kpInit();
    lcdInit();
    adcInit();
    pwmInit();

    lcdString("Aperte o botao s");
    lcdPosition(1,0);
    lcdString("para configurar!");

    for(int i=0; i<70;i++){
        for(int j=0;j<10;j++){
           kpDebounce();
        if(kpReadKey() == 's'){
            configuraPSI();
            num=0;
        }
        }
    }
    conf=0;
    instrucaoLCD();

    for(;;){

        comandos();
        if(prt == 1){
            lcdCommand(0x01);
            if(num<4){
                infoIndividual();
                prt = 0;
            }
            else if(num == 4){
                infoPress();
                prt = 0;
            }
            else {
                infoTemp();
                prt = 0;
            }
            comandos();
        }
        teste();
        block=block+1;
        if(block == 100){
            ssdHeat();
            prt = 1;
            block = 0;
            if(p[1]<50 && tblock == 0 )
                buff=1;
            else{
                buff= -1;
                tblock = 1;
            }
            if(p[1]<15 && tblock == 1)
                tblock=0;

        }
    }
}

void configuraPSI(void){
    lcdCommand(0x01);

    while(conf){

        comandos();
        kpDebounce();

        if(prt == 1){
            prt = 0;
            lcdPosition(0,0);
            lcdCommand(0xC0);
            lcdString("Aperte S p/ sair");
            lcdCommand(0x80);

            lcdString("Pneu ");
            lcdNumberUnit(num+1);
            lcdString(": ");
            pC[num] = pC[num]+buff;
            lcdNumber(pC[num]);
        }

    kpDebounce();
    }
}

void instrucaoLCD(void){
    lcdCommand(0x01);
    lcdChar(0b01111111);
    lcdString("Prev.");
    lcdPosition(0,11);
    lcdString("Next");
    lcdChar(0b01111110);
    lcdCommand(0xC0);
    lcdPosition(1,2);
    lcdString("L");
    lcdPosition(1,13);
    lcdString("R");
    for(int i=0; i<500;i++){
        for(int j=0;j<300;j++){}
    }
}

void comandos(void){

pot = adcRead(0);
tol = 50-(((float)pot/1023)*100);

if(pot1 != adcRead(0)){
    lcdCommand(0x01);
    lcdString(" Temp max: ");
    lcdNumber((100*(1+((float)tol/100))));
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
    pot1 = pot;
}

kpDebounce();
if(kpReadKey() == 'U'){
    buff=1;
    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
}
else if(kpReadKey() == 'D'){
    buff=-1;
    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
}
else if(kpReadKey() == 'R'){
    buff = 0;
    if(conf == 1)
        num = (num>2)? 0:num+1;
    else
        num = (num>4)? 0:num+1;

    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
    lcdCommand(0x01);
}
else if (kpReadKey() == 'L'){
    buff = 0;
    if(conf == 1)
        num = (num<1)? 3:num-1;
    else
        num = (num<1)? 5:num-1;

    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
    lcdCommand(0x01);
}
else if (kpReadKey() == 'S'){
    conf = 0;
}

kpDebounce();
}

void infoIndividual(void){
    lcdString(" P");
    lcdNumberUnit(num+1);
    lcdString(" Pres:");
    lcdNumber(p[num]);
    lcdString("PSI");
    lcdPosition(1,1);
    lcdString(" Temp.:");
    lcdNumber(tp[num]);
    lcdChar(0b1101111);
    lcdString("C");


    if(p[num] > 1.5*(pC[num]) || p[num] < 0.7*(pC[num])){
        rgbColor(1);

    }
    else if(p[num] > 1.3*(pC[num]) || p[num] < 0.9*(pC[num])){
        rgbColor(3);

    }
    else{
        rgbColor(2);
    }
}

void infoPress(void){
    lcdString("P1:");
    lcdNumber(p[0]);
    lcdString("    P2:");
    lcdNumber(p[1]);
    lcdCommand(0xC0);
    lcdString("P3:");
    lcdNumber(p[2]);
    lcdString("    P4:");
    lcdNumber(p[3]);
    infoGeral();
}

void infoTemp(void){
    lcdString("T1:");
    lcdNumber(tp[0]);
    lcdString("    T2:");
    lcdNumber(tp[1]);
    lcdCommand(0xC0);
    lcdString("T3:");
    lcdNumber(tp[2]);
    lcdString("    T4:");
    lcdNumber(tp[3]);
    infoGeral();
}

void infoGeral(void){
    tpG = (int)(tp[0]+tp[1]+tp[2]+tp[3])/4;
}

void ssdHeat(void){
    if(num<4){
        if(tp[num] > (char)(100*(1+((float)tol/100)))){
            ssdDigit(1,0);
            ssdDigit(2,1);
            ssdDigit(3,2);
            ssdDigit(4,3);
            for(int i=0; i<100;i++){
                ssdUpdate();
                pwmSet(0);
                for(int j=0;j<10;j++){
                    pwmSet(100);
                }
            }
            pwmSet(0);
        }
    }
    else{
        if(tpG > (char)(100*(1+((float)tol/100)))){
            ssdDigit(1,0);
            ssdDigit(2,1);
            ssdDigit(3,2);
            ssdDigit(4,3);
            for(int i=0; i<100;i++){
                ssdUpdate();
                pwmSet(0);
                for(int j=0;j<10;j++){
                    pwmSet(100);
                }
            }
            pwmSet(0);
        }
    }
}
void teste(void){
    p[0] = buff + p[0];
    p[1] = buff + p[1];
    p[2] = buff + p[2];
    p[3] = buff + p[3];
    tp[0] = buff + tp[0];
    tp[1] = buff + tp[1];
    tp[2] = buff + tp[2];
    tp[3] = buff + tp[3];
    buff = 0;
}
