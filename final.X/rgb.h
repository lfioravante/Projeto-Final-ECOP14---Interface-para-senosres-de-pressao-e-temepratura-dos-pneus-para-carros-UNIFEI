#ifndef RGB_H_
#define RGB_H_

//todos desligados
#define OFF    0

//cores primárias
#define RED    1
#define GREEN  2
#define BLUE   4

//cores secundárias
#define YELLOW (RED+GREEN)
#define CYAN   (GREEN+BLUE)
#define PURPLE (RED+BLUE)

//todos acesos
#define WHITE  (RED+GREEN+BLUE)

	void rgbColor(int led);
	void turnOn(int led);
	void turnOff(int led);
	void rgbInit(void);
#endif
