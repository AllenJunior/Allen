#include "vector_ram.h"
#include "string.h"
#include "dbg.h"

#define CALI_PARA 13
#define PIC_INIT 6
#define ER_PARA 12
#define GET_CALI 14
#define VERIFY_CALI 15

/*
typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uiSectorSizeInByte;
	unsigned int uiBlockSizeInByte;
	unsigned int uiChipSizeInPage;
	
	unsigned int uInitParameter;
	unsigned int uiEraseParameter;
	unsigned int uiProgramParameter;
	unsigned int uiReadParameter;
	unsigned int uiUnlockParameter;
	unsigned int uiLockParameter;
}IC_INFO,*PIC_INFO;
*/

typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uInitParameter;
	unsigned int uiReadParameter;
	unsigned int uiEraseParameter; //chipsize-2 
}IC_INFO,*PIC_INFO;

unsigned int get_last_success(const void* cvpIcInformation,unsigned int sram[VSRAM_SIZE]){
	unsigned int val;
	unsigned int GetOP;
	const IC_INFO* info = (const IC_INFO*)cvpIcInformation;
        
	read_sram(VOFS_COMMAND,&val,1);
	read_sram(VOFS_OP_AREA,&GetOP,1);
        
	if(((val&0xF) == VCMD_BLANK_CHECK || (val&0xF) == VCMD_VERIFY)  && (GetOP==0)){
		//TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]/info->uiPageSizeInByte*2); TraceString("\r\n");  
		return sram[VOFS_LAST_SUCCESS]/info->uiPageSizeInByte*2;
	}
	else{
		//TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]); TraceString("\r\n");  
		return sram[VOFS_LAST_SUCCESS];
	}

}

void CheckStatus(int Timeout)
{
	unsigned Time=0;
	unsigned int val;
	unsigned int cmd=VCMD_RESET_VECTOR;
	do
	{
		read_sram(VOFS_COMMAND,&val,1);
		if(val & 0x8000)break; //ready
		Time++;
	}while(Time<Timeout);
	  
	if(Time>Timeout)
	{
		vector_reset();
		vector_run();
    
		write_sram(VOFS_COMMAND,&cmd,1);
		for(int i = 0;i<10000;i++){
			read_sram(VOFS_COMMAND,&val,1);
			if(val & 0x8000)
				return;
		}
		TraceString("StatusTimeout_vec_reset\r\n");
	}
}

static void PROCESS_DECLARE(command_vec_reset)
{
	TraceString("vec reset\r\n");
	
	unsigned int cmd = VCMD_RESET_VECTOR;
	
	vector_reset();
	vector_run();
	write_sram(VOFS_COMMAND,&cmd,1);
	CheckStatus(10000);
}
static void PROCESS_DECLARE(command_bus_reset)
{
	TraceString("bus reset\r\n");
	
	unsigned int cmd = VCMD_RESET_BUS;
	
	write_sram(VOFS_COMMAND,&cmd,1);
	vector_run();
	CheckStatus(10000);
}

static void PROCESS_DECLARE(command_initchip)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	TraceString("Initial\r\n");
	//write_sram(15,(unsigned int*)&Value,1);
	sram[VOFS_COMMAND] = VCMD_INIT_CHIP;
	sram[PIC_INIT] = info->uInitParameter;
	sram[CALI_PARA] = (((info->uInitParameter&0x0F00)>>8)+1) | (info->uInitParameter&0xF000)<<4; //for PIC16F
	sram[ER_PARA] = info->uiEraseParameter; //for PIC12F
	sram[15] = info->aucId[0] | (info->aucId[1]<<8);

	TraceString("info->uInitParameter=");TraceInt(info->uInitParameter); TraceString("\r\n");
	TraceString("ER_PARA=");TraceInt(sram[ER_PARA]); TraceString("\r\n");
	TraceString("CALI_PARA=");TraceInt(sram[CALI_PARA]); TraceString("\r\n");
}

static BOOL CHK_RDY_DECLARE(chkrdy_initchip)
{
#if 1
	unsigned int Value;
	for(int i=0;i<4;i++)
	{
		read_sram(0x10+i,&Value,1);
		TraceString("Value=");TraceInt(Value); TraceString("\r\n");  
	}
#endif
	return TRUE;
}

static void PROCESS_DECLARE(command_readid)
{
	TraceString("ReadId\r\n"); 
	sram[VOFS_COMMAND] = VCMD_READ_ID;
	//sram[VOFS_OP_CODE_METHOD] = ((const IC_INFO*)vector->cvpIcInformation)->uiReadIdParameter;    
}

static BOOL CHK_RDY_DECLARE(chkrdy_readid)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	read_sram(15,sram,(min(vector->uiParameter,info->uiIdLength)+3)/sizeof(unsigned int));
	memcpy(vector->vpParameter,sram,vector->uiParameter);
	if(memcmp(vector->vpParameter,info->aucId,info->uiIdLength)){
		return FALSE;
	}
	return TRUE;
}

static void PROCESS_DECLARE(command_erase)
{
	TraceString("Erase\r\n"); 
        
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;;
        
	sram[VOFS_COMMAND] = VCMD_ERASE;
	sram[PIC_INIT] = info->uInitParameter;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[ER_PARA] = info->uiEraseParameter; //for PIC12F
        
	if(vector-> usOperateArea==0)
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage; 
	else
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*2+vector->usOffsetInPage;   
        
	TraceString("ER_PARA=");TraceInt(sram[ER_PARA]); TraceString("\r\n");
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]); TraceString("\r\n");
}

static void PROCESS_DECLARE(command_read)
{
	TraceString("Read\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	sram[VOFS_COMMAND] = VCMD_READ;
	sram[PIC_INIT] = info->uInitParameter;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2; 

	if(vector-> usOperateArea==0)
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage; 
	else
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*2+vector->usOffsetInPage;   
        
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]); TraceString("\r\n");
}

static void PROCESS_DECLARE(command_blankcheck)
{
	TraceString("Blankcheck\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	sram[VOFS_COMMAND] = VCMD_BLANK_CHECK;
	sram[PIC_INIT] = info->uInitParameter;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[ER_PARA] = info->uiEraseParameter; //for PIC12F

	if(vector-> usOperateArea==0)
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage; 
	else
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*2+vector->usOffsetInPage;       
        
	TraceString("ER_PARA=");TraceInt(sram[ER_PARA]); TraceString("\r\n"); 
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]); TraceString("\r\n");
}


static void PROCESS_DECLARE(command_verify)
{
	TraceString("Verify\r\n");  

	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	write_sram(0x20,vector->vpParameter,vector->uiParameter/4);  
        
	sram[VOFS_COMMAND] = VCMD_VERIFY;
	sram[PIC_INIT] = info->uInitParameter;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[ER_PARA] = info->uiEraseParameter; //for PIC12F
        
	if(vector-> usOperateArea==0)
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage; 
	else
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*2+vector->usOffsetInPage;       
	
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]); TraceString("\r\n");
}

static void PROCESS_DECLARE(command_program)
{
	TraceString("Program\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	write_sram(0x20,vector->vpParameter,vector->uiParameter/4);  
         
	sram[VOFS_COMMAND] = VCMD_PROGRAM;
	sram[PIC_INIT] = info->uInitParameter;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
	sram[ER_PARA] = info->uiEraseParameter; //for PIC12F
        
	if(vector-> usOperateArea==0){
		sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage; 
	}
	else{
		sram[VOFS_LENGTH] = vector->uiImageLength/2;
		sram[VOFS_START_ADDRESS] = vector->uiStartPage*2+vector->usOffsetInPage;   
	}
        
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]); TraceString("\r\n");
}

static BOOL CHK_RDY_DECLARE(chkrdy_program_erase)
{
	TraceString("chkrdy_Program_erase\r\n"); 
        
	unsigned int cmd = VERIFY_CALI;
	unsigned int Value;
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        
	Value = info->uInitParameter;
	write_sram(PIC_INIT,&Value,1);
	Value = (((info->uInitParameter&0x0F00)>>8)+1) | (info->uInitParameter&0xF000)<<4; //for PIC16F
	write_sram(CALI_PARA,&Value,1);
	
	//start verify calibration
	write_sram(VOFS_COMMAND,&cmd,1);
	CheckStatus(10000);
	
	read_sram(VOFS_COMMAND,&cmd,1);
	if((cmd&0xC000) == 0xC000)
		return FALSE;
	else
		return TRUE;
}

static BOOL CHK_RDY_DECLARE(chkrdy_verify)
{
	return TRUE;
}

const VECTOR_FUNC vector_func[16] = {
	{command_vec_reset, NULL},		//VCMD_RESET_VECTOR		0{this will be called before power off, pass or error occured}
	{command_bus_reset,NULL},		//VCMD_RESET_BUS		1
	{command_initchip,chkrdy_initchip},		//VCMD_INIT_CHIP		2
	{command_readid,chkrdy_readid},//VCMD_READ_ID			3
	{NULL,NULL},			//VCMD_UNLOCK			4
	{command_erase,chkrdy_program_erase},			//VCMD_ERASE			5
	{command_blankcheck,NULL},			//VCMD_BLANK_CHECK		6
	{command_program,chkrdy_program_erase},			//VCMD_PROGRAM			7
	{command_verify,chkrdy_verify},	//VCMD_VERIFY			8
	{command_read,NULL},			//VCMD_READ				9
	{NULL,NULL},			//VCMD_LOCK				10
	{NULL,NULL},				//VCMD_NRE			11
	{NULL,NULL},					//VCMD_NULL			12
	{NULL,NULL},					//VCMD_NULL			13
	{NULL,NULL},					//VCMD_NULL			14
	{NULL,NULL},					//VCMD_NULL			15
};
