/** ******************** *******
**auther:caiyunlong.2017.06.29
**email:ylcai@miivii.com
*******************************/

#include "image_process.h"
#include "Mat2Binary.h"

int get_facefeature_size(Mat img,int &img_size){
    img_size = img.elemSize() * img.total();
    if (img_size == 0){
    	return -1;
    }
    return 0;
}
int get_facefeature(Mat image,uint8_t *data_out){
   return Mat2binary(image,data_out);
}
int restore_facefeature(uint8_t *feature,Mat &img){
	return binary2Mat(feature,img);
}