#include "vector_ram.h"
#include "string.h"
#include "dbg.h"

//trim value=(1/((feq/7)*64/2))*128 us


#define DEVICEID 0xF
#define REG_LOC 6
#define INT_PARA 7
#define EE_ADDR 11

/*
typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uiSectorSizeInByte;
	unsigned int uiBlockSizeInByte;
	unsigned int uiChipSizeInPage;
	
	unsigned int uiReadIdParameter;
	unsigned int uiEraseParameter;
	unsigned int uiEraseParameter;
	unsigned int uiReadParameter;
	unsigned int uiUnlockParameter;
	unsigned int uiProtectAddr;
    unsigned int Rfu;
}IC_INFO,*PIC_INFO;
*/

typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uiChipSizeInPage;
	unsigned int uiInitParameter;
	unsigned int uiEraseParameter; //eeprom addr
	unsigned int uiConfigLocation;
}IC_INFO,*PIC_INFO;

unsigned int get_last_success(const void* cvpIcInformation,unsigned int sram[VSRAM_SIZE]){
	//unsigned int value;
	
	//value=read_register(0x304b);
	//TraceString("stack=");TraceInt(value); TraceString("\r\n"); 
	TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]); TraceString("\r\n"); 
	return sram[VOFS_LAST_SUCCESS];
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
  
	//TraceString("BREAK\r\n");
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
		
		TraceString("timeout_vec_reset\r\n");
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
	CheckStatus(10000);
	vector_run();
}
static void PROCESS_DECLARE(command_initchip)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	TraceString("Initial\r\n");
	sram[VOFS_COMMAND] = VCMD_INIT_CHIP;	
	sram[INT_PARA] = info->uiInitParameter;
	sram[EE_ADDR] = info->uiEraseParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	
	TraceString("EE_ADDR=");TraceInt(sram[EE_ADDR]); TraceString("\r\n"); 
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n"); 
	TraceString("INT_PARA=");TraceInt(sram[INT_PARA]); TraceString("\r\n"); 
}

static void PROCESS_DECLARE(command_readid)
{
	TraceString("Read ID\r\n");
	sram[VOFS_COMMAND] = VCMD_READ_ID;
}

static void PROCESS_DECLARE(command_erase)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	
	sram[VOFS_COMMAND] = VCMD_ERASE;
	sram[EE_ADDR] = info->uiEraseParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	
	TraceString("EE_ADDR=");TraceInt(sram[EE_ADDR]); TraceString("\r\n"); 
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n"); 

}

static void PROCESS_DECLARE(command_read)
{
	TraceString("Read\r\n");  
        
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	
	sram[VOFS_COMMAND] = VCMD_READ;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[REG_LOC] = info ->uiConfigLocation;
	sram[INT_PARA] = info->uiInitParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[EE_ADDR] = info->uiEraseParameter;
	
	TraceString("INT_PARA=");TraceInt(sram[INT_PARA]); TraceString("\r\n"); 
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");
	TraceString("VOFS_OP_AREA=");TraceInt(sram[VOFS_OP_AREA]); TraceString("\r\n"); 
}

static void PROCESS_DECLARE(command_blankcheck)
{
 	TraceString("BlankCheck\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
         
	sram[VOFS_COMMAND] = VCMD_BLANK_CHECK;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[REG_LOC] = info ->uiConfigLocation;
	sram[INT_PARA] = info->uiInitParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[EE_ADDR] = info->uiEraseParameter;
	
	//TraceString("PROTECT_ADDR=");TraceInt(sram[PROTECT_ADDR]);TraceString("\r\n"); 
	//TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
	//TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
}


static void PROCESS_DECLARE(command_verify)
{
	TraceString("Verify\r\n");
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	
	sram[VOFS_COMMAND] = VCMD_VERIFY;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[REG_LOC] = info ->uiConfigLocation;
	sram[INT_PARA] = info->uiInitParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[EE_ADDR] = info->uiEraseParameter;
}

static BOOL CHK_RDY_DECLARE(chkrdy_verify)
{
	return TRUE;
}

static void PROCESS_DECLARE(command_program)
{
	TraceString("Program\r\n");

	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;

	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	write_sram(0x10,vector->vpParameter,vector->uiParameter/4);

	sram[VOFS_COMMAND] = VCMD_PROGRAM;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[REG_LOC] = info ->uiConfigLocation;
	sram[INT_PARA] = info->uiInitParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/2;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[EE_ADDR] = info->uiEraseParameter;
	
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]); TraceString("\r\n");      
	TraceString("REG_LOC=");TraceInt(sram[REG_LOC]); TraceString("\r\n");      
	TraceString("VOFS_OP_AREA=");TraceInt(sram[VOFS_OP_AREA]); TraceString("\r\n");     
}

static BOOL CHK_RDY_DECLARE(chkrdy_readid)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	unsigned int DeviceId;

	read_sram(DEVICEID,&DeviceId,1);
	TraceString("SDID=");TraceInt(DeviceId);TraceString("\r\n");
	DeviceId=DeviceId&0x0FFF;

	memcpy(vector->vpParameter,&DeviceId,4);
	if(memcmp(vector->vpParameter,&info->aucId,4)){
		return FALSE;
	}

	return TRUE;
}

const VECTOR_FUNC vector_func[16] = {
	{command_vec_reset,NULL},		//VCMD_RESET_VECTOR		0{this will be called before power off, pass or error occured}
	{command_bus_reset,NULL},		//VCMD_RESET_BUS		1
	{command_initchip, NULL},		//VCMD_INIT_CHIP		2
	{command_readid,chkrdy_readid},//VCMD_READ_ID			3
	{NULL,NULL},			//VCMD_UNLOCK			4
	{command_erase,NULL},			//VCMD_ERASE			5
	{command_blankcheck,NULL},			//VCMD_BLANK_CHECK		6
	{command_program,NULL},			//VCMD_PROGRAM			7
	{command_verify,chkrdy_verify},	//VCMD_VERIFY			8
	{command_read,NULL},			//VCMD_READ				9
	{NULL,NULL},			//VCMD_LOCK				10
	{NULL,NULL},				//VCMD_NRE			11
	{NULL,NULL},					//VCMD_NULL			12
	{NULL,NULL},					//VCMD_NULL			13
	{NULL,NULL},					//VCMD_NULL			14
	{NULL,NULL},					//VCMD_NULL			15
};
