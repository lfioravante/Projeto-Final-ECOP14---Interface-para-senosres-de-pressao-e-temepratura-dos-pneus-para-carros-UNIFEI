#include "lcd.h"
#include "keypad.h"
#include "config.h"
#include "ssd.h"
#include "io.h"
#include "so.h"
//Não esqueça de incluir as outras bibliotecas necessárias
void main(void) {
  
 lcdInit();
 kpInit();
 ssdInit();
 
 unsigned char tecla, buff=0;
 char msg1[16] = "Teclado PQDB";
 char msg2[16] = ">>> ";
 char cont = 1, cont2 = 0, tempo;

 for (;;) {
    kpDebounce();
    lcdCommand(0x80);
    lcdString(msg1);
    if (kpReadKey() != 0) {
        tecla = kpReadKey();
        lcdCommand(0xC0);
        lcdString(msg2);
        lcdChar(tecla);
        buff = 1;
    }
    while(kpReadKey() != 0){
        kpDebounce();
        ssdUpdate();
        for (tempo = 0;tempo < 180;tempo++){
        int resta = ((int)tempo%15) ;
        if(resta == 0)
            ssdUpdate();
        }
    }
    if(tecla == 'R' && buff == 1){
    ssdDigit(cont, 3);//10s
    ssdDigit(cont2, 2);//10s
    if(cont == 9){
        cont2 = (cont2 > 9)? 0:cont2+1;
    }
    cont = (cont > 8)? 0:cont+1;
    buff = 0;
    }
    for (tempo = 0;tempo < 180;tempo++){
    int resta = ((int)tempo%15) ;
    if(resta == 0)
       ssdUpdate();
}// +- 10ms


}
}
