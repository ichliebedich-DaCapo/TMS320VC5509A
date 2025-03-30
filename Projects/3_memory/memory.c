#include<app_conf.h>
#if PROGRAM_INDEX==3

#include<stdint.h>

int main()
{
	uint16_t i;
	uint16_t * px;
	uint16_t * py;
	uint16_t * pz;
	
	px=(uint16_t *)0x4080;		// 字地址，字节地址为8100H
	py=(uint16_t *)0x4100;		// 字地址，字节地址为8200H
	for ( i=0,pz=px;i<16;i++,pz++ )
		(*pz)=i;
	for ( i=0,pz=py;i<16;i++,pz++ )	// 在此加软件断点
		(*pz)=0x1234;
	for ( i=0;i<16;i++,px++,py++ )	// 在此加软件断点
		(*py)=(*px);
	while(1)	
	{
	}	// 在此加软件断点
	
}

#endif
