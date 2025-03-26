/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
osThreadId_t defaultTaskHandle;
UART_HandleTypeDef huart1;

const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);
void Unpack_Rec_Data(void);

uint8_t recieve_buffer[7] = {0};
char UserTxBuffer[40];
uint8_t temp[5];
unsigned char x,y,ch,*ptr;
unsigned long pll_x=0;
unsigned char ucRF_Freq,ucLO_Freq;
unsigned int uiAttenuation;

void Delay(int Delay_ns){
	for(int Di=0;Di<Delay_ns;Di++)	__NOP();
}

//************************************************************************************************

#define	RF_SEN_HIGH HAL_GPIO_WritePin(RF_LE_GPIO_Port,RF_LE_Pin,GPIO_PIN_SET);
#define	RF_SEN_LOW  HAL_GPIO_WritePin(RF_LE_GPIO_Port,RF_LE_Pin,GPIO_PIN_RESET);

#define RF_SDI_HIGH HAL_GPIO_WritePin(RF_DATA_GPIO_Port,RF_DATA_Pin,GPIO_PIN_SET);
#define RF_SDI_LOW  HAL_GPIO_WritePin(RF_DATA_GPIO_Port,RF_DATA_Pin,GPIO_PIN_RESET);

#define RF_SCK_HIGH HAL_GPIO_WritePin(RF_CLK_GPIO_Port,RF_CLK_Pin,GPIO_PIN_SET);
#define RF_SCK_LOW  HAL_GPIO_WritePin(RF_CLK_GPIO_Port,RF_CLK_Pin,GPIO_PIN_RESET);

unsigned long RR112	= 0x700000;
unsigned long RR111	= 0x6F0000;
unsigned long RR110	= 0x6E0000;
unsigned long RR109	= 0x6D0000;
unsigned long RR108	= 0x6C0000;
unsigned long RR107	= 0x6B0000;
unsigned long RR106	= 0x6A0000;
unsigned long RR105	= 0x690021;
unsigned long RR104	= 0x680000;
unsigned long RR103	= 0x670000;
unsigned long RR102	= 0x663F80;
unsigned long RR101	= 0x650011;
unsigned long RR100	= 0x640000;
unsigned long RR99	= 0x630000;
unsigned long RR98	= 0x620200;
unsigned long RR97	= 0x610888;
unsigned long RR96	= 0x600000;
unsigned long RR95	= 0x5F0000;
unsigned long RR94	= 0x5E0000;
unsigned long RR93	= 0x5D0000;
unsigned long RR92	= 0x5C0000;
unsigned long RR91	= 0x5B0000;
unsigned long RR90	= 0x5A0000;
unsigned long RR89	= 0x590000;
unsigned long RR88	= 0x580000;
unsigned long RR87	= 0x570000;
unsigned long RR86	= 0x56FFFC;
unsigned long RR85	= 0x55E5FF;
unsigned long RR84	= 0x540001;
unsigned long RR83	= 0x53FFFD;
unsigned long RR82	= 0x5230FF;
unsigned long RR81	= 0x510000;
unsigned long RR80	= 0x506666;
unsigned long RR79	= 0x4F0026;
unsigned long RR78	= 0x4E0003;
unsigned long RR77	= 0x4D0000;
unsigned long RR76	= 0x4C000C;
unsigned long RR75	= 0x4B0880;
unsigned long RR74	= 0x4A0000;
unsigned long RR73	= 0x49003F;
unsigned long RR72	= 0x480001;
unsigned long RR71	= 0x470081;
unsigned long RR70	= 0x46C350;
unsigned long RR69	= 0x450000;
unsigned long RR68	= 0x4403E8;
unsigned long RR67	= 0x430000;
unsigned long RR66	= 0x4201F4;
unsigned long RR65	= 0x410000;
unsigned long RR64	= 0x401388;
unsigned long RR63	= 0x3F0000;
unsigned long RR62	= 0x3E0322;
unsigned long RR61	= 0x3D00A8;
unsigned long RR60	= 0x3C0000;
unsigned long RR59	= 0x3B0001;
unsigned long RR58	= 0x3A9001;
unsigned long RR57	= 0x390020;
unsigned long RR56	= 0x380000;
unsigned long RR55	= 0x370000;
unsigned long RR54	= 0x360000;
unsigned long RR53	= 0x350000;
unsigned long RR52	= 0x340820;
unsigned long RR51	= 0x330080;
unsigned long RR50	= 0x320000;
unsigned long RR49	= 0x314180;
unsigned long RR48	= 0x300300;
unsigned long RR47	= 0x2F0300;
unsigned long RR46	= 0x2E07FC;
unsigned long RR45	= 0x2DD0DF;
unsigned long RR44	= 0x2C1FA3;
unsigned long RR43	= 0x2B0019;
unsigned long RR42	= 0x2A0000;
unsigned long RR41	= 0x290000;
unsigned long RR40	= 0x280000;
unsigned long RR39	= 0x2703E8;
unsigned long RR38	= 0x260000;
unsigned long RR37	= 0x250304;
unsigned long RR36	= 0x240026;
unsigned long RR35	= 0x230004;
unsigned long RR34	= 0x220000;
unsigned long RR33	= 0x211E21;
unsigned long RR32	= 0x200393;
unsigned long RR31	= 0x1F43EC;
unsigned long RR30	= 0x1E318C;
unsigned long RR29	= 0x1D318C;
unsigned long RR28	= 0x1C0488;
unsigned long RR27	= 0x1B0003;
unsigned long RR26	= 0x1A0DB0;
unsigned long RR25	= 0x190C2B;
unsigned long RR24	= 0x18071A;
unsigned long RR23	= 0x17007C;
unsigned long RR22	= 0x160001;
unsigned long RR21	= 0x150401;
unsigned long RR20	= 0x14E048;
unsigned long RR19	= 0x1327B7;
unsigned long RR18	= 0x120064;
unsigned long RR17	= 0x11012C;
unsigned long RR16	= 0x100080;
unsigned long RR15	= 0x0F064F;
unsigned long RR14	= 0x0E1E70;
unsigned long RR13	= 0x0D4000;
unsigned long RR12	= 0x0C5001;
unsigned long RR11	= 0x0B0018;
unsigned long RR10	= 0x0A10D8;
unsigned long RR9	= 0x091604;
unsigned long RR8	= 0x082000;
unsigned long RR7	= 0x0740B2;
unsigned long RR6	= 0x06C802;
unsigned long RR5	= 0x0500C8;
unsigned long RR4	= 0x040A43;
unsigned long RR3	= 0x030642;
unsigned long RR2	= 0x020500;
unsigned long RR1	= 0x010808;
unsigned long RR0	= 0x00251C;


unsigned long RR36_LUT_RF[11] = {0x240026,0x24002A,0x24002B,0x24002B};
unsigned long RR43_LUT_RF[11] = {0x2B0019,0x2B03CF,0x2B0113,0x2B023F};

void RF_Freq_Write(unsigned long *add_reg){
	ptr=(unsigned char *)add_reg;
	RF_SEN_LOW
	for(y=3;y>0;y--)
	{
		ch=*(ptr+y-1);		//Inc pointer copy bit
		for(x=0;x<8;x++)	//Total 8*3=24 bits
		{
			RF_SCK_LOW		//Clock low
			if(ch&0x80)		//Bitwise & with from 23-0 bit
				RF_SDI_HIGH		//Place data high
            else
            	RF_SDI_LOW		//Place data low
				Delay(40);
            RF_SCK_HIGH
			Delay(40);
			ch<<=1; 		//Shift MSB values
		}
	}
	RF_SEN_HIGH
	RF_SCK_LOW
	RF_SDI_LOW
}

void Default_RF_Frequency(){
    RF_Freq_Write(&RR112);
    RF_Freq_Write(&RR111);
    RF_Freq_Write(&RR110);
    RF_Freq_Write(&RR109);
    RF_Freq_Write(&RR108);
    RF_Freq_Write(&RR107);
    RF_Freq_Write(&RR106);
    RF_Freq_Write(&RR105);
    RF_Freq_Write(&RR104);
    RF_Freq_Write(&RR103);
    RF_Freq_Write(&RR102);
    RF_Freq_Write(&RR101);
    RF_Freq_Write(&RR100);
    RF_Freq_Write(&RR99);
    RF_Freq_Write(&RR98);
    RF_Freq_Write(&RR97);
    RF_Freq_Write(&RR96);
    RF_Freq_Write(&RR95);
    RF_Freq_Write(&RR94);
    RF_Freq_Write(&RR93);
    RF_Freq_Write(&RR92);
    RF_Freq_Write(&RR91);
    RF_Freq_Write(&RR90);
    RF_Freq_Write(&RR89);
    RF_Freq_Write(&RR88);
    RF_Freq_Write(&RR87);
    RF_Freq_Write(&RR86);
    RF_Freq_Write(&RR85);
    RF_Freq_Write(&RR84);
    RF_Freq_Write(&RR83);
    RF_Freq_Write(&RR82);
    RF_Freq_Write(&RR81);
    RF_Freq_Write(&RR80);
    RF_Freq_Write(&RR79);
    RF_Freq_Write(&RR78);
    RF_Freq_Write(&RR77);
    RF_Freq_Write(&RR76);
    RF_Freq_Write(&RR75);
    RF_Freq_Write(&RR74);
    RF_Freq_Write(&RR73);
    RF_Freq_Write(&RR72);
    RF_Freq_Write(&RR71);
    RF_Freq_Write(&RR70);
    RF_Freq_Write(&RR69);
    RF_Freq_Write(&RR68);
    RF_Freq_Write(&RR67);
    RF_Freq_Write(&RR66);
    RF_Freq_Write(&RR65);
    RF_Freq_Write(&RR64);
    RF_Freq_Write(&RR63);
    RF_Freq_Write(&RR62);
    RF_Freq_Write(&RR61);
    RF_Freq_Write(&RR60);
    RF_Freq_Write(&RR59);
    RF_Freq_Write(&RR58);
    RF_Freq_Write(&RR57);
    RF_Freq_Write(&RR56);
    RF_Freq_Write(&RR55);
    RF_Freq_Write(&RR54);
    RF_Freq_Write(&RR53);
    RF_Freq_Write(&RR52);
    RF_Freq_Write(&RR51);
    RF_Freq_Write(&RR50);
    RF_Freq_Write(&RR49);
    RF_Freq_Write(&RR48);
    RF_Freq_Write(&RR47);
    RF_Freq_Write(&RR46);
    RF_Freq_Write(&RR45);
    RF_Freq_Write(&RR44);
    RF_Freq_Write(&RR43);
    RF_Freq_Write(&RR42);
    RF_Freq_Write(&RR41);
    RF_Freq_Write(&RR40);
	RF_Freq_Write(&RR39);
	RF_Freq_Write(&RR38);
	RF_Freq_Write(&RR37);
    RF_Freq_Write(&RR36);
	RF_Freq_Write(&RR35);
	RF_Freq_Write(&RR34);
	RF_Freq_Write(&RR33);
	RF_Freq_Write(&RR32);
	RF_Freq_Write(&RR31);
    RF_Freq_Write(&RR30);
    RF_Freq_Write(&RR29);
    RF_Freq_Write(&RR28);
    RF_Freq_Write(&RR27);
    RF_Freq_Write(&RR26);
    RF_Freq_Write(&RR25);
    RF_Freq_Write(&RR24);
    RF_Freq_Write(&RR23);
    RF_Freq_Write(&RR22);
    RF_Freq_Write(&RR21);
    RF_Freq_Write(&RR20);
    RF_Freq_Write(&RR19);
    RF_Freq_Write(&RR18);
    RF_Freq_Write(&RR17);
    RF_Freq_Write(&RR16);
    RF_Freq_Write(&RR15);
    RF_Freq_Write(&RR14);
    RF_Freq_Write(&RR13);
    RF_Freq_Write(&RR12);
    RF_Freq_Write(&RR11);
    RF_Freq_Write(&RR10);
    RF_Freq_Write(&RR9);
    RF_Freq_Write(&RR8);
    RF_Freq_Write(&RR7);
    RF_Freq_Write(&RR6);
    RF_Freq_Write(&RR5);
    RF_Freq_Write(&RR4);
    RF_Freq_Write(&RR3);
    RF_Freq_Write(&RR2);
    RF_Freq_Write(&RR1);
    RF_Freq_Write(&RR0);
}

//************************************************************************************************

//************************************************************************************************

//LO FREQ  -- 16.770, 16.845, 16.920 MHz  100 MHz Ref //75MHz step

#define	LO_SEN_HIGH HAL_GPIO_WritePin(LO_LE_GPIO_Port,LO_LE_Pin,GPIO_PIN_SET);
#define	LO_SEN_LOW  HAL_GPIO_WritePin(LO_LE_GPIO_Port,LO_LE_Pin,GPIO_PIN_RESET);

#define LO_SDI_HIGH HAL_GPIO_WritePin(LO_DATA_GPIO_Port,LO_DATA_Pin,GPIO_PIN_SET);
#define LO_SDI_LOW  HAL_GPIO_WritePin(LO_DATA_GPIO_Port,LO_DATA_Pin,GPIO_PIN_RESET);

#define LO_SCK_HIGH HAL_GPIO_WritePin(LO_CLK_GPIO_Port,LO_CLK_Pin,GPIO_PIN_SET);
#define LO_SCK_LOW  HAL_GPIO_WritePin(LO_CLK_GPIO_Port,LO_CLK_Pin,GPIO_PIN_RESET);

unsigned long R112	= 0x700000;
unsigned long R111	= 0x6F0000;
unsigned long R110	= 0x6E0000;
unsigned long R109	= 0x6D0000;
unsigned long R108	= 0x6C0000;
unsigned long R107	= 0x6B0000;
unsigned long R106	= 0x6A0000;
unsigned long R105	= 0x690021;
unsigned long R104	= 0x680000;
unsigned long R103	= 0x670000;
unsigned long R102	= 0x663F80;
unsigned long R101	= 0x650011;
unsigned long R100	= 0x640000;
unsigned long R99	= 0x630000;
unsigned long R98	= 0x620200;
unsigned long R97	= 0x610888;
unsigned long R96	= 0x600000;
unsigned long R95	= 0x5F0000;
unsigned long R94	= 0x5E0000;
unsigned long R93	= 0x5D0000;
unsigned long R92	= 0x5C0000;
unsigned long R91	= 0x5B0000;
unsigned long R90	= 0x5A0000;
unsigned long R89	= 0x590000;
unsigned long R88	= 0x580000;
unsigned long R87	= 0x570000;
unsigned long R86	= 0x56FFFD;
unsigned long R85	= 0x55EDFF;
unsigned long R84	= 0x540001;
unsigned long R83	= 0x53FFFE;
unsigned long R82	= 0x5238FF;
unsigned long R81	= 0x510000;
unsigned long R80	= 0x506666;
unsigned long R79	= 0x4F0026;
unsigned long R78	= 0x4E0003;
unsigned long R77	= 0x4D0000;
unsigned long R76	= 0x4C000C;
unsigned long R75	= 0x4B0880;
unsigned long R74	= 0x4A0000;
unsigned long R73	= 0x49003F;
unsigned long R72	= 0x480001;
unsigned long R71	= 0x470081;
unsigned long R70	= 0x46C350;
unsigned long R69	= 0x450000;
unsigned long R68	= 0x4403E8;
unsigned long R67	= 0x430000;
unsigned long R66	= 0x4201F4;
unsigned long R65	= 0x410000;
unsigned long R64	= 0x401388;
unsigned long R63	= 0x3F0000;
unsigned long R62	= 0x3E0322;
unsigned long R61	= 0x3D00A8;
unsigned long R60	= 0x3C0000;
unsigned long R59	= 0x3B0001;
unsigned long R58	= 0x3A9001;
unsigned long R57	= 0x390020;
unsigned long R56	= 0x380000;
unsigned long R55	= 0x370000;
unsigned long R54	= 0x360000;
unsigned long R53	= 0x350000;
unsigned long R52	= 0x340820;
unsigned long R51	= 0x330080;
unsigned long R50	= 0x320000;
unsigned long R49	= 0x314180;
unsigned long R48	= 0x300300;
unsigned long R47	= 0x2F0300;
unsigned long R46	= 0x2E07FC;
unsigned long R45	= 0x2DD0DF;
unsigned long R44	= 0x2C1FA3;
unsigned long R43	= 0x2B015E;
unsigned long R42	= 0x2A0000;
unsigned long R41	= 0x290000;
unsigned long R40	= 0x280000;
unsigned long R39	= 0x2703E8;
unsigned long R38	= 0x260000;
unsigned long R37	= 0x250304;
unsigned long R36	= 0x240028;
unsigned long R35	= 0x230004;
unsigned long R34	= 0x220000;
unsigned long R33	= 0x211E21;
unsigned long R32	= 0x200393;
unsigned long R31	= 0x1F43EC;
unsigned long R30	= 0x1E318C;
unsigned long R29	= 0x1D318C;
unsigned long R28	= 0x1C0488;
unsigned long R27	= 0x1B0003;
unsigned long R26	= 0x1A0DB0;
unsigned long R25	= 0x190C2B;
unsigned long R24	= 0x18071A;
unsigned long R23	= 0x17007C;
unsigned long R22	= 0x160001;
unsigned long R21	= 0x150401;
unsigned long R20	= 0x14E048;
unsigned long R19	= 0x1327B7;
unsigned long R18	= 0x120064;
unsigned long R17	= 0x11012C;
unsigned long R16	= 0x100080;
unsigned long R15	= 0x0F064F;
unsigned long R14	= 0x0E1E10;
unsigned long R13	= 0x0D4000;
unsigned long R12	= 0x0C5001;
unsigned long R11	= 0x0B0018;
unsigned long R10	= 0x0A10D8;
unsigned long R9	= 0x091604;
unsigned long R8	= 0x082000;
unsigned long R7	= 0x0740B2;
unsigned long R6	= 0x06C802;
unsigned long R5	= 0x0500C8;
unsigned long R4	= 0x040A43;
unsigned long R3	= 0x030642;
unsigned long R2	= 0x020500;
unsigned long R1	= 0x010808;
unsigned long R0	= 0x00251C;


unsigned long R36_LUT_LO[11] = {0x240028,0x240028,0x240028};
unsigned long R43_LUT_LO[11] = {0x2B015E,0x2B028A,0x2B03B6};

void LO_Freq_Write(unsigned long *add_reg){
	pll_x = *add_reg;
	LO_SEN_LOW
	for(x=0;x<24;x++)
	{
		LO_SCK_LOW
		if(pll_x&0x800000)
			LO_SDI_HIGH		//Place data high
        else
        	LO_SDI_LOW		//Place data low
			Delay(40);
        LO_SCK_HIGH
		Delay(40);
		pll_x<<=1;
	}
	LO_SEN_HIGH
	LO_SCK_LOW
	LO_SDI_LOW
}

void Default_LO_Frequency(){
    LO_Freq_Write(&R112);
    LO_Freq_Write(&R111);
    LO_Freq_Write(&R110);
    LO_Freq_Write(&R109);
    LO_Freq_Write(&R108);
    LO_Freq_Write(&R107);
    LO_Freq_Write(&R106);
    LO_Freq_Write(&R105);
    LO_Freq_Write(&R104);
    LO_Freq_Write(&R103);
    LO_Freq_Write(&R102);
    LO_Freq_Write(&R101);
    LO_Freq_Write(&R100);
    LO_Freq_Write(&R99);
    LO_Freq_Write(&R98);
    LO_Freq_Write(&R97);
    LO_Freq_Write(&R96);
    LO_Freq_Write(&R95);
    LO_Freq_Write(&R94);
    LO_Freq_Write(&R93);
    LO_Freq_Write(&R92);
    LO_Freq_Write(&R91);
    LO_Freq_Write(&R90);
    LO_Freq_Write(&R89);
    LO_Freq_Write(&R88);
    LO_Freq_Write(&R87);
    LO_Freq_Write(&R86);
    LO_Freq_Write(&R85);
    LO_Freq_Write(&R84);
    LO_Freq_Write(&R83);
    LO_Freq_Write(&R82);
    LO_Freq_Write(&R81);
    LO_Freq_Write(&R80);
    LO_Freq_Write(&R79);
    LO_Freq_Write(&R78);
    LO_Freq_Write(&R77);
    LO_Freq_Write(&R76);
    LO_Freq_Write(&R75);
    LO_Freq_Write(&R74);
    LO_Freq_Write(&R73);
    LO_Freq_Write(&R72);
    LO_Freq_Write(&R71);
    LO_Freq_Write(&R70);
    LO_Freq_Write(&R69);
    LO_Freq_Write(&R68);
    LO_Freq_Write(&R67);
    LO_Freq_Write(&R66);
    LO_Freq_Write(&R65);
    LO_Freq_Write(&R64);
    LO_Freq_Write(&R63);
    LO_Freq_Write(&R62);
    LO_Freq_Write(&R61);
    LO_Freq_Write(&R60);
    LO_Freq_Write(&R59);
    LO_Freq_Write(&R58);
    LO_Freq_Write(&R57);
    LO_Freq_Write(&R56);
    LO_Freq_Write(&R55);
    LO_Freq_Write(&R54);
    LO_Freq_Write(&R53);
    LO_Freq_Write(&R52);
    LO_Freq_Write(&R51);
    LO_Freq_Write(&R50);
    LO_Freq_Write(&R49);
    LO_Freq_Write(&R48);
    LO_Freq_Write(&R47);
    LO_Freq_Write(&R46);
    LO_Freq_Write(&R45);
    LO_Freq_Write(&R44);
    LO_Freq_Write(&R43);
    LO_Freq_Write(&R42);
    LO_Freq_Write(&R41);
    LO_Freq_Write(&R40);
	LO_Freq_Write(&R39);
	LO_Freq_Write(&R38);
	LO_Freq_Write(&R37);
    LO_Freq_Write(&R36);
	LO_Freq_Write(&R35);
	LO_Freq_Write(&R34);
	LO_Freq_Write(&R33);
	LO_Freq_Write(&R32);
	LO_Freq_Write(&R31);
    LO_Freq_Write(&R30);
    LO_Freq_Write(&R29);
    LO_Freq_Write(&R28);
    LO_Freq_Write(&R27);
    LO_Freq_Write(&R26);
    LO_Freq_Write(&R25);
    LO_Freq_Write(&R24);
    LO_Freq_Write(&R23);
    LO_Freq_Write(&R22);
    LO_Freq_Write(&R21);
    LO_Freq_Write(&R20);
    LO_Freq_Write(&R19);
    LO_Freq_Write(&R18);
    LO_Freq_Write(&R17);
    LO_Freq_Write(&R16);
    LO_Freq_Write(&R15);
    LO_Freq_Write(&R14);
    LO_Freq_Write(&R13);
    LO_Freq_Write(&R12);
    LO_Freq_Write(&R11);
    LO_Freq_Write(&R10);
    LO_Freq_Write(&R9);
    LO_Freq_Write(&R8);
    LO_Freq_Write(&R7);
    LO_Freq_Write(&R6);
    LO_Freq_Write(&R5);
    LO_Freq_Write(&R4);
    LO_Freq_Write(&R3);
    LO_Freq_Write(&R2);
    LO_Freq_Write(&R1);
    LO_Freq_Write(&R0);
}


void Attenuation_control(unsigned int ch)
{
	//ch/=2;
	switch(ch)
	{
	case 1:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 2:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 3:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 4:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 5:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 6:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 7:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 8:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 9:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 10:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 11:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 12:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 13:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 14:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 15:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
			break;

		case 16:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 17:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 18:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 19:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 20:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 21:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 22:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 23:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 24:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 25:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 26:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 27:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 28:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 29:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 30:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		case 31:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_RESET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_RESET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_RESET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_RESET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_RESET);  //BIT5
			break;

		default:
			HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
			HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
			HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
			HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
			HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5
	}
}
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void Unpack_Rec_Data(void)
{
	ucRF_Freq = hex2int(recieve_buffer[1]);
	RF_Freq_Write(&RR43_LUT_RF[ucRF_Freq]);
	RF_Freq_Write(&RR36_LUT_RF[ucRF_Freq]);
	RF_Freq_Write(&RR0);
	ucLO_Freq = hex2int(recieve_buffer[2]);
	LO_Freq_Write(&R36_LUT_LO[ucLO_Freq]);
	LO_Freq_Write(&R43_LUT_LO[ucLO_Freq]);
	LO_Freq_Write(&R0);

	memcpy(temp, &recieve_buffer[3], 2);
	uiAttenuation = atoi(temp);
	Attenuation_control(uiAttenuation);
	HAL_Delay(1);
}

void PLLs_Reset(){
	LO_SEN_HIGH
	LO_SCK_LOW
	LO_SDI_LOW

	RF_SEN_HIGH
	RF_SCK_LOW
	RF_SDI_LOW
	ucRF_Freq = ucLO_Freq=0;
	uiAttenuation = 0;
}

unsigned char LO1_LD,RF_LD,flag1=0;

//************************************************************************************************
int main(void)
{
	MPU_Config();
	SCB_EnableICache();
	SCB_EnableDCache();
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_HSEM_CLK_ENABLE();
	HAL_HSEM_FastTake(HSEM_ID_0);
	HAL_HSEM_Release(HSEM_ID_0,0);
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	osKernelInitialize();
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
	osKernelStart();
	while (1)
	{
	}
}


static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 32;
  RCC_OscInitStruct.PLL.PLLN = 480;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  HAL_GPIO_WritePin(ATT1_GPIO_Port, ATT1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ATT2_GPIO_Port, ATT2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ATT3_GPIO_Port, ATT3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ATT4_GPIO_Port, ATT4_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ATT5_GPIO_Port, ATT5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LO_LE_GPIO_Port, LO_LE_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LO_DATA_GPIO_Port, LO_DATA_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LO_CLK_GPIO_Port, LO_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RF_CLK_GPIO_Port, RF_CLK_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RF_DATA_GPIO_Port, RF_DATA_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RF_LE_GPIO_Port, RF_LE_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = ATT1_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(ATT1_GPIO_Port, &GPIO_InitStruct);

   GPIO_InitStruct.Pin = ATT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATT2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ATT3_Pin;
     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
     HAL_GPIO_Init(ATT3_GPIO_Port, &GPIO_InitStruct);

     GPIO_InitStruct.Pin = ATT4_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(ATT4_GPIO_Port, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = ATT5_Pin;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       HAL_GPIO_Init(ATT5_GPIO_Port, &GPIO_InitStruct);

       /*Configure GPIO pin : RF_CLK_Pin */
       GPIO_InitStruct.Pin = RF_CLK_Pin;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       HAL_GPIO_Init(RF_CLK_GPIO_Port, &GPIO_InitStruct);

       /*Configure GPIO pin : RF_DATA_Pin */
       GPIO_InitStruct.Pin = RF_DATA_Pin;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       HAL_GPIO_Init(RF_DATA_GPIO_Port, &GPIO_InitStruct);

       GPIO_InitStruct.Pin = RF_LE_Pin;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       HAL_GPIO_Init(RF_LE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_CLK_Pin */
  GPIO_InitStruct.Pin = LO_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LO_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_DATA_Pin */
  GPIO_InitStruct.Pin = LO_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LO_DATA_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LO_LE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LO_LE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LO_LD_Pin */
  GPIO_InitStruct.Pin = LO_LD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LO_LD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_LD_Pin */
  GPIO_InitStruct.Pin = RF_LD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RF_LD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CEC_CK_MCO1_Pin */
  GPIO_InitStruct.Pin = CEC_CK_MCO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(CEC_CK_MCO1_GPIO_Port, &GPIO_InitStruct);

  /**/
  HAL_I2CEx_EnableFastModePlus(SYSCFG_PMCR_I2C_PB8_FMP);

}


void StartDefaultTask(void *argument)
{
	    HAL_UART_Transmit(&huart1, (uint8_t *)"AKASH FER SIGNAL GENERATOR\r", sizeof("KU BAND FER SIGNAL GENERATOR"), 300);
	  	PLLs_Reset();
	  	Default_RF_Frequency();   //RF
	  	HAL_Delay(10);
	  	HAL_UART_Transmit(&huart1, (uint8_t *)"RF FREQ PLL LOCKED AT 16.995 GHz and 100MHz Ref\r", sizeof("RF FREQ PLL LOCKED AT 16.995 GHz and 100MHz Ref"), 300);
	  	Default_LO_Frequency();   //LO
	  	HAL_Delay(10);
	  	HAL_UART_Transmit(&huart1, (uint8_t *)"LO FREQENCY PLL LOCKED AT 16.920 GHz and 100MHz Ref\r", sizeof("LO  FREQENCYPLL LOCKED AT 16.995 GHz and 100MHz Ref"), 300);

	  	HAL_GPIO_WritePin(GPIOC,ATT1_Pin,GPIO_PIN_SET);  //BIT1
	  	HAL_GPIO_WritePin(GPIOD,ATT2_Pin,GPIO_PIN_SET);  //BIT2
	  	HAL_GPIO_WritePin(GPIOB,ATT3_Pin,GPIO_PIN_SET);  //BIT3
	  	HAL_GPIO_WritePin(GPIOB,ATT4_Pin,GPIO_PIN_SET);  //BIT4
	  	HAL_GPIO_WritePin(GPIOA,ATT5_Pin,GPIO_PIN_SET);  //BIT5

	  	for(;;)
	  	{
	  		memset(recieve_buffer,0,5);
	  		HAL_UART_Receive(&huart1, recieve_buffer,5, 300);
	  		if(recieve_buffer[0]==2){
	  			Unpack_Rec_Data();
	  		}
	  		else if(recieve_buffer[0]==5)
	  		{
	  			HAL_Delay(10);
	  			LO1_LD = HAL_GPIO_ReadPin (LO_LD_GPIO_Port, LO_LD_Pin);
	  			RF_LD = HAL_GPIO_ReadPin (RF_LD_GPIO_Port, RF_LD_Pin);
	  			memset(UserTxBuffer, 0, sizeof(UserTxBuffer));
	  			sprintf(UserTxBuffer,"%d%d,%d%d%d\n",LO1_LD,RF_LD,ucRF_Freq,ucLO_Freq,uiAttenuation);
	  			HAL_UART_Transmit(&huart1, (uint8_t*)UserTxBuffer,(uint16_t)strlen((const char*)UserTxBuffer), HAL_MAX_DELAY);
	  		}
	  	}
}

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30020000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x30040000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512B;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
