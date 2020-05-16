

#ifndef _SIM_H    /* Guard against multiple inclusion */
#define _SIM_H
#define HEXVALUE 9
#define MEMINLEN 257
#define REGLEN 16
#define MEMLEN 512


int signedhex2int(char hexstring[]);
int hexstring2int(char hexstring[]);
int hexchar2int(char hexchar);
int performInstruction(char memin_arr[MEMINLEN][HEXVALUE], char memin_line[], int PC_counter, char reg_array[REGLEN][HEXVALUE], char mem_array[MEMLEN][HEXVALUE],char IOReg_array[5][HEXVALUE]);
void sim(char memin_arr0[MEMINLEN][HEXVALUE],char memin_arr1[MEMINLEN][HEXVALUE],int Mem_lctn);
#endif


 