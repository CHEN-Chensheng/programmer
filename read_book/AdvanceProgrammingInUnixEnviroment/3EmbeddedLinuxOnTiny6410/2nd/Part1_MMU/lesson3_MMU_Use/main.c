/*
@Platform: Friendly Arm tiny6410
@CPU: S3C6410
@Function: Bootloader0 ,init cpu 
@Written by Armstrong Wall
@Start wrirte date: 2016.3.3
@Daily: 		2016.3.6  Finish init MMU
*/

#define GPKCON0 (volatile unsigned long*)0xA0008800	 //The Memory address of GPK Control Register
#define GPKDAT  (volatile unsigned long*)0xA0008808  //The Memory address of GPK Data  Register


/* 
 * ���ڶ���������һЩ�궨��
 */ 
#define MMU_FULL_ACCESS     (3<<10)/* ����Ȩ�� */
#define MMU_DOMAIN		      (0<<5)/* �����ĸ��� */
#define MMU_SPECIAL         (1<<4)/* ������1 */
#define MMU_CACHEABLE       (1<<3)/* cacheable */
#define MMU_BUFFERABLE      (1<<2)/* bufferable */
#define MMU_SECTION         (2<<0)/* ��ʾ���Ƕ������� */

#define SECDISC             MMU_SECTION|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS
#define SECDISC_WB          MMU_SECTION|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS|MMU_CACHEABLE|MMU_BUFFERABLE


void create_page_table()
{
	unsigned long *ttb = (unsigned long*) 0x50000000;		//����һ��ָ����ָ��TTB��,��ַ���ڴ���׵�ַ
	unsigned long vaddr,paddr;													//�����ַ ��������ַ
	
	
	/*��GPIO����ӳ��*/
	vaddr = 0xA0000000;
	paddr = 0x7f000000;
	
	*(ttb + (vaddr>>20))= (paddr&0xfff00000)|SECDISC;			//ȡ�������ַ�ĸ�12λ��base��,�������ַ��    
	//����ʵ�ֱ����12λ����������ַ,�������ͨ����ƴ�������ģ��д���ע��
	
	
	/*���ڴ������Ӧ��ӳ��*/
	vaddr = 0x50000000;
	paddr = 0x50000000;
	
	while(vaddr < 0x54000000)//����64MB���ڴ�ӳ�䣬����ԭ��ַӳ��
	{
		*(ttb + (vaddr>>20))= (paddr&0xfff00000)|SECDISC_WB;
		
		vaddr += 0x100000;//��һ�׵ĵ�ַ
		paddr += 0x100000;//��һ�׵ĵ�ַ
		}
	
}

void mmu_init()
{
	__asm__(
	
	/*���ƶ��õ�TTBд�뵽cp15��C2*/
	"ldr r0, =0x50000000\n"
	"mcr p15,0,r0,c2,c0,0\n"  //��TTBװ�뵽cp15��C2�Ĵ�����
	
	
	/*���ʿ��ƻ���(Access Permission )������*/
	"mvn r0,#0\n"
	"mcr p15,0,r0,c3,c0,0\n"  //���öεķ���Ȩ�ޣ���������һ��domain���򣩾����ģ���Ϊ�ɶ���д
	
	/*ʹ��MMU*/
	"mrc p15,0,r0,c1,c0,0\n"  //��cp15��c1�Ĵ�����ֵ����r0��
	"orr r0,r0,#0x0001\n"
	"mcr p15,0,r0,c1,c0,0\n"  //ʹ��MMU,�����������˴�����mcrд����mrc
	:
	:
	);


}


int gboot_main()
{
	//1.����ҳ��
	create_page_table();
	
	//2.д��TTB
	
	
  //3.ʹ��MMU	
  mmu_init();
  
	*(GPKCON0) = 0x11110000;
	*(GPKDAT)  = 0xa0;
	
	
	return 0;
}


/*
int gboot_main()
{
	unsigned long a=500000;
	*(GPKCON0) = 0x11110000;
	
	while(1)
	{
		light_led();
		
	  *(GPKDAT)  = 0xb0;
	  while(a--);
	  a=500000;
	  
	  
	  *(GPKDAT)  = 0x70;
	  while(a--);
	  a=500000;
	  
	  *(GPKDAT)  = 0xd0;
	  while(a--);
	  a=500000;
	  
	  *(GPKDAT)  = 0xe0;
	  while(a--);
	  a=500000;
	  
	  
		
		}
	
	
	
	return 0;
}
*/