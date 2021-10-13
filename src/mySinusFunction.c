#include "bootcamp/mySinusFunction.h"
#include <math.h>
#define PI 3.14159265359
#include <inttypes.h>

float pi = 3.14159265359;




 float SinusFunction(float degreeValue){
    float sinusValue = 0.0f;
    float radiantValue = fmod(degreeValue * pi/180,2*pi);

    for(uint16_t x = 0; x < 20; x++){
        sinusValue += (powerFunction(-1,x) / FactorialFunction(2*x+1) * powerFunction(radiantValue,2*x+1));
    }
    return sinusValue;   
}

float FactorialFunction(uintmax_t numberforFactorial){
    float resultforFactorial = 1; 
    for (uint8_t y = 1; y <= numberforFactorial; y++){
        resultforFactorial = resultforFactorial * y;
    }
    return resultforFactorial;
}

float powerFunction(float base,uint8_t exponent){
    if(exponent){
        float result = 1;
        for(uint8_t i = 0; i < exponent; i++ ){
            result = base*result;
        }
        return result;
    }
    else{
        return 1;
    }
}

