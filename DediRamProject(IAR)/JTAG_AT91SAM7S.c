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
}IC_INFO,*PIC_INFO;

unsigned int get_last_success(const void* cvpIcInformation, unsigned int sram[VSRAM_SIZE]) {
        const IC_INFO* info = (const IC_INFO*)cvpIcInformation;
        //TraceHex32(sram[VOFS_LAST_SUCCESS]);TraceChar(',');
        return sram[VOFS_LAST_SUCCESS] / info->uiPageSizeInByte;
        TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
}
static void PROCESS_DECLARE(command_bus_reset) {
        TraceString("vector_run\r\n");
        vector_run();
}
const unsigned char IAP[] = {
0x00,0x00,0x00,0xEA,0x00,0x00,0xA0,0xE1,0x9F,0x00,0xE0,0xE3,0x00,0x10,0xA0,0xE3, 
0x00,0x10,0x80,0xE5,0xCC,0x01,0x9F,0xE5,0x80,0x1C,0xA0,0xE3,0x00,0x10,0x80,0xE5, 
0xC4,0x01,0x9F,0xE5,0xC4,0x11,0x9F,0xE5,0x00,0x10,0x80,0xE5,0xC0,0x01,0x9F,0xE5, 
0x00,0x10,0x90,0xE5,0x01,0x00,0x11,0xE3,0xFC,0xFF,0xFF,0x0A,0xB4,0x01,0x9F,0xE5, 
0xB4,0x11,0x9F,0xE5,0x00,0x10,0x80,0xE5,0xA4,0x01,0x9F,0xE5,0x00,0x10,0x90,0xE5, 
0x04,0x00,0x11,0xE3,0xFC,0xFF,0xFF,0x0A,0x94,0x01,0x9F,0xE5,0x00,0x10,0x90,0xE5, 
0x08,0x00,0x11,0xE3,0xFC,0xFF,0xFF,0x0A,0x90,0x01,0x9F,0xE5,0x04,0x10,0xA0,0xE3, 
0x00,0x10,0x80,0xE5,0x78,0x01,0x9F,0xE5,0x00,0x10,0x90,0xE5,0x08,0x00,0x11,0xE3, 
0xFC,0xFF,0xFF,0x0A,0x74,0x01,0x9F,0xE5,0x07,0x10,0xA0,0xE3,0x00,0x10,0x80,0xE5, 
0x5C,0x01,0x9F,0xE5,0x00,0x10,0x90,0xE5,0x03,0x00,0x11,0xE3,0xFC,0xFF,0xFF,0x0A, 
0x5C,0x61,0x9F,0xE5,0x14,0x10,0x96,0xE5,0x00,0x00,0x51,0xE3,0xFC,0xFF,0xFF,0x0A, 
0x01,0x00,0x51,0xE3,0x06,0x00,0x00,0x0A,0x02,0x00,0x51,0xE3,0x0F,0x00,0x00,0x0A, 
0x03,0x00,0x51,0xE3,0x32,0x00,0x00,0x0A,0x04,0x00,0x51,0xE3,0x3D,0x00,0x00,0x0A, 
0xFE,0xFF,0xFF,0xEA,0x9B,0x00,0xE0,0xE3,0x28,0x11,0x9F,0xE5,0x00,0x10,0x80,0xE5, 
0x97,0x00,0xE0,0xE3,0x00,0x10,0x90,0xE5,0x01,0x00,0x11,0xE3,0xFC,0xFF,0xFF,0x0A, 
0x10,0x10,0x86,0xE5,0x00,0x10,0xA0,0xE3,0x14,0x10,0x86,0xE5,0xE8,0xFF,0xFF,0xEA, 
0x40,0x00,0x2D,0xE9,0x18,0x50,0x96,0xE5,0x05,0x40,0xA0,0xE1,0x40,0x49,0x44,0xE2, 
0xA4,0x43,0xA0,0xE1,0x1C,0x30,0x96,0xE5,0xA3,0x33,0xA0,0xE1,0x08,0x00,0x2D,0xE9, 
0x20,0x30,0xA0,0xE3,0x20,0x10,0x96,0xE5,0x00,0x10,0x85,0xE5,0x04,0x50,0x85,0xE2, 
0x04,0x60,0x86,0xE2,0x01,0x30,0x53,0xE2,0xF9,0xFF,0xFF,0x1A,0x9B,0x00,0xE0,0xE3, 
0xC4,0x10,0x9F,0xE5,0x04,0x44,0xA0,0xE1,0x04,0x10,0x81,0xE1,0x00,0x10,0x80,0xE5, 
0x24,0x44,0xA0,0xE1,0x01,0x40,0x84,0xE2,0xB0,0x30,0x9F,0xE5,0x01,0x30,0x53,0xE2, 
0xFD,0xFF,0xFF,0x1A,0x97,0x00,0xE0,0xE3,0x00,0x10,0x90,0xE5,0x01,0x00,0x11,0xE3, 
0xFC,0xFF,0xFF,0x0A,0x08,0x00,0xBD,0xE8,0x01,0x30,0x53,0xE2,0xE6,0xFF,0xFF,0x1A, 
0x40,0x00,0xBD,0xE8,0x10,0x10,0x86,0xE5,0x00,0x10,0xA0,0xE3,0x14,0x10,0x86,0xE5, 
0xC3,0xFF,0xFF,0xEA,0x40,0x00,0x2D,0xE9,0x18,0x50,0x96,0xE5,0x1C,0x30,0x96,0xE5, 
0x00,0x00,0xE0,0xE3,0x00,0x10,0x95,0xE5,0x01,0x00,0x71,0xE3,0x02,0x00,0x00,0x1A, 
0x04,0x50,0x85,0xE2,0x01,0x30,0x53,0xE2,0xF9,0xFF,0xFF,0x1A,0x1C,0x10,0x86,0xE5, 
0x40,0x00,0xBD,0xE8,0xB6,0xFF,0xFF,0xEA,0x9B,0x00,0xE0,0xE3,0x40,0x10,0x9F,0xE5, 
0x00,0x10,0x80,0xE5,0x97,0x00,0xE0,0xE3,0x00,0x10,0x90,0xE5,0x01,0x00,0x11,0xE3, 
0xFC,0xFF,0xFF,0x0A,0xAE,0xFF,0xFF,0xEA,0x44,0xFD,0xFF,0xFF,0x20,0xFC,0xFF,0xFF, 
0x01,0x40,0x00,0x00,0x68,0xFC,0xFF,0xFF,0x2C,0xFC,0xFF,0xFF,0x0E,0x10,0x48,0x10, 
0x30,0xFC,0xFF,0xFF,0x00,0x04,0x20,0x00,0x08,0x00,0x00,0x5A,0x01,0x00,0x00,0x5A, 
0xFF,0x1F,0x00,0x00,0x0F,0x00,0x00,0x5A,
};
static void PROCESS_DECLARE(command_IRREBPV) {
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        sram[VOFS_OP_AREA] = vector->usOperateArea; //sram0
        sram[VOFS_COMMAND] = vector->usOperation;	//sram8
        sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte + vector->usOffsetInPage;
        sram[VOFS_LENGTH] = vector->uiImageLength;  //sram3
        sram[VOFS_OP_CODE_METHOD] = info->uiProgramParameter; // for delay time
        sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;	//sram4
        sram[VOFS_ID] = *(unsigned int*)info->aucId;	//sram5
        write_register(0x3030, (vector->ucFlag & VEC_F_FIFO) ? 1 : 0);
        unsigned short stack=read_register(0x304b);
        TraceString("\r\n");
        switch(vector->usOperation) {
        case VCMD_INIT_CHIP:
                TraceString("INIT_CHIP");
                vector->cpCallbacks->set_sktvcc(m_slot, OFF);
                for(int i = 0x1000;i>0;i--);
                vector->cpCallbacks->set_sktvcc(m_slot, ON);
                sram[VOFS_START_ADDRESS] = 0x200000; //ram Base
                sram[VOFS_LENGTH] = sizeof(IAP);
                write_register(0x3030, 1);
                write_fifo(0x3034, (const unsigned short*)IAP, (sram[VOFS_LENGTH] + 1) / sizeof(unsigned short));
                break;
        case VCMD_READ_ID:
                TraceString("READ_ID\r\n"); 
                break;
        case VCMD_READ:
                TraceString("READ\r\n");    
                break;
        case VCMD_ERASE:
                TraceString("ERASE\r\n");
                break;
        case VCMD_BLANK_CHECK:
                TraceString("BLANK\r\n");
                TraceString("stack=");TraceInt(stack);TraceString("\r\n");
                
                //TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
                //TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
                //TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
                //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
                 break;
        case VCMD_PROGRAM:
                TraceString("PROGRAM\r\n");
                sram[VOFS_LENGTH] = vector->uiImageLength / info->uiPageSizeInByte;  //sram3
                sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;	//sram4
                //unsigned short stack=read_register(0x304b);
                TraceString("stack=");TraceInt(stack);TraceString("\r\n");
                if(vector->usOperateArea == 0x01)
                {
                        //TraceHex32(*(unsigned int*)(vector->vpParameter));
                        sram[VOFS_OP_CODE_METHOD] = *(unsigned int*)(vector->vpParameter);
                }
                //TraceString("VOFS_START_ADDRESS=");TraceInt(sram[VOFS_START_ADDRESS]);TraceString("\r\n");
                //TraceString("VOFS_PAGE_SIZE=");TraceInt(sram[VOFS_PAGE_SIZE]);TraceString("\r\n");
                //TraceString("VOFS_LENGTH=");TraceInt(sram[VOFS_LENGTH]);TraceString("\r\n");
                //TraceString("VOFS_LAST_SUCCESS=");TraceInt(sram[VOFS_LAST_SUCCESS]);TraceString("\r\n");
                break;
        case VCMD_VERIFY:
                TraceString("VERIFY\r\n");
                vector->cpCallbacks->set_sktvcc(m_slot, OFF);
                for(int i = 0xF000;i>0;i--);
                vector->cpCallbacks->set_sktvcc(m_slot, ON);
                
                //unsigned short stack=read_register(0x304b);
                TraceString("stack=");TraceInt(stack);TraceString("\r\n");
                break;
        default:
                break;
        }
        if (vector->usOperateArea == 0x02) {
                write_register(0x3030,1);
		write_fifo(0x3034, (const unsigned short*)vector->vpParameter, (vector->uiParameter)>>1);
                //TraceHex32(*(unsigned int *)vector->vpParameter);
	}
        TraceString("\r\n"); 
}
static void PROCESS_DECLARE(command_verify) {
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        sram[VOFS_OP_AREA] = vector->usOperateArea;
        sram[VOFS_COMMAND] = vector->usOperation;
        sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte + vector->usOffsetInPage;
        sram[VOFS_LENGTH] = vector->uiImageLength;
        sram[VOFS_OP_CODE_METHOD] = info->uiProgramParameter; // for delay time
        sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        sram[VOFS_ID] = *(unsigned int*)info->aucId;
        
        vector->cpCallbacks->set_sktvcc(m_slot, OFF);
        for(int i = 0x1000;i>0;i--);
        vector->cpCallbacks->set_sktvcc(m_slot, ON);
        
        write_register(0x3030, (vector->ucFlag & VEC_F_FIFO) ? 1 : 0);
        TraceString("VERIFY");
}
static void PROCESS_DECLARE(command_vec_reset) {
        TraceString("vector_reset\r\n");
        vector_reset();
}

static BOOL CHK_RDY_DECLARE(chkrdy_verify) {        /*sram allready read in check_ready() */
        vector->uiImageChecksum = sram[VOFS_CHECKSUM];
        return TRUE;
}
static BOOL CHK_RDY_DECLARE(chkrdy_readid) {
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        read_sram(VOFS_ERROR_CODE, sram, (min(vector->uiParameter, info->uiIdLength) + 3) / sizeof(unsigned int));
        memcpy(vector->vpParameter, sram, vector->uiParameter);
        if (memcmp(vector->vpParameter, info->aucId, info->uiIdLength)) {
                return FALSE;
        }
        return TRUE;
}

static void PROCESS_DECLARE(command_lock) {
        TraceString("command_lock");
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        sram[VOFS_COMMAND] = VCMD_LOCK;
        sram[VOFS_OP_CODE_METHOD] = info->uiLockParameter;
        sram[VOFS_START_ADDRESS] = 0;
        sram[VOFS_LENGTH] = vector->uiImageLength;
        sram[VOFS_ID] = *(unsigned int*)info->aucId;
}
static void PROCESS_DECLARE(command_unlock) {
        TraceString("command_unlock");
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        sram[VOFS_COMMAND] = VCMD_UNLOCK;
        sram[VOFS_OP_CODE_METHOD] = info->uiUnlockParameter;
        sram[VOFS_START_ADDRESS] = 0;
        sram[VOFS_LENGTH] = vector->uiImageLength;
        sram[VOFS_ID] = *(unsigned int*)info->aucId;
}
static void PROCESS_DECLARE(command_nre) {
        TraceString("command_nre");
        const IC_INFO* info = (const IC_INFO*)vector->cvpIcInformation;
        write_sram(16, vector->vpParameter, vector->uiParameter / sizeof(unsigned int));
        sram[VOFS_COMMAND] = vector->usOperation;
        sram[VOFS_OP_CODE_METHOD] = info->uiProgramParameter;
        sram[VOFS_START_ADDRESS] = vector->uiStartPage*info->uiPageSizeInByte + vector->usOffsetInPage;
        sram[VOFS_LENGTH] = vector->uiImageLength;
        sram[VOFS_PAGE_SIZE] = info->uiPageSizeInByte;
        sram[VOFS_ID] = *(unsigned int*)info->aucId;
}

const VECTOR_FUNC vector_func[16] = {
        { command_vec_reset, NULL },		//VCMD_RESET_VECTOR		0{this will be called before power off, pass or error occured}
        { command_bus_reset,NULL },			//VCMD_RESET_BUS		1
        { command_IRREBPV, NULL },			//VCMD_INIT_CHIP		2
        { command_IRREBPV, chkrdy_readid },	//VCMD_READ_ID			3
        { command_unlock,NULL },			//VCMD_UNLOCK			4
        { command_IRREBPV,NULL },           //VCMD_ERASE			5
        { command_IRREBPV,NULL },           //VCMD_BLANK_CHECK		6
        { command_IRREBPV,NULL },           //VCMD_PROGRAM			7
        { command_verify,chkrdy_verify },	//VCMD_VERIFY			8
        { command_IRREBPV,NULL },           //VCMD_READ			9
        { command_lock,NULL },				//VCMD_LOCK			10
        { command_nre,NULL },				//VCMD_NRE			11
        { NULL,NULL },						//VCMD_NULL			12
        { NULL,NULL },						//VCMD_NULL			13
        { NULL,NULL },						//VCMD_NULL			14
        { NULL,NULL },						//VCMD_NULL			15
};
#define rfuncCMD	0x0D
#define rfuncDAT	0x0E
#define rfuncCMDrst	0x00
void check_ready(VECTOR* vector, unsigned int sram[VSRAM_SIZE]) {
        switch (sram[rfuncCMD]) {
        case 0x02:
                vector->cpCallbacks->set_sktvcc(m_slot, OFF);
                goto rstFlag;
        case 0x04:
                vector->cpCallbacks->set_sktvcc(m_slot, ON);
                goto rstFlag;
        case 0x08:
                //TraceHex32(sram[0x0E]);TraceChar(',');
                goto rstFlag;
        default:
                break;
        rstFlag:
                sram[rfuncCMD] = rfuncCMDrst;
                write_sram(rfuncCMD, &sram[rfuncCMD], 1);
                break;
        }
                //TraceHex32(read_register(0x304B)); TraceChar(',');      //print pc pointer
        write_register(0x3030, (vector->ucFlag & VEC_F_FIFO) ? 1 : 0);
        if (sram[VOFS_RESPONSE] & VRES_BIT_READY) {
                if (sram[VOFS_RESPONSE] & VRES_BIT_ERROR)
                        vector->ucFlag |= (VEC_F_READY | VEC_F_FAIL);
                else {
                        vector->ucFlag |= VEC_F_READY;
                        if (vector_func[vector->usOperation].chk_rdy) {
                                if (vector_func[vector->usOperation].chk_rdy(vector, sram) == FALSE)
                                        vector->ucFlag |= (VEC_F_FAIL);
                        }
                        else if (vector->usOperation == VCMD_VERIFY) {
                                vector->uiImageChecksum = sram[VOFS_CHECKSUM];
                        }
                }
        }
        vector->uiLastSuccess = get_last_success(vector->cvpIcInformation, sram);
}
void ic_ramfunction(VECTOR* vector) {
        unsigned int sram[VSRAM_SIZE] = { 0 };
        if (vector->ucFlag & VEC_F_TRIGGED) {
                vector->usOperation = GetOperate();
                read_sram(0x00, sram, VSRAM_SIZE);
#ifdef _DEBUG
                if (sram[VOFS_RESPONSE] & VRES_BIT_ERROR) {
                        TraceString("\r\noperation fail!\r\n");
                        unsigned int fail_res; unsigned char i;
                        for (i = 0x00; i < 0xF0; i++) {
                                read_sram(i, &fail_res, 1);
                                //TraceHex32(fail_res); TraceChar(',');
                        }
                }
#endif
                check_ready(vector, sram);
        }
        else if ((vector->usOperation < 16) && vector_func[vector->usOperation].process) {
                SetOperate(vector->usOperation);
                vector_func[vector->usOperation].process(vector, sram);//update operation perameter
                write_sram(0, sram, sizeof(sram) / sizeof(sram[0])); //write to sram												 //is triged//vector busy
                vector->ucFlag |= VEC_F_TRIGGED;
                vector->ucFlag &= ~VEC_F_READY;
        }
        else {
                vector->ucFlag |= (VEC_F_READY | VEC_F_FAIL);
        }
}