# StereoImages
The program creates a stereo image from the normal image and the disparity map from the same view. If a seed image is given instead of a disparity map is not given, it tries to estimate the disparity using Markov Random Field energy minimization.
(C++)

• Part 1 – Creating Stereograms
  ./render image.png disp.png

• Part 2 – Naïve and mrf implementation
  ./segment input.png seeds.png

• Part 3 – Stereo
  ./stereo left_image.png right_image.png gt.png
