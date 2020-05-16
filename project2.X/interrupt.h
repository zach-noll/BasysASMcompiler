
#ifndef _SSD_H    /* Guard against multiple inclusion */
#define _SSD_H


extern int switch_flag0;
extern int switch_flag1;
extern int switch_flag2;
extern int switch_flag3;
extern int switch_flag4;

extern int btnl_flag;
extern int btnc_flag;
extern int btnd_flag;
extern int btnu_flag;
extern int btnr_flag;
extern int btnd_flag;

extern unsigned int counter;
extern int btnc_counter;
extern int btnd_counter;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

void Initialize();
void SSD_WriteDigits(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, \
            unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4);
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp);
unsigned char SSD_GetDigitSegments(unsigned char d);

// private functions
void SSD_ConfigurePins();
void Timer1Setup();


    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
#endif /* _SSD_H */

/* *****************************************************************************
 End of File
 */
