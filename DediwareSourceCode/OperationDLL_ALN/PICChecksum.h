

typedef struct _st_opt_para{
	unsigned short ProtectPos;
	unsigned short MaskValue[3];
}PIC16F_PARAMETER;

typedef struct _st_opt_device{
	const PIC16F_PARAMETER* parameter;
}DEVICE;

const PIC16F_PARAMETER pic16f690[] = {
	{ 0x0040, { 0x0FFF, 0, 0 } }
};
const PIC16F_PARAMETER pic16f639[] = {
	{ 0x0040, { 0x1FFF, 0, 0 } }
};
const PIC16F_PARAMETER pic16f616[] = {
	{ 0x0040, { 0x03FF, 0, 0 } }
};
const PIC16F_PARAMETER pic12f617[] = {
	{ 0x0040, { 0x0FFF, 0, 0 } }
};
const PIC16F_PARAMETER pic16f946[] = {
	{ 0x0040, { 0x0FFF, 0, 0 } }
};
const DEVICE device[] = {
	{0},
	{pic16f690},
	{pic16f639},
	{pic16f616},
	{pic12f617},
	{pic16f946}
};