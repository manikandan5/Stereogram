// Skeleton code for B657 A4 Part 3.
// D. Crandall
//
// Run like this, for example:
//   ./stereo part3/Aloe/view1.png part3/Aloe/view5.png part3/Aloe/gt.png
// and output files will appear in part3/Aloe
//
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include <CImg.h>
#include <assert.h>

using namespace cimg_library;
using namespace std;


typedef struct NeighbrCost
{
  double NeighborsCost[5][2];

}NCost;


double sqr(double a) { return a*a; }

// This code may or may not be helpful. :) It computes a 
//  disparity map by looking for best correspondences for each
//  window independently (no MRF).
//
CImg<double> naive_stereo(const CImg<double> &input1, const CImg<double> &input2, int window_size, int max_disp)
{  
  CImg<double> result(input1.width(), input1.height());

  for(int i=0; i<input1.height(); i++)
    for(int j=0; j<input1.width(); j++)
      {
	pair<int, double> best_disp(0, INFINITY);

	for (int d=0; d < max_disp; d++)
	  {
	    double cost = 0;
	    for(int ii = max(i-window_size, 0); ii <= min(i+window_size, input1.height()-1); ii++)
	      for(int jj = max(j-window_size, 0); jj <= min(j+window_size, input1.width()-1); jj++)
		cost += sqr(input1(min(jj+d, input1.width()-1), ii) - input2(jj, ii));

	    if(cost < best_disp.second)
	      best_disp = make_pair(d, cost);
	  }
	result(j,i) = best_disp.first;
      }

  return result;
}

// implement this!
//  this placeholder just returns a random disparity map
//
void intializeEnergyMessages(const CImg<double> &img, const vector<Point> &fg, const vector<Point> &bg, vector< vector <NCost> > &NeighborEnergyValues)
{
  int pixeltype = 0; // all background
  double NodeDCost = 0;
  for(int i=1; i<img.height()-1; i++)
  {
    for(int j=1; j<img.width()-1; j++)
    {
        Point currPixel(j,i);
        //cout << "test"<<endl;
        pixeltype = checkIfBGorFG(j,i, fg, bg);
        if(pixeltype == 0)
        {
              NodeDCost = 0;
              for(int k=0;k<3;k++)
      			  {
      				  NodeDCost = NodeDCost + (-1.0)*log(get_gaussian_probability(img(j,i,0,k),mean[k],var[k]));
      			  }
              NeighborEnergyValues[i][j].NeighborsCost[4][0] = beta;
              NeighborEnergyValues[i][j].NeighborsCost[4][1] = NodeDCost;

              //cout <<  NodeDCost << endl;
        }
        else if(pixeltype == 1)
        {
          NeighborEnergyValues[i][j].NeighborsCost[4][0] = 1000;
          NeighborEnergyValues[i][j].NeighborsCost[4][1] = 0;
        }
        else
        {
          NeighborEnergyValues[i][j].NeighborsCost[4][0] = 0;
          NeighborEnergyValues[i][j].NeighborsCost[4][1] = 1000;
        }
        for(int k=0; k<4; k++)
        {
          NeighborEnergyValues[i][j].NeighborsCost[k][0] = 0;
          NeighborEnergyValues[i][j].NeighborsCost[k][1] = 0;
        }

    }
  }

}



CImg<double> mrf_stereo(const CImg<double> &img1, const CImg<double> &img2)
{
  CImg<double> result(img1.width(), img1.height());

  for(int i=0; i<img1.height(); i++)
    for(int j=0; j<img1.width(); j++)
      result(j,i) = rand() % 256;


  int neighbor_Count = 4;
  double alpha = 0.40;
  double entropy_j =1000;
  double min_entropy_label = 0;
  double energy_value_from_neighbors = 0;
  double entropy_m_i_j = 100000;
  double D_i = 1.0;
  double V = 0;
  double D_j = 0;  //D = findDistance(); for j

   int no_of_neighbors = 4;
   int no_of_labels = 2;


   min_entropy_label = 0;
   Point TempNghbr;


   vector< vector <NCost> > NeighborEnergyValues(img.height(), vector<NCost>(img.width()));

   intializeEnergyMessages(img, fg, bg, NeighborEnergyValues);

  // testIntitilaization(img,NeighborEnergyValues);

   cout << "Loopy BP started" << endl;
   double cost = 0;
   int N = 100;
   double Energy_I = 0;
   double Energy_J = 0;
   int sentNode =0;


//while(N>0)
//result =  naive_segment(img, fg, bg);



  while(N > 0)
  {
   for(int i=1; i<img.height()-1; i++)
  {
    for(int j=1; j<img.width()-1; j++)
    {
      for (int k = 0; k < no_of_neighbors; k++)
      {
       min_entropy_label = 0;
       entropy_m_i_j = 0;
        for(int l = 0; l < no_of_labels ; l++)  // all the labels node j can take ( i->j )
        {

          for(int mi = 0; mi < no_of_labels ; mi++)  // all the labels node i take
          {  Energy_I =0;
              D_i =  NeighborEnergyValues[i][j].NeighborsCost[4][mi];
              for (int tempK = 0 ; tempK < 4 ; tempK++ ) // receive message from all neighbors except from the one i am sending
              {
                if(k != tempK)
                Energy_I += NeighborEnergyValues[i][j].NeighborsCost[tempK][mi];
              }
              //cout << D_i << endl;

    				   if(l != mi) V = 1;

               Energy_I += D_i + alpha * V;
               //cout << "E_i  " << Energy_I << endl;

       				 if( Energy_I < entropy_m_i_j ) // get minimum of energry values that node i thinks j should have
        					entropy_m_i_j =  Energy_I;

           }
            sentNode =0;

             if(k == 0)       // left neighbor
             {
                 D_j = NeighborEnergyValues[i][j-1].NeighborsCost[4][l];
                 TempNghbr.col = j-1;
                 TempNghbr.row = i;
                 sentNode =2;
             }
             else if(k == 1) // up neighbor
             {
                 D_j = NeighborEnergyValues[i-1][j].NeighborsCost[4][l];
                 TempNghbr.col = j;
                 TempNghbr.row = i-1;
                 sentNode =3;
             }
             else if(k == 2) // right neighbor
             {
                 D_j = NeighborEnergyValues[i][j+1].NeighborsCost[4][l];
                 TempNghbr.col = j+1;
                 TempNghbr.row = i;
                 sentNode =0;
             }
             else           //  down neighbor
             {
                 D_j = NeighborEnergyValues[j][i+1].NeighborsCost[4][l];
                 TempNghbr.col = j;
                 TempNghbr.row = i+1;
                 sentNode = 1;
             }

            // NeighborEnergyValues[j][i].NeighborsCost[k][l] = entropy_m_i_j + D_j;  // updating neighbors messages what i think of them

             Energy_J = entropy_m_i_j + D_j;


             //cout << "E_J: " << Energy_J << endl;
             NeighborEnergyValues[TempNghbr.row][TempNghbr.col].NeighborsCost[sentNode][l] = Energy_J ;



      }
            int dummy;
            //cin >> dummy;
     } // end of k loop
    } // end of j loop
  }  // end of i loop

  cout << N <<endl;
    N--;
 } // end of while loop



 double final_cost = 0;
 double compare_cost = 10000;
 int minClass = 0;
 CImg<double> result(img.width(), img.height());
 cout << "checkpoint 1" << endl;
 result.fill(img);
 result.fill(0);

 /*
 for(int i = 0;i<img.height();i++)
 {
    cout << i << endl;
    for(int j = 0; j < img.width(); j++)
    {
        result(j,i) = 0;
    }
 }
 */

 cout << "h: " << img.height() << " w: " << img.width() << endl;

 for(int a=1; a<img.height()-1; a++)
  {
    for(int b=1; b<img.width()-1; b++)
    {
      compare_cost = 100000;
      for(int nc=0; nc < 2 ;nc++)
      {
        final_cost = 0;
        final_cost += NeighborEnergyValues[a][b].NeighborsCost[4][nc];
        //cout << final_cost << endl;
        for(int dk =0 ; dk < 4; dk++)
        {
          final_cost += NeighborEnergyValues[a][b].NeighborsCost[dk][nc];
         // cout << final_cost << endl;
        }

         // cout << final_cost << endl;

          int dummy;
          //cin >> dummy;

          if (final_cost < compare_cost)
          {
            compare_cost = final_cost;
            minClass = nc;
          }
      }

      //cout << "a: "  << a << " b: "<< b << endl;



        if(minClass == 0)
        {
         //   cout << "a: "  << a << " b: "<< b << " in if then" << endl;
            result(b,a) = 0;
            //result(b,a,0,1) = 0;
            //result(b,a,0,2) = 0;

        }
     	  else
        {
           // cout << "a: "  << a << " b: "<< b << " in if else" << endl;
            result(b,a) = 1;
            //result(b,a,0,1) = 1;
            //result(b,a,0,2) = 1;

        }
      }
    }

  result.save("result_mrf_stereo.png");

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
  
  // do naive stereo (matching only, no MRF)
  CImg<double> naive_disp = naive_stereo(image1, image2, 5, 50);
  naive_disp.get_normalize(0,255).save((input_filename1 + "-disp_naive.png").c_str());

  // do stereo using mrf
  CImg<double> mrf_disp = mrf_stereo(image1, image2);
  mrf_disp.get_normalize(0,255).save((input_filename1 + "-disp_mrf.png").c_str());

  // Measure error with respect to ground truth, if we have it...
  if(gt_filename != "")
    {
      cout << "Naive stereo technique mean error = " << (naive_disp-gt).sqr().sum()/gt.height()/gt.width() << endl;
      cout << "MRF stereo technique mean error = " << (mrf_disp-gt).sqr().sum()/gt.height()/gt.width() << endl;

    }

  return 0;
}
