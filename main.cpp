#include <robotarm/LED.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    robotarm::LED led;

    double loop = 0.;

    while (true)
    {
        double s = 300.*(sin(loop)+1.01); // in [0, 2]

        led.blink(s, true);

        loop += 0.1;
    }

    return 0;
}
