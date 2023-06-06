#include "malloc.h"

//�ڴ��(32�ֽڶ���)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000+320*480*2)));					//�ⲿSRAM�ڴ��

//�ڴ�����
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE+320*480*2)));	//�ⲿSRAM�ڴ��MAP

//�ڴ�������	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};	//�ڴ���С
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};					//�ڴ�ֿ��С
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};							//�ڴ��ܴ�С

//�ڴ���������
struct malloc_cortol_struct malloc_cortol=
{
	my_mem_init,						//�ڴ��ʼ��
	my_mem_perused,						//�ڴ�ʹ����
	mem1base,mem2base,			//�ڴ��
	mem1mapbase,mem2mapbase,//�ڴ����״̬��
	0,0,  		 					//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//len:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void my_mem_cpy(void *des,void *src,u32 len)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(len--)*xdes++=*xsrc++;  
}  

//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//num:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void my_mem_set(void *s,u8 c,u32 num)  
{  
    u8 *xs = s;  
    while(num--)*xs++=c;  
}	   

//�ڴ�����ʼ��  
//memx:�����ڴ��
void my_mem_init(u8 memx)  
{  
    my_mem_set(malloc_cortol.memmap[memx], 0,memtblsize[memx]*2);//�ڴ�״̬����������  
	my_mem_set(malloc_cortol.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
	malloc_cortol.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
}

//��ȡ�ڴ�ʹ����
//memx:�����ڴ��
//����ֵ:ʹ����(0~100)
u8 my_mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(malloc_cortol.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}

//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 xmemb;	//��Ҫ���ڴ����  
	u32 kmemb=0;//�������ڴ����
    u32 i;  
    if(!malloc_cortol.memrdy[memx])malloc_cortol.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    xmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])xmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!malloc_cortol.memmap[memx][offset])kmemb++;//�������ڴ��������
		else kmemb=0;								//�����ڴ������
		if(kmemb==xmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<xmemb;i++)  					//��ע�ڴ��ǿ� 
            {  
                malloc_cortol.memmap[memx][offset+i]=xmemb;  
            }  
            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  

//�ͷ��ڴ�(�ڲ�����) 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!malloc_cortol.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		malloc_cortol.init(memx);    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=malloc_cortol.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            malloc_cortol.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  

//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(u8 memx,void *paddr)  
{  
	u32 offset;   
	if(paddr==NULL)return;//��ַΪ0.  
 	offset=(u32)paddr-(u32)malloc_cortol.membase[memx];     
    my_mem_free(memx,offset);	//�ͷ��ڴ�      
}  

//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)malloc_cortol.membase[memx]+offset);  
}  

//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(u8 memx,void *paddr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    my_mem_cpy((void*)((u32)malloc_cortol.membase[memx]+offset),paddr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx,paddr);  											  		//�ͷž��ڴ�
        return (void*)((u32)malloc_cortol.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }  
}

