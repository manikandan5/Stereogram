#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

using namespace cimg_library;
using namespace std;

CImg<double> mrf_stereo(const CImg<double> &img1, const CImg<double> &img2)
{
  // implement this in step 4...
  //  this placeholder just returns a random disparity map
  CImg<double> result(img1.height(), img1.width());

  for(int i=0; i<img1.height(); i++)
    for(int j=0; j<img1.width(); j++)
      result(j,i) = rand() % 256;

  return result;
}



int main(int argc, char *argv[])
{
  if(argc != 4 && argc != 3)
    {
      cerr << "usage: " << argv[0] << " image_file1 image_file2 [gt_file]" << endl;
      return 1;
    }

  string input_filename1 = argv[1], input_filename2 = argv[2];
  string gt_filename;
  if(argc == 4)
    gt_filename = argv[3];

  // read in images and gt
  CImg<double> image1(input_filename1.c_str());
  CImg<double> image2(input_filename2.c_str());
  CImg<double> gt;

  if(gt_filename != "")
  {
    gt = CImg<double>(gt_filename.c_str());

    // gt maps are scaled by a factor of 3, undo this...
    for(int i=0; i<gt.height(); i++)
      for(int j=0; j<gt.width(); j++)
        gt(j,i) = gt(j,i) / 3.0;
  }
  
  // do stereo using mrf
  CImg<double> disp3 = mrf_stereo(image1, image2);
  disp3.get_normalize(0,255).save((input_filename1 + "-disp_mrf.png").c_str());

  // Measure error with respect to ground truth, if we have it...
  if(gt_filename != "")
    {
      double err=0;
      for(int i=0; i<gt.height(); i++)
	for(int j=0; j<gt.width(); j++)
	  err += sqrt((disp3(j,i) - gt(j,i))*(disp3(j,i) - gt(j,i)));

      cout << "MRF stereo technique mean error = " << err/gt.height()/gt.width() << endl;

    }

  return 0;
}
