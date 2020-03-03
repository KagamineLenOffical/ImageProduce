#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
const int ROW=1000;
const int COL=1000;
const int LEN=20;
int r=ROW/LEN,c=COL/LEN;
char a[10000]="Bernie Sanders has cemented his status as the Democratic front-runner to take on Donald Trump in November's US presidential election.He is projected to win Nevada's caucuses, and early results suggest he is on course for a large victory.There is a long way to go, however, until a nominee is confirmed.Early results also suggest former vice-president Joe Biden has performed better in Nevada than in the other two states which have voted so far.He had underwhelming results in Iowa and New Hampshire. Those states kicked off the four-month long primaries process, in which candidates are jostling to convince voters why they are the best candidate to challenge Mr Trump.";
//            Bernie Sanders has cemented his status as the Democratic front-runner to take on Donald Trump in November's US presidential election.He is projected to win Nevada's caucuses and early results suggest he is on course for a large victory.There is a long way to go, however, until a nominee is confirmed.Early results also suggest former vice-president Joe Biden has performed better in Nevada than in the other two states which have voted so far.He had underwhelming results in Iowa and New Hampshire. Those states kicked off the four-month long primaries process, in which candidates are jostling to convince voters why they are the best candidate to c
bool src[100000];
int loc_point[10][10]={
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,0,0},
        {0,1,0,0,0,0,0,1,0,0},
        {0,1,0,1,1,1,0,1,0,0},
        {0,1,0,1,1,1,0,1,0,0},
        {0,1,0,1,1,1,0,1,0,0},
        {0,1,0,0,0,0,0,1,0,0},
        {0,1,1,1,1,1,1,1,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
};
int originpoint[ROW/LEN][COL/LEN];
int srcpoint[ROW/LEN][COL/LEN];
int contain,srclen;
int image_produce();// 产生图片形式
void video_produce(int piccnt);
void srcpoint_produce();
void src_produce();
int main() {
//    int cnt=image_produce();
//    video_produce(cnt);
    src_produce();
    srcpoint_produce();
    video_produce(2);
    waitKey(0);
}
VideoWriter Vwriter;
void video_produce(int piccnt){
    int fps=30,perPicFps=15;
    Vwriter=VideoWriter("dst.avi",VideoWriter::fourcc('M','J','P','G'),fps,Size(COL,ROW),1);
    for(int i=0;i<piccnt;i++){
        if(!Vwriter.isOpened()){
            cout<<"failed to open writer";
            return;
        }
        char file_name[20];
        sprintf(file_name,"%d.png",i);

        printf("%s\n",file_name);

        Mat src=imread(file_name,IMREAD_COLOR);
        //printf("%d %d===\n",Vwriter.)
        printf("%d %d\n",src.rows,src.cols);
        namedWindow("src");
        imshow("src",src);
        waitKey(0);

        for(int j=1;j<=perPicFps;j++) {
            Vwriter << src;
        }
    }
}

void srcpoint_produce(){
    for(int i=0;i<c;i++)
        for(int j=0;j<r;j++){
            originpoint[i][j]=-1;
        }
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++){
            int k=loc_point[i][j]>0?7:0;
            originpoint[i][j]=k;
            originpoint[c-i-1][j]=k;
            originpoint[i][r-j-1]=k;
            originpoint[c-i-1][r-j-1]=k;
        }
    int cnt=0,piccnt=0;
    printf("数据量（bit）：%d\n\n\n",srclen);
    Mat dst=Mat(ROW,COL,CV_8UC3);

    dst.setTo(255);
    imwrite("0.png",dst);
    while(cnt<srclen) {
        piccnt++;
        dst.setTo(255);
        for (int i = 0; i < c; i++)
            for (int j = 0; j < r; j++)srcpoint[i][j] = originpoint[i][j];

        for(int i=0;i<c;i++)
            for(int j=0;j<r;j++){
                if(srcpoint[i][j]==-1){
                    int k=(src[cnt]<<2)|(src[cnt+1]<<1)|(src[cnt+2]);
                    cnt+=3;
                    srcpoint[i][j]=k;
                }
            }
        for(int i=0;i<c;i++)
            for(int j=0;j<r;j++){
                Scalar color;
                for(int k=0;k<3;k++){
                    if(((1<<k)&srcpoint[i][j])!=0)color.val[k]=0;
                    else color.val[k]=255;

                }
                rectangle(dst, Point(i*LEN, j*LEN), Point(i*LEN + LEN, j*LEN + LEN), color, FILLED);
            }
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < r; j++) {
                printf("%d", srcpoint[i][j]);
            }
            printf("\n");
        }
        char dst_file_name[20];
        sprintf(dst_file_name,"%d.png",piccnt);
        imwrite(dst_file_name,dst);
    }
}
void src_produce(){
    int len=strlen(a);
    srclen=0;
    contain=ROW/LEN*(COL/LEN)-12;
    for(int i=0;i<len;i++){
        for(int j=0;j<8;j++){
            src[i*8+j]=(a[i]>>j)&1;
            //printf("%d",src[i*8+j]);
            srclen++;
        }
    }
}
//this is the old version
int image_produce(){
    Mat dst=Mat(ROW+2*LEN,COL+2*LEN,CV_8U);

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