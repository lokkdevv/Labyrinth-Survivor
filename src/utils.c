#include "../include/utils.h"

Color debugGreen = {50, 205, 50, 100};
Color debugYellow = {255, 255, 0, 100};
Color debugRed = {255, 0, 0, 100};
Color LIGHTPURPLE = {112, 41, 99, 20};

float minf(float num1, float num2) {
    if (num1==NAN)
        return num1;
    if (num2==NAN)
        return num2;
    return (num1<num2)? num1 : num2;
} 

float maxf(float num1, float num2) {
    if (num1==NAN)
        return num2;
    if (num2==NAN)
        return num1;
    return (num1<num2)? num2 : num1;
}

float clamp(float value, float min, float max) {
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
} 
