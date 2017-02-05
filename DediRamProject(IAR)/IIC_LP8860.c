#include "vector_ram.h"
#include "string.h"
#include "dbg.h"
/*
Programming Area EEPROM REG0 to EEPROM REG24 
*/
unsigned int typeA_default[] = {
        0xED,0xDF,0xDC,0xF0,0xDF,0xE5,0xF2,0x77,
        0x77,0x71,0x3F,0xB7,0x17,0xEF,0xB0,0x87,
        0xCE,0x72,0xE5,0xDF,0x35,0x06,0xDC,0x88,
        0x3E
};
unsigned int typeB_default[]={
        0x6E,0x02,0x5C,0xC2,0x5C,0xEC,0x78,0x77,
        0x77,0xE1,0xBF,0x00,0xC0,0xF7,0x00,0x07,
        0x0F,0xF2,0xE5,0x10,0x35,0x06,0x8A,0xDF,
        0x81
};
unsigned int typeD_default[]={
  
        0xEF,0xFF,0xDC,0xF0,0xDF,0xE5,0xF8,0x77,
        0x77,0x71,0x3F,0xB7,0x17,0xEF,0xB0,0x87,
        0xCE,0x73,0xE5,0xD2,0x35,0x06,0xDC,0xFF,
        0x3E
};
unsigned int typeE_default[]={
        0xF2,0x00,0xD4,0xDB,0xDF,0x1D,0x72,0x77,
        0x77,0x71,0x3B,0x00,0xCA,0x10,0xB0,0x84,
        0xC4,0xF3,0xE5,0xDC,0x35,0x06,0xDC,0xFF,
        0x3E
};

typedef struct _st_icinfo{
	unsigned char aucId[8];
	unsigned int uiIdLength;
	unsigned int uiPageSizeInByte;
	unsigned int uiSectorSizeInByte;
	unsigned int uiBlockSizeInByte;
	unsigned int uiChipSizeInPage;
	unsigned int uiInitParameter;
	unsigned int uiReadIdParameter;
	unsigned int uiEraseParameter;
	unsigned int uiProgramParameter;
	unsigned int uiReadParameter;
	unsigned int uiUnlockParameter;
	unsigned int uiLockParameter;
}IC_INFO,*PIC_INFO;

BOOL CheckStack()
{
	BOOL Result=TRUE;
	unsigned short stack=read_register(0x304B);
	
	if(stack!=0x17F){
		vector_reset();
		TraceString("StackOverFlow_vec_reset\r\n");
		return FALSE;
	}
	
	return Result;
}

BOOL CheckStatus(int Timeout)
{
	//TraceString("CheckStatus\r\n");
	unsigned Time=0;
	unsigned int val;
	unsigned int cmd=VCMD_RESET_VECTOR;
	BOOL Result=TRUE;

	do
	{
		read_sram(VOFS_COMMAND,&val,1);
		if(val & 0x8000)break; //ready
		Time++;
	}while(Time<Timeout);
  
	if(Time>=Timeout)
	{
		Result=FALSE;
		vector_reset();
		vector_run();
    
		write_sram(VOFS_COMMAND,&cmd,1);
		for(int i = 0;i<10000;i++)
		{
			read_sram(VOFS_COMMAND,&val,1);
			if(val & 0x8000)
				break;
		}
		TraceString("Timeout_vec_reset\r\n");
	}
	
	return Result;
}

unsigned int get_last_success(const void* cvpIcInformation,unsigned int sram[VSRAM_SIZE])
{
	const IC_INFO* info = (const IC_INFO*)cvpIcInformation;
	//TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
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
	TraceString("bus_reset\r\n");
	
	unsigned int cmd = VCMD_RESET_BUS;
	
	CheckStack();
        
	vector_run();
	write_sram(VOFS_COMMAND,&cmd,1);
	CheckStatus(50000);

}

static void PROCESS_DECLARE(command_initchip)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("Initial\r\n");
	sram[VOFS_COMMAND] = VCMD_INIT_CHIP;
}

static void PROCESS_DECLARE(command_readid)
{
	TraceString("ReadId\r\n");
	sram[VOFS_COMMAND] = VCMD_READ_ID;
}

static BOOL CHK_RDY_DECLARE(chkrdy_readid)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	unsigned int id;
	read_sram(0x20,&id,1);
	TraceString("id=");TraceInt(id);TraceString("\r\n");
	read_sram(0x20,sram,(min(vector->uiParameter,info->uiIdLength)+3)/sizeof(unsigned int));
	memcpy(vector->vpParameter,sram,vector->uiParameter);
	if(memcmp(vector->vpParameter,info->aucId,info->uiIdLength)){
		return FALSE;
	}
	return TRUE;
}

static void PROCESS_DECLARE(command_read)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("Read\r\n"); 
	sram[VOFS_COMMAND] = VCMD_READ;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte;
        sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;        
        /*
        TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
        */
}

static void PROCESS_DECLARE(command_erase)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        
        TraceString("Erase\r\n");
	sram[VOFS_COMMAND] = VCMD_ERASE;
        sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        
        
        unsigned int type_Select = info->uiInitParameter;
        if(type_Select == 0)
        {
          write_sram(0x21,(unsigned int*)typeA_default,sizeof(typeA_default));
        }
        else if(type_Select == 1)
        {
          write_sram(0x21,(unsigned int*)typeB_default,sizeof(typeB_default));
        }
        else if(type_Select == 2)
        {
          write_sram(0x21,(unsigned int*)typeD_default,sizeof(typeD_default));
        }
        else if(type_Select == 3)
        {
          write_sram(0x21,(unsigned int*)typeE_default,sizeof(typeE_default));
        }
        //TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
               
}

static void PROCESS_DECLARE(command_blankcheck)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("BlankCheck\r\n"); 
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
        
	sram[VOFS_COMMAND] = VCMD_BLANK_CHECK;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        
               
        unsigned int type_Select = info->uiInitParameter;
        if(type_Select == 0)
        {
          write_sram(0x21,(unsigned int*)typeA_default,sizeof(typeA_default));
        }
        else if(type_Select == 1)
        {
          write_sram(0x21,(unsigned int*)typeB_default,sizeof(typeB_default));
        }
        else if(type_Select == 2)
        {
          write_sram(0x21,(unsigned int*)typeD_default,sizeof(typeD_default));
        }
        else if(type_Select == 3)
        {
          write_sram(0x21,(unsigned int*)typeE_default,sizeof(typeE_default));
        }
        
        //TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");

}


static void PROCESS_DECLARE(command_verify)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("Verify\r\n");
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
        
	sram[VOFS_COMMAND] = VCMD_VERIFY;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        
	//TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");

}

static void PROCESS_DECLARE(command_program)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("Program\r\n");
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
        
	sram[VOFS_COMMAND] = VCMD_PROGRAM;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        
              
        //TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
 
}

static BOOL CHK_RDY_DECLARE(chkrdy_verify)
{
        vector->uiImageChecksum = sram[VOFS_CHECKSUM];
	return TRUE;
}

const VECTOR_FUNC vector_func[16] = {
	{command_vec_reset,NULL},		//VCMD_RESET_VECTOR		0{this will be called before power off, pass or error occured}
	{command_bus_reset,NULL},		//VCMD_RESET_BUS		1
	{command_initchip, NULL},		//VCMD_INIT_CHIP		2
	{command_readid,chkrdy_readid},         //VCMD_READ_ID			3
	{NULL,NULL},			        //VCMD_UNLOCK			4
	{command_erase,NULL},			//VCMD_ERASE			5
	{command_blankcheck,NULL},		//VCMD_BLANK_CHECK		6
	{command_program,NULL},			//VCMD_PROGRAM			7
	{command_verify, chkrdy_verify},	//VCMD_VERIFY			8
	{command_read,NULL},			//VCMD_READ			9
	{NULL,NULL},			        //VCMD_LOCK			10
	{NULL,NULL},				//VCMD_NRE			11
	{NULL,NULL},				//VCMD_NULL			12
	{NULL,NULL},				//VCMD_NULL			13
	{NULL,NULL},				//VCMD_NULL			14
	{NULL,NULL},				//VCMD_NULL			15
};
