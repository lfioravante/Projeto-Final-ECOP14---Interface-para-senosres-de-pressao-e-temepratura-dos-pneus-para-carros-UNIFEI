
#include "keypad.h"
#include "config.h"
#include "lcd.h"
#include "rgb.h"
#include "ssd.h"
#include "adc.h"
#include "pwm.h"

void configuraPSI(void);
void comandos(void);
void instrucaoLCD(void);
void infoIndividual(void);
void infoPress(void);
void infoTemp(void);
void infoGeral(void);
void ssdHeat(void);
void teste(void);

unsigned char tecla, conf=1, prt=1,tblock, block=0;
char p[4]={30, 28, 20, 45}, pC[4]={32, 32, 32, 32}, tp[4]={60,70,72,97};
char num = 0, buff, pG, tpG;
int pot, pot1=0, tol;

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
    
    for(int i=0; i<70;i++){ // aguarda alguns instantes para configuracao
        for(int j=0;j<10;j++){
           kpDebounce(); // atualiza teclas
        if(kpReadKey() == 's'){ //se tecla SW102 acionar abre config
            configuraPSI(); // loop de config de pressao ideal
            num=0;
        }
        }
    }
    conf=0;
    instrucaoLCD(); // instrução para navegar no menu principal
    
    for(;;){ //loop principal
        
        comandos();  // funcao que atualiza as teclas  
        if(prt == 1){ // bloquei a impressao do LCD
            lcdCommand(0x01); //clear no lcd
            if(num<4){ //de num 0 a 3 = pneus individuais
                infoIndividual(); //funcao de impressao no lcd
                prt = 0; // bloquei a atualizacao lcd
            }
            else if(num == 4){ // info geral de pressao
                infoPress();
                prt = 0;
            } 
            else {
                infoTemp(); //info geral de temperatura
                prt = 0;
            }
            comandos();
        }
        teste();
        block=block+1; // count
        if(block == 100){ //permite a impressao e funcao heat
            ssdHeat(); // funcao que pisca "HEAT" no ssd caso temperatura alta
            prt = 1; // permite a impressao
            block = 0; // reset count
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
    lcdCommand(0x01); // clear LCD
    
    while(conf){ //loop de config

        comandos(); //verifica keypads
        kpDebounce(); // atualiza keypads
    
        if(prt == 1){
            prt = 0;
            lcdPosition(0,0);
            lcdCommand(0xC0);
            lcdString("Aperte S p/ sair"); //S sai do menu de configuracao
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

void instrucaoLCD(void){ //funcao apenas mostra quais botoes movimentam programa
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

pot = adcRead(0); //atualiza o potenciometro para regulagem da tolerancia
tol = 50-(((float)pot/1023)*100);// % da tol com 50% do pot sendo o 0

if(pot1 != adcRead(0)){
    lcdCommand(0x01);
    lcdString(" Temp max: ");
    lcdNumber((100*(1+((float)tol/100))));
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
    lcdCommand(0x01);
    pot1 = pot;
}

kpDebounce();
if(kpReadKey() == 'U'){ //U soma um na pressao de config
    buff=1;
    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
}
else if(kpReadKey() == 'D'){//D subtrai um na pressao de config
    buff=-1;
    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
}
else if(kpReadKey() == 'R'){ //R navega para a direita nos menus
    buff = 0;
    if(conf == 1) // conf = 1 -> menuConfiguracao      conf = 0 -> principal
        num = (num>2)? 0:num+1;
    else
        num = (num>4)? 0:num+1;
        
    prt=1;
    for(int i=0; i<1000;i++){
    for(int j=0;j<100;j++){}}
    lcdCommand(0x01);  
} 
else if (kpReadKey() == 'L'){ //L navega para a esquerda nos menus
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
else if (kpReadKey() == 'S'){ // sai da configuracao
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
           
//se pneu 50% a mais ou 30% a menos de pressao config, RGB = vermelho
    if(p[num] > 1.5*(pC[num]) || p[num] < 0.7*(pC[num])){
        rgbColor(1);
//se pneu 30% a mais ou 10% a menos de pressao config, RGB = amarelo
    }
    else if(p[num] > 1.3*(pC[num]) || p[num] < 0.9*(pC[num])){
        rgbColor(3);
// se OK = verde
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
