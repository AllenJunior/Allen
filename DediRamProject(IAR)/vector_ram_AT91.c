#include "vector_ram.h"
#include <string.h>
#include "dbg.h"

__no_init unsigned short s_uiOperate[8];
__no_init unsigned short s_gpio_dir;

extern const VECTOR_FUNC vector_func[];
extern unsigned int get_last_success(const void* info,unsigned int sram[VSRAM_SIZE]);

static void check_ready(VECTOR* vector,unsigned int sram[VSRAM_SIZE]){
	read_sram(0,sram,VSRAM_SIZE);
	//if(sram[VOFS_RESPONSE] & (1UL<<31))
        //TraceString("VOFS_RESPONSE");TraceInt(sram[VOFS_RESPONSE]);TraceString("\r\n");   
	if(sram[VOFS_RESPONSE] & VRES_BIT_READY)
        {
                if(sram[VOFS_RESPONSE] & VRES_BIT_ERROR){
                        TraceString("CHK_FAIL");TraceString("\r\n");   
			vector->ucFlag |= (VEC_F_READY|VEC_F_FAIL);          
                }
		else {
			vector->ucFlag |= VEC_F_READY;
			if(vector_func[vector->usOperation].chk_rdy)
                        {

				if(vector_func[vector->usOperation].chk_rdy(vector,sram)==FALSE)
					vector->ucFlag |= (VEC_F_FAIL);
                              
			}
			else if(vector->usOperation == VCMD_VERIFY){
				vector->uiImageChecksum = sram[VOFS_CHECKSUM];  
			}
 
		}
	}
	else if(VRES_GET_OPERATION(sram)==vector->usOperation){
		;
	}
	//else{
	//	vector->ucFlag |= (VEC_F_READY|VEC_F_FAIL);
        //}
        
	vector->uiLastSuccess = get_last_success(vector->cvpIcInformation,sram);     
}
/*
void ic_ramfunction(VECTOR* vector){
	unsigned int sram[VSRAM_SIZE] = {0};
	if(vector->ucFlag & VEC_F_TRIGGED){
		vector->usOperation = GetOperate();
		check_ready(vector,sram);
	}
	else if((vector->usOperation < 16) && vector_func[vector->usOperation].process){
		SetOperate(vector->usOperation);
		vector_func[vector->usOperation].process(vector,sram);
		vector->ucFlag |= VEC_F_TRIGGED;
		vector->ucFlag &=~VEC_F_READY;
		write_sram(0,sram,16);
	}
	else{
		vector->ucFlag |= (VEC_F_READY|VEC_F_FAIL);
	}
}
*/