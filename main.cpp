#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
const int ROW=1500;
const int COL=2400;
const int LEN=100;
char a[1000]="Bernie Sanders has cemented his status as the Democratic front-runner to take on Donald Trump in November's US presidential election.He is projected to win Nevada's caucuses, and early results suggest he is on course for a large victory.There is a long way to go, however, until a nominee is confirmed.Early results also suggest former vice-president Joe Biden has performed better in Nevada than in the other two states which have voted so far.He had underwhelming results in Iowa and New Hampshire. Those states kicked off the four-month long primaries process, in which candidates are jostling to convince voters why they are the best candidate to challenge Mr Trump.";
//            Bernie Sanders has cemented his status as the Democratic front-runner to take on Donald Trump in November's US presidential election.He is projected to win Nevada's caucuses and early results suggest he is on course for a large victory.There is a long way to go, however, until a nominee is confirmed.Early results also suggest former vice-president Joe Biden has performed better in Nevada than in the other two states which have voted so far.He had underwhelming results in Iowa and New Hampshire. Those states kicked off the four-month long primaries process, in which candidates are jostling to convince voters why they are the best candidate to c
bool src[10000];
int contain,srclen;
int image_produce();// 产生图片形式
void video_produce(int piccnt);
int main() {
    int cnt=image_produce();

    video_produce(cnt);
    waitKey(0);
}
VideoWriter Vwriter;
void video_produce(int piccnt){
    int fps=30,perPicFps=5;
    Vwriter=VideoWriter("dst.avi",VideoWriter::fourcc('M','J','P','G'),fps,Size(COL+2*LEN,ROW+2*LEN),0);
    for(int i=0;i<piccnt;i++){
        if(!Vwriter.isOpened()){
            cout<<"failed to open writer";
            return;
        }
        char file_name[20];
        sprintf(file_name,"%d.png",i);

//        printf("%s\n",file_name);

        Mat src=imread(file_name,CV_8U);
        //printf("%d %d===\n",Vwriter.)
        printf("%d %d\n",src.rows,src.cols);


        for(int j=1;j<=perPicFps;j++) {
            Vwriter << src;
        }
    }
}

int image_produce(){
    Mat dst=Mat(ROW+2*LEN,COL+2*LEN,CV_8U);
    int len=strlen(a);
    srclen=0;
    contain=ROW/LEN*(COL/LEN)-12;
    for(int i=0;i<len;i++){
        for(int j=0;j<8;j++){
            src[i*8+j]=(a[i]>>j)&1;
            printf("%d",src[i*8+j]);
            srclen++;
        }
    }// 10000110 01001100 01100110 11001100 00100110
    dst.setTo(255);
    imwrite("0.png",dst);
    int cnt=0,piccnt=0;
    while(cnt<srclen) {
        dst.setTo(255);
        for (int i = 0; i < COL / LEN; i++)
            for (int j = 0; j < ROW / LEN; j++) {
                int x = (i + 1) * LEN, y = (j + 1) * LEN;
                if ((i == 0 && j == 0) || (i == COL / LEN - 1 && j == 0) || (i == 0 && j == ROW / LEN - 1)) {
                    int p = LEN / 5;
                    rectangle(dst, Point(x, y), Point(x + LEN, y + LEN), Scalar(0, 0, 0), FILLED);
                    rectangle(dst, Point(x + p, y + p), Point(x + LEN - p, y + LEN - p), Scalar(255, 255, 255), FILLED);
                    rectangle(dst, Point(x + 2 * p, y + 2 * p), Point(x + 3 * p, y + 3 * p), Scalar(0, 0, 0), FILLED);
                } else if ((i == 0 && (j == 1 || j == ROW / LEN - 2)) ||
                           (i == 1 && (j == 0 || j == 1 || j == ROW / LEN - 2 || j == ROW / LEN - 1)) ||
                           (i == COL / LEN - 2 && (j == 0 || j == 1)) || (i == COL / LEN - 1 && j == 1)) { continue; }
                else if (src[cnt++] == 1)rectangle(dst, Point(x, y), Point(x + LEN, y + LEN), Scalar(0, 0, 0), FILLED);
            }
        piccnt++;
        char dst_file_name[20];
        sprintf(dst_file_name,"%d.png",piccnt);
        imwrite(dst_file_name,dst);
    }

    return piccnt;
}