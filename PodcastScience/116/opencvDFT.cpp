#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    const char* filename = argc >=2 ? argv[1] : "lena.jpg";


    int nbCoeff2Keep = argc >= 3 ? atoi(argv[2]) : -1 ;

    // Chargement de l'image
    Mat I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    if( I.empty())
        return -1;

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    // Calcul de la fft
    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);






// Création et initialisation du vecteur
std::vector <float> ivec;
 

    // compute sum of positive matrix elements, iterator-based variant

    double sum=0;
    MatConstIterator_<float> it = magI.begin<float>(), it_end = magI.end<float>();
    for(; it != it_end; ++it)
       ivec.push_back (*it);

// Tri du vecteur grâce à la fonction std::sort
std::sort (ivec.begin(), ivec.end(), std::greater <float>());

//std::sort (magI.begin<float>(), magI.end<float>(), std::greater <float>());

//for (vector<float>::iterator it2 = ivec.begin(); it2!=ivec.begin()+nbCoeff2Keep; ++it2) 
//    cout << *it2 << endl;

cout << ivec[nbCoeff2Keep] << endl;

// acceder aux valeurs des pixels et les changer!!!
for(int i=1;i<magI.rows;i++)
    for(int j=1;j<magI.cols;j++){
        magI.at<float>(i,j) = magI.at<float>(i,j)>ivec[nbCoeff2Keep]?magI.at<float>(i,j):0.;
    }


    // crop the spectrum, if it has an odd number of rows or columns

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 255, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).


    imwrite( "Gray_Image.jpg", magI );

//    imshow("Input Image"       , I   );    // Show the result
//    imshow("spectrum magnitude", magI);
//    waitKey();

    return 0;
}