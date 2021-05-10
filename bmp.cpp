#include "bmp.h"

bmp::bmp(const std::string file_name, const std::string mode) {
	if(mode == "READ"){
		if( (fp = fopen(file_name.c_str(),"rb" )) == NULL ) {
			cout << "Input File open Error! : " << file_name  << endl;
			exit(1);
		}
	}else if(mode == "WRITE"){
		if( (fp = fopen(file_name.c_str(),"wb")) == NULL ){
			cout << "Output file create error! : " << file_name << endl;
			exit(1);
		}
	}else{
		cout << "Code Error ! : The constractor 2nd argument should be set READ or WRITE. " << mode << " is invalid." << endl;
	exit(1);
	}
}

bmp::~bmp() {
  fclose(fp);
}

BMP_Header_Data bmp::ReadHeader(void) {
	BMP_Header_Data		BHD = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	BHD.Magic		= getShort();
	BHD.FileSize 		= getLong();
	BHD.Resv1		= getShort();
	BHD.Resv2		= getShort();
	BHD.Offset		= getLong();
	BHD.HeaderSize		= getLong();
	BHD.Xsize		= getLong();
	BHD.Ysize		= getLong();
	BHD.ColorPlane		= getShort();
	BHD.BPP			= getShort();
	BHD.CompressSel		= getLong();
	BHD.ImageBodySize	= getLong();
	BHD.HorisontalRes	= getLong();
	BHD.VerticalRes		= getLong();
	BHD.NumOfColor1		= getLong();
	BHD.NumOfColor2		= getLong();
	return( BHD );
}

BMP_Pixel_Data bmp::ReadPixel(void) {
	BMP_Pixel_Data	BPD = {0,0,0};
	BPD.B	= getChar();
	BPD.G	= getChar();
	BPD.R	= getChar();
	return( BPD );
}

unsigned char bmp::getChar(void) {
	unsigned char c0 = fgetc(fp);
	return( c0 );
}

unsigned short bmp::getShort(void) {
	unsigned char c0 = fgetc(fp);
	unsigned char c1 = fgetc(fp);
	return( (short) (c1<<8) + (short) c0 );
}

unsigned long int bmp::getLong(void) {
	unsigned char c0 = fgetc(fp);
	unsigned char c1 = fgetc(fp);
	unsigned char c2 = fgetc(fp);
	unsigned char c3 = fgetc(fp);
	return( (unsigned long int) (c3<<24) + (unsigned long int) (c2<<16) + (unsigned long int) (c1<<8) + (unsigned long int) c0 );
}

void bmp::WriteHeader(const BMP_Header_Data& BHD) {
	putShort(BHD.Magic);
	putLong(BHD.FileSize);
	putShort(BHD.Resv1);
	putShort(BHD.Resv2);
	putLong(BHD.Offset);
	putLong(BHD.HeaderSize);
	putLong(BHD.Xsize);
	putLong(BHD.Ysize);
	putShort(BHD.ColorPlane);
	putShort(BHD.BPP);
	putLong(BHD.CompressSel);
	putLong(BHD.ImageBodySize);
	putLong(BHD.HorisontalRes);
	putLong(BHD.VerticalRes);
	putLong(BHD.NumOfColor1);
	putLong(BHD.NumOfColor2);
}

void bmp::WritePixel( const BMP_Pixel_Data& BPD ) {
	putChar(BPD.B);
	putChar(BPD.G);
	putChar(BPD.R);
}

void bmp::putChar(unsigned char c) {
	fputc(c, fp);
	return;
}

void bmp::putShort(unsigned short st) {
	unsigned char c0 = (unsigned char)  (st & 0x00ff);
	unsigned char c1 = (unsigned char) ((st & 0xff00) >> 8);
	fputc(c0, fp);
	fputc(c1, fp);
	return;
}

void bmp::putLong(unsigned long int i) {
	unsigned char c0 = (unsigned char)  (i & 0x000000ff);
	unsigned char c1 = (unsigned char) ((i & 0x0000ff00) >> 8);
	unsigned char c2 = (unsigned char) ((i & 0x00ff0000) >> 16);
	unsigned char c3 = (unsigned char) ((i & 0xff000000) >> 24);
	fputc(c0, fp);
	fputc(c1, fp);
	fputc(c2, fp);
	fputc(c3, fp);
	return;
}
