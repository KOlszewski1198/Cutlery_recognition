#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat obr_wyj;
    Mat obr_bin_down;
    Mat obr_wej = imread("6.jpg");
    obr_wyj = obr_wej.clone();

    cvtColor(obr_wej, obr_bin_down, CV_BGR2GRAY);

    GaussianBlur(obr_bin_down, obr_bin_down, Size(15, 15), 15);
    Mat element_struk = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));

    threshold(obr_bin_down, obr_bin_down, 87, 255, THRESH_BINARY);

    morphologyEx(obr_bin_down, obr_bin_down, MORPH_OPEN, element_struk);
    morphologyEx(obr_bin_down, obr_bin_down, MORPH_CLOSE, element_struk);

    vector<vector<Point> > kontury;
    findContours(obr_bin_down, kontury, CV_RETR_LIST, CHAIN_APPROX_SIMPLE);

    pyrDown(obr_wej, obr_wej);
    pyrDown(obr_wej, obr_wej);
    imshow("Obraz wejsciowy", obr_wej);

    pyrDown(obr_bin_down, obr_bin_down);
    pyrDown(obr_bin_down, obr_bin_down);
    imshow("Po binaryzacji", obr_bin_down);

    int licznik = 0;
    for (int i = 0; i < kontury.size(); i++)
    {
        if (contourArea(kontury[i]) > 40000 && contourArea(kontury[i])
            < 200000)
        {
            licznik++;
            drawContours(obr_wyj, kontury, i, Scalar(0, 200, 0), 6);
            Moments M = moments(kontury[i]);
            double t[7];
            HuMoments(M, t);

            if ((contourArea(kontury[i]) / arcLength(kontury[i], true)) <= 30)
            {
                putText(obr_wyj, "WIDELEC", kontury[i][i], FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 5);
            }
            else
            {
                if (t[4] < 0.025)
                {
                    putText(obr_wyj, "NOZ", kontury[i][i], FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 5);
                }
                else
                {
                    if (t[4] > 0.025 && t[4] < 0.04)
                    {
                        putText(obr_wyj, "WIDELEC", kontury[i][i], FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 5);
                    }
                    else
                    {
                        putText(obr_wyj, "LYZKA", kontury[i][i], FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 5);
                    }
                }
            }
        }
    }
    imwrite("rozpoznane_6.jpg", obr_wyj);
    pyrDown(obr_wyj, obr_wyj);
    pyrDown(obr_wyj, obr_wyj);

    waitKey(0);
    return 0;
}

