#include "bmp.h"

int clip0M(int data, int max) {

	int data_clip;
	if(data < 0) {
		data_clip = 0;
	} else if(data >= max) {
		data_clip = max-1;
	} else {
		data_clip = data;
	}

	return data_clip;
}

void coef_sel( int flt, int coef[4]) {
	switch(flt) {
	case 0: coef[0] = -3;
			coef[1] = 498;
			coef[2] = 18;
			coef[3] = -1;
			break;
	case 1: coef[0] = -38;
			coef[1] = 376;
			coef[2] = 202;
			coef[3] = -28;
			break;
	case 2: coef[0] = -28;
			coef[1] = 202;
			coef[2] = 376;
			coef[3] = -38;
			break;
	case 3: coef[0] = -1;
			coef[1] = 18;
			coef[2] = 498;
			coef[3] = -3;
			break;
	default:
			coef[0] = 0;
			coef[1] = 512;
			coef[2] = 0;
			coef[3] = 0;
			break;
	}
}

void main() {

	char param_file[256]   = "param.txt";
	FILE *fp0 = fopen(param_file, "r");

	if(fp0==NULL){
		cout << "Input File open Error! : param.txt"  << endl;
		exit(1);
	}

	char buf[256] = {0};
	char imgin_file[256]  = {0};
	char imgout_file[256] = {0};
	int  i = 0;
	int scl_cfg_mode = 0;
	int scl_cfg_rsz  = 0;
	int scl_cfg_flt  = 0;

	while(fgets(buf, 256, fp0)){
		if( buf[0] == '#' ){
			continue;
		}

		switch(i){
		case 0: sscanf(buf, "%d", &scl_cfg_mode);
			break;
		case 1: sscanf(buf, "%d", &scl_cfg_rsz); 
			break;
		case 2: sscanf(buf, "%d", &scl_cfg_flt); 
			break;
		case 3: sscanf(buf, "%s", &imgin_file);
			break;
		case 4: sscanf(buf, "%s", &imgout_file);
			break;
		default:
			break;
		}

		i++;
	}

	bmp			*rbmp;
	bmp			*wbmp;
	BMP_Header_Data		rheader, wheader;

	rbmp = new bmp(imgin_file, "READ");
	wbmp = new bmp(imgout_file, "WRITE");

	rheader = rbmp->ReadHeader();

	wheader = rheader;

	int resize = 1;
	if(scl_cfg_mode == 1) {
		if(scl_cfg_rsz == 0) {
			resize = 2;
		} else if(scl_cfg_rsz == 1) {
			resize = 4;
		}
	}

	wheader.ImageBodySize /= resize;
	wheader.FileSize = wheader.ImageBodySize + 54;
	wheader.Xsize /= resize;

	wbmp->WriteHeader(wheader);

	int **imgin_buf_R;
	imgin_buf_R = (int **)malloc(sizeof(int *) * rheader.Ysize);
	for(unsigned int i=0; i < rheader.Ysize; i++)  
	{
		imgin_buf_R[i] = (int *)malloc(sizeof(int) * rheader.Xsize);
	}

	int **imgin_buf_G;
	imgin_buf_G = (int **)malloc(sizeof(int *) * rheader.Ysize);
	for(unsigned int i=0; i < rheader.Ysize; i++)  
	{
		imgin_buf_G[i] = (int *)malloc(sizeof(int) * rheader.Xsize);
	}

	int **imgin_buf_B;
	imgin_buf_B = (int **)malloc(sizeof(int *) * rheader.Ysize);
	for(unsigned int i=0; i < rheader.Ysize; i++)  
	{
		imgin_buf_B[i] = (int *)malloc(sizeof(int) * rheader.Xsize);
	}

	for(unsigned int y=0; y<rheader.Ysize; y++) {
		for(unsigned int x=0; x<rheader.Xsize; x++) {
			BMP_Pixel_Data  iPD = rbmp->ReadPixel();
			imgin_buf_R[y][x] = (int)(iPD.R);
			imgin_buf_G[y][x] = (int)(iPD.G);
			imgin_buf_B[y][x] = (int)(iPD.B);
		}
	}

	int coef[4];
	coef_sel(scl_cfg_flt, coef);

	for(unsigned int y=0; y<rheader.Ysize; y++) {
		for(unsigned int x=0; x<rheader.Xsize; x++) {
			int itmp_R[4], itmp_G[4], itmp_B[4];

			itmp_R[0] = imgin_buf_R[y][clip0M(int(x)-1,rheader.Xsize)];
			itmp_R[1] = imgin_buf_R[y][x];
			itmp_R[2] = imgin_buf_R[y][clip0M(x+1,rheader.Xsize)];
			itmp_R[3] = imgin_buf_R[y][clip0M(x+2,rheader.Xsize)];

			itmp_G[0] = imgin_buf_G[y][clip0M(int(x)-1,rheader.Xsize)];
			itmp_G[1] = imgin_buf_G[y][x];
			itmp_G[2] = imgin_buf_G[y][clip0M(x+1,rheader.Xsize)];
			itmp_G[3] = imgin_buf_G[y][clip0M(x+2,rheader.Xsize)];

			itmp_B[0] = imgin_buf_B[y][clip0M(int(x)-1,rheader.Xsize)];
			itmp_B[1] = imgin_buf_B[y][x];
			itmp_B[2] = imgin_buf_B[y][clip0M(x+1,rheader.Xsize)];
			itmp_B[3] = imgin_buf_B[y][clip0M(x+2,rheader.Xsize)];

			int imgout_pixel_R;
			int imgout_pixel_G;
			int imgout_pixel_B;

			if(scl_cfg_mode == 0) {
				imgout_pixel_R = itmp_R[1];
				imgout_pixel_G = itmp_G[1];
				imgout_pixel_B = itmp_B[1];
			} else {
				imgout_pixel_R = (itmp_R[0]*coef[0] + itmp_R[1]*coef[1] + itmp_R[2]*coef[2] + itmp_R[3]*coef[3]);
				imgout_pixel_G = (itmp_G[0]*coef[0] + itmp_G[1]*coef[1] + itmp_G[2]*coef[2] + itmp_G[3]*coef[3]);
				imgout_pixel_B = (itmp_B[0]*coef[0] + itmp_B[1]*coef[1] + itmp_B[2]*coef[2] + itmp_B[3]*coef[3]);

				if(imgout_pixel_R < 0 ) {
                    imgout_pixel_R = 0;
				} else if(imgout_pixel_R > 130560) {
                    imgout_pixel_R = 130560;
				}

				if(imgout_pixel_G < 0 ) {
                    imgout_pixel_G = 0;
				} else if(imgout_pixel_G > 130560) {
                    imgout_pixel_G = 130560;
				}

				if(imgout_pixel_B < 0 ) {
                    imgout_pixel_B = 0;
				} else if(imgout_pixel_B > 130560) {
                    imgout_pixel_B = 130560;
				}

				imgout_pixel_R /= 512;
				imgout_pixel_G /= 512;
				imgout_pixel_B /= 512;

			}

			BMP_Pixel_Data oPD;
			oPD.R = (unsigned char)(imgout_pixel_R);
			oPD.G = (unsigned char)(imgout_pixel_G);
			oPD.B = (unsigned char)(imgout_pixel_B);

			if(x%resize == 0) {
				wbmp->WritePixel(oPD);
			}
		}
	}
	
	free(imgin_buf_R);
	free(imgin_buf_G);
	free(imgin_buf_B);
	
}