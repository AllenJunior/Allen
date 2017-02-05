#include "vector_ram.h"
#include "string.h"
#include "dbg.h"
unsigned int defaultvalue[] = {
         //Address
        //01,00    //03,02   //05,04    //07,06
        0x00000000,0x3D003D00,0x3D003D00,0x3D003D00,//~0x07
        //09,08    //0B,0A   //0D,0C     //0F,0E
	0x3D003D00,0x3D003D00,0x3D003D00,0x3D003D00,//~0xF
        //11,10    //13,12   //15,14     //17,16
	0x00000000,0x00003D00,0x00000000,0x00000000,//~0x17
        //19,18    //1B,1A   //1D,1C     //1F,1E
	0x2DFF0000,0x3D000000,0x2DFF0000,0x3D000000,//~0x1F
	0x3D003D00,0x3D003D00,0x3D003D00,0x3D003D00,//~0x27
	0x3D003D00,0x3D003D00,0x3D003D00,0x00000000,//~0x2F
	0x00003D00,0x3D002DFF,0x2DFF0000,0x00000000,//~0x37
	0x00000000,0x00000000,0x00000000,0x0C011802,//~0x3F
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
	
        
        TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
	TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
        
}

static void PROCESS_DECLARE(command_erase)
{
	const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        
        TraceString("Erase\r\n");
	sram[VOFS_COMMAND] = VCMD_ERASE;
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte;
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte / 4;
        write_sram(0x21,(unsigned int*)defaultvalue,sizeof(defaultvalue));        
        
        
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
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte / 4;
        write_sram(0x21,(unsigned int*)defaultvalue,sizeof(defaultvalue));
        
        TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
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
       
	TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
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
	sram[VOFS_LENGTH] = vector->uiImageLength/info->uiPageSizeInByte; //LoopCounter = 0xC0
	sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte / 2 ; //pageSize = 64Bytes
        
        
        TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
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
