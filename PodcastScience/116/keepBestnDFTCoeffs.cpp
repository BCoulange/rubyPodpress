#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    const char* filename = argc >=2 ? argv[1] : "lena.jpg";

    int nbCoeff2Keep = argc >= 3 ? atoi(argv[2]) : -1 ; // nb coeff on veut garder

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
    Mat magI;    
    magnitude(planes[0], planes[1], magI);     // calcul du module
    

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);


    // tri des elements de la matrice
    Mat sorted;
    magI.copyTo(sorted); // copy de la matrice à trier pour ne pas perdre la pos des pixels
    std::sort (sorted.begin<float>(), sorted.end<float>(), std::greater <float>());
    
    // Récupération du seuil
    MatConstIterator_<float> it = sorted.begin<float>();
    float seuil = it[nbCoeff2Keep];

    // seuillage (il existe sans doute une fonction built in)
    int nbKept=0;


    for(int i=0;i<magI.rows;i++)
        for(int j=0;j<magI.cols;j++){
            if(magI.at<float>(i,j)<seuil || nbKept >= nbCoeff2Keep){
                planes[0].at<float>(i,j) = 0.;
                planes[1].at<float>(i,j) = 0.;
            }else{
                nbKept+=1;
            }
//            magI.at<float>(i,j) = magI.at<float>(i,j)>seuil?magI.at<float>(i,j):0.;
    }


    magnitude(planes[0], planes[1], magI); 
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

// CONCAT 2 STRINGDS
//    char ret[128];
//    *ret = 0;
//    for (int i = 0; i < argc; i++)
//    { 
//         strcat(ret,argv[i]);
//    }

    cout <<  ret  << endl;

    cout << nbKept << " ont été gardés." << endl;


    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    dft(complexI,complexI, DFT_INVERSE);
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))


    normalize(planes[0], planes[0], 0, 255, CV_MINMAX); // Transform the matrix with float values into a
    imwrite( argv[3], planes[0] );



//    normalize(magI,magI,0,255,CV_MINMAX);
//    imwrite( argv[3], magI);

    return 0;
}