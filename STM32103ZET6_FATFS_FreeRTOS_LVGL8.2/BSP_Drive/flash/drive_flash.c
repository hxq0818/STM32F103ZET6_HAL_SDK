#include "drive_flash.h"
#include "drive_spi.h"
#include "SysTick.h"   
#include "usart.h"



u16 EN25QXX_TYPE=EN25Q128;	//Ĭ����EN25Q128


//4KbytesΪһ��Sector
//16������Ϊ1��Block
//EN25Q128
//����Ϊ16M�ֽ�,����256��Block,4096��Sector 												 
//��ʼ��SPI FLASH��IO��
void EN25QXX_Init(void)
{
	u8 temp;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();           //ʹ��GPIOBʱ��
    
    //PB12
    GPIO_InitStructure.Pin=GPIO_PIN_12;            //PB12
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;     //����         
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);     //��ʼ��
	
	EN25QXX_CS=1;			//SPI FLASH��ѡ��
	SPI2_Init();		   			        //��ʼ��SPI
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_2); //����Ϊ45Mʱ��,����ģʽ		
	EN25QXX_TYPE=EN25QXX_ReadID();	//��ȡFLASH ID.
	printf("EN25QXX_TYPE=%X\r\n",EN25QXX_TYPE);
	if(EN25QXX_TYPE==EN25Q256)                //SPI FLASHΪEN25Q256
    {
        temp=EN25QXX_ReadSR(3);              //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp&0X01)==0)			        //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{
			EN25QXX_CS=0; 			        //ѡ��
			SPI2_ReadWriteByte(EN25X_Enable4ByteAddr);//���ͽ���4�ֽڵ�ַģʽָ��   
			EN25QXX_CS=1;       		        //ȡ��Ƭѡ   
		}
    }
}

//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
u8 EN25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=EN25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=EN25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=EN25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=EN25X_ReadStatusReg1;    
            break;
    }    
	EN25QXX_CS=0;                            //ʹ������   
	SPI2_ReadWriteByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI2_ReadWriteByte(0Xff);          //��ȡһ���ֽ�  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
void EN25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=EN25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
            break;
        case 2:
            command=EN25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
            break;
        case 3:
            command=EN25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
            break;
        default:
            command=EN25X_WriteStatusReg1;    
            break;
    }   
	EN25QXX_CS=0;                            //ʹ������   
	SPI2_ReadWriteByte(command);            //����дȡ״̬�Ĵ�������    
	SPI2_ReadWriteByte(sr);                 //д��һ���ֽ�  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void EN25QXX_Write_Enable(void)   
{
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_WriteEnable);   //����дʹ��  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void EN25QXX_Write_Disable(void)   
{  
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_WriteDisable);  //����д��ָֹ��    
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256
u16 EN25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	EN25QXX_CS=0;				    
	SPI2_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	    
	SPI2_ReadWriteByte(0x00); 	 			   
	Temp|=SPI2_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI2_ReadWriteByte(0xFF);	 
	EN25QXX_CS=1;				    
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void EN25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_ReadData);      //���Ͷ�ȡ����  
    if(EN25QXX_TYPE==EN25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((ReadAddr)>>24));    
    }
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));   //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI2_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);    //ѭ������  
    }
	EN25QXX_CS=1;  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void EN25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    EN25QXX_Write_Enable();                  //SET WEL 
	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_PageProgram);   //����дҳ����   
    if(EN25QXX_TYPE==EN25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((WriteAddr)>>24)); 
    }
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI2_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ 
	EN25QXX_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void EN25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		EN25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 EN25QXX_BUFFER[4096];		 
void EN25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * EN25QXX_BUF;	  
   	EN25QXX_BUF=EN25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		EN25QXX_Read(EN25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(EN25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			EN25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				EN25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EN25QXX_Write_NoCheck(EN25QXX_BUF,secpos*4096,4096);//д����������  

		}else EN25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void EN25QXX_Erase_Chip(void)   
{                                   
    EN25QXX_Write_Enable();                  //SET WEL 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_ChipErase);        //����Ƭ��������  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
	EN25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void EN25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    EN25QXX_Write_Enable();                  //SET WEL 	 
    EN25QXX_Wait_Busy();   
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_SectorErase);   //������������ָ�� 
    if(EN25QXX_TYPE==EN25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((Dst_Addr)>>24)); 
    }
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    EN25QXX_Wait_Busy();   				    //�ȴ��������
}  
//�ȴ�����
void EN25QXX_Wait_Busy(void)   
{   
	while((EN25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void EN25QXX_PowerDown(void)   
{ 
  	EN25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(EN25X_PowerDown);     //���͵�������  
	EN25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                            //�ȴ�TPD  
}   
//����
void EN25QXX_WAKEUP(void)   
{  
  	EN25QXX_CS=0;                                //ʹ������   
    SPI2_ReadWriteByte(EN25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	EN25QXX_CS=1;                                //ȡ��Ƭѡ     	      
    delay_us(3);                                //�ȴ�TRES1
}   

