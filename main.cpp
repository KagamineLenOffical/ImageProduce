#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

#define Buffer_Size_By_Byte 1024*1024*10
#define Buffer_Size_By_Bit 1024*1024*10*8
#define Data_Block_Size_By_Bit 256      //改变这个可以调节数据块大小。具体多大最合适和错误率挂钩，我没算。暂时不要改动。否则会出错。

using namespace std;
using namespace cv;
const int ROW=1000;
const int COL=1800;
const int LEN=10;
int r=ROW/LEN,c=COL/LEN;
unsigned char Raw_Data_Input_Buffer[Buffer_Size_By_Byte];
bool Raw_Data_Buffer_By_Bit[Buffer_Size_By_Bit];
bool Processed_Data_Buffer_By_Bit[Buffer_Size_By_Bit];
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
    for(int i=0;i<piccnt;i++)
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
    cout << endl <<"Finished!" << endl;
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

    printf("处理后数据量（bit）：%d\n",srclen);
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
                    int k=(Processed_Data_Buffer_By_Bit[cnt]<<2)|(Processed_Data_Buffer_By_Bit[cnt+1]<<1)|(Processed_Data_Buffer_By_Bit[cnt+2]);
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
        
 
        
        char dst_file_name[20];
        sprintf(dst_file_name,"%d.png",piccnt);
        imwrite(dst_file_name,dst);
    }
    return piccnt;
}




void src_produce()
{
    int Parity_Current_Parity_Tracker = 0;    //采用奇校验。亦即算上校验位本身有奇数个1.
    int Parity_Number_Tracker = 0;
    int Processed_Buffer_Marker = 0;
    
    read_file();

    
    

   
    srclen=0;
    contain=ROW/LEN*(COL/LEN)-12;
    FILE *fout=fopen("Raw.out","w+");
    
    for(int i = 0;i < Raw_Data_Length_By_Byte;i++)
    {
        for(int j = 0;j < 8;j++)
        {
            Raw_Data_Buffer_By_Bit[i*8+j]=(Raw_Data_Input_Buffer[i]>>j)&1;
            fprintf(fout,"%d",Raw_Data_Buffer_By_Bit[i*8+j]);
            srclen++;
        }
    }
    printf("处理后数据量（bit）：%d\n",srclen);
    srclen = 0;
    fclose(fout);
    /*----------------------MARK，出错找这里--------------------------*/
    
    
    fout=fopen("Processed.out","w+");
    
    
    for(int i = 0;i < Raw_Data_Length_By_Byte;i++)
    {
        for(int j = 0;j < 8;j++)
        {
            Processed_Data_Buffer_By_Bit[Processed_Buffer_Marker]=(Raw_Data_Input_Buffer[i]>>j)&1;
            
            fprintf(fout,"%d",Processed_Data_Buffer_By_Bit[Processed_Buffer_Marker]);
            srclen++;
            
            
            if( Processed_Data_Buffer_By_Bit[Processed_Buffer_Marker] )
            {
                Parity_Current_Parity_Tracker ++;
            }
            
            
            Processed_Buffer_Marker ++;
            Parity_Number_Tracker ++ ;

            
            
            if( Parity_Number_Tracker == Data_Block_Size_By_Bit)
            {
                if( Parity_Current_Parity_Tracker % 2)
                {
                    fputc('0', fout);
                    Processed_Data_Buffer_By_Bit[Processed_Buffer_Marker] = 0;
                    Processed_Buffer_Marker ++;
                    srclen++;
                }
                else
                {
                    fputc('1', fout);
                    Processed_Data_Buffer_By_Bit[Processed_Buffer_Marker] = 1;
                    Processed_Buffer_Marker ++;
                    srclen++;
                }
                Parity_Current_Parity_Tracker = 0;
                Parity_Number_Tracker = 0;
                
            }
            
        }
    }
    fclose(fout);

    printf("校验添加后为%d比特\n",srclen);
}


