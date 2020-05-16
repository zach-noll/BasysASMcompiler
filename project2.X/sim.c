/* ************************************************************************** */
/** Simulator

  @Company
    Company Name

  @File Name
    sim.c

  @Summary
 Simulates execution of assembly code


 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
#define _CRT_SECURE_NO_WARNINGS
#define HEXVALUE 9
#define REGVALUE 500 //16 registers x 8 bits +7 white spaces + /0 + /n
#define TRACELEN 500 //18 values x (8 bits + 1 space) -1 (irrelevant " ") + 2 (/0+/n)
#define IMMVALUE 4 //4 hex digits in imm field of instruction
#define MEMLEN 512

#define REGLEN 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "btn.h"
#include "led.h"
#include "ssd1.h"
#include "sim.h"


int signedhex2int(char hexstring[]) {	//converts signed hex string "XXX" to integer
	short int number = 0;
	char temp_hex[5] = "F000";
	if (hexstring[0] == 'F' || hexstring[0] == 'f') {
		temp_hex[1] = hexstring[0];
		temp_hex[2] = hexstring[1];
		temp_hex[3] = hexstring[2];
		number = (int)strtol(temp_hex, NULL, 16);
	}
	else {
		number = strtol(hexstring, NULL, 16);
	}
	return number;
}

int hexstring2int(char hexstring[]) { //converts signed hex string to integer

	char hex_string[HEXVALUE] = { 0 };
	strncpy(hex_string, hexstring, 9);
	int number;
	sscanf(hex_string, "%X", &number);
	return number;

}

int hexchar2int(char hexchar) {		//converts unsigned hex char 'X' to corresponding integer value
	switch (hexchar) {
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	case 'A':
		return 10;
		break;
	case 'B':
		return 11;
		break;
	case 'C':
		return 12;
		break;
	case 'D':
		return 13;
		break;
	case 'E':
		return 14;
		break;
	case 'F':
		return 15;
		break;
		return 0;

	}
}

//function which performs the given instruction from memin
int performInstruction(char memin_arr[MEMLEN][HEXVALUE], char memin_line[], int PC_counter, char reg_array[REGLEN][HEXVALUE], char mem_array[MEMLEN][HEXVALUE],char IOReg_array[5][HEXVALUE]) {
	char PC_counter_hex[HEXVALUE] = { 0 };
	char regarray2write[REGVALUE] = { 0 };
	char reg_value_temp[HEXVALUE] = { 0 };
	char array2write[TRACELEN] = { 0 };
	char line[REGVALUE] = { 0 };
	char memin_line2write[HEXVALUE] = { 0 };

	char opcode;
	sprintf(PC_counter_hex, "%08X", PC_counter);	//gets PC counter in it's integer value for loops
	int j = 0;


	for (j = 0; j < 8; j++) {
		memin_line2write[j] = memin_line[j];
	}
	memin_line2write[8] = '\0';
    int i = 0;
	for (i = 0; i < 16; i++) { //prints trace line to trace file
		
		sprintf(regarray2write,"%s%s ",regarray2write, reg_array[i]);
		

	}
	regarray2write[143] = '\n';

	sprintf(array2write, "%s %s %s", PC_counter_hex, memin_line2write, regarray2write);

	//updates reg_array to new register values (after completion of instruction)
	opcode = memin_line[0];
	char imm_str[IMMVALUE] = { 0 };
	char reg_val_temp[HEXVALUE] = { 0 };
	int rd = 0;
	int rs = 0;
	int rt = 0;
	int rm = 0;
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	signed int temp4; //for shift right arithmetic
	int PC_temp = PC_counter;
	int line_temp;
	i = 0;

	imm_str[0] = memin_line[5];
	imm_str[1] = memin_line[6];
	imm_str[2] = memin_line[7];
	imm_str[3] = '\0';



	switch (opcode) { //checks which instruction to perform

	case '0': //add: R[rd] = R[rs] + R[rt] + simm
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = temp1 + temp2 + signedhex2int(imm_str);
		sprintf(reg_val_temp, "%08X", temp3);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '1': //sub: R[rd] = R[rs] ? R[rt] ? simm
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = temp1 - temp2 - signedhex2int(imm_str);
		sprintf(reg_val_temp, "%08X", temp3);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '2': //and: R[rd] = R[rs] & R[rt] & simm
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = temp1 & temp2 & signedhex2int(imm_str);
		sprintf(reg_val_temp, "%08X", temp3);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '3': //or: R[rd] = R[rs] | R[rt] | simm
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = temp1 | temp2 | signedhex2int(imm_str);
		sprintf(reg_val_temp, "%08X", temp3);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '4': //sll: R[rd] = R[rs] << [R[rt] + imm]
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = temp1 << (temp2 + hexstring2int(imm_str));
		sprintf(reg_val_temp, "%08X", temp3);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '5': //sra: R[rd] = R[rs] >> [R[rt] + imm] with sign extension
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp4 = temp1 >> (temp2 + hexstring2int(imm_str));
		sprintf(reg_val_temp, "%08X", temp4);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '6': //mac: R[rd] = R[rs] * R[rt] + R[rm] + simm
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);
		temp3 = hexstring2int(reg_array[rm]);
		temp4 = temp1 * temp2 + temp3 + signedhex2int(imm_str);
		sprintf(reg_val_temp, "%08X", temp4);
		strcpy(reg_array[rd], reg_val_temp);
		PC_temp++;
		break;

	case '7': //branch
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		rt = hexchar2int(memin_line[3]);
		rm = hexchar2int(memin_line[4]);
		temp1 = hexstring2int(reg_array[rs]);
		temp2 = hexstring2int(reg_array[rt]);

		switch (rm) {	//checks which branch to perform
		case 0:
			if (temp1 == temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		case 1:
			if (temp1 != temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		case 2:
			if (temp1 > temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		case 3:
			if (temp1 < temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		case 4:
			if (temp1 >= temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		case 5:
			if (temp1 <= temp2) PC_temp = hexstring2int(imm_str);
			else PC_temp++;
			break;
		}
		break;
    
    case '8': //R[rd]=IORegister[R[rs]+simm] Get I/0 Value
        rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
        temp1 = hexstring2int(reg_array[rs])+signedhex2int(imm_str);
        strcpy(reg_array[rd],IOReg_array[temp1]);
        PC_temp++;
        break;
    case '9': //IORegister[R[rs]+simm]=R[rd] Set I/O value
        rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
        temp1 = hexstring2int(reg_array[rs])+signedhex2int(imm_str);
        if(temp1 == 0 || temp1 == 2 || temp1 == 3) break;  //cannot write to IORegister[0],IORegister[2],IORegister[3]
        strcpy(IOReg_array[temp1],reg_array[rd]);
        PC_temp++;
        break;
    
	case 'B': //jal: R[15] = (pc + 1) & 0xfff (next instruction address), sets pc = imm
		temp2 = (PC_temp + 1) & 0xFFF;
		sprintf(reg_value_temp, "%08X", temp2);
		strcpy(reg_array[15], reg_value_temp);
		PC_temp = hexstring2int(imm_str);
		break;
	case 'C': //lw: R[rd] = MEM[(R[rs]+simm) & 0xfff]
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		line_temp = (hexstring2int(reg_array[rs]) + signedhex2int(imm_str) & 0xFFF);
		strncpy(reg_array[rd], mem_array[line_temp], 8);
		PC_temp++;
		break;
	case 'D': //sw: MEM[(R[rs]+simm) & 0xfff] = R[rd]
		rd = hexchar2int(memin_line[1]);
		rs = hexchar2int(memin_line[2]);
		line_temp = ((hexstring2int(reg_array[rs]) + signedhex2int(imm_str)) & 0xFFF); //line_temp is the line number to write data to in memout
		strncpy(mem_array[line_temp], reg_array[rd], 9);
		PC_temp++;
		break;
	case 'E': //jr: pc = R[rd] & 0xfff
		rd = hexchar2int(memin_line[1]);
		PC_temp = hexstring2int(reg_array[rd]) & 0xfff;
		break;
	
    case 'F': //Halt, return same PC counter to simulate pause at end of program
        break;
}

	return PC_temp;	//returns the next line to go to in memin file
}

void light_LED(char IORegister_1[HEXVALUE])
{
    
    int i = 0;
    
    for(i=0;i<8;i++){
        if(IORegister_1[7-i]=='1') LED_SetValue(i,1);
        if(IORegister_1[7-i]=='0') LED_SetValue(i,0);
    }
    return;
}
void write_SSD(char IORegister_4[HEXVALUE])
{
    SSD_WriteDigits(hexchar2int(IORegister_4[7]),hexchar2int(IORegister_4[6]),hexchar2int(IORegister_4[5]),hexchar2int(IORegister_4[4]),0,0,1,0);
    return;
}

void sim(char memin_arr0[MEMLEN][HEXVALUE],char memin_arr1[MEMLEN][HEXVALUE],int Mem_lctn) { //calls functions to perform instructions and writes new values to output .txt files.
    
	char last_line[TRACELEN] = { 0 };
	char line[TRACELEN] = { 0 };
	char PC_string[HEXVALUE] = { 0 };
	char regarray2write[TRACELEN] = { 0 };
	char reg_value_temp[HEXVALUE] = { 0 };

	char reg_array[REGLEN][HEXVALUE] = { 0 };
	char mem_array0[MEMLEN][HEXVALUE] = { 0 };
    char mem_array1[MEMLEN][HEXVALUE] = { 0 };
  	char mem_array[MEMLEN][HEXVALUE] = { 0 };
    char memin_arr[MEMLEN][HEXVALUE]={0};
    char IORegister[5][HEXVALUE]= {"00000000","00000000","00000000","00000000","00000000"};
    char PC_str[HEXVALUE];
    char Reg_str[REGLEN];
    char Mem_str[REGLEN];
    char Mem_val[REGLEN];
    char Rsp_str[REGLEN];
	char memin_line[TRACELEN] = { 0 };
    char RSP_str[4]={0};
    char counter_str[REGLEN];
   
    
	/*struct Register reg_temp;*/

	int i = 0;
	int PC_counter = 0;
	int count = 0;
    int Reg_num = 0;
    int lctn_flag = 1;

    
	for (i = 0; i < 16; i++) strcpy(reg_array[i], "00000000");							//sets all register values to "00000000" 
		
    for (i=0;i<4;i++) strcpy(IORegister[i],"00000000");
    
	int linecount = 0;

	for (i = 0;i < 257;i++) {

		strncpy(mem_array0[i], memin_arr0[i], 8);
        strncpy(mem_array1[i], memin_arr1[i], 8);
		linecount++;
	}

	for (i = linecount; i < 512; i++) {				
		strcpy(mem_array0[i], "00000000");
        strcpy(mem_array1[i], "00000000");
	}

    int PC_prev;
    int flag_check=1;

	while (1) {
        
       
        if((flag_check==1) && (switch_flag7==0))
        {
                
            for(i = 0; i<MEMLEN; i++)
            {
                strcpy(mem_array[i],mem_array0[i]);
                strcpy(memin_arr[i],memin_arr0[i]);
                
            }
            
        }
        else if((flag_check==1)&& (switch_flag7 == 1))
        {
            
            for(i = 0; i<MEMLEN; i++)
            {
                strcpy(mem_array[i],mem_array1[i]);
                strcpy(memin_arr[i],memin_arr1[i]);
                
            }

        }
        
        //IORegister:
        sprintf(IORegister[0],"%08X",counter);//0 - counter that advances by 1 every 31.25 milliseconds - resets at FFFFFFF
        light_LED(IORegister[1]); // register with either 1 or 0 showing which LEDs to light
        sprintf(IORegister[2],"%08X",btnc_counter);//2 - counter that advances by 1 every time BTNC is pushed 
        sprintf(IORegister[3],"%08X",btnd_counter);//3 - counter that advances by 1 every time BTND is pushed
        
        if(SSD_Writeflag==1) 
        {
            SSD_Writeflag=0;
            write_SSD(IORegister[4]); //4 - SSD - every 4 bits shows what number to write in each position
        }

		strncpy(memin_line, memin_arr[PC_counter], 8);
        memin_line[9]='\0';
        
		//updates PC_counter, performs instruction, and writes to trace file
        PC_prev=PC_counter;
        if(SSD_Flag == 1)
        {
            sprintf(counter_str,"%-16d",counter);
            PC_counter = performInstruction(memin_arr, memin_line, PC_counter, reg_array, mem_array, IORegister);
            SSD_Flag = 0;
            if(btnu_flag==1&&(switch_flag0==1)&&(switch_flag1==0)) 
            { //go up a register in register display
                Reg_num++;
                if(Reg_num == 16) Reg_num = 0;
                btnu_flag=0;

            }
            else if(btnu_flag==1&&(switch_flag0==0)&&(switch_flag1==1))
            { //go up one address in memory in memory display
                Mem_lctn++;
                if(Mem_lctn == 512) Mem_lctn = 0;
                btnu_flag=0;
                lctn_flag=0;
            }

            else if((switch_flag0==0)&&(switch_flag1==0))
            { //LCD: instruction and PC
                sprintf(PC_str,"%03X",PC_prev);
                sprintf(PC_str,"%-16s",PC_str);
                sprintf(Mem_val,"%-16s",memin_line);
                if(LCD_Writeflag==1){
                LCD_WriteStringAtPos(Mem_val,0,0);
                LCD_WriteStringAtPos(PC_str,1,0);
                LCD_Writeflag=0;
                }
                lctn_flag=1;
            }

            else if((switch_flag0==1)&&(switch_flag1==0))
            { //LCD: Register number and data
               sprintf(Reg_str,"R%02d = %s",Reg_num,reg_array[Reg_num]);
               sprintf(Reg_str,"%-16s",Reg_str);
               if(LCD_Writeflag==1){
               LCD_WriteStringAtPos(Reg_str,0,0);
               LCD_WriteStringAtPos("                ",1,0);
               LCD_Writeflag=0;
               }
               lctn_flag=1;
            }
            else if((switch_flag0==0)&&(switch_flag1==1))
            { //LCD: Memory address and data, RSP register and data
                    if(lctn_flag)
                    { //starting memory location (doesn't check after first time pressed btnu)
                        if((switch_flag5==1)&&(switch_flag6==0))  Mem_lctn = 256;

                        else if((switch_flag5==1)&&(switch_flag6==1)) Mem_lctn = 511;
                        
                        else if((switch_flag5==0)&&(switch_flag6==0)) Mem_lctn = 0;
                    }
                sprintf(Mem_str,"M%03X = %s",Mem_lctn,mem_array[Mem_lctn]);
                strncpy(RSP_str,reg_array[13]+5,8);
                sprintf(Rsp_str,"RSP = %s       ",RSP_str);
                if(LCD_Writeflag==1){
                    LCD_WriteStringAtPos(Mem_str,0,0);
                    LCD_WriteStringAtPos(Rsp_str,1,0);
                    LCD_Writeflag=0;
                                }
                lctn_flag=0;
            }
            else if((switch_flag0 == 1) && (switch_flag1 == 1))
            {
                if(LCD_Writeflag==1){    
                    LCD_WriteStringAtPos(counter_str,0,0);
                    LCD_WriteStringAtPos("                ",1,0);
                    LCD_Writeflag=0;
                }
                    lctn_flag=1;
            }
            if (memin_line[0] != 'F') count++;

            /*Pause implementation*/
            if(btnl_flag==1) 
            {
                btnl_flag = 0;

                while(1)
                {

                    sprintf(IORegister[0],"%08X",counter);//0 - counter that advances by 1 every 31.25 milliseconds - resets at FFFFFFFF
                    sprintf(counter_str,"%-16d",counter);
                    if((switch_flag0==0)&&(switch_flag1==0)) //LCD: instruction and PC
                    {   
                        sprintf(PC_str,"%03X",PC_prev);
                        sprintf(PC_str,"%-16s",PC_str);
                        if(LCD_Writeflag==1){
                            LCD_WriteStringAtPos(Mem_val,0,0);
                            LCD_WriteStringAtPos(PC_str,1,0);
                            LCD_Writeflag=0;
                        }
                        lctn_flag=1;
                    }   
                    if((btnu_flag==1)&&(switch_flag0==1)&&(switch_flag1==0))
                    {
                        Reg_num++;
                        if(Reg_num == 16) Reg_num = 0;
                        btnu_flag=0;
                        sprintf(Reg_str,"R%02d = %s",Reg_num,reg_array[Reg_num]);
                        if(LCD_Writeflag==1){
                        LCD_WriteStringAtPos(Reg_str,0,0);
                        LCD_WriteStringAtPos("                ",1,0);
                        LCD_Writeflag=0;
                        }
                    }
                    else if((btnu_flag==1)&&(switch_flag0==0)&&(switch_flag1==1)) 
                    {
                        Mem_lctn++;
                        if(Mem_lctn == 512) Mem_lctn = 0;
                        btnu_flag=0;
                        
                        sprintf(Mem_str,"M%03X = %s",Mem_lctn,mem_array[Mem_lctn]);
                        sprintf(Rsp_str,"RSP = %s",reg_array[13]);
                        if(LCD_Writeflag==1){
                        LCD_WriteStringAtPos(Mem_str,0,0);
                        LCD_WriteStringAtPos(Rsp_str,1,0);
                        LCD_Writeflag=0;
                        }
                    }
               
                    else if((switch_flag0==1)&&(switch_flag1==0)) //LCD: Register number and data
                    {
                       sprintf(Reg_str,"R%02d = %s",Reg_num,reg_array[Reg_num]);
                       sprintf(Reg_str,"%-16s",Reg_str);
                       if(LCD_Writeflag==1){
                       LCD_WriteStringAtPos(Reg_str,0,0);
                       LCD_WriteStringAtPos("                ",1,0);
                       LCD_Writeflag=0;
                       }
                       lctn_flag=1;
                    }
                    else if((switch_flag0==0)&&(switch_flag1==1))
                    {
                        if(lctn_flag)
                        { //starting memory location (doesn't check after first time pressed btnu)
                            if((switch_flag5==1)&&(switch_flag6==0))  Mem_lctn = 256;

                            else if((switch_flag5==1)&&(switch_flag6==1)) Mem_lctn = 511;

                            else if((switch_flag5==0)&&(switch_flag6==0)) Mem_lctn = 0;
                        }
                       sprintf(Mem_str,"M%03X = %s",Mem_lctn,mem_array[Mem_lctn]);
                       strncpy(RSP_str,reg_array[13]+5,8);
                       sprintf(Rsp_str,"RSP = %s       ",RSP_str);
                       if(LCD_Writeflag==1){
                       LCD_WriteStringAtPos(Mem_str,0,0);
                       LCD_WriteStringAtPos(Rsp_str,1,0);
                       LCD_Writeflag=0;
                       }
                       lctn_flag=0;
                    }
                    else if((switch_flag0 == 1) && (switch_flag1 == 1))
                    {
                        
                        if(LCD_Writeflag==1){
                        LCD_WriteStringAtPos(counter_str,0,0);
                        LCD_WriteStringAtPos("                ",1,0);
                        LCD_Writeflag=0;
                        }
                    }

                    if(btnl_flag==1)
                    { //unpause
                        btnl_flag = 0;
                        break;
                    }
                    if(btnr_flag ==1)
                    { //do next instruction
                        btnl_flag =1;
                        btnr_flag =0;
                        break;
                    } 

                }
            }
        }


	
    
    

    }

}