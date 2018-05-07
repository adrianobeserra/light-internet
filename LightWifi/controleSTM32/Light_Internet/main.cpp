#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut d7(D7);
DigitalIn d10(D10);

int inverseNumber(int number)
{
   return number * -1 + 1;
}

int validarValor(int valor)
{
    if (valor != 0 && valor != 1) {
        return 1;
    } else {
        return valor;
    }
}

int main()
{
    //d10.mode(PullUp);
    int lampadaLigada = inverseNumber(d10);//validarValor(d10);
    d7 = lampadaLigada;
    led1 = lampadaLigada;
    while (true) {
        int deveLigarLampada = inverseNumber(d10);//validarValor(d10);
        //deveLigarLampada = ligarLampada(deveLigarLampada);
        if (deveLigarLampada != lampadaLigada) {
            d7 =  deveLigarLampada;
            led1 =  deveLigarLampada;
            lampadaLigada = deveLigarLampada;
            Thread::wait(1000);
        }
    }
}

