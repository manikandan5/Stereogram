// Skeleton code for B657 A4 Part 2.
// D. Crandall
//
//
#define PI 3.14
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

using namespace cimg_library;
using namespace std;

class Point
{
public:
	Point() {}
	Point(int _col, int _row) : row(_row), col(_col) {}
	int row, col;
};

double get_gaussian_probability(const double val, const double mean, const double variance)
{
	return ((1/(sqrt(variance*2*PI)))*exp(-1*pow((val-mean),2)/(2*variance)));
}

double get_cost(const double label, const double val, const double mean, const double variance)
{

}


CImg<double> naive_segment(const CImg<double> &img, const vector<Point> &fg, const vector<Point> &bg)
{
	// implement this in step 2...
	//  this placeholder just returns a random disparity map
	CImg<double> result(img.width(), img.height(),1,3);
	double tot[3], mean[3], var[3];
	double beta = 22;
	double cost = 0;

	//Calculating mean for each of the Red, Green and Blue channels of the Foreground Pixels
	int fg_length = fg.size();

	tot[0] = tot[1] = tot[2] = 0;

	for(int i=0; i<fg_length;i++)
	{
		tot[0] = tot[0] + img(fg[i].col,fg[i].row,0,0);
		tot[1] = tot[1] + img(fg[i].col,fg[i].row,0,1);
		tot[2] = tot[2] + img(fg[i].col,fg[i].row,0,2);
	}

	mean[0] = tot[0]/fg_length;
	mean[1] = tot[1]/fg_length;
	mean[2] = tot[2]/fg_length;

	tot[0] = tot[1] = tot[2] = 0;

	//Calculating Variance for the Foreground Pixels
	for(int i=0;i<fg_length;i++)
	{
		tot[0] = tot[0] + pow((img(fg[i].col,fg[i].row,0,0) - mean[0]),2);
		tot[1] = tot[1] + pow((img(fg[i].col,fg[i].row,0,1) - mean[1]),2);
		tot[2] = tot[2] + pow((img(fg[i].col,fg[i].row,0,2) - mean[2]),2);
	}
	
	var[0] = tot[0]/fg_length;
	var[1] = tot[1]/fg_length;
	var[2] = tot[2]/fg_length;

	for(int i=0; i < fg_length; i++)
	{
		result(fg[i].col,fg[i].row,0,0) = 1;
		result(fg[i].col,fg[i].row,0,1) = 1;
		result(fg[i].col,fg[i].row,0,2) = 1;
	}

	for(int i=0;i<bg.size();i++)
	{
		result(bg[i].col,bg[i].row,0,0) = 0;
		result(bg[i].col,bg[i].row,0,1) = 0;
		result(bg[i].col,bg[i].row,0,2) = 0;
	}

	for(int i=0; i<img.height(); i++)
	{
		for(int j=0; j<img.width(); j++)
		{
			if((result(j,i,0,0) != 0) || (result(j,i,0,0) != 1))
			{
				cost= 0;
				for(int k=0;k<3;k++)
				{
					cost = cost + (-1.0)*log(get_gaussian_probability(img(j,i,0,k),mean[k],var[k]));
				}

				if(cost < beta)
				{
					result(j,i,0,0) = 1;
					result(j,i,0,1) = 1;
					result(j,i,0,2) = 1;
				}
				else
				{
					result(j,i,0,0) = 0;
					result(j,i,0,1) = 0;
					result(j,i,0,2) = 0;
				}

			}
		}
	}
	return result;
}

CImg<double> mrf_segment(const CImg<double> &img, const vector<Point> &fg, const vector<Point> &bg)
{
	// implement this in step 3...
	//  this placeholder just returns a random disparity map by calling naive_segment
	return naive_segment(img, fg, bg);
}

// Take in an input image and a binary segmentation map. Use the segmentation map to split the 
//  input image into foreground and background portions, and then save each one as a separate image.
//
// You'll just need to modify this to additionally output a disparity map.
//
void output_segmentation(const CImg<double> &img, const CImg<double> &labels, const string &fname)
{
	// sanity checks. If one of these asserts fails, you've given this function invalid arguments!
	assert(img.height() == labels.height());
	assert(img.width() == labels.width());

	CImg<double> img_fg = img, img_bg = img;

	for(int i=0; i<labels.height(); i++)
	{
		for(int j=0; j<labels.width(); j++)
		{
			if(labels(j,i) == 0)
				img_fg(j,i,0,0) = img_fg(j,i,0,1) = img_fg(j,i,0,2) = 0;
			else if(labels(j,i) == 1)
				img_bg(j,i,0,0) = img_bg(j,i,0,1) = img_bg(j,i,0,2) = 0;
			else
				assert(0);
		}
	}

	img_fg.get_normalize(0,255).save((fname + "_fg.png").c_str());
	img_bg.get_normalize(0,255).save((fname + "_bg.png").c_str());
}

int main(int argc, char *argv[])
{
	string input_filename1, input_filename2;
	if(argc != 3)
	{
		input_filename1 = "./part2/cardinal.png";
		input_filename2 = "./part2/cardinal-seeds.png";
		//cerr << "usage: " << argv[0] << " image_file seeds_file" << endl;
		//return 1;
	}
	else
	{
		input_filename1 = argv[1];
		input_filename2 = argv[2];
	}
	// read in images and gt
	CImg<double> image_rgb(input_filename1.c_str());
	CImg<double> seeds_rgb(input_filename2.c_str());

	// figure out seed points
	vector<Point> fg_pixels, bg_pixels;

	for(int i=0; i<seeds_rgb.height(); i++)
	{
		for(int j=0; j<seeds_rgb.width(); j++)
		{
			// blue --> foreground
			if(max(seeds_rgb(j, i, 0, 0), seeds_rgb(j, i, 0, 1)) < 100 && seeds_rgb(j, i, 0, 2) > 100)
				fg_pixels.push_back(Point(j, i));

			// red --> background
			if(max(seeds_rgb(j, i, 0, 2), seeds_rgb(j, i, 0, 1)) < 100 && seeds_rgb(j, i, 0, 0) > 100)
				bg_pixels.push_back(Point(j, i));
		}
	}

	// do naive segmentation
	CImg<double> labels = naive_segment(image_rgb, fg_pixels, bg_pixels);
	output_segmentation(image_rgb, labels, input_filename1 + "-naive_segment_result");

	// do mrf segmentation
	labels = naive_segment(image_rgb, fg_pixels, bg_pixels);
	output_segmentation(image_rgb, labels, input_filename1 + "-mrf_segment_result");

	return 0;
}
