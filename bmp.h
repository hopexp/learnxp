#include <iostream>
#include <string>

using namespace std; 

struct BMP_Header_Data {
public:
	unsigned short		Magic;
	unsigned long int	FileSize;
	unsigned short		Resv1;
	unsigned short		Resv2;
	unsigned long int	Offset;
	unsigned long int	HeaderSize;
	unsigned long int	Xsize;
	unsigned long int	Ysize;
	unsigned short		ColorPlane;
	unsigned short		BPP;
	unsigned long int	CompressSel;
	unsigned long int	ImageBodySize;
	unsigned long int	HorisontalRes;
	unsigned long int	VerticalRes;
	unsigned long int	NumOfColor1;
	unsigned long int	NumOfColor2;
};

struct BMP_Pixel_Data {
public:
	unsigned char		R;
	unsigned char		G;
	unsigned char		B;
};

class bmp {
public :
	bmp(const std::string file_name, const std::string mode);
	~bmp();

	BMP_Header_Data ReadHeader( void );
	BMP_Pixel_Data  ReadPixel( void );
	void WriteHeader( const BMP_Header_Data& BHD );
	void WritePixel ( const BMP_Pixel_Data& BPD );

private :
	FILE *fp;

	unsigned char getChar( void );
	unsigned short getShort( void );
	unsigned long int getLong( void );

	void putChar ( unsigned char c );
	void putShort( unsigned short st );
	void putLong ( unsigned long int i );
};
