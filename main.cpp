#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

#define Buffer_Size_By_Byte 1024*1024*10
#define Buffer_Size_By_Bit 1024*1024*10*8

using namespace std;
using namespace cv;
const int ROW=1000;
const int COL=1800;
const int LEN=10;
int r=ROW/LEN,c=COL/LEN;
unsigned char Raw_Data_Input_Buffer[Buffer_Size_By_Byte];
bool Data_Buffer_By_Bit[Buffer_Size_By_Bit];
int loc_point[10][10]=
        {
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

int originpoint[COL/LEN][ROW/LEN];
int srcpoint[COL/LEN][ROW/LEN];
int contain,srclen;

int image_produce();// 产生图片形式
void video_produce(int piccnt);
int srcpoint_produce();
void src_produce();
void read_file();

char Input_Path_Buffer[200];
char Output_Path_Buffer[200];
int Video_Time_Buffer_By_MS;
int Raw_Data_Length_By_Byte=0;
long Zipped_Data_Length_By_Byte;


int main(int argc,char *argv[]) {
//    int cnt=image_produce();
//    video_produce(cnt);

    Video_Time_Buffer_By_MS = atoi(argv[3]);
    strcpy(Input_Path_Buffer, argv[1]);
    strcpy(Output_Path_Buffer, argv[2]);





    src_produce();

    int _piccnt=srcpoint_produce();
    video_produce(_piccnt);
    waitKey(0);
}





void read_file()
{
    FILE *fin=fopen(Input_Path_Buffer,"rb");
    if(fin == NULL)
    {
        cout << "打开文件失败。" << endl;
        exit(0);
    }

    int i=0;
    while(!feof(fin))
    {
        Raw_Data_Length_By_Byte+=fread(Raw_Data_Input_Buffer+i, 1, 1024, fin);
        i+=1024;
    }

    printf("总计读取到%d字节\n",Raw_Data_Length_By_Byte);
}



VideoWriter Vwriter;

void video_produce(int piccnt)
{
    int fps=30,perPicFps=5;
    float Video_Length_Tracker = 0;
    float Time_Per_Frame_By_Ms = 1000*(1.0/(float)fps);
    Vwriter=VideoWriter(Output_Path_Buffer,VideoWriter::fourcc('M','J','P','G'),fps,Size(COL,ROW),1);
    for(int i=0;i<=piccnt;i++)
    {
        if(!Vwriter.isOpened())
        {
            cout<<"failed to open writer";
            return;
        }
        char file_name[20];
        sprintf(file_name,"%d.png",i);

        //printf("%s\n",file_name);

        Mat src=imread(file_name,IMREAD_COLOR);
        //printf("%d %d===\n",Vwriter.)
        //printf("%d %d\n",src.rows,src.cols);
        namedWindow("src");
        imshow("src",src);
        waitKey(0);

        for(int j=1;j<=perPicFps;j++)
        {
            Vwriter << src;
            Video_Length_Tracker += Time_Per_Frame_By_Ms;
            if(Video_Length_Tracker >= Video_Time_Buffer_By_MS)
            {
                goto Break_The_Loop;
            }
        }
    }
    Break_The_Loop:
    cout << "/nFinished!" << endl;
}




int srcpoint_produce()
{
    for(int i=0;i<c;i++)
        for(int j=0;j<r;j++)
        {
            originpoint[i][j]=-1;
        }
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
        {
            int k=loc_point[i][j]>0?7:0;
            originpoint[i][j]=k;
            originpoint[c-i-1][j]=k;
            originpoint[i][r-j-1]=k;
            originpoint[c-i-1][r-j-1]=k;
        }
    for(int i=0;i<c;i++)
    {
        originpoint[i][0]=originpoint[i][r-1]=0;
    }
    for(int i=0;i<r;i++)
    {
        originpoint[0][i]=originpoint[c-1][i]=0;
    }
    int cnt=0;
    int piccnt=0;
    printf("数据量（bit）：%d\n",srclen);
    Mat dst=Mat(ROW,COL,CV_8UC3);

    dst.setTo(255);
    imwrite("0.png",dst);
    while(cnt<srclen)
    {
        piccnt++;
        dst.setTo(255);
        for (int i = 0; i < c; i++)
        {
            for (int j = 0; j < r; j++)
            {
                srcpoint[i][j] = originpoint[i][j];
            }
        }

        for(int i=0;i<c;i++)
        {
            for(int j=0;j<r;j++)
            {
                if(srcpoint[i][j]==-1)
                {
                    int k=(Data_Buffer_By_Bit[cnt]<<2)|(Data_Buffer_By_Bit[cnt+1]<<1)|(Data_Buffer_By_Bit[cnt+2]);
                    cnt+=3;
                    srcpoint[i][j]=k;
                }
            }
        }
        for(int i=0;i<c;i++)
        {
            for(int j=0;j<r;j++)
            {
                Scalar color;
                for(int k=0;k<3;k++)
                {
                    if(((1<<k)&srcpoint[i][j])!=0)color.val[k]=0;
                    else color.val[k]=255;

                }
                rectangle(dst, Point(i*LEN, j*LEN), Point(i*LEN + LEN, j*LEN + LEN), color, FILLED);
            }
        }
        /*for (int i = 0; i < c; i++) {
            for (int j = 0; j < r; j++) {
                printf("%d", srcpoint[i][j]);
            }
            printf("\n");
        }*/
        char dst_file_name[20];
        sprintf(dst_file_name,"%d.png",piccnt);
        imwrite(dst_file_name,dst);
    }
    return piccnt;
}




void src_produce()
{
    read_file();





    srclen=0;
    contain=ROW/LEN*(COL/LEN)-12;
    FILE *fout=fopen("2.out","w+");
    for(int i = 0;i < Raw_Data_Length_By_Byte;i++)
    {
        for(int j = 0;j < 8;j++)
        {
            Data_Buffer_By_Bit[i*8+j]=(Raw_Data_Input_Buffer[i]>>j)&1;
            fprintf(fout,"%d",Data_Buffer_By_Bit[i*8+j]);
            srclen++;
        }
    }
    fclose(fout);
    //printf("%d\n",srclen);
}







//this is the old version
//int image_produce(){
//    Mat dst=Mat(ROW+2*LEN,COL+2*LEN,CV_8U);
//
//    dst.setTo(255);
//    imwrite("0.png",dst);
//    int cnt=0,piccnt=0;
//    while(cnt<srclen) {
//        dst.setTo(255);
//        for (int i = 0; i < COL / LEN; i++)
//            for (int j = 0; j < ROW / LEN; j++) {
//                int x = (i + 1) * LEN, y = (j + 1) * LEN;
//                if ((i == 0 && j == 0) || (i == COL / LEN - 1 && j == 0) || (i == 0 && j == ROW / LEN - 1)) {
//                    int p = LEN / 5;
//                    rectangle(dst, Point(x, y), Point(x + LEN, y + LEN), Scalar(0, 0, 0), FILLED);
//                    rectangle(dst, Point(x + p, y + p), Point(x + LEN - p, y + LEN - p), Scalar(255, 255, 255), FILLED);
//                    rectangle(dst, Point(x + 2 * p, y + 2 * p), Point(x + 3 * p, y + 3 * p), Scalar(0, 0, 0), FILLED);
//                } else if ((i == 0 && (j == 1 || j == ROW / LEN - 2)) ||
//                           (i == 1 && (j == 0 || j == 1 || j == ROW / LEN - 2 || j == ROW / LEN - 1)) ||
//                           (i == COL / LEN - 2 && (j == 0 || j == 1)) || (i == COL / LEN - 1 && j == 1)) { continue; }
//                else if (Source_Data_Buffer_By_Bit[cnt++] == 1)rectangle(dst, Point(x, y), Point(x + LEN, y + LEN), Scalar(0, 0, 0), FILLED);
//            }
//        piccnt++;
//        char dst_file_name[20];
//        sprintf(dst_file_name,"%d.png",piccnt);
//        imwrite(dst_file_name,dst);
//    }
//
//    return piccnt;
//}