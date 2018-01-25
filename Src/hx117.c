#include "hx117.h"
#include "stm32f10x.h"    


void Delay_MS(u16 dly)
{
        u16 i,j;
        for(i=0;i<dly;i++)
        for(j=1000;j>0;j--);
}
void delay_us(u16 dly1)
{
        u16 i;
        for(i=dly1;i>0;i--);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : ��ʼ��GPIO����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Hx117Init(void)
{
	//CLK:PB6    Data:PB7
	GPIO_InitTypeDef        GPIO_InitStructure;                //����һ���ṹ�����

	        //---------����Ӧ����ʱ��--------------------
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);        //ʹ��APB2�����GPIOA��ʱ��        

	GPIO_InitStructure.GPIO_Pin = HX117DOUT_PIN;         //DIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;         //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�Ϊ50MHz
	GPIO_Init(HX117_GPIO,&GPIO_InitStructure);                                 //��ʼ��PB5

	GPIO_InitStructure.GPIO_Pin = HX117CLK_PIN; //CLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //�ܽ�Ƶ��Ϊ50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_Init(HX117_GPIO,&GPIO_InitStructure);                                 //��ʼ��PB5
}



uint32_t Read_HX711(void)  //��HX711оƬ��������ݡ� 
{ 
	uint32_t val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(HX117_GPIO,HX117DOUT_PIN);    //DOUT=1 
	GPIO_ResetBits(HX117_GPIO,HX117CLK_PIN);    //SCK=0 
	while(GPIO_ReadInputDataBit(HX117_GPIO,HX117DOUT_PIN));   //�ȴ�DOUT=0  
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(HX117_GPIO,HX117CLK_PIN);    //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(HX117_GPIO,HX117CLK_PIN);    //SCK=0 
		if(GPIO_ReadInputDataBit(HX117_GPIO,HX117DOUT_PIN))   //DOUT=1 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(HX117_GPIO,HX117CLK_PIN); //SCK
	delay_us(1); 
	GPIO_ResetBits(HX117_GPIO,HX117CLK_PIN); //SCK
	delay_us(1);  
	return val;  
}   




