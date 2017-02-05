#include "vector_ram.h"
#include "string.h"
#include "dbg.h"
unsigned int defaultvalue[] = 
{
       0xB3008E93,0x240840A6,0x00FA7968,0x40A600FA,
       0x79680240,0x40404040,0x7D800000,0x00000000,
       0x008080AA,0x170E0202,0x02023881,0x1D420600,
       0x400E1814,0x07F244B8,0x80800000,0x0020F10B,
       0x578F35FC,0x8F2901DD,0x11870701,0x70D8AFAA,
       0xFCBFFCFF,0x47000F80,0xA8560200,0x00000000,
       0x00005A03,0x00000000,0x00000000,0x8197FFFE,
       0x8930AD88,0xCD000100,0x00008E00,0x000000F0,
       0xFFFF0000,0xFCFFFF3F,0x80FF0002,0x066000C0,
       0xFFCD03C8,0x95390000,0x00000080,0x0C400A00,
       0x69006B80,0x6D006603,0x00000091,0xD4F2FF8B,
       0x615B6DB5,0xD5565BCD,0x3F004071,0xA91ABBAC,
       0xD250C500,0xFFFFF114,0x141414FF,0xFF22FE00,
       0x01000062,0x6A606C64,0x68627000,0x00003524,
};

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
	TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
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
}

static void PROCESS_DECLARE(command_erase)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("Erase\r\n");	
	sram[VOFS_COMMAND] = VCMD_ERASE;
        sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
	write_sram(0x32,(unsigned int*)defaultvalue,sizeof(defaultvalue));

}

static void PROCESS_DECLARE(command_blankcheck)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
	TraceString("BlankCheck\r\n"); 
	write_register(0x3030,(vector->ucFlag & VEC_F_FIFO)?1:0);
	sram[VOFS_COMMAND] = VCMD_BLANK_CHECK;
	sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte+vector->usOffsetInPage;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        sram[VOFS_LAST_SUCCESS] = vector->uiStartPage;
        write_sram(0x32,(unsigned int*)defaultvalue,sizeof(defaultvalue));       
        //TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        //TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
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
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
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
