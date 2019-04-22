#define	MHZ19_RESULT_OK 0
#define	MHZ19_RESULT_ERR_FB 1
#define	MHZ19_RESULT_ERR_SB 2
#define	MHZ19_RESULT_ERR_CRC 3

typedef unsigned char byte;

int result;

int retrieveData();
int getPPM();
int getTemperature();
int getStatus();