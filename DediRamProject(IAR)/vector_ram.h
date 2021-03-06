#ifndef _VEC_RAM_H_
#define _VEC_RAM_H_

//#define FF_SDRAM_TRIGGER
extern __no_init unsigned short m_slot;
extern unsigned short s_uiOperate[8];
extern unsigned short s_gpio_dir;

typedef enum {OFF=0,ON=!OFF}SWITCH;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define max(a,b)				( ((a)>(b))?(a):(b) )			//ȡ��
#define min(a,b)				( ((a)>(b))?(b):(a) )			//ȡС

	#define VCMD_RESET_VECTOR	        0
	#define VCMD_RESET_BUS		        1
	#define VCMD_INIT_CHIP		        2
	#define VCMD_READ_ID		        3
	#define VCMD_UNLOCK			4
	#define VCMD_ERASE			5
	#define VCMD_BLANK_CHECK	        6
	#define VCMD_PROGRAM		        7
	#define VCMD_VERIFY			8
	#define VCMD_READ			9
	#define VCMD_LOCK			10

	#define VOFS_OP_AREA			0
	#define VOFS_OP_CODE_METHOD		1
	#define VOFS_START_ADDRESS		2
	#define VOFS_START_PAGE			2
	#define VOFS_LENGTH			3
	#define VOFS_PAGE_NUMBER		3
	#define VOFS_SECTOR_NUMBER		3
	#define VOFS_BLOCK_NUMBER		3
	#define VOFS_PAGE_SIZE			4
	#define VOFS_SECTOR_SIZE		4
	#define VOFS_BLOCK_SIZE			4
	#define VOFS_ID				5
	#define VOFS_COMMAND			8
	#define VOFS_RESPONSE			8
	#define VOFS_LAST_SUCCESS		9
	#define VOFS_CHECKSUM			10
	#define VOFS_ERROR_CODE			15
        
	#define VOFS_PATTERN_DIR		16
	#define VOFS_PATTERN_VALUE		17

	#define VSRAM_SIZE			16

	#define VRES_GET_OPERATION(v)	((v)[VOFS_RESPONSE]&0xF)

	#define VRES_BIT_READY			(1<<15)
	#define VRES_BIT_ERROR			(1<<14)
	#define VRES_BIT_UNSUPPORT		(1<<13)

//#define VCODE_PATTERN2			0x3000
//#define VCODE_PATTERN1			0x1000

#define VEC_F_TRIGGED		(1<<4)
#define VEC_F_READY			(1<<5)
#define VEC_F_FAIL			(1<<0)
	#define VEC_F_FIFO			(1<<1)
	#define VEC_F_SDRAM			(0<<1)

#ifdef FF_SDRAM_TRIGGER

	#define VEC_F_IS_FIFO(v)	((((v)->usFlag & VEC_F_FIFO) == VEC_F_FIFO) && (v)->uiImageLength)
	#define VEC_F_IS_SDRAM(v)	((((v)->usFlag & VEC_F_FIFO) == VEC_F_SDRAM) && (v)->uiImageLength)
#endif

typedef struct __st_callback{
	void (*set_sktvcc)(unsigned short,SWITCH);
	void (*set_sktvpp)(unsigned short,SWITCH);
//	unsigned int (*write)(void*,const void*,unsigned int,unsigned int*);
//	unsigned int (*read)(void*,void*,unsigned int,unsigned int*);
//	void* rw_target;
	unsigned int (*write_fifo)(unsigned short,unsigned int);
	unsigned int (*read_fifo)(unsigned short,unsigned int);
}VEC_CALLBACKS,*PVEC_CALLBACKS;

typedef struct __st_vector{
	unsigned char  ucSlot;
	unsigned char  ucFlag;
	unsigned short usOperation;
	unsigned int   uiLastSuccess;
	
	unsigned short usOperateArea;
	unsigned short usOffsetInPage;
	unsigned int   uiStartPage;
	unsigned int   uiImageLength;
	unsigned int   uiImageChecksum;
	
//	unsigned int   uiSdRamAddress;
//	unsigned int   uiSdRamLength;
//	unsigned int   uiSdRamChecksum;
	
	unsigned int   uiParameter;
	void*          vpParameter;
	const void*    cvpIcInformation;
	const VEC_CALLBACKS* cpCallbacks;
#ifdef _SIMULATE
	unsigned int   debug_ticks;
	unsigned short debugging[2];
#endif
}VECTOR,*PVECTOR;

typedef enum __e_boolean{
	FALSE = 0,
	TRUE = 1
}BOOL;

typedef void (*PFUNC_PROCESS)(const VECTOR* vector,unsigned int sram[VSRAM_SIZE]);
typedef BOOL (*PFUNC_CHK_RDY)(VECTOR* vector,unsigned int sram[VSRAM_SIZE]);
#define PROCESS_DECLARE(name)	name(const VECTOR* vector,unsigned int sram[VSRAM_SIZE])
#define CHK_RDY_DECLARE(name)	name(VECTOR* vector,unsigned int sram[VSRAM_SIZE])

typedef struct __st_vec_func{
	PFUNC_PROCESS process;
	PFUNC_CHK_RDY chk_rdy;
}VECTOR_FUNC;

/*
#pragma section = ".entry"
#pragma location = ".entry"

#define VECTOR_PLACEMENT() ((PVECTOR)*(unsigned int*)__section_begin(".entry"))
//#define VECTOR_PLACEMENT() ((PVECTOR)*(unsigned int*)(0x20100000))
*/

#define VECTOR_ENTRY		0x20100000

#define VECTOR_PLACEMENT() ((PVECTOR)*(unsigned int*)VECTOR_ENTRY)

extern void ic_ramfunction(PVECTOR vector);

#define BOARD_FPGA_CMD_ADDR		(unsigned short*)(0x60000002)
#define BOARD_FPGA_DAT_ADDR		(unsigned short*)(0x60000000)

#define FPGA_RW_DELAY()	for(int i=0;i<5;i++)
#define _FPGA_SET_REG(a)	(*BOARD_FPGA_CMD_ADDR = (a))
#define _FPGA_SET_DATA(a)	(*BOARD_FPGA_DAT_ADDR = (a))
#define _FPGA_GET_DATA()	(*BOARD_FPGA_DAT_ADDR)

static inline void write_register(unsigned short usAddress, unsigned short usData){
	usAddress += (m_slot<<8);
	_FPGA_SET_REG(usAddress);
	FPGA_RW_DELAY();
	_FPGA_SET_DATA(usData);
}

static inline void write_fifo( unsigned short usAddress, const unsigned short* cuspData,unsigned int uiNumberOfData){
	usAddress += (m_slot<<8);
	_FPGA_SET_REG(usAddress);
	FPGA_RW_DELAY();
	while(uiNumberOfData--)
		_FPGA_SET_DATA(*cuspData++);
}

static inline unsigned short read_register( unsigned short usAddress){
	usAddress += (m_slot<<8);
	_FPGA_SET_REG(usAddress);
	FPGA_RW_DELAY();
	return _FPGA_GET_DATA();
}

static inline void read_fifo( unsigned short usAddress, unsigned short* uspData,unsigned int uiNumberOfData){
	usAddress += (m_slot<<8);
	_FPGA_SET_REG(usAddress);
	FPGA_RW_DELAY();
	while(uiNumberOfData--)
		*uspData++ = _FPGA_GET_DATA();
}

static inline void write_sram(unsigned short usAddress,const unsigned int* cuipBuffer,unsigned int uiNumberOfData){
	write_register(0x3021,usAddress<<1);
	write_fifo(0x3020,(const unsigned short*)cuipBuffer,uiNumberOfData<<1);
}

static inline void read_sram(unsigned short usAddress,unsigned int* uipBuffer,unsigned int uiNumberOfData){
	write_register(0x3021,usAddress<<1);
	read_fifo(0x3020,(unsigned short*)uipBuffer,uiNumberOfData<<1);
}

static inline void gpio_dir(unsigned char mask){
	s_gpio_dir = ((unsigned short)mask)<<8;
}

static inline void gpio_write(unsigned char mask){
	unsigned short data = s_gpio_dir + mask;
	write_register(0x3060,data);
}

static inline unsigned char gpio_read(){
	return read_register(0x3060);
}

#define BUS_RUN				(1<<1)
#define BUS_STEP			(1<<2)
#define BUS_RESET			(1<<3)
#define BUS_BREAK_POINT		(1<<4)

static inline void vector_reset(){
	write_register(0x3010,BUS_RESET);
	write_register(0x3010,0x00);
}

static inline void vector_run(){
	write_register(0x3010,BUS_RUN);
}

#define GetOperate()				s_uiOperate[m_slot]
#define SetOperate(uiOperate)	(s_uiOperate[m_slot] = uiOperate)

#define GPIO_OUT				1
#define GPIO_IN					0
#define GPIO_DIR(bit,io)		((io)<<(bit))

#define VectorSetSocketVcc(v,o)		((v)->set_sktvcc(m_slot,o))
#define VectorSetSocketVpp(v,o)		((v)->set_sktvpp(m_slot,o))
#define VectorWriteFifo(v,o)		((v)->write_fifo(m_slot,o))
#define VectorReadFifo(v,o)		((v)->read_fifo(m_slot,o))

#endif

