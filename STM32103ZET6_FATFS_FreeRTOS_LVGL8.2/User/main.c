#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "drive_led.h"
#include "tftlcd.h"
#include "touch.h"
#include "malloc.h"
#include "drive_sdio_sdcard.h"
#include "drive_flash.h"
#include "ff.h" 
#include "fatfs_app.h"
#include "drive_key.h"
#include "font_show.h"
#include "drive_sram.h"
#include "drive_time.h"


#include "FreeRTOS_text.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

//#include "st7789v.h"
//#include "cst816t.h"

const u8 text_buf[]="www.prechin.net";
uint8_t data[] = "huangxiaoqing" ;
uint8_t data1[15];
uint8_t data2[15];
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
//	u8 i=0;
	u8 key;
	FIL* FF_FILE;
	FRESULT ff_res;
	uint32_t wr,br;
	uint8_t data_buff[16];
	lv_fs_file_t lv_fs;
	lv_fs_res_t lv_res;
//	WRCACE_InitTypedef WRCACE_InitStruct;
//	WRCACE_InitStruct.Adaptive_Brightness_Goal=0x10;
//	WRCACE_InitStruct.Color_Enhancement_Cmd=1;
//	WRCACE_InitStruct.Color_Enhancement_Extent=11;
	
	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(RCC_PLL_MUL9); //设置时钟,72M
	SysTick_Init(72);
	USART1_Init(115200);
	TIM6_Init(1000-1,72-1);  //定时1ms
	LED_Init();
	TFTLCD_Init();			//LCD初始化
	TP_Init();
	KEY_Init();
	EN25QXX_Init();				//初始化EN25Q128	
	FSMC_SRAM_Init();  
	my_mem_init(SRAMIN);		//初始化内部内存池
//	my_mem_init(SRAMEX);		//初始化内部内存池
	
	FRONT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN STM32F1");	
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"Font Test");
	FRONT_COLOR=BLUE;	//设置字体为蓝色
	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");
		delay_ms(500);
		LED2=!LED2;
		LCD_Fill(10,80,tftlcd_data.width,80+16,BACK_COLOR);
	}
	
	FATFS_Init();				//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 		//挂载SD卡
	f_mount(fs[1],"1:",1); 		//挂载SPI FLASH
	
//	ST7789V_LcdInit();
//	ST7789V_AdaptiveBrightnessColorEnhancementInit(&WRCACE_InitStruct);
//	printf("ST7789V Ready!\n");
//	CST816T_Init();
//	printf("CST816T Ready!\n");
	
	while(font_init()) 		        //检查字库
	{
//Update:    
		LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"Font Updating...");
		key=update_font(10,100,16,"0:");//更新字库
		while(key)//更新失败		
		{		
			 LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Failed! ");
			 delay_ms(200);
		} 		  
		LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Font Update Success!    ");
		delay_ms(1500);
	} 
	LCD_ShowFontString(10,130,tftlcd_data.width,tftlcd_data.height,"普中科技-PRECHIN",16,0);
	LCD_ShowFontString(10,150,tftlcd_data.width,tftlcd_data.height,"www.prechin.net",16,0);
	LCD_ShowFontString(10,170,tftlcd_data.width,tftlcd_data.height,"字库显示实验",16,0);
	LCD_ShowFontString(10,190,tftlcd_data.width,tftlcd_data.height,"K_UP键进行字库更新...",16,0);
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_bmp_init();
	
	printf("*********FATFS*********\r\n");
	FF_FILE = (FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	ff_res = f_open(FF_FILE,"0:huangxiaoqing.txt",FA_CREATE_NEW);
	printf("0:huangxiaoqing.txt-->create ff_res:%d\r\n",ff_res);
	ff_res = f_open(FF_FILE,"0:huangxiaoqing.txt",FA_WRITE);
	printf("0:huangxiaoqing.txt-->open ff_res:%d\r\n",ff_res);
	ff_res = f_write(FF_FILE,&text_buf,sizeof(text_buf),&wr);
	printf("0:huangxiaoqing.txt-->write ff_res:%d wr:%d\r\n",ff_res,wr);
	ff_res = f_close(FF_FILE);
	myfree(SRAMIN,FF_FILE);	//释放内存
	printf("0:huangxiaoqing.txt-->close ff_res:%d\r\n",ff_res);
	
	FF_FILE = (FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	ff_res = f_open(FF_FILE,"0:huangxiaoqing.txt",FA_READ);
	printf("0:huangxiaoqing.txt-->open ff_res:%d\r\n",ff_res);
	ff_res = f_read(FF_FILE,&data_buff,sizeof(text_buf),&br);
	printf("0:huangxiaoqing.txt-->read ff_res:%d wr:%d\r\n",ff_res,br);
	ff_res = f_close(FF_FILE);
	myfree(SRAMIN,FF_FILE);	//释放内存
	printf("0:huangxiaoqing.txt-->close ff_res:%d\r\n",ff_res);
	
	FF_FILE = (FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	ff_res = f_open(FF_FILE,"1:huangxiaoqing.txt",FA_CREATE_NEW);
	printf("1:huangxiaoqing.txt-->create ff_res:%d\r\n",ff_res);
	ff_res = f_open(FF_FILE,"1:huangxiaoqing.txt",FA_WRITE);
	printf("1:huangxiaoqing.txt-->open ff_res:%d\r\n",ff_res);
	ff_res = f_write(FF_FILE,&text_buf,sizeof(text_buf),&wr);
	printf("1:huangxiaoqing.txt-->write ff_res:%d wr:%d\r\n",ff_res,wr);
	ff_res = f_close(FF_FILE);
	myfree(SRAMIN,FF_FILE);	//释放内存
	printf("1:huangxiaoqing.txt-->close ff_res:%d\r\n",ff_res);
	
	FF_FILE = (FIL*)mymalloc(SRAMIN,sizeof(FIL));	//分配内存	
	ff_res = f_open(FF_FILE,"1:huangxiaoqing.txt",FA_READ);
	printf("1:huangxiaoqing.txt-->open ff_res:%d\r\n",ff_res);
	ff_res = f_read(FF_FILE,&data_buff,sizeof(text_buf),&br);
	printf("1:huangxiaoqing.txt-->read ff_res:%d wr:%d\r\n",ff_res,br);
	ff_res = f_close(FF_FILE);
	myfree(SRAMIN,FF_FILE);	//释放内存
	printf("1:huangxiaoqing.txt-->close ff_res:%d\r\n",ff_res);
	
	lv_res = lv_fs_open(&lv_fs,"0:huangxiaoqing.txt",LV_FS_MODE_RD);
	printf("lv_fs-->open lv_res:%d\r\n",lv_res);
	lv_res = lv_fs_read(&lv_fs,data2,sizeof(data),&br);
	printf("lv_fs-->read lv_res:%d br:%d data2:%s\r\n",lv_res,br,&data2[0]);
	lv_res = lv_fs_close(&lv_fs);
	printf("lv_fs-->close lv_res:%d \r\n",lv_res);
	
//	lv_res = lv_fs_open(&lv_fs,"0:huangxiaoqing.txt",LV_FS_MODE_WR);
//	printf("lv_fs-->open lv_res:%d\r\n",lv_res);
//	lv_res = lv_fs_write(&lv_fs,&text_buf,sizeof(text_buf),&wr);
//	printf("lv_fs-->write lv_res:%d\r\n",lv_res);
//	lv_res = lv_fs_close(&lv_fs);
//	printf("lv_fs-->close lv_res:%d \r\n",lv_res);
	
	printf("*********FATFS*********\r\n");
	
	printf("*********GUI*********\r\n");
//	lv_obj_t *label = lv_label_create(lv_scr_act());
//	lv_obj_set_style_text_color(label,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
//	lv_obj_set_style_bg_color(lv_scr_act(),lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
//	lv_obj_set_style_text_font(label,&lv_font_montserrat_14,LV_STATE_DEFAULT);
//	lv_label_set_text(label,"Hello World!!!");
	printf("*********GUI*********\r\n");
	
	printf("*********RTOS*********\r\n");
	freeRTOS_start_dome();
	printf("*********RTOS*********\r\n");
	
	while(1)
	{
//		key=KEY_Scan(0);
//		if(key==KEY_UP_PRESS)
//			goto Update;
//		i++;
//		if(i%10==0)
//		{
//			LED1=!LED1;
//		}
//		delay_ms(10);	
	}
}
