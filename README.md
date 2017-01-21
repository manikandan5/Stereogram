# Stereogram

This code was written for the course Computer Vision (CSCI-B 657) at Indiana University handled by Professor David Crandall. Skeleton code was provided by the Professor to get us started with the assignment.


**What does the program do?** <br/>
* The program creates a stereo image from the normal image and the disparity map from the same view.
* If a seed image is given instead of a disparity map is not given, it tries to estimate the disparity using Markov Random Field energy minimization.
* If the seed image and disparity map are not given, the program tries to find the depth of the objects in the image using loopy belief propagation.

**How does it find it?** <br/>

Detailed explanation about how the code works and the reason why we chose this implementation could be found [here](https://github.com/manikandan5/Stereogram/blob/master/Assignment%204%20report.pdf).

**How to run the program?** 

This command compiles the program: <br>
```
      make 
```
</br>
To create stereograms: <br>
```
      ./render image.png disp.png
```
</br>
Naïve and MRF implementation: <br>
```
      ./segment input.png seeds.png
```
</br>
Stereo: <br>
```
      ./stereo left_image.png right_image.png gt.png

```
