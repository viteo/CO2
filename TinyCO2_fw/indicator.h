#define SCALE unsigned char
#define PPM 0
#define TEMPERATURE 1

void InitIndicators();
void IndicatorDance();
void IndicatorSetArrow(SCALE scale, int value);
void IndicatorSetLed(int value);