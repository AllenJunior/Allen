#include "vector_ram.h"
#include "string.h"
#include "dbg.h"

typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uiSectorSizeInByte;
	unsigned int uiBlockSizeInByte;
	unsigned int uiChipSizeInPage;
	
	unsigned int uiReadIdParameter;
	unsigned int uiEraseParameter;
	unsigned int uiProgramParameter;
	unsigned int uiReadParameter;
	unsigned int uiUnlockParameter;
	unsigned int uiLockParameter;
    unsigned int Rfu;
}IC_INFO,*PIC_INFO;

void CheckStatus(int Timeout)
{
  unsigned Time=0;
  unsigned int val;
  unsigned int cmd=VCMD_RESET_VECTOR;
  do
  {
    read_sram(8,&val,1);
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
      TraceString("vec_reset\r\n");
    }
}

unsigned int get_last_success(const void* cvpIcInformation,unsigned int sram[VSRAM_SIZE]){
	//const IC_INFO* info = (const IC_INFO*)cvpIcInformation;
        
  	TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
	return sram[VOFS_LAST_SUCCESS];
}

static void PROCESS_DECLARE(command_vec_reset)
{
    TraceString("vec_reset\r\n");
    unsigned int cmd = VCMD_RESET_VECTOR;
    
    vector_reset();
    vector_run();
    
    write_sram(VOFS_COMMAND,&cmd,1);
    CheckStatus(50000);
}

static void PROCESS_DECLARE(command_bus_reset)
{
    TraceString("bus reset\r\n");
    unsigned int cmd = VCMD_RESET_BUS;
    
    write_sram(VOFS_COMMAND,&cmd,1);  
    vector_run();
    CheckStatus(50000);
}

static void PROCESS_DECLARE(command_lock)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	sram[VOFS_COMMAND] = VCMD_LOCK;
	sram[VOFS_OP_CODE_METHOD] = info->uiLockParameter;
	sram[VOFS_START_ADDRESS] = 0;
	sram[VOFS_LENGTH] = info->uiChipSizeInPage*info->uiPageSizeInByte;
	sram[VOFS_ID] = *(unsigned int*)info->aucId;
}

static void PROCESS_DECLARE(command_unlock)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	sram[VOFS_COMMAND] = VCMD_UNLOCK;
	sram[VOFS_OP_CODE_METHOD] = info->uiUnlockParameter;
	sram[VOFS_START_ADDRESS] = 0;
	sram[VOFS_LENGTH] = info->uiChipSizeInPage*info->uiPageSizeInByte;
	sram[VOFS_ID] = *(unsigned int*)info->aucId;
}

static void PROCESS_DECLARE(command_initchip)
{
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("initial\r\n");
	sram[VOFS_COMMAND] = VCMD_INIT_CHIP;
         unsigned int DID = *(unsigned int*)info->aucId;
        write_sram(0x17,&DID,1);	
        TraceString("DID=");TraceInt(DID);TraceString("\r\n");
}

static void PROCESS_DECLARE(command_readid)
{       
 	TraceString("ReadId\r\n"); 
	sram[VOFS_COMMAND] = VCMD_READ_ID;   
}

static void PROCESS_DECLARE(command_erase)
{
	sram[VOFS_COMMAND] = VCMD_ERASE;
}

static void PROCESS_DECLARE(command_read)
{
 	TraceString("Read\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        unsigned int StrAddr=vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
        
	sram[VOFS_COMMAND] = VCMD_READ;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_START_ADDRESS] = (StrAddr<<16)&0xFF0000 | StrAddr&0xFF00 | (StrAddr>>16)&0xFF;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
        
  	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
}

static void PROCESS_DECLARE(command_blankcheck)
{
 	TraceString("blankcheck\r\n");  
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        unsigned int StrAddr=vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	sram[VOFS_COMMAND] = VCMD_BLANK_CHECK;
	sram[VOFS_START_ADDRESS] = (StrAddr<<16)&0xFF0000 | StrAddr&0xFF00 | (StrAddr>>16)&0xFF;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
        
        TraceString("uiStartPage=");TraceInt(vector->uiStartPage);TraceString("\r\n");
 	TraceString("ImageLength=");TraceInt(vector->uiImageLength);TraceString("\r\n");
        
}

static void PROCESS_DECLARE(command_program)
{
	TraceString("Program\r\n");
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        unsigned int StrAddr=vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
        
        write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
        
	sram[VOFS_COMMAND] = VCMD_PROGRAM;
	sram[VOFS_OP_AREA] = vector-> usOperateArea;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_START_ADDRESS] = (StrAddr<<16)&0xFF0000 | StrAddr&0xFF00 | (StrAddr>>16)&0xFF;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
        write_sram(0x20,vector->vpParameter,vector->uiParameter/4); 
        TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
  	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
}

static void PROCESS_DECLARE(command_verify)
{
	TraceString("verify\r\n");
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        unsigned int StrAddr=vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	sram[VOFS_COMMAND] = VCMD_VERIFY;
        sram[VOFS_OP_AREA] = vector-> usOperateArea;
        sram[VOFS_START_ADDRESS] = (StrAddr<<16)&0xFF0000 | StrAddr&0xFF00 | (StrAddr>>16)&0xFF;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
        write_sram(0x20,vector->vpParameter,vector->uiParameter/4);
        
        TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        TraceString("VOFS_OP_AREA=");TraceInt(sram[VOFS_OP_AREA]);TraceString("\r\n");
}

static void PROCESS_DECLARE(command_nre)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	write_sram(16,vector->vpParameter,vector->uiParameter/sizeof(unsigned int));
	sram[VOFS_COMMAND] = vector->usOperation;
	sram[VOFS_OP_CODE_METHOD] = info->uiProgramParameter;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
	sram[VOFS_ID] = *(unsigned int*)info->aucId;
}

static BOOL CHK_RDY_DECLARE(chkrdy_readid)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	read_sram(16,sram,(min(vector->uiParameter,info->uiIdLength)+3)/sizeof(unsigned int));
	memcpy(vector->vpParameter,sram,vector->uiParameter);
	if(memcmp(vector->vpParameter,info->aucId,info->uiIdLength)){
		return FALSE;
	}
	return TRUE;
}

static BOOL CHK_RDY_DECLARE(chkrdy_verify)
{
	vector->uiImageChecksum = sram[VOFS_CHECKSUM];
	return TRUE;
}

const VECTOR_FUNC vector_func[16] = {
	{command_vec_reset, NULL},		//VCMD_RESET_VECTOR		0{this will be called before power off, pass or error occured}
	{command_bus_reset,NULL},		//VCMD_RESET_BUS		1
	{command_initchip, NULL},		//VCMD_INIT_CHIP		2
	{command_readid,chkrdy_readid},//VCMD_READ_ID			3
	{command_unlock,NULL},			//VCMD_UNLOCK			4
	{command_erase,NULL},			//VCMD_ERASE			5
	{command_blankcheck,NULL},			//VCMD_BLANK_CHECK		6
	{command_program,NULL},			//VCMD_PROGRAM			7
	{command_verify,chkrdy_verify},	//VCMD_VERIFY			8
	{command_read,NULL},			//VCMD_READ				9
	{command_lock,NULL},			//VCMD_LOCK				10
	{command_nre,NULL},				//VCMD_NRE			11
	{NULL,NULL},					//VCMD_NULL			12
	{NULL,NULL},					//VCMD_NULL			13
	{NULL,NULL},					//VCMD_NULL			14
	{NULL,NULL},					//VCMD_NULL			15
};
