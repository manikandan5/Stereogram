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
#include <limits>

using namespace cimg_library;
using namespace std;

double beta = 22;
double tot[3], mean[3], var[3];

#define INF numeric_limits<float>::infinity()


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
	//double tot[3], mean[3], var[3];
	//double beta = 24;
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
	cout << "debug 1" << endl;

   CImg <double> new_message(img.width(), img.height(),1,4);
   CImg <double> old_message(img.width(), img.height(),1,4);
   int no_of_neighbors = 4;
   int no_of_labels = 2;
   cout << "No. of Channels:"<< new_message.spectrum() << endl;
   
   for(int i=0; i<img.height(); i++)
  {
    for(int j=0; j<img.width(); j++)
    {   
      for(int k = 0; k < no_of_neighbors ; k++)
      {
        for(int l = 0; l < no_of_labels ; l++)
        {
            new_message(j,i,l,0) = 0;        //Bottle Neck #############################

            old_message(j,i,l,0) = 0;        // Here apart from 0 , 3rd parameter not taking any value
        }
      }
    }
  }
  
  
  cout << "debug 2" << endl;

  // assigning 0 to bg pixel , 1 to foreground pixel
  int fg_length = fg.size();
  CImg<double> result(img.width(), img.height(),1,3);
  
  
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
  
  cout << "debug 3" << endl; 

  int neighbor_Count = 4;
  int message_tuple_size = 2;
  double alpha = 0.5;
  double entropy_j =1;  
  double min_entropy_label = 0;
  double energy_value_from_neighbors = 0;
  double entropy_m_i_j = 1000;         
  double D_i = 1.0;
  double V = 0;
  double D_j = 0;  //D = findDistance(); for j
	  	
  min_entropy_label = 0;
      			  	//energy_value_from_neighbors for 0,1
  
  
  cout << "before 5 loops" << endl;
  
  double cost = 0;
  int N = 10;   // needs to run for 5 times
  
  while(N > 0)
  {
   for(int i=1; i<img.height()-1; i++)
  {
    for(int j=1; j<img.width()-1; j++)
    {     
      //if((result(j,i,0,0) != 0) || (result(j,i,0,0) != 1))  //And operator makes more sense here.
      //for (int k = 0; k < no_of_neighbors; k++) 
      //{
        for(int l = 0; l < no_of_labels ; l++)  // all the labels node j can take ( i->j )
        {
          double jNodeDCost = 0;


          if(l == 0)
          	jNodeDCost = beta;
          else
          {
			  for(int k=0;k<3;k++)
			  {
				jNodeDCost = jNodeDCost + (-1.0)*log(get_gaussian_probability(img(j,i+1,0,k),mean[k],var[k]));   //if label = 1 , calculate G_pdf for node j
			  }
		  }
          
          for(int mi = 0; mi < no_of_labels ; mi++)  // all the labels node i take
          {
                //cout << i << "  " << j << "   "<< k << "   " << l <<endl;
				energy_value_from_neighbors +=   old_message(j,i-1,l,0) + old_message(j,i+1,l,1) + old_message(j-1,i,l,2) ; //here 3rd parameter l can take 0 or 1

				if(l != mi)
				 V = 1;

				 if (mi == 0)
				 D_i = beta;
				 else
				 {
					 for(int k=0;k<3;k++)
					 {
						cost = cost + (-1.0)*log(get_gaussian_probability(img(j,i,0,k),mean[k],var[k])); //if label = 1 , calculate G_pdf for node i
					 }
					 D_i = cost;
				 }

				 int dummy;

				 //cin >> dummy;
				if( (D_i + alpha * V + energy_value_from_neighbors) < entropy_m_i_j ) // get minimum of energry values that node i thinks j should have
					entropy_m_i_j =  D_i + alpha * V + energy_value_from_neighbors;
          }
            
              D_j = jNodeDCost;
             if(entropy_m_i_j + D_j < entropy_j)
           	    min_entropy_label = l;		//this is alphabet l not number one		 
           }
         // }
         
       			  
                  
         if(min_entropy_label == 0)
          {	
            result(j,i+1,0,0) = 0;
            result(j,i+1,0,0) = 0;
            result(j,i+1,0,0) = 0;
       	  }
       	  else
       	  {
       	    result(j,i+1,0,0) = 255;
            result(j,i+1,0,0) = 255;
            result(j,i+1,0,0) = 255;
		  }

    }
  }
         
    N--;
  }

  //return naive_segment(img, fg, bg);
  
  result.save("resultt.png");
  
  return result;
  
  //return naive_segment(img, fg, bg);
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
    
    CImg<double> img_disp = img;
    
	for(int i=0; i<labels.height(); i++)
	{
		for(int j=0; j<labels.width(); j++)
		{
			if(labels(j,i) == 0)
            {
                img_fg(j,i,0,0) = img_fg(j,i,0,1) = img_fg(j,i,0,2) = 0;
                img_disp(j,i,0,0) = img_disp(j,i,0,1) = img_disp(j,i,0,2) = 0;
            }
			else if(labels(j,i) == 1)
            {
				img_bg(j,i,0,0) = img_bg(j,i,0,1) = img_bg(j,i,0,2) = 0;
                img_disp(j,i,0,0) = img_disp(j,i,0,1) = img_disp(j,i,0,2) = 255;
            }
            else
				assert(0);
		}
	}
    
    img_disp.get_normalize(0,255).save((fname + "_disp.png").c_str());
	img_fg.get_normalize(0,255).save((fname + "_fg.png").c_str());
	img_bg.get_normalize(0,255).save((fname + "_bg.png").c_str());
}

int main(int argc, char *argv[])
{
	string input_filename1, input_filename2;
	if(argc != 3)
	{
		cerr << "usage: " << argv[0] << " image_file seeds_file" << endl;
		return 1;
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
