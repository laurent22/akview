#include <QDebug>

#include "application.h"

//#include <FreeImage.h>

int main(int argc, char *argv[]) {
	mv::Application app(argc, argv);
	app.initialize();


//	BOOL bResult;
//	  // assume source has a width x height = 600x400 and a iMCU size = 16
//	  const char *src_file = "/Users/laurent/Desktop/test/CH_12_05_2014.jpg";
//	  const char *dst_file = "/Users/laurent/Desktop/test/CH_12_05_2014_CONV.jpg";
//	  // cropped rectangle
//	  int left, top, right, bottom;
//	  // we require a perfect transformation
//	  BOOL perfect = TRUE;
//	  left = 50; top = 50; right = 100; bottom = 100;
//	  // (optional) simulate the transform and get the new rectangle coordinates
//	  // (adjusted to the iMCU size and to the dst image width & height)
//	  bResult = FreeImage_JPEGTransformCombined(src_file, NULL, FIJPEG_OP_ROTATE_90,
//	&left, &top, &right, &bottom, perfect);
//	 // assert(bResult == TRUE);
//	  // check the cropped rectangle, adjusted for a WxH = 400x600 after rotation
//	//  assert( (left == 48) && (right == 400) && (top == 96) && (bottom == 500) );
//	  // then apply the transform
//	  bResult = FreeImage_JPEGTransformCombined(src_file, dst_file, FIJPEG_OP_ROTATE_90,
//	&left, &top, &right, &bottom, perfect);
//	  //assert(bResult == TRUE);



	return app.exec();
}
