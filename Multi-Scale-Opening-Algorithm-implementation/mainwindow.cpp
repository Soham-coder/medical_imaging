#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include<iostream>
#include<fstream>
#include<list>
#include<cmath>
#include<map>
#include<ctime>
#include<algorithm>
#include <string.h>
#include <stdlib.h>
#define heights 100//512//100//512
#define widths 200//512//200//512
#define depths 190//406//190//287//402//406//250//336//336//287
#define imagefile3 "RFDT_binary.img"//"newAnalyze1016_130-450_dt.img"//"newAnalyze3029.img"//"newAnalyze1016.img"//"newAnalyze2001.img"//"newAnalyze001.img"//"newAnalyze1016.img"
#define intenl 1
#define dividendop 1
#define intenu 450
#define corevessel1 100
#define corevessel2 5
#define fixedmodinten 200
#define threshold 0.01
#define INT_MAX 999999
#define imagefile "dt_RFDT.img"//"newAnalyze1016_130-450_dt.img"//"RFDT_binary.img"// "newAnalyze3029_130-450_dt.img"//"newAnalyze1016_130-450_dt.img"//"newAnalyze2001_130-450_dt.img"//"newAnalyze2001_130-450_dt.img"//"newAnalyze001_130-450_dt.img"//"newAnalyze2005_130-450_dt.img"//"OUTbinarydt_240x240x240.img"//"newAnalyze2005_130-450_dt.img"//"RFDT.img"//"dtbinary1016.img"////"OUT1_1016_512x512x406.img"
#define imagefile2 "RFDT_binary13.img"//"newAnalyze1016_130-450_dt1.img"//"RFDT_binary.img""newAnalyze3029_130-450_dt12.img"//"newAnalyze1016_130-450_dt13.img"//"newAnalyze2001_130-450_dt12.img"//"newAnalyze2005_130-450_dt1.img"//"OUTbinarydt_240x240x2401.img"//"newAnalyze2005_130-450_dt1.img"//"RFDT1.img"// "dtbinary10161.img"//OUT1_1016_512x512x406.img"
#define thetype   unsigned short//unsigned  char //
#define thetype1 short
#define no 1
#define PI 3.14159265
#define multiplier 100
#define precision 0.65
#define spinten 200
#define roundfact 20
using namespace std;

void readimage(short***,char***);
void deallocate_spath(short*** , short*** ,long int* );
int multp=0;
int modifiedinten=fixedmodinten;
int sphereintensity=spinten ;

int height;
int width ;
int depth;
 fstream fout;
 int x_dir[26]={0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,1};
 int y_dir[26]={-1,1,0,0,1,-1,1,-1,-1,1,0,1,1,-1,-1,1,1,-1,-1,-1,0,0,1,0,0,0};
 int z_dir[26]={0,0,1,-1,1,1,-1,-1,0,0,-1,1,-1,1,-1,1,-1,0,1,-1,1,-1,0,0,0,1};
long long int imagedimension;

void getcurrenttime()
{
    time_t rawtime ;
    struct tm* timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    cout<<" time and date: "<<asctime(timeinfo);
}

long long int calculateindex(short x, short y, short z)   //converts a 3d array index into 1d index
{
    return x*depth*width+y*depth+z;
}

float calculatedistance(short xs,short ys,short zs,short xe,short ye,short ze)
{
    return sqrt(pow(xs-xe,2)+pow(ys-ye,2)+pow(zs-ze,2));
}



/****************************************************************MIN HEAP CLASS*************************************************************************/

class latest_start_end
{
public:
    short xs,ys,zs,xe,ye,ze;
    short intenssity;
    latest_start_end()
    {
        xs=ys=zs=xe=ye=ze=-1;
        intenssity = -1;
    }

    latest_start_end(int x1,int y1, int z1, int x2, int y2, int z2)
    {
        xs=x1;ys=y1;zs=z1;xe=x2;ye=y2;ze=z2;
    }
    void change_points(int x1,int y1, int z1, int x2, int y2, int z2)
    {
        xs=x1;ys=y1;zs=z1;xe=x2;ye=y2;ze=z2;
    }
    void change_intensity(short intenss)
    {
        intenssity  = intenss;
    }
};

class minheapnode                           //stores a node of minheap
{
    short x,y,z;

public:
    minheapnode(){}
    minheapnode(short x1,short y1,short z1)
    {
        x=x1;y=y1;z=z1;
    }

    short returnhx(){return x;}
    short returnhy(){return y;}
    short returnhz(){return z;}
};


class minheap                           //minheap class
{
    long long int Size;
    minheapnode* array;

public:

    minheap()
    {

        cout<<"Image dimension : "<<imagedimension<<"\n";
        Size=imagedimension;
        array=new minheapnode[Size];   //allocating memory for the minheap

    }
    void deallocate(){free(array);array=NULL;}
    long long int returnsize(){return Size;}
    void minheapify(short*** dist, long int* pos,long long int idx);
    void Minheap(short*** dist, long int* pos);
    bool isinminheap( long int* pos,long long int v);
    void insertinheap(minheapnode newnode);
    minheapnode extractmin(short*** dist, long int* pos);
    void update(short*** dist, long int* pos,short x, short y, short z,short newdist);
    void setposition( long int* pos,short x, short y,short z);
    void swap(minheapnode* a ,minheapnode* b);
};

/*************************************************************MIN HEAP IMPLEMENTATION*******************************************************************/

 void minheap :: swap(minheapnode* a, minheapnode* b)
{
    minheapnode t;
    t=*a;
    *a=*b;
    *b=t;
}

bool minheap :: isinminheap( long int* pos,long long int v)
{
    if(pos[v]<Size)
        return true;
    return false;
}

void minheap :: minheapify(short*** dist, long int* pos,long long int idx)
{
        long long int left=2*idx+1;
        long long int right=2*idx+2;

        long long smallest=idx;

        if(left < Size && dist[array[left].returnhx()][array[left].returnhy()][array[left].returnhz()]<dist[array[smallest].returnhx()][array[smallest].returnhy()][array[smallest].returnhz()])
        {
            smallest=left;
        }

         if(right<Size && dist[array[right].returnhx()][array[right].returnhy()][array[right].returnhz()]<dist[array[smallest].returnhx()][array[smallest].returnhy()][array[smallest].returnhz()])
        {
            smallest=right;
        }

        if(idx!=smallest)
        {
           minheapnode smallestnode = array[smallest];
            minheapnode idxNode =array[idx];
            pos[calculateindex(smallestnode.returnhx(),smallestnode.returnhy(),smallestnode.returnhz())]=idx;
            pos[calculateindex(idxNode.returnhx(),idxNode.returnhy(),idxNode.returnhz())]=smallest;

            swap(&array[idx],&array[smallest]);
            minheapify(dist,pos,smallest);
        }
    }

void minheap :: Minheap(short*** dist, long int* pos)
{
    long long int i,n=Size-1;
    for(i=(n-2)/2;i>=0;i--)
    {
        minheapify(dist,pos,i);
    }
}

minheapnode minheap :: extractmin(short*** dist, long int* pos)
{
    if(Size==0)
    {
        cout<<"Heap is Empty\n";
    }
    else
    {
        minheapnode exmin = array[0];
        array[0]=array[Size-1];
        pos[calculateindex(exmin.returnhx(),exmin.returnhy(),exmin.returnhz())]=Size-1;
        pos[calculateindex(array[Size-1].returnhx(),array[Size-1].returnhy(),array[Size-1].returnhz())]=0;
        Size=Size-1;
        minheapify(dist,pos,0);
        return exmin;
    }
}

void minheap :: setposition(long  int* pos,short x, short y,short z)
{
    long long int index=x*width*depth+y*depth+z;
    pos[index]=index;
}

void minheap :: update(short*** dist, long int* pos,short x, short y, short z, short newdist)
{
    long long int index=x*width*depth+y*depth+z;
    long long int in=pos[index];
    array[in] = minheapnode(x,y,z);
    long long int i=in;
    while (i && dist[array[i].returnhx()][array[i].returnhy()][array[i].returnhz()] < dist[array[(i - 1) / 2].returnhx()][array[(i-1)/2].returnhy()][array[(i-1)/2].returnhz()])
    {

        pos[calculateindex(array[i].returnhx(),array[i].returnhy(),array[i].returnhz())] = (i-1)/2;
        pos[calculateindex(array[(i-1)/2].returnhx(),array[(i-1)/2].returnhy(),array[(i-1)/2].returnhz())] = i;
        swap(&array[i], &array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

/***************************************************END OF MINHEAP IMPLEMENTATION*************************************************************************/

/***************************************************SPHERE DRAWING CODE***********************************************************************************/



class spathfind
{
    short xs; short ys ; short zs;
    short xe; short ye ; short ze;
    short dtvalue;
    public:
    spathfind(short dtval,short xe1,short ye1,short ze1,short xs1,short ys1,short zs1 )
    {
        xs=xs1;ys=ys1;zs=zs1;
        xe=xe1;ye=ye1;ze=ze1;
        dtvalue=dtval;

    }
    spathfind(){}
    void setdtvalue(short dtval){dtvalue=dtval;}
    short returnxsr(){return xs;}
    short returnysr(){return ys;}
    short returnzsr(){return zs;}
    short returnxm(){return xe;}
    short returnym(){return ye;}
    short returnzm(){return ze;}
    short returndt(){return dtvalue;}

};

map<long long int , spathfind> maps;
map<long long int , short> intensitymap;
map<long long int , short> sintensitymap;
map<int,list<spathfind> > pathmap;
map<int,float> distmap;
int sphereflag=0;

map<long long int , short> smoothspherex( short*** a,list<spathfind> intenlist2,unsigned short r, int xc,int yc,int zc)
{
 //  list<spathfind> intenlist2;
    if(r==0)
        r=1;
   int k=xc;
   for(int i=zc-r;i<zc+r;i++)
   {
       for(int j=yc;j<yc+r;j++)
       {

           {
           if(i-1>=0 && j-1>=0 && i+1<depth && j+1<width && a[k][j][i]<sphereintensity && a[k][j-1][i]!=0 && a[k][j][i-1]!=0 && a[k][j][i+1]!=0 && a[k][j+1][i]!=0)
           {
               if(a[k][j][i]<sphereintensity){
               intensitymap.insert(std :: make_pair(calculateindex(k,j,i),a[k][j][i]));
               intenlist2.push_back(spathfind(a[k][j][i],k,j,i,-1,-1,-1));
               }
               a[k][j][i]=sphereintensity;
           }
           }
       }
   }

   for(int i=zc-r;i<zc+r;i++)
   {
       for(int j=yc;j>=yc-r;j--)
       {
           //for(k=zc;k>=zc-r;k--)
           {
          if(i-1>=0 && j-1>=0 && i+1<depth && j+1<width && a[k][j][i]<sphereintensity && a[k][j-1][i]!=0 && a[k][j][i-1]!=0 && a[k][j][i+1]!=0 && a[k][j+1][i]!=0)
           {
               if(a[k][j][i]<sphereintensity){
               intensitymap.insert(std :: make_pair(calculateindex(k,j,i),a[k][j][i]));
               intenlist2.push_back(spathfind(a[k][j][i],k,j,i,-1,-1,-1));
               }
               a[k][j][i]=sphereintensity;
           }
       }
       }
   }
   return intensitymap;
}

map<long long, short> draw_pointsx(short*** a,list<spathfind>& intenlist2,int r,int k,int yc,int zc,int x, int y )//( change here you morone
{
   fstream sfp;


   if(zc+x<depth && yc-y>=0){

    a[k][yc-y][zc+x]=sphereintensity;
   }

    if(zc-x>=0 && yc-y>=0){

    a[k][yc-y][zc-x]=sphereintensity;
    }

    if(zc+x<depth && yc+y<width){

    a[k][yc+y][zc+x]=sphereintensity;
    }

    if(zc-x>=0 && yc+y<width){

        a[k][yc+y][zc-x]=sphereintensity;
    }

    if(zc+y<depth && yc-x>=0){

   a[k][yc-x][zc+y]=sphereintensity;
    }

    if(zc-y>=0 && yc-x>=0){

    a[k][yc-x][zc-y]=sphereintensity;
    }

    if(zc+y<depth && yc+x<width){

    a[k][yc+x][zc+y]=sphereintensity;
    }

   if(zc-y>=0 && yc+x<width){

    a[k][yc+x][zc-y]=sphereintensity;
   }

    return intensitymap;
}





map<long long int,short> drawspherex( short*** a,list<spathfind>& intenlist2,int r,int xc,int yc,int zc)
{
    QMessageBox qm;

  //  list<spathfind> intenlist2;
  // printf("Draw sphere : %d %d %d : %d\n",xc,yc,zc,a[xc][yc][zc]);
    if(r<=0)
        r=1;
  cout<<xc<<" "<<yc<<" "<<zc<<" : radius : "<<r<<"\n";
   unsigned char* b=new unsigned char[depth];
   int x=0;
   int z=r;
   b[xc]=z;
   b[xc+z]=x;
   b[xc-z]=x;
   int p=1-r;
   while(x<=z)
   {
      if (p<0)
       {
          p=(p+(2*x)+3);
          //x=x;
          x=x+1;
       }
      else
      {
          p=p+(2*(x-z)+5);
          z=z-1;
          x=x+1;
      }

   if(z>0)
   {
    /*
     *change bc[zc+z] = z;
     *change bc[zc-z] = z;
      */
    b[xc+z]=x;
    b[xc-z]=x;
    b[xc+x]=z;
    b[xc-x]=z;
   }
  }

   for(int k=xc;k>=xc-r;k--)
   {

       int y=b[k];
       int r1=y;
       for(int c=0;c<=r1;c++)
       {
          int x=0;
          //int c=r;
          int y=c;
          int p=1-c;
          if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = draw_pointsx(a,intenlist2,c,k,yc,zc,x,y); //do not replace c by r
           while(x<=y)
           {
            if (p<=0)
            {
              p=(p+(2*x)+3);
              y=y;
              x++;
            }
            else
            {
              p=p+(2*(x-y)+5);
              y=y-1;
              x++;
            }
          if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = draw_pointsx(a,intenlist2,c,k,yc,zc,x,y);
        }
           if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = smoothspherex(a,intenlist2,c,k,yc,zc);
       }
}
   for(int k=xc;k<=xc+r;k++)
   {

       int y=b[k];
       int r1=y;
       for(int c=0;c<=r1;c++)
       {
           int x=0;
           int y=c;
           int p=1-c;
          if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = draw_pointsx(a,intenlist2,c,k,yc,zc,x,y);
           while(x<=y)
           {
            if (p<=0)
            {
              p=(p+(2*x)+3);
              y=y;
              x++;
            }
            else
            {
              p=p+(2*(x-y)+5);
              y=y-1;
              x++;
            }
            if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = draw_pointsx(a,intenlist2,c,k,yc,zc,x,y);
           }
              if(zc-r>0 && yc-r>0 && zc+r<depth && yc+r<width && k+r<height && k-r>=0)
          intensitymap = smoothspherex(a,intenlist2,c,k,yc,zc);
       }

}

   return intensitymap;
}





map<long long, short> draw_points(short*** a,list<spathfind>& intenlist2,int r,int xc,int yc,int k,int x, int y )//( change here you morone
{
    fstream sfp;
  //  sfp.open("Spherefileinten.txt",ios::in);
  //  if(!sfp)
   // {
//
   // }
    //else
   // sfp.read((char*)&sintensitymap,sizeof(map<long long int , short> ));
   // sfp.close();
  //  sfp.open("Spherefileinten.txt",ios::out);

    if(xc+x<height && yc-y>=0){
     //   if(a[xc+x][yc-y][k]<fixedmodinten)
    //   sintensitymap.insert(std :: make_pair(calculateindex(xc+x,yc-y,k),a[xc+x][yc-y][k]));
     a[xc+x][yc-y][k]=sphereintensity;
    }

     if(xc-x>=0 && yc-y>=0){
//          if(a[xc-x][yc-y][k]<fixedmodinten)
//     sintensitymap.insert(std :: make_pair(calculateindex(xc-x,yc-y,k),a[xc-x][yc-y][k]));
     a[xc-x][yc-y][k]=sphereintensity;
     }

     if(xc+x<height && yc+y<width){
       //    if(a[xc+x][yc+y][k]<fixedmodinten)
     // sintensitymap.insert(std :: make_pair(calculateindex(xc+x,yc+y,k),a[xc+x][yc+y][k]));
     a[xc+x][yc+y][k]=sphereintensity;
     }

     if(xc-x>=0 && yc+y<width){
     //      if(a[xc-x][yc+y][k]<fixedmodinten)
    //   sintensitymap.insert(std :: make_pair(calculateindex(xc-x,yc+y,k),a[xc-x][yc+y][k]));
         a[xc-x][yc+y][k]=sphereintensity;
     }

     if(xc+y<height && yc-x>=0){
        //   if(a[xc+y][yc-x][k]<fixedmodinten)
        //  sintensitymap.insert(std :: make_pair(calculateindex(xc+y,yc-x,k),a[xc+y][yc-x][k]));
    a[xc+y][yc-x][k]=sphereintensity;
     }

     if(xc-y>=0 && yc-x>=0){
         //  if(a[xc-y][yc-x][k]<fixedmodinten)
        //  sintensitymap.insert(std :: make_pair(calculateindex(xc-y,yc-x,k),a[xc-y][yc-x][k]));
     a[xc-y][yc-x][k]=sphereintensity;
     }

     if(xc+y<height && yc+x<width){
         //  if(a[xc+y][yc+x][k]<fixedmodinten)
         //  sintensitymap.insert(std :: make_pair(calculateindex(xc+y,yc+x,k),a[xc+y][yc+x][k]));
     a[xc+y][yc+x][k]=sphereintensity;
     }

    if(xc-y>=0 && yc+x<width){
         // if(a[xc-y][yc+x][k]<fixedmodinten)
         // sintensitymap.insert(std :: make_pair(calculateindex(xc-y,yc+x,k),a[xc-y][yc+x][k]));
     a[xc-y][yc+x][k]=sphereintensity;
    }
  //  sfp.write((char*)&sintensitymap,sizeof(map<long long int , short> ));
  //  sfp.close();
     return intensitymap;
}


 map<long long int , short> smoothsphere( short*** a,list<spathfind> intenlist2,unsigned short r, int xc,int yc,int zc)
{
  //  list<spathfind> intenlist2;
     if(r==0)
         r=1;
    int k=zc;
    for(int i=xc-r;i<xc+r;i++)
    {
        for(int j=yc;j<yc+r;j++)
        {
            //for(k=zc;k<zc+r;k++)
            {
            if(i-1>=0 && j-1>=0 && i+1<height && j+1<width && a[i][j][k]<sphereintensity && a[i][j-1][k]!=0 && a[i-1][j][k]!=0 && a[i+1][j][k]!=0 && a[i][j+1][k]!=0)
            {
                if(a[i][j][k]<sphereintensity){
                intensitymap.insert(std :: make_pair(calculateindex(i,j,k),a[i][j][k]));
                intenlist2.push_back(spathfind(a[i][j][k],i,j,k,-1,-1,-1));
                }
                a[i][j][k]=sphereintensity;
            }
            }
        }
    }

    for(int i=xc-r;i<xc+r;i++)
    {
        for(int j=yc;j>=yc-r;j--)
        {
            //for(k=zc;k>=zc-r;k--)
            {
            if(i-1>=0 && j-1>=0 && i+1<height && j+1<width && a[i][j][k]<sphereintensity && a[i-1][j][k]!=0 && a[i+1][j][k]!=0 && a[i][j+1][k]!=0&& a[i][j-1][k]!=0)
            {
                 if(a[i][j][k]<sphereintensity){
                 intensitymap.insert(std :: make_pair(calculateindex(i,j,k),a[i][j][k]));
                 intenlist2.push_back(spathfind(a[i][j][k],i,j,k,-1,-1,-1));
                 }
                a[i][j][k]=sphereintensity;
            }
        }
        }
    }
    return intensitymap;
}



 map<long long int,short> drawsphere( short*** a,list<spathfind>& intenlist2,int r,int xc,int yc,int zc)
{
     QMessageBox qm;

   //  list<spathfind> intenlist2;
   // printf("Draw sphere : %d %d %d : %d\n",xc,yc,zc,a[xc][yc][zc]);
     if(r<=0)
         r=1;
   //  r+=2;

   //cout<<xc<<" "<<yc<<" "<<zc<<" : radius : "<<r<<"\n";
    unsigned char* b=new unsigned char[depth];
    int z=0;
    int x=r;
    b[zc]=x;
    b[zc+x]=z;
    b[zc-x]=z;
    int p=1-r;
    while(z<=x)
    {
       if (p<0)
        {
           p=(p+(2*z)+3);
           x=x;
           z=z+1;
        }
       else
       {
           p=p+(2*(z-x)+5);
           x=x-1;
           z=z+1;
       }

    if(z>0)
    {
     /*
      *change bc[zc+z] = z;
      *change bc[zc-z] = z;
       */
     b[zc+z]=x;
     b[zc-z]=x;
     b[zc+x]=z;
     b[zc-x]=z;
    }
   }

    for(int k=zc;k>=zc-r;k--)
    {

        int y=b[k];
        int r1=y;
        for(int c=0;c<=r1;c++)
        {
           int x=0;
           //int c=r;
           int y=c;
           int p=1-c;
           if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
           intensitymap = draw_points(a,intenlist2,c,xc,yc,k,x,y); //do not replace c by r
            while(x<=y)
            {
             if (p<=0)
             {
               p=(p+(2*x)+3);
               y=y;
               x++;
             }
             else
             {
               p=p+(2*(x-y)+5);
               y=y-1;
               x++;
             }
             if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0 )
                intensitymap = draw_points(a,intenlist2,c,xc,yc,k,x,y); //replace c by r
         }
            if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth)
           intensitymap =  smoothsphere(a,intenlist2,c,xc,yc,k);
        }
 }
    for(int k=zc;k<=zc+r;k++)
    {

        int y=b[k];
        int r1=y;
        for(int c=0;c<=r1;c++)
        {
            int x=0;
            int y=c;
            int p=1-c;
            if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
            intensitymap = draw_points(a,intenlist2,c,xc,yc,k,x,y); //replace c by r
            while(x<=y)
            {
             if (p<=0)
             {
               p=(p+(2*x)+3);
               y=y;
               x++;
             }
             else
             {
               p=p+(2*(x-y)+5);
               y=y-1;
               x++;
             }
             if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
              intensitymap = draw_points(a,intenlist2,c,xc,yc,k,x,y);  //replace c by r
            }
            if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
            intensitymap = smoothsphere(a,intenlist2,c,xc,yc,k);
        }

 }

    return intensitymap;
}



 map<long long, short> draw_circle(short*** a,list<spathfind>& intenlist2,int r,int xc,int yc,int zc)
 {
     //  list<spathfind> intenlist2;
     // printf("Draw sphere : %d %d %d : %d\n",xc,yc,zc,a[xc][yc][zc]);
         //    cout<<xc<<" "<<yc<<" "<<zc<<" : radius : "<<r<<"\n";
             int k=zc;
             int x=0;
             int c=r;
             int y=c;
             int p=1-c;
             if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
             intensitymap = draw_points(a,intenlist2,r,xc,yc,k,x,y);

              while(x<=y)
              {

               if (p<=0)
               {
                 p=(p+(2*x)+1);
                 x++;
               }
               else
               {
                 p=p+(2*(x-y)+1);
                 y=y-1;
                 x++;
               }
               if(xc-r>0 && yc-r>0 && xc+r<height && yc+r<width && k+r<depth && k-r>=0)
               intensitymap = draw_points(a,intenlist2,r,xc,yc,k,x,y);
           }


             return intensitymap;
          }


/***************************************************END OF SPHERE DRAWING CODE****************************************************************************/

class Point
{
    public:
    short x1,  y1,  z1 , x2 , y2 , z2;
    short rcnt;
    Point(int rc,short x , short y, short z , short x3 = -1 , short y3 = -1, short z3=-1 )
    {
        x1=x;y1=y;z1=z;
        x2=x3;y2=y3;z2=z3;
        rcnt=rc;
    }
    Point()
    {

    }

};
class storemin
{
    short x,y,z;

    public:
    storemin(short x1, short y1, short z1)
    {
        x=x1;
        y=y1;
        z=z1;
    }
    storemin(){x=y=z=-1;}
    void setcoordinate(int x1,int y1,int z1)
    {   x=x1;
        y=y1;
        z=z1;
        }
    short returnx(){return x;}
    short returny(){return y;}
    short returnz(){return z;}
};
/*****************************************************Connectivity Graph******************************************************************************/

/*
* keeps track of the connectivity of the segmented paths.if path 1 and 2 are connected then graph has a undirected path from
* node-1 to node-2. so to find all the segmented path to which a particular segment is connected we just have to do a dfs from
* the given segment.
*/

class cgraph
{
    short size;
    short pathno;
    short intensity;
    bool* visited;
    list<int>* adjlist;
    list<int> conlist;
public:
    cgraph(int len)
    {
        size=len;
        adjlist = new list<int> [size];
        visited = new bool[size];
    }

    void addedge(int a , int b)
    {
        adjlist[a].push_back(b);
        adjlist[b].push_back(a);
    }

    void remove_edge(int src)
    {
        QMessageBox dmsg  ;
        QString sstr;
        sstr.setNum(src);
        dmsg.setText("Entering Delete_edge : "+sstr);
        dmsg.exec();
        cout<<"Entering Edge\n";
       list<int> :: iterator i;//=adjlist[src].begin();
      // cout<<"size of list : "<<adjlist[src].size()<<endl;
       for(i=adjlist[src].begin(); i!=adjlist[src].end() && adjlist[src].size()>0;i++)
       {
           cout<<"deleting ... "<<*i<<endl;
          // adjlist[src].remove(*i);
           adjlist[*i] .remove(src);
       }
       adjlist[src].clear();

       cout<<"Delete Operation Complete\n";
    }

    void setfalse(){for(int i=0;i<size;i++) visited[i]=false;}

    bool connectivity(int src, int dest)
    {
        visited[src]=true;
        list<int> :: iterator i;
        for(i=adjlist[src].begin(); i!=adjlist[src].end();i++)
        {
           // cout<<"Neighbour of "<<src<<" : "<<*i<<"\n";
            if(*i==dest) {cout<<"Connected to : "<<*i<<"\n"; return true;}
            else if(visited[*i]==false)
            {
                return connectivity(*i,dest);
            }
        }

        return false;
    }
    list<int> getconnectedcomp(int src)
    {
        visited[src]=true;
        conlist.push_back(src);
        list<int> :: iterator i;
       for(i=adjlist[src].begin(); i!=adjlist[src].end();i++)
        {

             if(visited[*i]==false)
            {
               // cout<<*i<<"\n";
                return getconnectedcomp(*i);
            }
        }
        return conlist;
    }
};
/*****************************************************Other Necessary Functions************************************************************************/


void printlist(list<int> intlist,bool* cvisited)
{
    list<int> :: iterator i;
    for(i=intlist.begin();i!=intlist.end();i++)
    {
        cout<<*i<<" ";

    }
}

float calculategaussian(float mean , float varience, float nratio)
{

    return (1/sqrt(2*PI*varience*varience))*(exp(-(pow((double)(nratio-mean),2))/(2*varience*varience)));
}


class distclass
{
public:
    float dist;
    short x,y,z;
    int intens;

    distclass(){x=y=z=-1;intens=0;dist=0;}
    distclass(short x1, short y1, short z1,int in)
    {
        x=x1;y=y1;z=z1;intens=in;
    }
};

distclass findclosestpoint(int sno , int dno)
{
    list<spathfind> slist = pathmap[sno];
    list<spathfind> dlist = pathmap[dno];

    list<spathfind> :: iterator it1;
    list<spathfind> :: iterator it2;
    distclass cdc;

    float dist,mindist=999;
    short xs,ys,zs,xe,ye,ze;
    for(it1=slist.begin();it1!=slist.end();it1++)
    {
        xs=(*it1).returnxsr();ys=(*it1).returnysr();zs=(*it1).returnzsr();
        for(it2=dlist.begin();it2!=dlist.end();it2++)
        {
             xe=(*it2).returnxsr();ye=(*it2).returnysr();ze=(*it2).returnzsr();
             dist = calculatedistance(xs,ys,zs,xe,ye,ze);
             if(dist<mindist)
             {
                 mindist = dist;
                 cdc.x=xs;cdc.y=ys;cdc.z=zs;
             }
        }
    }

    return cdc;
}

/**
 * find minimum distance segment from the given point
 */
distclass findmindistance(list<spathfind> pathlist, short x , short y , short z)
{
    list<spathfind> :: iterator itr;
    int mindistance = 999;
    int distance;
    distclass dsc;
    for(itr=pathlist.begin();itr!=pathlist.end();itr++)
    {

        distance = calculatedistance((*itr).returnxsr(),(*itr).returnysr(),(*itr).returnzsr(),x,y,z);

        if(distance<mindistance)
        {
            mindistance=distance;
            dsc.x = (*itr).returnxsr();
            dsc.y = (*itr).returnysr();
            dsc.z = (*itr).returnzsr();
            dsc.dist = mindistance;

        }
    }

    return dsc;
}

float findangle(distclass ssp , distclass sep , distclass dsp, distclass dep)
{
    short xs=ssp.x-sep.x;short ys=ssp.y-sep.y;short zs=ssp.z-sep.z;
    short xd=dsp.x-dep.x;short yd=dsp.y-dep.y;short zd=dsp.z-dep.z;
    float numerator = xs*xd+ys*yd+zs*zd;
    float denominator = sqrt(xs*xs+ys*ys+zs*zs)*sqrt(xd*xd+yd*yd+zd*zd);
    float angle = numerator/denominator;
     angle = asin(angle)*180/PI;
     return angle;
}

int findclosestpath (bool* cvisited,cgraph cg , short xd , short yd, short zd , short xs, short ys , short zs , int dpno , int spno)
{
    float minangle = 999 , angle;
    list<spathfind> npi;
    distclass fdsp,fdep;
    int fp;

    list<int> nlist = cg.getconnectedcomp(spno);

    list<spathfind> spat = pathmap[dpno];
    spathfind temp = spat.front();
    distclass dsp(temp.returnxsr(),temp.returnysr(),temp.returnzsr(),dpno);
    temp = spat.back();
    distclass dep(temp.returnxsr(),temp.returnysr(),temp.returnzsr(),dpno);

    list<int> :: iterator it;
    it=nlist.begin();

    for(;it!=nlist.end();it++)
    {
        cvisited[*it]=true;
        npi = pathmap[*it];
        temp = spat.front();
        distclass ssp(temp.returnxsr(),temp.returnysr(),temp.returnzsr(),spno);
        temp = spat.back();
        distclass sep(temp.returnxsr(),temp.returnysr(),temp.returnzsr(),spno);
        angle = findangle(ssp,sep,dsp,dep);
        if(minangle>angle)
        {
            angle = minangle;
            fp = *it;
        }
    }

    return fp;
}


/*
* finds the longest path in shortest distance from the given point.
*/

distclass findsegpath(short x,short y,short z)
{
    float length,distance;
    distclass maxdc;
    float ratio,maxratio=-999;
    for (std::map<int,list<spathfind> > ::iterator it=pathmap.begin(); it!=pathmap.end(); ++it)
    {
        int pathnum = it->first;
        list<spathfind> plist=it->second;
        distclass dc = findmindistance(plist,x,y,z);
        distance = dc.dist;
        length = distmap[pathnum];

        length++;
        ratio = length/distance;

       // ratio = distance/length;

        if(ratio>maxratio)
        {
            dc.intens=pathnum+fixedmodinten;
            maxdc = dc;
            maxratio = ratio;
        }
    }

    return maxdc;
}

short setweight(short xs,short ys, short zs, short xe,short ye, short ze)
{

    float weight = sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys)+(ze-zs)*(ze-zs));
    return weight*10;

}

/**
 * @brief setfdtweight determines the fdt weight between the two given points
 * @param visited keeps track of the segments visited by the algorithm in the current query
 * @param cg instance of the connected graph
 * @param intensitymap holds the original intenisty of the modifed points
 * @param types type of the query
 * @param intype1 intensity of start point
 * @param intype2 intensity of end point

 * @return
 */

short setfdtweight(list<int> intlist,bool* visited,cgraph cg,short*** arr,map<long long int,short> intensitymap , int types,short intype1,short intype2,short xs,short ys, short zs, short xe,short ye, short ze)
{
    float dens,ndens;


    //if intensity of the source point is greater than 200(fixedmodinten)
    if(arr[xs][ys][zs]==-1 || arr[xs][ys][zs]>=fixedmodinten)
    {

        if(types==0)
        dens=1;

        else if(types==2)
        {
          if(intype1==arr[xs][ys][zs]) dens=arr[xs][ys][zs];

          else if(arr[xs][ys][zs]!=intype1 && visited[arr[xs][ys][zs]-fixedmodinten]==false)
          {

              cg.setfalse();
              if(cg.connectivity(arr[xs][ys][zs]-fixedmodinten,intype1-fixedmodinten)==true)
              {
                  dens=arr[xs][ys][zs];
              }

              else dens = intensitymap[calculateindex(xs,ys,zs)]/dividendop;
              visited[arr[xs][ys][zs]-fixedmodinten]=true;
          }

          else dens =intensitymap[calculateindex(xs,ys,zs)]/dividendop;
         // else dens=1;
        }

        else if(types==5)
        {
          if(intype1==arr[xs][ys][zs]) dens=arr[xs][ys][zs];

          else if(arr[xs][ys][zs]!=intype1 && visited[arr[xs][ys][zs]-fixedmodinten]==false)
          {


              dens=intensitymap[calculateindex(xs,ys,zs)]/dividendop;

              visited[arr[xs][ys][zs]-fixedmodinten]=true;
          }

          else dens = intensitymap[calculateindex(xs,ys,zs)]/dividendop;

        }

        else if(types==3 || types==4)
            {

                     if(intype1==arr[xs][ys][zs])
                     {
                        if(types==3) {dens=1; }
                        else if(types==4)
                        {
                        dens=intensitymap[calculateindex(xs,ys,zs)]/dividendop;
                        }
                     }
                else if(intype2==arr[xs][ys][zs]) {dens=arr[xs][ys][zs];}
                else if(arr[xs][ys][zs]!=intype2  && visited[arr[xs][ys][zs]-fixedmodinten]==false)
                {
                   if(types==3)
                    {

                        if(std::find(intlist.begin(), intlist.end(), arr[xs][ys][zs]-fixedmodinten) != intlist.end())
                        {

                            dens=1;
                        }
                        else {dens=intensitymap[calculateindex(xs,ys,zs)]/dividendop; }
                    }
                    else if(types==4)
                    {

                      dens = intensitymap[calculateindex(xs,ys,zs)]/dividendop;

                    }
                    visited[arr[xs][ys][zs]-fixedmodinten]=true;
                }
                else dens = intensitymap[calculateindex(xs,ys,zs)]/dividendop;
           }

        //change here***********************
        else dens = 1;//intensitymap[calculateindex(xs,ys,zs)];//1

    }

     else
     dens = abs(arr[xs][ys][zs]);


      if(arr[xe][ye][ze]==-1 || arr[xe][ye][ze]>=fixedmodinten)
     {

         if(types==0)
         {
          ndens=1;
         }
         else if(types==2)
        {
          if(intype1==arr[xe][ye][ze])ndens=arr[xe][ye][ze];
          else if(arr[xe][ye][ze]!=intype1 && visited[arr[xe][ye][ze]-fixedmodinten]==false)
          {
              cg.setfalse();
              if(cg.connectivity(arr[xe][ye][ze]-fixedmodinten,intype1-fixedmodinten)==true)
              {
                  ndens=arr[xe][ye][ze];
              }
              else ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;
               visited[arr[xe][ye][ze]-fixedmodinten]=true;
          }
               else ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;
        }

        else if(types==5)
        {
          if(intype1==arr[xe][ye][ze])ndens=arr[xe][ye][ze];
          else if(arr[xe][ye][ze]!=intype1 && visited[arr[xe][ye][ze]-fixedmodinten]==false)
          {

               ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;
               visited[arr[xe][ye][ze]-fixedmodinten]=true;
          }
               else ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;
        }
         else if(types==3 || types==4)
            {

                    if(intype1==arr[xe][ye][ze])
                    {
                        if(types==3) {ndens=1;  }

                        else if(types==4)
                        ndens=intensitymap[calculateindex(xe,ye,ze)]/dividendop;
                    }

                else if(intype2==arr[xe][ye][ze]) {ndens=arr[xe][ye][ze];}

                else if(arr[xe][ye][ze]!=intype2  && visited[arr[xe][ye][ze]-fixedmodinten]==false)
                {
                    if(types==3)
                    {
                        if(std::find(intlist.begin(), intlist.end(), arr[xe][ye][ze]-fixedmodinten) != intlist.end())
                        {

                            ndens=1;
                        }
                        else {ndens=intensitymap[calculateindex(xe,ye,ze)]/dividendop;}
                    }
                    else if(types==4)
                    {

                      ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;

                    }
                    visited[arr[xe][ye][ze]-fixedmodinten]=true;
                }
                else ndens = intensitymap[calculateindex(xe,ye,ze)]/dividendop;
           }
           //change here***********************
         else ndens =1;//intensitymap[calculateindex(xe,ye,ze)];//1

     }
     else
     ndens=abs(arr[xe][ye][ze]);

    float fdt = 1/ndens;//float fdt = 2/(dens+ndens);

    float weight = sqrt((xe-xs)*(xe-xs)+(ye-ys)*(ye-ys)+(ze-zs)*(ze-zs))*fdt;
    //float weight=fdt;

   // cout <<weight <<" ";
    return weight*multiplier;

}



/*************************************************Read , Modify , Display Functions*****************************************************************/

void update_spherearr(short*** arr , char*** spherearr)
{
    QMessageBox qmsg;
    long int zncnt=0;
    int i,j,k;
    cout<<"height : "<<height<<" "<<"width "<<width<<" "<<"depth : "<<depth<<"\n";
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(arr[i][j][k]>=spinten){
                    zncnt++;
                if(spherearr[k][j][i]!=corevessel2)
                spherearr[k][j][i]=corevessel1;
                }
            }
        }
    }

    cout<<"Total artery voxels : "<<zncnt<<"\n";
}

void write_vessel_file( char*** spherearr)
{
    fstream outf;
    outf.open("vseedfile.txt",ios::out);
    for(int i=0; i<height; i++){
        for(int j=0;j<width;j++){
            for(int k=0;k<depth;k++){
                outf.write((char*)&spherearr[k][j][i],sizeof(char));
            }
        }
    }
    outf.close();
}

void modifyimagegui(short*** arr)
{
   fstream outfile;
   outfile.open(imagefile, ios::out | ios :: binary);
   QMessageBox msg;

   long long int i,j,k,x,y,z,is1005=0;
   for( i=0;i<depths;i++)
   {
       for( j=0;j<widths;j++)
       {
           for(k=0;k<heights;k++)
           {
                   outfile.write((char*)&arr[k][j][i],sizeof(thetype1));
           }
       }
   }


   outfile.close();
}
 void modifyimage(short*** arr)
{
    fstream outfile;
    outfile.open(imagefile2, ios::out | ios :: binary);
    QMessageBox msg;
    //if(modifiedinten>=205)
    {
  //  msg.setText("Writing Image\n");
    //msg.exec();
    }
    long long int i,j,k,x,y,z,is1005=0;
    if(is1005!=0){
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(arr[i][j][k]>=spinten){

                   // arr[i][j][k]=0;
                    outfile.write((char*)&arr[i][j][k],sizeof(thetype));
                }
                else
                {
                    int mintensity=0;
                    outfile.write((char*)(&mintensity),sizeof(thetype));
                }
            }
        }
    }
    }
    else if(is1005==0)
    {
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(arr[i][j][k]>=spinten){

                   // arr[i][j][k]=0;
                    outfile.write((char*)&arr[i][j][k],sizeof(thetype));
                }
                else
                {
                    int mintensity=0;
                    outfile.write((char*)(&mintensity),sizeof(thetype));
                }
            }
        }
    }
    }
    outfile.close();
}

map<long long int , short> delete_edge(cgraph cg,short*** arr,int inten)
{
    fstream dout;
    dout.open("Delete_intensity.txt",ios::out);
    QMessageBox dmsg;
    list<spathfind> intenlist = pathmap[inten];
    cg.remove_edge(inten);

    pathmap.erase(inten);

    list<spathfind> ::  iterator i;
    int hi=0;
    spathfind sp;
    for(i=intenlist.begin();i!=intenlist.end();i++)
    {
        sp = *i;

        dout<<sp.returnxm()<<" "<<sp.returnym()<<" "<<sp.returnzm()<<" "<<sp.returndt()<<"\n";
        arr[sp.returnxm()][sp.returnym()][sp.returnzm()]=sp.returndt();

        intensitymap.erase(calculateindex(sp.returnxm(),sp.returnym(),sp.returnzm()));
    }

        arr[sp.returnxsr()][sp.returnysr()][sp.returnzsr()]=sp.returndt();
        intensitymap.erase(calculateindex(sp.returnxsr(),sp.returnysr(),sp.returnzsr()));
//    spathfind sp = *i;
//    arr[sp.returnxm()][sp.returnym()][sp.returnzm()]=sp.returndt();
//    intensitymap.erase(calculateindex(sp.returnxm(),sp.returnym(),sp.returnzm()));
      distmap.erase(inten); // erase seed points of the artery
      distmap.insert(std::make_pair(inten,1));
    dout.close();
    return intensitymap;
}

int round(int  num)
{
    int rad = num/roundfact;
    int remain = num%roundfact;
    if(remain>=5 )
    {

        return rad+1;
    }
    if(remain<5 && rad<=0)
    {
        if(remain>=1 && remain<=3)
        {
            rad=1;
        }
        else if(remain>=3 && remain<5)
        {
            rad=2;
        }
    }
     return rad;
}

/**
 * @brief readandmodifyimage draws sphere in each point found by the shortest path algorithm
 * where radius of the sphere is the (dtvalue of the point)/10.
 * also adds edges between segments if there is any intersection between new segment and old segments
 */
map<long long int, short> readandmodifyimage(cgraph cg,short*** arr,short xs,short ys,short zs,short xe,short ye,short ze,map<long long int,short> intensitymap,char*** spherearr)
{
    fstream readf;
    fstream infile,sfp;
    fstream outfile,foutfile;
    list<spathfind> intenlist;
    list<spathfind> intenlist2;
    bool* visited=new bool[1000];
    readf.open("read_file.txt",ios::out);
    for(int i=0;i<1000;i++)
    {
        visited[i]=false;
    }
    sfp.open("sep",ios::out);
    infile.open("ShortestPath",ios :: in);
   // outfile.open("AllShortestPath",ios::out | ios::app);
    QMessageBox fmsg,qm1;
   // qm1.setText("Draw sphere\n");
   // qm1.exec();
    spathfind spth;

    infile.read((char*)&maps,sizeof(maps));

    if(modifiedinten>=205){
        // QMessageBox fmsg;
         QString xst,yst,zst;
         xst.setNum(xs);
                     yst.setNum(ys);
                     zst.setNum(zs);
                     xst.setNum(xe);
                     yst.setNum(ye);
            zst.setNum(ze);
    }

    short d=2;
    int haha=1;
   cout<<"modified Inten  : "<<modifiedinten<<"\n";
   short x=xe;short y=ye; short z=ze;
    int ecnt=0;
    int pint;

    while(1)
    {

        if(arr[x][y][z]>=fixedmodinten  && arr[x][y][z]==modifiedinten)
        {

            visited[arr[x][y][z]-fixedmodinten]=true;
            QMessageBox qm;
           QString rads;
           d = spherearr[z][y][x];
           sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
            pint = d;
            d=round(d);

            if(d<=0 || d>9)
            {
                d=2;
            }
            d=1;
          //intensitymap = drawsphere(arr,intenlist2,d,x,y,z);
               spherearr[z][y][x]=corevessel2;
        }

        else if(arr[x][y][z]>=fixedmodinten  && arr[x][y][z]!=modifiedinten && visited[arr[x][y][z]-fixedmodinten] == false)
        {
            QMessageBox qm;
            QString rads;

            cg.addedge(arr[x][y][z]-fixedmodinten,modifiedinten-fixedmodinten);
            visited[arr[x][y][z]-fixedmodinten]=true;
            d =spherearr[z][y][x];
            sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
            pint = d;
            d=round(d);

            if(d<=0 || d>9)
            {

                d=2;
            }
     d=1;
        // intensitymap = drawsphere(arr,intenlist2,d,x,y,z);
            spherearr[z][y][x]=corevessel2;
        }

        else if(arr[x][y][z]<fixedmodinten )
        {
            QMessageBox qm;
            QString rads;
        d=round(arr[x][y][z]);
        pint = arr[x][y][z];

        sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
        if(d<=0 || d>9)
        {
            d=2;
        }
 d=1;
        intensitymap.insert(std :: make_pair(calculateindex(x,y,z),arr[x][y][z]));
       //  intensitymap = drawspherex(arr,intenlist2,d,x,y,z);
    // intensitymap = drawsphere(arr,intenlist2,d,x,y,z);
           spherearr[z][y][x]=corevessel2;
       // intensitymap = drawspherex(arr,intenlist2,d/2,x,y,z);
        arr[x][y][z]=modifiedinten;

        }

        spth = maps[calculateindex(x,y,z)];

        intenlist.push_front(spth);

        outfile.write((char*)&spth,sizeof(spth));
        x=spth.returnxm(); y=spth.returnym(); z=spth.returnzm();

        if((x==xs && y==ys && z==zs) || (x==0 && y==0 && z==0))
        {
            spth = maps[calculateindex(x,y,z)];
            intensitymap.insert(std :: make_pair(calculateindex(x,y,z),arr[x][y][z]));
            intenlist.push_front(spth);
            break;
        }

    }

    pathmap.insert(std::make_pair(modifiedinten-fixedmodinten,intenlist));
  //  sfp.close();
    outfile.close();
    infile.close();
    if(modifiedinten>=205){
    }
    modifyimage(arr);

    modifiedinten++;
    sphereintensity++;
    readf.close();
  //  qm1.setText(" sphere drawn\n");
 //   qm1.exec();
    sfp.close();
    return intensitymap;

}

void printimagearray( short*** arr)
{
    long long int i,j,k,x,y,z;
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(((short) arr[i][j][k])>=intenl && (short)arr[i][j][k]<=intenu)
                cout<<i<<" "<<j<<" "<<k<<"||";
            }

            cout<<"\n";
        }

        cout<<"\n";
    }

    cout<<"\n";
}


void printimagearraylongintensity( short*** arr)
{
         long long int i,j,k,x,y,z;
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                cout<<(short)arr[i][j][k]<<" ";
            }

            cout<<"\n";
        }

        cout<<"\n";
    }

    cout<<"\n";
}


void readimage( short*** arr,char*** spherearr)
{
        QMessageBox rmsg;
    rmsg.setText("Entering ReadImage\n");
   // rmsg.exec();
    long long int zncnt=0;
    fstream infile;

    infile.open(imagefile, ios::in | ios :: binary);

    if(!infile){
        rmsg.setText("file Not Found\n");
        rmsg.exec();
    }
    cout<<"File read\n";
    long long int i,j,k,x,y,z;

    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(!infile.eof())
                {
                    infile.read((char*)(&arr[i][j][k]),sizeof(thetype));
                    spherearr[k][j][i]=(char)(arr[i][j][k]);
                  //  if(arr[i][j][k]>0)
                 //  cout<<arr[i][j][k]<<" ";
                    if(arr[i][j][k]<=0)
                    {
                        zncnt++;
                    }
                }

                else
                {
                    cout<<"EndOf File\n";
                    cout<<"Here\n";
                    break;
                }
            }
        }

    }

      //infile.close();
     rmsg.setText("Image File read:......\n");
    // rmsg.exec();
}

void dobinarize(short*** arr)
{
    fstream outfile;
    outfile.open("Binarizedimage.img", ios::out | ios :: binary);
    QMessageBox msg;
    long long int i,j,k,x,y,z;
    short binten;

    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(arr[i][j][k]>=spinten){

                   binten = 300;
                    outfile.write((char*)(&binten),sizeof(thetype));
                }
                else
                {
                    int mintensity=0;
                    outfile.write((char*)(&mintensity),sizeof(thetype));
                }
            }
        }
    }

    outfile.close();
}


/************************************************************Update Function**************************************************************************/



/*
 * updates the values of the neighbours of the current point i.e. the minimum distance point. neighbours of the point should
 * be in the intensity range specified.
 * this can be implemented more efficiently as a loop but I have not changed it due to lack of time sorry.
 *
*/
void update (bool* visited,cgraph cg,minheap mhp, long int * pos,short x, short y, short z, short*** arr, short*** dist, int nno,int types,map<long long int , short> intensitymap,list<int> intlist,short intype1=0, short intype2=0)
{
    QMessageBox qmsg;
    qmsg.setText("Enter Update");
  //  qmsg.exec();
    spathfind spthobj;
    ifstream fin;
    fstream fout;
    fin.open("ShortestPath", ios :: in);
    float weight;
    fin.read((char*)&maps,sizeof(maps));
    fin.close();
    for(int i=0;i<26;i++)
    {
        if(x+x_dir[i]>=0 && x+x_dir[i]<height && y+y_dir[i]>=0 && y+y_dir[i]<width && z+z_dir[i]>=0 && z+z_dir[i]<depth && arr[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]]>=intenl && arr[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]]<=intenu && mhp.isinminheap(pos,calculateindex(x+x_dir[i],y+y_dir[i],z+z_dir[i])) && dist[x][y][z]!=INT_MAX)
        {

            weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+x_dir[i],y+y_dir[i],z+z_dir[i]);
            if(dist[x][y][z]+weight<dist[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]])
            {
              dist[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]]=dist[x][y][z]+weight;
              mhp.update(dist,pos,x+x_dir[i],y+y_dir[i],z+z_dir[i],dist[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]]);
              spthobj=spathfind(arr[x+x_dir[i]][y+y_dir[i]][z+z_dir[i]],x,y,z,x+x_dir[i],y+y_dir[i],z+z_dir[i]);

              if(maps.count(calculateindex(x+x_dir[i],y+y_dir[i],z+z_dir[i]))==1)
              {
                  maps.erase(calculateindex(x+x_dir[i],y+y_dir[i],z+z_dir[i]));
              }
              maps.insert(std :: make_pair(calculateindex(x+x_dir[i],y+y_dir[i],z+z_dir[i]),spthobj));
            }
        }
    }
    ofstream out;
    out.open("ShortestPath",ios :: out);
    out.write((char*)&maps,sizeof(maps));
    out.close();
    qmsg.setText("leaving Update");
  //  qmsg.exec();
}

/************************************************************Shortest Path Functions*********************************************************************/



/* This Function
 * performs almost same actions as findspath3d, only difference is that if search starting from the source point and reaches
 * a segment that is connected to the desired segment then it will start from the destination point and will try to reach the
 * encountered segment . this process continues untill there is a intersection or destination point is found.
 * section starting from line 2057 - 2074  , that is commented out, implements the above functionality . it is commented out
 * because some portion of the segmentation found by applying this  algorithm is wrong so we have to delet those portions.
 * As inputs are given as manual points hence deleting the undesired segments are bit difficult. Hence I am only considering
 * the portion of the segment from the source point to the destination point if reachable or to the segment
 * that has encountered first during the search .
*/

storemin findspath3dsingle (cgraph cg,minheap mhp, long int* pos,short*** arr , short*** dist,short xs, short ys, short zs, short xf, short yf, short zf,map<long long int,short> intensitymap)
{
    spathfind spthobj=spathfind(arr[xs][ys][zs],xs,ys,zs,xs,ys,zs);
      maps.insert(std :: make_pair(calculateindex(xs,ys,zs),spthobj));
     spthobj=spathfind(arr[xf][yf][zf],xf,yf,zf,xf,yf,zf);
       maps.insert(std :: make_pair(calculateindex(xf,yf,zf),spthobj));
       bool cvisited[100];

    for(int i=0;i<100;i++)
    {
        cvisited[i]=false;
    }

    int intype2=-1;
    int change=0;

   // begining:
        int prevdist;
      cout<<"Starting Point :"<<xs<<" "<<ys<<" "<<zs<<" : "<<"Ending Point :"<<xf<<" "<<yf<<" "<<zf<<"\n";
      QString xst,yst,zst;
      QMessageBox fmsg;

    int type=5,intype1=arr[xf][yf][zf],confirmation=0;
    //cout<<"Starting Intensity : "<<intype1<<"\n";
    short xn,yn,zn;
    list<int> intlist;

    storemin fpoint;


    fout.open("ShortestPath",ios :: out );

    fout.write((char*)&maps,sizeof(maps));

    fout.close();

    cout<<"Starting : ";getcurrenttime();cout<<"\n";

    for( short i=0 ; i<height ; i++)
    {
        for(short j=0; j<width ; j++)
        {
            for(short k=0; k<depth ; k++)
            {
               pos[calculateindex(i,j,k)]=calculateindex(i,j,k);
            }
        }
    }


    for( short i=0 ; i<height ; i++)
    {
        for(short j=0; j<width ; j++)
        {
            for(short k=0; k<depth ; k++)
            {
                dist[i][j][k]=INT_MAX;
                mhp.update(dist,pos,i,j,k,INT_MAX);

            }
        }
    }


    dist[xs][ys][zs]=0;

    mhp.update(dist,pos,xs,ys,zs,0);
    bool flg=true;
    cout<<"Type : "<<type <<" Query\n";
    while(mhp.returnsize()>0)
    {
        again:
        minheapnode u = mhp.extractmin(dist,pos);

        short x = u.returnhx(); short y=u.returnhy(); short z=u.returnhz();

        if(modifiedinten==204)
        {
            cout<<"ERROR 204 : "<<x<<" "<<y<<" "<<z<<"\n";
        }

        if(x==xf && y==yf && z==zf)
        {

            fpoint.setcoordinate(x,y,z);
            cout<<"\nShortest path is found at the point and the distance is : "<<dist[x][y][z]<<"\n\n";
            confirmation=1;
            break;
        }

         else if( type ==5 && arr[x][y][z]==intype1)
        {

            cout<<"\nShortest path is found on the path and the distance is : "<<dist[x][y][z]<<"\n\n";
            confirmation=1;
            fpoint.setcoordinate(x,y,z);
            break;
        }
        else if( type==5 && arr[x][y][z]!=intype1 && arr[x][y][z]>=fixedmodinten )
        {
           // cout<<"Reached Path :"<<arr[x][y][z]<<"\n";
            cg.setfalse();
            if(cg.connectivity(arr[x][y][z]-fixedmodinten,intype1-fixedmodinten)==true)
            {

                 cout<<"\nShortest path is found at connect cond and the distance is : "<<dist[x][y][z]<<"\n\n";
                 fpoint.setcoordinate(x,y,z);
                 confirmation=1;
                 break;
            }

            else if(change==1 && cvisited[arr[x][y][z]-fixedmodinten]==false) //remove all the commented lines below to implement the multiple segment generation
            {
               //  cout<<"Looking for another path ..... \n";
               // int pnum = findclosestpath(cvisited,cg,xf,yf,zf,x,y,z,arr[xf][yf][zf]-fixedmodinten,arr[x][y][z]-fixedmodinten);
               // cout<<"Path to restart searching is : "<<pnum<<"\n";
               // distclass dcl = findclosestpoint(pnum,arr[xf][yf][zf]-fixedmodinten);

               // xn = x;
               // yn = y;
               // zn = z;

                fpoint.setcoordinate(x,y,z);
               // list<int> flist = cg.getconnectedcomp(arr[x][y][z]-fixedmodinten);
               // list<int>::iterator it;
               // for(it=flist.begin();it!=flist.end();it++)
              //  {

               //     cvisited[*it]=true;
              //  }
              //  cvisited[arr[x][y][z]-fixedmodinten]=true;
                break;
            }
            else if(change==0)
            {
              //  cout<<"Trying to find another path ..... \n";
               // int pnum = findclosestpath(cvisited,cg,xf,yf,zf,x,y,z,arr[xf][yf][zf]-fixedmodinten,arr[x][y][z]-fixedmodinten);
               // cout<<"Path to start search next is : "<<pnum<<"\n";
               // distclass dcl = findclosestpoint(pnum,arr[xf][yf][zf]-fixedmodinten);
             //   xn = x;
             //   yn = y;
             //   zn = z;
                fpoint.setcoordinate(x,y,z);

            //    list<int> flist = cg.getconnectedcomp(arr[x][y][z]-fixedmodinten);
            //    list<int>::iterator it;
           //    for(it=flist.begin();it!=flist.end();it++)
           //     {

           //         cvisited[*it]=true;
           //     }
           //     cvisited[arr[x][y][z]-fixedmodinten]=true;
                break;
            }

        }

        else if(arr[x][y][z]<intenl || arr[x][y][z]>intenu)
        {
            goto again;
        }
        else if(dist[x][y][z]>=INT_MAX)
        {
            cout<<"No Path Possible....\n";
            confirmation=1;
            fpoint.setcoordinate(-1,-1,-1);
           // goto gone;
            break;
        }

        if(x!=INT_MAX && y!=INT_MAX && z!=INT_MAX )
        {
                fpoint.setcoordinate(x,y,z);
                if(dist[x][y][z]>0)
                prevdist = dist[x][y][z];
                update(cvisited,cg,mhp,pos,x,y,z,arr,dist,0,type,intensitymap,intlist,intype1,intype2);

        }

    }

    if(mhp.returnsize()<=0)
    {

            cout<<"Shortest Path does not exists\n\n";
            confirmation=1;
            fpoint.setcoordinate(-1,-1,-1);
    }

    cout<<"Ending ";
    if(dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]>0)
    distmap.insert(std::make_pair(modifiedinten-fixedmodinten,dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]));
    if(dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]<0){
    distmap.insert(std::make_pair(modifiedinten-fixedmodinten,prevdist));
    dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()] = prevdist;
    }


    // if(dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]!=0)
  //  intensitymap = readandmodifyimage(cg,arr,xs,ys,zs,fpoint.returnx(),fpoint.returny(),fpoint.returnz(),intensitymap);
    getcurrenttime();
    cout<<"\n\n==========================================================================\n";
    //gone:
    return fpoint;
}


/*find shortest path for the queries where both starting and ending points are given by the user*/
storemin findspath3d (cgraph cg,minheap mhp, long int* pos,short*** arr , short*** dist,short xs, short ys, short zs, short xf, short yf, short zf,map<long long int,short> intensitymap,int singtype=-1)
{
    cout<<"Starting Point :"<<xs<<" "<<ys<<" "<<zs<<" : "<<"Ending Point :"<<xf<<" "<<yf<<" "<<zf<<"\n";
    QString xstr,ystr,zstr;
    xstr.setNum(xs);ystr.setNum(ys);zstr.setNum(zs);
    QMessageBox nmsg;
    nmsg.setText("Points "+xstr+" "+ystr+" "+zstr);
  //  nmsg.exec();
    xstr.setNum(xf);ystr.setNum(yf);zstr.setNum(zf);
    nmsg.setText("Points "+xstr+" "+ystr+" "+zstr);
  //  nmsg.exec();
    int type,intype1,intype2;
    short prevdist;
    list<int> intlist;
    bool cvisited[100];
    for(int i=0;i<100;i++)
    {
        cvisited[i]=false; //keeps track of all the segment that are in the path between start and end point
    }

    storemin fpoint;


    if(arr[xs][ys][zs]<fixedmodinten && arr[xf][yf][zf]<fixedmodinten)
    {
         type=1;  //when both end points have intensity less than sphereintensity(macro definition). intensity of segmented arteries start from 200

    }

    else if(arr[xs][ys][zs]>=fixedmodinten && arr[xf][yf][zf]>=fixedmodinten && arr[xf][yf][zf]==arr[xs][ys][zs])
    {
         type=0; //if start and endpoint lies into an already segmented artery
    }
    else if((arr[xs][ys][zs] >=fixedmodinten && arr[xf][yf][zf]<fixedmodinten) ) //when one of the points have intensity less than 200(200 is the starting intensity of the segmented arteries)
        {type=2;intype1=arr[xs][ys][zs];} //point with intensity >200 is the destination and other one is source(this case is mainly for joining points)

    else if((arr[xs][ys][zs]<fixedmodinten && arr[xf][yf][zf]>=fixedmodinten))   //when one of the points have intensity less than 200(200 is the starting intensity of the segmented arteries)
     {type=2;intype1=arr[xf][yf][zf];}  //point with intensity >200 is the destination and other one is source(this case is mainly for joining points)

    /*
     *when both points have intensity greater than 200 but not equal then if they are connected just return for the funtion
     *else try to find a path between them. type of query is 4 then.
     *
    */

    else if(arr[xs][ys][zs]>=fixedmodinten && arr[xf][yf][zf]>=fixedmodinten && arr[xf][yf][zf]!=arr[xs][ys][zs] )
    {
        cg.setfalse();

        if(cg.connectivity(arr[xs][ys][zs]-fixedmodinten,arr[xf][yf][zf]-fixedmodinten)==true){

        type=3;intype1=arr[xs][ys][zs];intype2=arr[xf][yf][zf];cg.setfalse();intlist = cg.getconnectedcomp(arr[xs][ys][zs]-fixedmodinten);
         cout<<intype1-fixedmodinten<<"<----->"<<intype2-fixedmodinten<<"\n";
        intlist.remove(arr[xf][yf][zf]-fixedmodinten);
        cout<<"|||  ";
        printlist(intlist,cvisited);
        cvisited[arr[xf][yf][zf]-fixedmodinten]=false;
        cout<<" |||\n";
        //return fpoint;
        }

        else {type=4;intype1=arr[xs][ys][zs];intype2=arr[xf][yf][zf];}
    }

    QString qmstr;
    QMessageBox qmsb ;

    fout.open("ShortestPath",ios :: out );

    fout.write((char*)&maps,sizeof(maps));

    fout.close();

    cout<<"Starting ";
    getcurrenttime();cout<<"\n";

    for( short i=0 ; i<height ; i++)
    {
        for(short j=0; j<width ; j++)
        {
            for(short k=0; k<depth ; k++)
            {
               pos[calculateindex(i,j,k)]=calculateindex(i,j,k);  //initialize min heap position array
            }
        }
    }


    for( short i=0 ; i<height ; i++)
    {
        for(short j=0; j<width ; j++)
        {
            for(short k=0; k<depth ; k++)
            {
                dist[i][j][k]=INT_MAX;
                mhp.update(dist,pos,i,j,k,INT_MAX);  //initialize min heap array
            }
        }
    }

    //cout<<dist[xs][ys][zs]<<"\n";
    dist[xs][ys][zs]=0;

    mhp.update(dist,pos,xs,ys,zs,0);
    bool flg=true;
    qmstr.setNum(type);
    qmsb.setText("Type Of query : "+qmstr);
   // qmsb.exec();
    while(mhp.returnsize()>0) //until all points are processed or destination is reached loop continues
    {
        again:
       minheapnode u = mhp.extractmin(dist,pos); //extract shortest distance point


        short x = u.returnhx(); short y=u.returnhy(); short z=u.returnhz();


        if(x==xf && y==yf && z==zf) //if current point is the end point return to the main function
        {
            //cout<<x<<" "<<y<<" "<<z<<"\n";

            fpoint.setcoordinate(x,y,z); //update fpoint, needs to draw the segment

            cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
            break;
        }

         else if( type ==2 && arr[x][y][z]==intype1) //if intensity of the current point is equal to the destination segment then end search
        {
            //cout<<x<<" "<<y<<" "<<z<<"\n";

            cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
            fpoint.setcoordinate(x,y,z); //update fpoint
            break;
        }
        else if( type==2 && arr[x][y][z]!=intype1 && arr[x][y][z]>=fixedmodinten) //if point is on a segment then
            //check if this segment is connected to the destination segment if so then return
        {
            if(singtype==5)
            {
                cout<<"\nShortest path is found at connect and the distance is : "<<dist[x][y][z]<<"\n\n";
                fpoint.setcoordinate(x,y,z);
                break;
            }
            cg.setfalse();
            if(cg.connectivity(arr[x][y][z]-fixedmodinten,intype1-fixedmodinten)==true)
            {

                 cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
                 fpoint.setcoordinate(x,y,z);
                 break;
            }

        }
        // current point is on a segment then check if the segment is connected to the destionation or not
         else if( type==4 && arr[x][y][z]!=intype2 && arr[x][y][z]>=fixedmodinten )
        {
            cg.setfalse();
            if(cg.connectivity(arr[x][y][z]-fixedmodinten,intype2-fixedmodinten)==true)
            {

                 cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
                fpoint.setcoordinate(x,y,z);
                break;
            }

        }


        // current point is on a segment then check if the segment is connected to the destionation or not
         else if( type==3 && arr[x][y][z]!=intype2 && arr[x][y][z]>=fixedmodinten)
         {
             if(!(std::find(intlist.begin(), intlist.end(), arr[x][y][z]-fixedmodinten) != intlist.end()))
             {
               cg.setfalse();
               if(cg.connectivity(arr[x][y][z]-fixedmodinten,intype2-fixedmodinten)==true)
               {

                 cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
                fpoint.setcoordinate(x,y,z);
                break;
               }
             }
         }
        // when current point is the destionation
        else if( ( type ==3 || type==4 )&& arr[x][y][z]==intype2)
        {
            //cout<<x<<" "<<y<<" "<<z<<"\n";

            cout<<"\nShortest path is found and the distance is : "<<dist[x][y][z]<<"\n\n";
            fpoint.setcoordinate(x,y,z);
            break;
        }

        //if intensity of the current point is outside the range then pickup another point
        if(arr[x][y][z]<intenl || arr[x][y][z]>intenu)
        {
            goto again;
        }
        //if minimum distance point is INT_MAX then no path possible

        else if(dist[x][y][z]>=INT_MAX)
        {
            qmsb.setText("No Path Possible....");
            qmsb.exec();
            fpoint.setcoordinate(-1,-1,-1);
            break;
        }
        qmsb.setText("iteration");
       // qmsb.exec();
        //else update the neighbours of the point and continue
        if(x!=INT_MAX && y!=INT_MAX && z!=INT_MAX )
        {
                fpoint.setcoordinate(x,y,z);
                if(dist[x][y][z]>0)
                prevdist = dist[x][y][z];
                update(cvisited,cg,mhp,pos,x,y,z,arr,dist,0,type,intensitymap,intlist,intype1,intype2);

        }

    }

    //if all the points on the minheap has been processed but destination is not found then no path exists
    if(mhp.returnsize()<=0)
    {

            cout<<"Shortest Path does not exists\n\n";
            fpoint.setcoordinate(-1,-1,-1);
    }

    cout<<"Ending ";
    if(dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]<0){
   // distmap.insert(std::make_pair(modifiedinten-fixedmodinten,prevdist));
        dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()] = prevdist;
     cout<<"\nShortest path is found and the actual distance is : "<<dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]<<"\n\n";

    }
    //store the distance of the segment
    distmap.insert(std::make_pair(modifiedinten-fixedmodinten,dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]));
    getcurrenttime();
    cout<<"\n\n==========================================================================\n";
    return fpoint;
}


/****************************************************************************MAIN FUNCTION***************************************************************/


/*class that keeps details of the input points*/
class point_given
{
    public:
    short x1,  y1,  z1 , x2 , y2 , z2;
    short rcnt;
    point_given(short rc,short x , short y, short z , short x3 = -1 , short y3 = -1, short z3=-1 )
    {
        cout<<"inside point_given object\n";
        x1=x;y1=y;z1=z;
        x2=x3;y2=y3;z2=z3;
        rcnt=rc;
    }
    point_given()
    {}

};

int checkpoints(short*** arr)
{
    point_given p;
    QMessageBox qm;
    fstream fout;
    fstream fin;
    fout.open("testpoint.txt",ios::out);
    int flag=1;
    fin.open("PointsFile.txt",ios::in);
    int i=-1;
    while(fin.read((char*)&p, sizeof(point_given)))
    {
        i=i+1;
        //qm.setText("Checking file");
       // qm.exec();
        cout<<"[ "<<p.rcnt<<" : "<<p.x1<<" "<<p.y1<<" "<<p.z1<<"| "<<p.x2<<" "<<p.y2<<" "<<p.z2<<" : "<<i<<" ]\n";
        if((p.x1>=0 && p.y1>=0 && p.z1>=0 && arr[p.x1][p.y1][p.z1]<=0) || (p.x2>=0 && p.y2>=0 && p.z2>=0 && arr[p.x2][p.y2][p.z2]<=0))
       { fout<<"[ "<<p.rcnt<<" : "<<p.x1<<" "<<p.y1<<" "<<p.z1<<"| "<<p.x2<<" "<<p.y2<<" "<<p.z2<<"]**??\n";
        flag=0;
        }
    }
   // qm.setText("Processed");
    //qm.exec();
    fin.close();
    fout.close();
    return flag;
}

latest_start_end lse;

latest_start_end process_input()
{
       ifstream fin;
       ofstream out,fout;
       point_given newpoint;
       char cstr[15];
       fout.open("testpoint.txt",ios::out);
       out.open("PointsFile.txt",ios::out);
       fin.open("Spine_seed.txt",ios::in);
       short x1[4],x2[4];//,z1,x2,y2,z2;
       string str;
       getline(fin,str);
       strcpy(cstr,str.c_str());
       char* pch=strtok(cstr," ");
       int i=0;
       string::size_type sz;
       while (pch != NULL)
       {
           int num=0;
           int in=0;
           string s(pch);
           //in=s.length()-1;
           while(in<s.length())
           {
               num=num*10;
               num=num+(pch[in]-'0');
               in++;
           }
           x1[i]=num;
           pch = strtok (NULL, " ");
           i++;
       }

       getline(fin,str);
       strcpy(cstr,str.c_str());
       pch=strtok(cstr," ");
        i=0;
       while (pch != NULL)
       {
           int num=0;
           int in=0;
           string s(pch);
           //in=s.length()-1;
           while(in<s.length())
           {
               num=num*10;
               num=num+(pch[in]-'0');
               in++;
           }
           x2[i]=num;
           pch = strtok (NULL, " ");
           i++;
       }
      //  fout<<x1[0]<<" "<<x1[1]<<" "<<x1[2]<<" "<<x1[3]<<" "<<x2[0]<<" "<<x2[1]<<" "<<x2[2]<<" "<<x2[3]<<" "<<"\n";
        cout<<x1[0]<<" "<<x1[1]<<" "<<x1[2]<<" "<<x1[3]<<" "<<x2[0]<<" "<<x2[1]<<" "<<x2[2]<<" "<<x2[3]<<" "<<"\n";

        newpoint=point_given(2,x1[2],x1[1],x1[0],x2[2],x2[1],x2[0]);
       lse.change_points(x1[2],x1[1],x1[0],x2[2],x2[1],x2[0]);
       out.write((char*)&newpoint, sizeof(newpoint));
        short prev_count=-1;
        QMessageBox qmb;
     //   qmb.setText("start-end-point-processed");
     //   qmb.exec();
       while(getline(fin,str))
       {
           strcpy(cstr,str.c_str());
           if(cstr[0]=='$')
           {
               break;
           }
//           qmb.setText(cstr);
//           qmb.exec();
           pch=strtok(cstr," ");
           i=0;
           //string::size_type sz;
           while (pch != NULL)
           {
               int num=0;
               int in=0;
               string s(pch);
               //in=s.length()-1;
               while(in<s.length())
               {
                   num=num*10;
                   num=num+(pch[in]-'0');
                   //num=num*10;
                   in++;
               }
               x1[i]=num;

               pch = strtok (NULL, " ");
               i++;
           }
//           QMessageBox ms;
//           QString x1s;
//           x1s.setNum(x1[3]);
//           ms.setText("1st: "+x1s);
//           ms.exec();
           if(x1[3]==2)
           {
               getline(fin,str);
               strcpy(cstr,str.c_str());
               pch=strtok(cstr," ");
                i=0;
               while (pch != NULL)
               {
                   int num=0;
                   int in=0;
                   string s(pch);
                   //in=s.length()-1;
                   while(in<s.length())
                   {
                       num=num*10;
                       num=num+(pch[in]-'0');
                       in++;
                   }
                   x2[i]=num;
                   pch = strtok (NULL, " ");
                   i++;
               }
                if(x1[3]==2 && x2[3]!=2)
               {
                   cout<<"Enter Ending point after starting point\n";
                   exit(0);
               }

                newpoint=point_given(2,x1[2],x1[1],x1[0],x2[2],x2[1],x2[0]);
                cout<<"[ "<<newpoint.x1<<" "<<newpoint.y1<<" "<<newpoint.z1<<":"<<newpoint.x2<<" "<<newpoint.y2<<" "<<newpoint.z2<<"]\n";
                out.write((char*)&newpoint, sizeof(point_given));
              // lse.change_points(x1[2],x1[1],x1[0],x2[2],x2[1],x2[0]);
           }

           else if(x1[3]==1 || x1[3]==5)
           {
               fout<<x1[0]<<" "<<x1[1]<<" "<<x1[2]<<" "<<x1[3]<<" "<<x2[0]<<" "<<x2[1]<<" "<<x2[2]<<" "<<x2[3]<<" "<<"\n";
               if(prev_count==5 && x1[3]!=5)
               {
                    newpoint=point_given(6,x1[2],x1[1],x1[0],-1,-1,-1);
                   out.write((char*)&newpoint, sizeof(point_given));
               }
//               QMessageBox ms;
//               QString x1s;
//               x1s.setNum(x1[3]);
//               ms.setText("2nd "+x1s);
//               ms.exec();
                newpoint=point_given(x1[3],x1[2],x1[1],x1[0],-1,-1,-1);
                cout<<"[ "<<newpoint.x1<<" "<<newpoint.y1<<" "<<newpoint.z1<<":"<<newpoint.x2<<" "<<newpoint.y2<<" "<<newpoint.z2<<"]\n";
                out.write((char*)&newpoint,sizeof(point_given));
           }
           prev_count = x1[3];
         //  cout<<x1[0]<<" "<<x1[1]<<" "<<x1[2]<<" "<<x1[3]<<" "<<x2[0]<<" "<<x2[1]<<" "<<x2[2]<<" "<<x2[3]<<" "<<"\n";
       }

        fout.close();
       fin.close();
       out.close();
      // readfile();
       return lse;
}


minheap mhp;
void findspathforreal(char*** spherearr, int xdim, int ydim, int zdim) //Main function that reads the points from the file and finds shortest path
{
    QMessageBox nmsg;
    static int sinti=0;
    nmsg.setText("find shortest path\n");
   // nmsg.exec();
    /**
     * process_input() reads points from Spine_seed.txt(file where points given by the user is stored)
     * and converts it into object and writes those objects into PointsFile.txt
     */
    height = zdim;
    width = ydim;
    depth=xdim;
    imagedimension=height*width*depth;
    latest_start_end lstartend =  process_input();  //lstartend keeps track of first segment whose both end points are given

    cgraph cg(1000);
    short xs,ys,zs,xe,ye,ze;
    short *** arr,***dist;

    long int* pos;
    nmsg.setText("minheap before\n");
   // nmsg.exec();
     minheap mhp;
     nmsg.setText("minheap processed\n");
   //  nmsg.exec();

    arr=new short**[height];
    dist=new short**[height];

  //  delete pos;
    pos=new  long int[imagedimension]; //needed for shortestpath mean heap implementation

    nmsg.setText("arr dist pos allocated\n");
   // nmsg.exec();
    for(short i=0;i<height;i++)   //dynamic allocation of arrays
    {

       arr[i]=new short*[width];
        dist[i]=new short*[width];
        for(short j=0;j<width;j++)
        {

            arr[i][j]=new short[depth];
            dist[i][j]=new short[depth];
        }
    }

   // nmsg.setText("Memories allocated\n");
  //  nmsg.exec();

    storemin fpoints;
   // nmsg.setText("read imaget\n");
   //  nmsg.exec();
    readimage(arr,spherearr);  //reads the image file

   /*checks if any of the points is outside the preferred intensity range.If so see testpoint.txt file in the program
     folder to see those points.*/
   // nmsg.setText("points read\n");
   // nmsg.exec();
     if(checkpoints(arr)==0)
     {
         nmsg.setText("Zero intensity point\n");
          nmsg.exec();
         return;
     }
     nmsg.setText("points checked\n");
   //  nmsg.exec();
   //  return; //comment out this line only to inspect the validity of points

    point_given p;

    ifstream fin;

    fin.open("PointsFile.txt",ios::in);
    if(!fin)
    {
           nmsg.setText("File Not found\n");
           nmsg.exec();
    }
    ofstream outf;
    outf.open("Query.txt",ios::out);
    QMessageBox msgb;
    while(fin.read((char*)&p, sizeof(point_given)))
    {
        cout<<"Reading Point......\n";
        xs=p.x1;ys=p.y1;zs=p.z1;
        xe=p.x2;ye=p.y2;ze=p.z2;
        outf<<p.rcnt<<" "<<xs<<" "<<ys<<" "<<zs<<" "<<xe<<" "<<ye<<" "<<ze<<"\n";
        QString xst,yst,zst,sin,ein;
        QMessageBox fmsg;

        if(arr[xs][ys][zs]<=0) //if intensity of start-point is lessa than equal to zero throw an error
        {
            short snum = arr[xs][ys][zs];
            QString qstr,xstr,ystr,zstr;
            xstr.setNum(xs);
            ystr.setNum(ys);
            zstr.setNum(zs);
            qstr.setNum(snum);

            msgb.setText("first point "+xstr+" "+ystr+" "+zstr+" "+qstr);
            msgb.exec();
            break;
           // return;
        }
        if( p.rcnt==2 && (xe==-1 || ye==-1 || ze==-1 || arr[xe][ye][ze]<=0)) //if intensity of  end-point is less than equal to zero throw an error
        {
            short snum = arr[xe][ye][ze];
            QString qstr,xstr,ystr,zstr;
            xstr.setNum(xe);
            ystr.setNum(ye);
            zstr.setNum(ze);
            qstr.setNum(snum);

            msgb.setText("second point "+xstr+" "+ystr+" "+zstr+" "+qstr);
             msgb.exec();
             break;
        }
        else if(p.rcnt==2) //query with both end points given
        {
              int tx,ty,tz;

           if(arr[xs][ys][zs]>=fixedmodinten || arr[xs][ys][zs]<=0)
            {
                tx=xs;xs=xe;xe=tx;
                ty=ys;ys=ye;ye=ty;
                tz=zs;zs=ze;ze=tz;
            }
        lstartend.change_points(xs,ys,zs,xe,ye,ze);
        QString xst,yst,zst;
        QMessageBox fmsg;

        fpoints=findspath3d(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap); //finds shortest path between two points
        fmsg.setText("writing shortest path points\n");
        fmsg.text();
        if(fpoints.returnx()!=-1 && fpoints.returny()!=-1 && fpoints.returnz()!=-1)
        intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap,spherearr); //draws the sphere on the points found by the shortestpath algorithm

         lstartend.change_intensity(modifiedinten-1); //updating lstartedend to the current point with two endpoints given
        }

        else if(p.rcnt==1)  //handling joining points
        {
             QMessageBox fmsg;
             if(arr[xs][ys][zs]>=fixedmodinten) //if joining points is already on a segment then ignore
                 continue;
             distclass endp = findsegpath(xs,ys,zs);  //returns a point on the longest path which is also nearest to the point.
             QString xst,yst,zst;
             xe=endp.x; ye=endp.y; ze=endp.z;
             xst.setNum(xe);
             yst.setNum(ye);
             zst.setNum(ze);
             fpoints=findspath3d(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap,5); //finds shortest path between two points
             if(fpoints.returnx()!=-1 && fpoints.returny()!=-1 && fpoints.returnz()!=-1)
             intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap,spherearr);

        }
        else if(p.rcnt==3) //deletes a semgment.take any point on the segment to be deleted
        {
           cout<<"Delete : "<<p.rcnt<<"\n";
           QMessageBox fmsg;
           QString fstr ;


           int inten;
           inten = arr[xs][ys][zs]; //stores the intensity of the point

           fstr.setNum(inten-fixedmodinten);

           intensitymap = delete_edge(cg,arr,inten-fixedmodinten); //delete_edge function deletes the segment

           modifyimage(arr); //writes the modified image in the file

        }


        else if(p.rcnt == 5)  //handles intermediate points
        {

            /*say (a,b) is the segment to be modifed.and intermediate points are (c,d,e) then paths will be found in the folllowing order
              (a,c),(c,d),(d,e),(e,b)*/
            QMessageBox fmsg;
            QString fstr ;

            intensitymap = delete_edge(cg,arr,lstartend.intenssity - fixedmodinten); //first deletes the segment to be modified.

            xe = lstartend.xs; ye = lstartend.ys; ze = lstartend.zs;
            outf<<p.rcnt<<" "<<xs<<" "<<ys<<" "<<zs<<" "<<xe<<" "<<ye<<" "<<ze<<"\n";
            QString xst,yst,zst;

            fpoints=findspath3d(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap); //finds spath between starting and first intermediate point

            intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap,spherearr); //draws sphere and writes down modified image
            xe = xs ; ye = ys; ze = zs;

            while(fin.read((char*)&p, sizeof(point_given))) //untill all the intermediate points been processed pick up pair of points and finds path between them
            {
                xs=p.x1;ys=p.y1;zs=p.z1;
                outf<<p.rcnt<<" "<<xs<<" "<<ys<<" "<<zs<<" "<<xe<<" "<<ye<<" "<<ze<<"\n";
                if(p.rcnt != 5)
                {
                    break;
                }

                 fpoints=findspath3d(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap);
                 intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap,spherearr);
                 xe = xs ; ye = ys; ze = zs;
            }
             xs  = lstartend.xe; ys  = lstartend.ye ;  zs = lstartend.ze;
           int tx,ty,tz;
            if(arr[xs][ys][zs]>=fixedmodinten || arr[xs][ys][zs]==-1)
             {
                 tx=xs;xs=xe;xe=tx;
                 ty=ys;ys=ye;ye=ty;
                 tz=zs;zs=ze;ze=tz;
             }
            fpoints=findspath3d(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap);
             if(fpoints.returnx()!=-1 && fpoints.returny()!=-1 && fpoints.returnz()!=-1)
            intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap,spherearr);

        }

    }

    int choice;

    update_spherearr(arr,spherearr);
    write_vessel_file(spherearr);
    //showsphere(arr);

//    free(arr);arr=NULL;
//    free(dist);dist=NULL;
//    free(pos); pos=NULL;
    deallocate_spath(arr,dist,pos);
    mhp.deallocate();
    msgb.setText("Shortest Paths Found");
//    msgb.exec();
    outf.close();
    fin.close();

}

void deallocate_spath(short*** arr, short*** dist,long int* pos)//RESET-free all
{

        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                free(arr[i][j]);
                free(dist[i][j]);
            }
            free(arr[i]);
            free (dist[i]);
        }


    free(arr);
    free(dist);
    free(pos);
    arr=NULL;dist=NULL;pos=NULL;
}

int first=0;
int firstc=0;
int firstm=0;
unsigned short readShort( FILE* inff );
void readimage( short*** arr)
{
    std::cout<<"Entering ReadImage\n";
    long long int zncnt=0;
    fstream infile;
    QMessageBox rmsg;
    infile.open(imagefile3, ios::in | ios :: binary);

    if(!infile){
        rmsg.setText("file Not gui Found\n");
        rmsg.exec();
    }
    std::cout<<"File read\n";
    long long int i,j,k,x,y,z;

    for( i=0;i<depths;i++)
    {
        for( j=0;j<widths;j++)
        {
            for(k=0;k<heights;k++)
            {
                if(!infile.eof())
                {
                    infile.read((char*)(&arr[k][j][i]),sizeof(thetype));

                  //  if(arr[i][j][k]>0)
                 //  cout<<arr[i][j][k]<<" ";
                    if(arr[k][j][i]<=0)
                    {
                        zncnt++;
                    }
                }

                else
                {
                    cout<<"EndOf File\n";
                    cout<<"Here\n";
                    break;
                }
            }
        }

    }

      //infile.close();
      cout<<"Image File read:......\n";
}
void readimage_specific( short*** arr,string name)
{
    std::cout<<"Entering ReadImage\n";
    long long int zncnt=0;
    fstream infile;
    QMessageBox rmsg;
    infile.open(name.c_str(), ios::in | ios :: binary);

    if(!infile){
        rmsg.setText("file Not gui Found\n");
        rmsg.exec();
    }
    std::cout<<"File read\n";
    long long int i,j,k,x,y,z;

    for( i=0;i<depths;i++)
    {
        for( j=0;j<widths;j++)
        {
            for(k=0;k<heights;k++)
            {
                if(!infile.eof())
                {
                    infile.read((char*)(&arr[k][j][i]),sizeof(thetype));

                  //  if(arr[i][j][k]>0)
                 //  cout<<arr[i][j][k]<<" ";
                    if(arr[k][j][i]<=0)
                    {
                        zncnt++;
                    }
                }

                else
                {
                    cout<<"EndOf File\n";
                    cout<<"Here\n";
                    break;
                }
            }
        }

    }

      //infile.close();
      cout<<"Image File read:......\n";
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //filename="/local/vol00/guha/Work/CTA-Data01/newAnalyze1015.hdr";//Srinked slices
    //filename="/local/vol00/guha/Work/CTA-Data01/newAnalyze1016.hdr";//Good Image
    //filename="/local/vol00/guha/Work/CTA-Data01/newAnalyze1005.hdr";//Srinked slices-> Initially error in loading
    //filename="/local/vol00/guha/Work/CTA-Data01/newAnalyze3019.hdr";//Similar to 1016
    //  filename="/local/vol00/guha/Work/CTA-Data01/newAnalyze3029.hdr";//Good image..prominent Anearysm
    Xdim=512;
    Ydim=512;
    Zdim=287;//402;//406;//250;//287;;

    ui->setupUi(this);


}

void MainWindow::on_pushButton_2_clicked() //Load Image button
{
    QString temp;

    temp=QFileDialog::getOpenFileName(this,tr("Open File"), "/local/vol00/basu/Work/CTA-Data01",
                                      tr("Analyze Images (*.hdr)"));

    QByteArray qb=temp.toLatin1();
    filename =  qb.data();

    ui->label_19->setText(filename);
    //on_pushButton_14_clicked();


    histogram();//Main function to load image and populate histogram data
    on_pushButton_clicked();//Draw Histogram
    if(first!=0 && firstm!=0 && firstc!=0){on_pushButton_14_clicked(); de_allocate_memory();}
    else if(first!=0 && firstc!=0 && firstm==0){ de_allocate_memory();}
    else if(first!=0 && firstc==0 && firstm!=0){ on_pushButton_14_clicked();}
    if(ui->checkBox_13->isChecked()==false){
    allocate_memory();
    allocate_memorycore();
    firstm++;
    }
    else if(ui->checkBox_13->isChecked()==true)
    {
        allocate_memorycore();
        firstc++;
    }
    readimage(vol);

    ui->horizontalSlider_x->setMaximum(Xdim);
    ui->horizontalSlider_y->setMaximum(Ydim);
    ui->horizontalSlider_z->setMaximum(Zdim);

    curX=Xdim/2;
    curY=Ydim/2;
    curZ=Zdim/2;

  //  on_pushButton_14_clicked();//RESET. Free existing memory allocation, if any
  //  allocate_memory();

    draw_image();

    //on_pushButton_3_clicked();

    //load_seeds();

    //GITN=0;
    first++;
    std::cout<<"Memory allocation successful"<<std::endl;

}

void MainWindow::on_pushButton_14_clicked()//RESET-free all
{
    GITN=0;


    for (int i = 0 ;  i < Xdim; i++) {

     // vol[i] = ( short **)calloc(sizeof( short *) , Ydim);

      for (int j = 0; j < Ydim; j++){
        free(FDT[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(FDTb[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(FDTv[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(tFDT[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(LM[i][j]);// = (unsigned long *)calloc(sizeof(unsigned long) , Zdim);
        free(FCv[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(FCb[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(MRv[i][j]);// = (short *)calloc(sizeof(short) , Zdim);
        free(MRb[i][j]);// = (short *)calloc(sizeof(short) , Zdim);
        free(out[i][j]);// = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
        free(SS[i][j]);// = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
        free(GT[i][j]);// = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
        free(NML[i][j]);// = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
     //   vol[i][j] = ( short *)calloc(sizeof( short ) , Zdim);

      }
      free(FDT[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(FDTb[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(FDTv[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(tFDT[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(LM[i]);// = (unsigned long **)calloc(sizeof(unsigned long *) , Ydim);
      free(FCv[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(FCb[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(MRv[i]);// = (short **)calloc(sizeof(short *) , Ydim);
      free(MRb[i]);// = (short **)calloc(sizeof(short *) , Ydim);
      free(out[i]);// = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
      free(SS[i]);// = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
      free(GT[i]);// = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
      free(NML[i]);// = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
    }
        free(FDT);FDT=NULL;
        free(FDTv);FDTv=NULL;
        free(FDTb);FDTb=NULL;
        free(tFDT);tFDT=NULL;
        free(LM);LM=NULL;
        free(FCv);FCv=NULL;
        free(FCb);FCb=NULL;
        free(MRv);MRv=NULL;
        free(MRb);MRb=NULL;
        free(out);out=NULL;
        free(SS);SS=NULL;
        free(GT);GT=NULL;
        free(NML);NML=NULL;
     //   free(vol); vol=NULL;
}



void MainWindow :: allocate_memorycore(){
    vol = ( short***) calloc(sizeof(short**), Xdim);
    spherearr = (char***)calloc(sizeof(char**),Xdim);
    SS   = (unsigned char ***)calloc(sizeof(unsigned char **) , Xdim);
    GT   = (unsigned char ***)calloc(sizeof(unsigned char **) , Xdim);
    for(int i=0;i<Xdim;i++){
        vol[i]=(short**) calloc (sizeof(short*),Ydim);
        spherearr[i] = (char**)calloc(sizeof(char*),Ydim);
        GT[i] = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
        SS[i] = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
        for(int j=0; j<Ydim ; j++)
        {
            vol[i][j]=(short*) calloc(sizeof(short),Zdim);
            spherearr[i][j] = (char*)calloc(sizeof(char),Zdim);
            GT[i][j] = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
            SS[i][j] = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
        }
    }

    load_coreVessel_file();
}

void MainWindow::allocate_memory()
{
    cout<<"Enter Allocate_Memory\n";
    FDT  = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    FDTb = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    FDTv = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    tFDT = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    LM   = (unsigned long ***)calloc(sizeof(unsigned long **) , Xdim);
    FCv  = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    FCb  = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    MRv  = (short ***)calloc(sizeof(short **) , Xdim);
    MRb  = (short ***)calloc(sizeof(short **) , Xdim);
    out  = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    NML   = (unsigned char ***)calloc(sizeof(unsigned char **) , Xdim);
  //  vol = ( short ***)calloc(sizeof( short **) , Xdim);


  for (int i = 0 ;  i < Xdim; i++) {
    FDT[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    FDTb[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    FDTv[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    tFDT[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    LM[i] = (unsigned long **)calloc(sizeof(unsigned long *) , Ydim);
    FCv[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    FCb[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    MRv[i] = (short **)calloc(sizeof(short *) , Ydim);
    MRb[i] = (short **)calloc(sizeof(short *) , Ydim);
    out[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    NML[i] = (unsigned char **)calloc(sizeof(unsigned char *) , Ydim);
   // vol[i] = ( short **)calloc(sizeof( short *) , Ydim);

    for (int j = 0; j < Ydim; j++){
      FDT[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      FDTb[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      FDTv[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      tFDT[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      LM[i][j] = (unsigned long *)calloc(sizeof(unsigned long) , Zdim);
      FCv[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      FCb[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      MRv[i][j] = (short *)calloc(sizeof(short) , Zdim);
      MRb[i][j] = (short *)calloc(sizeof(short) , Zdim);
      out[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
      NML[i][j] = (unsigned char *)calloc(sizeof(unsigned char) , Zdim);
   //   vol[i][j] = ( short *)calloc(sizeof( short ) , Zdim);

    }
  }
  cout<<"Memory Allocation Successful\n";


}

void MainWindow :: de_allocate_memory()
{
    for(int i=0;i<Xdim;i++){

        for(int j=0; j<Ydim ; j++)
        {
            free(vol[i][j]);//=(short*) calloc(sizeof(short),Zdim);
            free(spherearr[i][j]);// = (char*)calloc(sizeof(char),Zdim);
        }
        free(vol[i]);//=(short**) calloc (sizeof(short*),Ydim);
        free(spherearr[i]);// = (char**)calloc(sizeof(char*),Ydim);
    }
    free(vol);vol=NULL;
    free(spherearr);spherearr=NULL;
}



void MainWindow::histogram()
{
    QMessageBox nmsg;
    //nmsg.setText("histogram");
    //nmsg.exec();
    offset=0;//for bioimaging data
    QMessageBox qmb;
    //ConstIteratorType inputIt(   image, image->GetRequestedRegion());
    // IteratorType      outputIt(  outputImage,         outputRegion );

    Xdim=0;//size[0];
    Ydim=0;//size[1];
    Zdim=0;//size[2];
    //QMessageBox qmb;
    FILE *fo;
    char buffer[100];
    hdr=malloc(480);

    fo = fopen(filename, "rb");

    unsigned short uns[3];

    if(fo)
    {

        fread(hdr,348,1,fo);

        fseek(fo,42,SEEK_SET);
        Xdim=readShort(fo);
        Ydim=readShort(fo);
        Zdim=readShort(fo);

    }
    fclose(fo);


//    vol = (short ***)calloc(sizeof(short **) , Xdim);

//    for (int i = 0 ;  i < Xdim; i++) {
//        vol[i] = (short **)calloc(sizeof(short *) , Ydim);

//        for (int j = 0; j < Ydim; j++)
//            vol[i][j] = (short *)calloc(sizeof(short) , Zdim);
//    }

    sprintf(buffer,"Xdim= %d Ydim=%d Zdim=%d",Xdim,Ydim,Zdim);
    QMessageBox::information(this, "Header Info:", buffer, QMessageBox::Ok);

//    filename.replace("hdr","img");
//    fo = fopen(filename, "rb");
//    if(fo)
//    {
//        for(int k=0;k<Zdim;k++)
//        {
//            for(int j=0;j<Ydim;j++)
//                for(int i=0;i<Xdim;i++)
//                    //vol[i][j][k] = (unsigned short)fgetc(fo);//readShort(fo);
//                    vol[i][j][k] = readShort(fo);
//            //std::cout<<"readShort() called: "<<k<<" Intensity: "<<vol[128][128][k]<<std::endl;
//        }
//    }
    unsigned short ret;
    unsigned char lowByte, hiByte;
    long count=0,max=-9999;//,hist[4000],min=9999,hmax=0;
    short val,x=0,y=0,z=0;


}

unsigned short readShort( FILE* inff )     // read a 16 bit integer
{

    QMessageBox nmsg;
    unsigned short ret;
    unsigned char lowByte, hiByte;


    lowByte = fgetc(inff);         // Read the low order byte (little endian form)
    hiByte = fgetc(inff);          // Read the high order byte
    ret = hiByte;
    ret <<= 8;
    ret |= lowByte;

    return ret;

    //std::cout<<"readShort() ended"<<std::endl;
}


void MainWindow::on_pushButton_clicked()//Draw Histogram image
{

    if (ui->spinBox_tap->value()<=0 ||
        ui->spinBox_ta->value() <=0 ||
        ui->spinBox_tb->value() <=0 ||
        ui->spinBox_tbp->value()<=0) return;


  QImage imageH(3000,200,QImage::Format_RGB32);//"/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/Hist.png");

//  imageH= imageH.convertToFormat(,Qt::ColorOnly);
  QRgb value = qRgb(255, 255, 255);
  for(int i=0;i<3000;i++)
      for(int j=1;j<200;j++)
          imageH.setPixel(QPoint(i,j), value );


  value = qRgb(0, 0, 0);
  float temp;

  for(int i=0;i<3000;i++)
  {
     // if (hist[i+2]>=

  //temp=(float)(hist[i+2])*200/(hmax/ui->verticalSlider->value());//skipping initial black regions in array
      temp=(float)(log(hist[i]))*200/(log(hmax));//skipping initial black regions in array

  if(temp>199) temp=200;//FORCED
  //int t2=floor(t);
    //   std::cout<<"temp= "<<t2<<" ";//std::endl;
  for(int j=1;j<floor(temp)&&i<2999;j++)
  {
    imageH.setPixel(QPoint(i+1,200-j), value );
  }
  }

   value = qRgb(0, 0, 255);
  for(int i=1;i<200;i++)
  {
    imageH.setPixel(QPoint(ui->spinBox_tap->value(),i), value );
    imageH.setPixel(QPoint(ui->spinBox_tap->value()-1,i), value );
    imageH.setPixel(QPoint(ui->spinBox_tap->value()+1,i), value );
  }
   value = qRgb(255, 0, 0);
  for(int i=1;i<200;i++)
  {
    imageH.setPixel(QPoint(ui->spinBox_ta->value(),i), value );
    imageH.setPixel(QPoint(ui->spinBox_ta->value()-1,i), value );
    imageH.setPixel(QPoint(ui->spinBox_ta->value()+1,i), value );
  }
   value = qRgb(0, 255, 0);
  for(int i=1;i<200;i++)
  {
    imageH.setPixel(QPoint(ui->spinBox_tb->value(),i), value );
    imageH.setPixel(QPoint(ui->spinBox_tb->value()-1,i), value );
    imageH.setPixel(QPoint(ui->spinBox_tb->value()+1,i), value );
  }
   value = qRgb(0, 0, 255);
  for(int i=1;i<200;i++)
  {
    imageH.setPixel(QPoint(ui->spinBox_tbp->value(),i), value );
    imageH.setPixel(QPoint(ui->spinBox_tbp->value()-1,i), value );
    imageH.setPixel(QPoint(ui->spinBox_tbp->value()+1,i), value );
  }

  ui->label_h->setPixmap(QPixmap::fromImage(imageH));

  //draw_image();
}



void MainWindow::load_seeds()
{
    int c,r,s;


      for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              SS[i][j][k]=0;


              FILE *fp;
            /*  fp=fopen("vseed","r");
              if(fp != NULL){
              while(!feof(fp))
              {
                  fscanf(fp,"%d%d%d",&c,&r,&s);
                  SS[c][r][s]=1;
                  SS[c-1][r][s]=1;
                  SS[c+1][r][s]=1;
                  SS[c][r-1][s]=1;
                  SS[c][r+1][s]=1;
                  SS[c][r][s+1]=1;
                  SS[c][r][s-1]=1;
              }
              fclose(fp);
                }*/


              fp=fopen("bseed","r");
              if(fp != NULL){
              while(!feof(fp))
              {
                  fscanf(fp,"%d%d%d",&c,&r,&s);
                  SS[c][r][s]=2;
                  SS[c-1][r][s]=2;
                  SS[c+1][r][s]=2;
                  SS[c][r-1][s]=2;
                  SS[c][r+1][s]=2;
                  SS[c][r][s+1]=2;
                  SS[c][r][s-1]=2;

              }
              fclose(fp);
                 }

              fp=fopen("sep","r");
              if(fp != NULL){
              while(!feof(fp))
              {
                  fscanf(fp,"%d%d%d",&c,&r,&s);
                  SS[c][r][s]=3;
                  SS[c-1][r][s]=3;
                  SS[c+1][r][s]=3;
                  SS[c][r-1][s]=3;
                  SS[c][r+1][s]=3;
                  SS[c][r][s+1]=3;
                  SS[c][r][s-1]=3;

              }
              fclose(fp);
                }
}


void MainWindow::load_GT()
{
    int c,r,s,GTvCnt=0,GTbCnt=0;
    char name[200];
    cout<<"name of files : "<<name<<"\n";
      for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              GT[i][j][k]=0;
              FILE *fp;
              if(ui->radioButton_L->isChecked() == TRUE)
              {
                  //sprintf(name,"/media/LACIE/Subhadip/Results/%s/GT/large/GTv",DataID);
                  sprintf(name,"/home/subhadip/Test3/%s/GT/large/GTv",DataID);
                  fp=fopen("GTv","r");
              }
              else
              if(ui->radioButton_F->isChecked() == TRUE)
              {
                  //sprintf(name,"/media/LACIE/Subhadip/Results/%s/GT/fine/GTv",DataID);
                  sprintf(name,"/home/subhadip/Test3/%s/GT/fine/GTv",DataID);
                  fp=fopen("GTv","r");
              }

              if(fp != NULL){
              while(!feof(fp))
              {
                  fscanf(fp,"%d%d%d",&c,&r,&s);
                  GT[c][r][s]=1;
                  GTvCnt++;

              }
              fclose(fp);
                }

               ui->GTvCnt->setNum(GTvCnt-1);

              if(ui->radioButton_L->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/large/GTb",DataID);
                  fp=fopen("GTb","r");
              }
              else
              if(ui->radioButton_F->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/fine/GTb",DataID);
                  fp=fopen("GTb","r");
              }

              if(fp != NULL){
              while(!feof(fp))
              {
                  fscanf(fp,"%d%d%d",&c,&r,&s);
                  GT[c][r][s]=2;
                  GTbCnt++;

              }
              fclose(fp);
                 }
              ui->GTbCnt->setNum(GTbCnt-1);


}


MainWindow::~MainWindow()
{
    de_allocate_memory();

    delete ui;
}


void MainWindow :: wheelEvent(QWheelEvent* e)
{
    int t,v,vx,vy;

    if(e->orientation() == Qt::Vertical)
    {
        t= (int)(e->delta()) / 120;
        v=ui->spinBox_z->value();
        vx = ui->spinBox_x->value();
        vy = ui->spinBox_y->value();
        if (v>=0 && v<=Zdim-1)
        {
            if(v==0 && t<0) return;
            if(v==Zdim-1 && t>0) return;
            if(v+t!=Zdim)
            ui->spinBox_z->setValue(v+t);
        }
        if(vx>=0 && v<=Xdim-1)
        {
            if(vx==0 && t<0) return;
            if(vx>=Xdim-1 && t>0) return;
            if(vx+t != Xdim)
            ui->spinBox_x->setValue(vx+t);
        }

        if(vy>=0 && vy<=Ydim-1)
        {
            if(vy==0 && t<0) return;
            if(vy==Ydim-1 && t>0) return;
            if(vy+t!=Ydim)
            ui->spinBox_y->setValue(vy+t);
        }
        //event->Quit;
    }

}

void MainWindow::mouseMoveEvent( QMouseEvent *e )
{
    QPoint pt,pt1;
    int tempX,tempY;

    pt=e->pos();
   if((pt.x()>ui->sliceLabel->x()) &&( pt.y()>ui->sliceLabel->y()) &&(pt.x()<ui->sliceLabel->x()+ui->sliceLabel->width()) && (pt.y()<ui->sliceLabel->y()+ui->sliceLabel->height()))
    {

    tempX=pt.x()-ui->sliceLabel->x()-1;
    tempY=pt.y()-ui->sliceLabel->y()-10;

    unsigned long count=LM[tempX][tempY][curZ];
    int x=count%Xdim;
    int z=count/(Xdim*Ydim);
    int y=(count/Xdim)-(z*Ydim);

    ui->label_4->setNum(FDT[x][y][z]);//x);
    ui->label_13->setNum(x);
    ui->label_16->setNum(y);
    ui->label_17->setNum(z);


    if(ui->checkBox_3->isChecked()==TRUE)
    {
        ui->label_4->setNum(FDT[tempX][tempY][curZ]);//x);
    }

    if(ui->checkBox_4->isChecked()==TRUE)
    {
    ui->label_14->setNum(FCv[tempX][tempY][curZ]);//z);
    ui->label_15->setNum(FCb[tempX][tempY][curZ]);//z);
    }
    if(ui->checkBox_5->isChecked()==TRUE)
    {
    ui->label_14->setNum(MRv[tempX][tempY][curZ]);//z);
    ui->label_15->setNum(MRb[tempX][tempY][curZ]);//z);
    }


    //ui->label_15->setNum(FDT[x][y][z]);
    //if(FDT[x][y][z]!=0) ui->label_15->setNum((float)FDTv[tempX][tempY][curZ]/FDT[x][y][z]);

    ui->label_12->setNum(FDTb[tempX][tempY][curZ]);
    ui->label_itn->setNum(FDTv[tempX][tempY][curZ]);
    //ui->label_itn->setNum(FDT[x][y][z]);

//    ui->spinBox_x->setValue(curX);//me->globalX());
//    ui->spinBox_y->setValue(curY);//me->globalX());

    //ui->checkBox->setCheckState(Qt::Unchecked);
    }

}


void MainWindow::mousePressEvent( QMouseEvent *e )
{
    QPoint pt,pt1;
    char name[200];

    pt=e->pos();
    if(pt.x()>ui->sliceLabel->x() && pt.y()>ui->sliceLabel->y() && pt.x()<ui->sliceLabel->x()+ui->sliceLabel->width() && pt.y()<ui->sliceLabel->y()+ui->sliceLabel->height())
    {

    curX=pt.x()-ui->sliceLabel->x()-1;
    curY=pt.y()-ui->sliceLabel->y()-10;
/*
    int count=0;//LM[curZ*Xdim*Ydim+curY*Xdim+curX];
    int x=count%Xdim;
    int z=count/(Xdim*Ydim);
    int y=(count/Xdim)-(z*Ydim);
    */
/*
    QString str="x=";
    str+=x;
    str+=",y=";
    str+=y;
    str+=",z";
    str+=z;
    QToolTip::showText(pt, str);
*/
    ui->label_12->setNum(vol[curX][curY][curZ]);

    ui->spinBox_x->setValue(curX);//me->globalX());
    ui->spinBox_y->setValue(curY);//me->globalX());

    if(ui->checkBox_2->isChecked()==TRUE)
    {
       draw_image();
    }

    if(ui->checkBox_11->isChecked()==TRUE)//GT
    {
        if(ui->radioButton->isChecked() == TRUE)
        {
        FILE *fpvs;

              if(ui->radioButton_L->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/large/GTv",DataID);
                  fpvs=fopen(name,"a");
              }
              else
              if(ui->radioButton_F->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/fine/GTv",DataID);
                  fpvs=fopen(name,"a");
              }

        //fpvs=fopen("GTv","a");
        fprintf(fpvs,"%d %d %d\n",curX,curY,curZ);

                  if(ui->checkBox_12->isChecked()==TRUE)
                  {
                      fprintf(fpvs,"%d %d %d\n",curX-1,curY,curZ);
                      fprintf(fpvs,"%d %d %d\n",curX+1,curY,curZ);
                      fprintf(fpvs,"%d %d %d\n",curX,curY-1,curZ);
                      fprintf(fpvs,"%d %d %d\n",curX,curY+1,curZ);
                      //fprintf(fpvs,"%d %d %d\n",curX,curY,curZ+1);
                      //fprintf(fpvs,"%d %d %d\n",curX,curY,curZ-1);
                  }

        fclose(fpvs);
        }
        else
        if(ui->radioButton_2->isChecked() == TRUE)
        {
        FILE *fpbs;
               if(ui->radioButton_L->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/large/GTb",DataID);
                  fpbs=fopen(name,"a");
              }
              else
              if(ui->radioButton_F->isChecked() == TRUE)
              {
                  sprintf(name,"/home/subhadip/Test3/%s/GT/fine/GTb",DataID);
                  fpbs=fopen(name,"a");
              }

//        fpbs=fopen("GTb","a");
        fprintf(fpbs,"%d %d %d\n",curX,curY,curZ);

                  if(ui->checkBox_12->isChecked()==TRUE)
                  {
                      fprintf(fpbs,"%d %d %d\n",curX-1,curY,curZ);
                      fprintf(fpbs,"%d %d %d\n",curX+1,curY,curZ);
                      fprintf(fpbs,"%d %d %d\n",curX,curY-1,curZ);
                      fprintf(fpbs,"%d %d %d\n",curX,curY+1,curZ);
                      //fprintf(fpbs,"%d %d %d\n",curX,curY,curZ+1);
                      //fprintf(fpbs,"%d %d %d\n",curX,curY,curZ-1);
                  }

        fclose(fpbs);
        }

        load_GT();
        draw_image();
    }



    //ui->checkBox->setCheckState(Qt::Unchecked);
    }
     else
       if(pt.x()>ui->rowLabel->x() && pt.y()>ui->rowLabel->y() && pt.x()<ui->rowLabel->x()+ui->rowLabel->width() && pt.y()<ui->rowLabel->y()+ui->rowLabel->height())
    {
        curX=pt.x()-ui->rowLabel->x()-1;
        curZ=(pt.y()-ui->colLabel->y()-10)*((float)Zdim/441);//441 is the height of the image containter colLabel
        ui->label_12->setNum(vol[curX][curY][curZ]);

        ui->spinBox_x->setValue(curX);//me->globalX());
        ui->spinBox_z->setValue(curZ);//me->globalX());
        if(ui->checkBox_2->isChecked()==TRUE)
        {
           draw_image();
        }

    }
     else
       if(pt.x()>ui->colLabel->x() && pt.y()>ui->colLabel->y() &&
       pt.x()<ui->colLabel->x()+ui->colLabel->width() &&
       pt.y()<ui->colLabel->y()+ui->colLabel->height())
    {
        curY=pt.x()-ui->colLabel->x()-1;
        curZ=(pt.y()-ui->colLabel->y()-10)*((float)Zdim/441);//441 is the height of the image containter colLabel
        ui->label_12->setNum(vol[curX][curY][curZ]);

        ui->spinBox_y->setValue(curY);//me->globalX());
        ui->spinBox_z->setValue(curZ);//me->globalX());
        if(ui->checkBox_2->isChecked()==TRUE)
        {
           draw_image();
        }

    }


}




void MainWindow::on_spinBox_x_valueChanged(int x)
{
    curX=x;
    draw_image();
}

void MainWindow::on_spinBox_y_valueChanged(int y)
{
    curY=y;
    draw_image();
}

void MainWindow::on_spinBox_z_valueChanged(int z)
{
    curZ=z;
    draw_image();
}






void MainWindow::draw_image()//Refresh
{
    QMessageBox qb;
    qb.setText("draw_image_entry");
   // qb.exec();
    QImage imagex(Ydim,Zdim,QImage::Format_RGB32);
    QImage imagey(Xdim,Zdim,QImage::Format_RGB32);
    QImage imagez(Xdim,Ydim,QImage::Format_RGB32);
    QRgb value = qRgb(0, 0, 0);
    int pixelValue;



         QRgb bone = qRgb(0, 255,0);
         QRgb fuzzy = qRgb(255,255,0);
         QRgb artery = qRgb(255,0,0);
          QRgb coreartery = qRgb(130,190,230);
         QRgb cartery = qRgb(0,0,255);
         QRgb bg = qRgb(0,0,0);

         int ta=ui->spinBox_ta->value();
         int tb=ui->spinBox_tb->value();
         int tap=ui->spinBox_tap->value();
         //int tbp=ui->spinBox_tbp->value();

         float mu;
  //       float factor=255.0/maxFDTb;

//////////////////////////////XXXXXXXXXXXXXXXXXXXXXXXXXXX

    for(int i=0;i<Ydim;i++)
  for(int j=0;j<Zdim;j++)
  {
    //  pixelValue=bright_cont((vol[curX][i][j]+offset)*scale);
      pixelValue  =vol[curX][i][j];
      if((short)spherearr[curX][i][j]==corevessel1 && ui->checkBox_13->isChecked()) {value=coreartery;}
      else if((short)spherearr[curX][i][j]==corevessel2 && ui->checkBox_13->isChecked()) {value=cartery;}
      else value = qRgb(pixelValue, pixelValue, pixelValue);
    imagex.setPixel(QPoint(i,j), value );


    if(ui->checkBox->isChecked()==TRUE)
    {
      pixelValue = vol[curX][i][j];

      mu=(float)(pixelValue-ta)/(tb-ta);
      if(pixelValue >=tb)// && pixelValue <=2048
          imagex.setPixel(i,j, bone );
       else
      if(pixelValue >=tap && pixelValue <=ta)
          imagex.setPixel(i,j, artery );
        else
      if(pixelValue >ta && pixelValue <tb)
          imagex.setPixel(i,j, qRgb(255*(1-mu),255*mu,0));

      }//checkbox if
    else
       if(ui->checkBox_3->isChecked()==TRUE
          && (FDT[curX][i][j]!=0 || FDTv[curX][i][j]!=0))//FDT
    {

      int b= FDTb[curX][i][j];
      int v= FDTv[curX][i][j];

      //if(b==0 && v==0) imagez.setPixel(i,j, qRgb(255,255,255));;

      //int index=j*Xdim*Ydim+i*Xdim+curX;

      //if(b==0 && v==0) imagez.setPixel(i,j, qRgb(255,255,255));;

      if(b>v)
          if(tFDT[curX][i][j]==0)//LM[curX][i][j]==index)//>0)//==maxInt)
          {
              imagex.setPixel(i  ,j  , qRgb(255,0,255));
          }
          else
          {
              unsigned long count=LM[curX][i][j];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);

              if(FDT[x][y][z]>0)
              {
                b= (b*255)/FDT[x][y][z];
                imagex.setPixel(i,j, qRgb(0,b,0));
              }
          }
      else
          {
          if(tFDT[curX][i][j]==0)//LM[curX][i][j]==index)
          {
              imagex.setPixel(i,j, qRgb(0,255,255));
          }
          else
          {
              unsigned long count=LM[curX][i][j];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);

              if(FDT[x][y][z]>0)
              {
                v= (v*255)/FDT[x][y][z];
              imagex.setPixel(i,j, qRgb(v,0,0));
                }
          }

      }


      }//checkbox if
        else
       if(ui->checkBox_4->isChecked()==TRUE)//FC
        {
            if(abs(FCv[curX][i][j]-FCb[curX][i][j])<=10)//originally 10
            {
              //  if (Flag!=5)
              //   imagex.setPixel(i,j, qRgb(255,255,255));
             }
            else
            if(FCv[curX][i][j]>FCb[curX][i][j])
             {
                 int v= (FCv[curX][i][j]*2.55);
                 imagex.setPixel(i,j, qRgb(v,0,0));
             }
            else
             {
                int b= (FCb[curX][i][j]*2.55);
                 imagex.setPixel(i,j, qRgb(0,b,0));
             }



    }
        else
        if(ui->checkBox_5->isChecked()==TRUE)//MR
        {
        if(MRv[curX][i][j]<MRb[curX][i][j])
             {
                 //int v= (-MRv[curX][i][j]*2.55);
                 imagex.setPixel(i,j, qRgb(200,0,0));
             }
            else
                if(MRv[curX][i][j]>MRb[curX][i][j] && ui->checkBox_8->isChecked()==FALSE)
             {
                //int b= (-MRb[curX][i][j]*2.55);
                 imagex.setPixel(i,j, qRgb(0,200,0));
             }
            //else
              //  imagex.setPixel(i,j, qRgb(255,255,255));
        }
    else
        if(ui->checkBox_6->isChecked()==TRUE)//Seeds and Sep
        {
            if(SS[curX][i][j]==1)
            {
                imagex.setPixel(i,j, qRgb(255,0,0));
            }
            else
            if(SS[curX][i][j]==2)
            {
                imagex.setPixel(i,j, qRgb(0,255,0));
            }
            else
            if(SS[curX][i][j]==3)
            {
                imagex.setPixel(i,j, qRgb(0,0,255));
            }


    }


   }

  if(ui->checkBox_2->isChecked()==TRUE)//Gridlines
  {
  value = qRgb(255, 0, 0);
  for(int i=0;i<Zdim;i++)
  {
    imagex.setPixel(curY,i, value);
  }
  value = qRgb(0, 255, 0);
  for(int i=0;i<Ydim;i++)
  {
    imagex.setPixel(i,curZ, value);
  }
  }

  ui->colLabel->setPixmap(QPixmap::fromImage(imagex));
//    QLabel *imageLabel = new QLabel;
//  imageLabel->setPixmap(QPixmap::fromImage(imagex));
 // ui->scrollArea_x->setWidget(imageLabel);


///////////////////////////////YYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  for(int i=0;i<Xdim;i++)
  for(int j=0;j<Zdim;j++)
  {
     // pixelValue=bright_cont((vol[i][curY][j]+offset)*scale);
      pixelValue = vol[i][curY][j];
      if((short)spherearr[i][curY][j]==corevessel1&& ui->checkBox_13->isChecked()) value=coreartery;
      else if((short)spherearr[i][curY][j]==corevessel2 && ui->checkBox_13->isChecked()) {value=cartery;}
     else value = qRgb(pixelValue, pixelValue, pixelValue);
    imagey.setPixel(QPoint(i,j), value );


    if(ui->checkBox->isChecked()==TRUE)//Fuzzy Segment
    {
      pixelValue = vol[i][curY][j];

      mu=(float)(pixelValue-ta)/(tb-ta);
      if(pixelValue >=tb)// && pixelValue <=2048
          imagey.setPixel(i,j, bone );
       else
      if(pixelValue >=tap && pixelValue <=ta)
          imagey.setPixel(i,j, artery );
        else
      if(pixelValue >ta && pixelValue <tb)
          imagey.setPixel(i,j, qRgb(255*(1-mu),255*mu,0));

      }//checkbox if
    else
        if(ui->checkBox_3->isChecked()==TRUE
          && (FDT[i][curY][j]!=0 || FDTv[i][curY][j]!=0))//FDT
    {

      int b= FDTb[i][curY][j];
      int v= FDTv[i][curY][j];

      //if(b==0 && v==0) imagez.setPixel(i,j, qRgb(255,255,255));;

//      int index=j*Xdim*Ydim+curY*Xdim+i;

      //if(b==0 && v==0) imagez.setPixel(i,j, qRgb(255,255,255));;

      if(b>v)
          if(tFDT[i][curY][j]==0)//LM[i][curY][j]==index)//>0)//==maxInt)
          {
              imagey.setPixel(i  ,j  , qRgb(255,0,255));
          }
          else
          {
              unsigned long count=LM[i][curY][j];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);

              if(FDT[x][y][z]>0)
              {
                b= (b*255)/FDT[x][y][z];
                imagey.setPixel(i,j, qRgb(0,b,0));
              }
          }
      else
          {
          if(tFDT[i][curY][j]==0)//LM[i][curY][j]==index)
          {
              imagey.setPixel(i,j, qRgb(0,255,255));
          }
          else
          {
              unsigned long count=LM[i][curY][j];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);

              if(FDT[x][y][z]>0)
              {
                v= (v*255)/FDT[x][y][z];
              imagey.setPixel(i,j, qRgb(v,0,0));
                }
          }

      }
      }//checkbox if
        else
       if(ui->checkBox_4->isChecked()==TRUE)
        {
            if(abs(FCv[i][curY][j]-FCb[i][curY][j])<=10)//originally 10
            {
                //if (Flag!=5)
                // imagey.setPixel(i,j, qRgb(255,255,255));
             }
            else
            if(FCv[i][curY][j]>FCb[i][curY][j])
             {
                 int v= (FCv[i][curY][j]*2.55);
                 imagey.setPixel(i,j, qRgb(v,0,0));
             }
            else
             {
                int b= (FCb[i][curY][j]*2.55);
                 imagey.setPixel(i,j, qRgb(0,b,0));
             }
        }
           else
        if(ui->checkBox_5->isChecked()==TRUE)//MR
        {
        if(MRv[i][curY][j]<MRb[i][curY][j])
             {
                 //int v= (-MRv[i][curY][j]*2.55);
                 imagey.setPixel(i,j, qRgb(200,0,0));
             }
            else
                if(MRv[i][curY][j]>MRb[i][curY][j] && ui->checkBox_8->isChecked()==FALSE)
             {
                //int b= (-MRb[i][curY][j]*2.55);
                 imagey.setPixel(i,j, qRgb(0,200,0));
             }
            //else
              //  imagey.setPixel(i,j, qRgb(255,255,255));
        }
    else
        if(ui->checkBox_6->isChecked()==TRUE)//Seeds and Sep
        {
            if(SS[i][curY][j]==1)
            {
                imagey.setPixel(i,j, qRgb(255,0,0));
            }
            else
            if(SS[i][curY][j]==2)
            {
                imagey.setPixel(i,j, qRgb(0,255,0));
            }
            else
            if(SS[i][curY][j]==3)
            {
                imagey.setPixel(i,j, qRgb(0,0,255));
            }


    }



   }

  if(ui->checkBox_2->isChecked()==TRUE)//Gridlines
  {
  value = qRgb(0, 0, 255);
  for(int i=0;i<Zdim;i++)
  {
    imagey.setPixel(curX,i, value );
  }
  value = qRgb(0, 255, 0);
  for(int i=0;i<Xdim;i++)
  {
    imagey.setPixel(i,curZ, value );
  }
  }

  ui->rowLabel->setPixmap(QPixmap::fromImage(imagey));
//    QLabel *imageLabel_y = new QLabel;
//  imageLabel_y->setPixmap(QPixmap::fromImage(imagey));
//  ui->scrollArea_y->setWidget(imageLabel_y);


  ////////////////////////ZZZZZZZZZZZZZZZZZZZZZZ
   for(int i=0;i<Xdim;i++)
  for(int j=0;j<Ydim;j++)
  {
      //pixelValue=bright_cont(img[i][j][z]);//(vol[i][j][5]+offset)*scale;
     // pixelValue=bright_cont((vol[i][j][curZ]+offset)*scale);
      pixelValue =(vol[i][j][curZ]);
        if((short)spherearr[i][j][curZ]==corevessel1 && ui->checkBox_13->isChecked()) value=coreartery;
        else if((short)spherearr[i][j][curZ]==corevessel2 && ui->checkBox_13->isChecked()) {value=cartery;}
     else value = qRgb(pixelValue, pixelValue, pixelValue);
      imagez.setPixel(QPoint(i,j), value );


    if(ui->checkBox->isChecked()==TRUE)//Fuzzy segment
    {
      pixelValue = vol[i][j][curZ];

      mu=(float)(pixelValue-ta)/(tb-ta);
      if(pixelValue >=tb)// && pixelValue <=2048
          imagez.setPixel(i,j, bone );
       else
      if(pixelValue >=tap && pixelValue <=ta)
          imagez.setPixel(i,j, artery );
        else
      if(pixelValue >ta && pixelValue <tb)
          //imagez.setPixel(i,j, fuzzy);
        imagez.setPixel(i,j, qRgb(255*(1-mu),255*mu,0));

      }//checkbox if
    else if(ui->checkBox_9->isChecked()==TRUE)//Core Vessel
    {
      pixelValue = vol[i][j][curZ];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu=(float)(pixelValue-tap)/(ta-tap);
          imagez.setPixel(i,j,qRgb(255*mu,0,0));
      }
        else if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu=(float)(tb-pixelValue)/(tb-ta);
          imagez.setPixel(i,j,qRgb(255*mu,0,0));
      }

      }//checkbox if
    else if(ui->checkBox_3->isChecked()==TRUE
          && (FDT[i][j][curZ]!=0 || FDTv[i][j][curZ]!=0))//FDT
    {

      int b= FDTb[i][j][curZ];
      int v= FDTv[i][j][curZ];

      //if(b==0 && v==0) imagez.setPixel(i,j, qRgb(255,255,255));;

 //     int index=curZ*Xdim*Ydim+j*Xdim+i;
              unsigned long count=LM[i][j][curZ];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);

              if(FDT[x][y][z]>0)
              {
                b= (b*255)/FDT[x][y][z];if(b>255) b=255;
                v= (v*255)/FDT[x][y][z];if(v>255) v=255;
            }



      if(b>v)
      {
          if(tFDT[i][j][curZ]==0)//index)//>0)//==maxInt)
          {
              imagez.setPixel(i  ,j  , qRgb(255,0,255));
          }
          else
          {


              //if(FDT[x][y][z]>0)
              //{
                //b= (b*255)/FDT[x][y][z];
                //if(b>255) b=255;
                imagez.setPixel(i,j, qRgb(0,b,0));
              //}
          }
      }
      else
      {
          if(tFDT[i][j][curZ]==0)//==maxInt)
          //if(LM[i][j][curZ]==index)
          {
              imagez.setPixel(i,j, qRgb(0,255,255));
          }
          else
          {
              /*
              int count=LM[i][j][curZ];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);
                */
              //if(FDT[x][y][z]>0)
              //{
                //v= (v*255)/FDT[x][y][z];
              imagez.setPixel(i,j, qRgb(v,0,0));
                //}
          }
      }

      }//checkbox if
    else if(ui->checkBox_4->isChecked()==TRUE)//FC
        {
        /*
              int count=LM[i][j][curZ];
              int x=count%Xdim;
              int z=count/(Xdim*Ydim);
              int y=(count/Xdim)-(z*Ydim);
        */
            if(abs(FCv[i][j][curZ]-FCb[i][j][curZ])<=10)//originally 10
              //if(FDT[x][y][z]>0)
              {
              //if(((100*FDTv[i][j][curZ])/FDT[x][y][z])<FCb[i][j][curZ] ||
              //   ((100*FDTb[i][j][curZ])/FDT[x][y][z])<FCv[i][j][curZ])
             //     if((FDTb[i][j][curZ]>FDTv[i][j][curZ]&&FCv[i][j][curZ]>=FCb[i][j][curZ])||
               //      (FDTb[i][j][curZ]<FDTv[i][j][curZ]&&FCv[i][j][curZ]<=FCb[i][j][curZ]))
               // if (Flag!=5)
               //  imagez.setPixel(i,j, qRgb(255,255,255));
             }
            else
            {
            if(FCv[i][j][curZ]>FCb[i][j][curZ])
             {
                 int v= (FCv[i][j][curZ]*2.55);
                 imagez.setPixel(i,j, qRgb(v,0,0));
             }
            else
             {
                int b= (FCb[i][j][curZ]*2.55);
                 imagez.setPixel(i,j, qRgb(0,b,0));
             }

        }


    }
    else if(ui->checkBox_5->isChecked()==TRUE)//MR
        {
        if(MRv[i][j][curZ]<MRb[i][j][curZ])
             {
                 //int v= (-MRv[i][j][curZ]*2.55);
                 imagez.setPixel(i,j, qRgb(200,0,0));
             }
            else
                if(MRv[i][j][curZ]>MRb[i][j][curZ] && ui->checkBox_8->isChecked()==FALSE)
             {
                //int b= (-MRb[i][j][curZ]*2.55);
                 imagez.setPixel(i,j, qRgb(0,200,0));
             }
            //else
              //  imagez.setPixel(i,j, qRgb(255,255,255));
        }
    else if(ui->checkBox_6->isChecked()==TRUE)//Seeds and Sep
        {
            if(SS[i][j][curZ]==1)
            {
                imagez.setPixel(i,j, qRgb(255,0,0));
            }
            else
            if(SS[i][j][curZ]==2)
            {
                imagez.setPixel(i,j, qRgb(0,255,0));
            }
            else
            if(SS[i][j][curZ]==3)
            {
                imagez.setPixel(i,j, qRgb(0,0,255));
            }
        }
     else if(ui->checkBox_11->isChecked()==TRUE)//GT
        {
            if(GT[i][j][curZ]==1)
            {
                imagez.setPixel(i,j, qRgb(255,255,0));//Vessel
            }
            else
            if(GT[i][j][curZ]==2)
            {
                imagez.setPixel(i,j, qRgb(0,255,255));//Bone
            }
        }
     else if(ui->checkBox_10->isChecked()==TRUE)//NML
            {
                if(NML[i][j][curZ]>0)
                imagez.setPixel(i  ,j  , qRgb(255,255,0));
            }



}//for loop

  if(ui->checkBox_2->isChecked()==TRUE)//Gridlines
  {
  value = qRgb(0, 0, 255);
  for(int i=0;i<Ydim;i++)
  {
    imagez.setPixel(curX,i, value );
  }
  value = qRgb(255, 0, 0);
  for(int i=0;i<Xdim;i++)
  {
    imagez.setPixel(i,curY, value );
  }
  }

    ui->sliceLabel->setPixmap(QPixmap::fromImage(imagez));
/////////////////////////////////////////////////////////////

ui->label_12->setNum(vol[curX][curY][curZ]);
}


void MainWindow::on_checkBox_clicked(bool checked)//Fuzzy Segment
{
    draw_image();
}

void MainWindow::on_checkBox_2_clicked()//Gridlines
{
    draw_image();
}

void MainWindow::on_horizontalSlider_b_valueChanged(int b_value)
{
    ui->checkBox->setCheckState(Qt::Unchecked);
    draw_image();
}

void MainWindow::on_horizontalSlider_c_valueChanged(int c_value)
{
  ui->checkBox->setCheckState(Qt::Unchecked);
  draw_image();
}

int MainWindow::bright_cont(int value)
{
      int b_val= ui->horizontalSlider_b->value() + value;
   //   std::cout<<"B-Value= "<<b_val<<std::endl;
      b_val=b_val>255?255:b_val;
      b_val=b_val<0?0:b_val;

    float c,val,PIby4=0.785398163397;

    c=(float) ui->horizontalSlider_c->value()/100;
    val=(float)b_val/255;
       val=(val-0.5)*tan ((c+1) * PIby4 )+0.5;
        //val+=0.5;
        b_val=val*255;

      b_val=b_val>255?255:b_val;
      b_val=b_val<0?0:b_val;

      return b_val;
}



void MainWindow::on_checkBox_3_clicked()//FDT
{
 draw_image();
}


void MainWindow::on_pushButton_test_clicked()//FDT
{
    load_seeds();
    load_coreVessel();
    ui->spinBox_ta->setValue(ui->spinBox_tap->value());

    short pixelValue ;
    int ta=ui->spinBox_ta->value();
    int tb=ui->spinBox_tb->value();
    int tap=ui->spinBox_tap->value();
    int tbp=ui->spinBox_tbp->value();


 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
              pixelValue = vol[i][j][k];

              if(pixelValue <tap || pixelValue > tb)//Vessel
                FDTv[i][j][k]=0;
              else
                FDTv[i][j][k]=65000;



              if(i==0 ||j==0||k<=ui->spinBox_TA->value()//Ignore slices causing Tooth Artifacts
                  ||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDTv[i][j][k]=0;

          }
/*
  FILE *fpbs;
  fpbs=fopen("bseed","r");
  if(fpbs != NULL){
  while(!feof(fpbs))
  {
      int x,y,z;
      fscanf(fpbs,"%d%d%d",&x,&y,&z);
      FDTv[x][y][z]=0;
  }
  fclose(fpbs);
 }
*/
     for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==2)//Bone
                  FDTv[i][j][k]=0;



 Flag=2;//0;//2;
 ui->Plabel->setText("Computing FDT for Vessel... Iterration:");
 compute_FDT(FDTv);






 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
              pixelValue = vol[i][j][k];

              if(pixelValue <ta || pixelValue > tbp)//Bone
                FDTb[i][j][k]=0;
              else
                FDTb[i][j][k]=65000;


              if(i==0 ||j==0||k<=ui->spinBox_TA->value()//Ignore slices causing Tooth Artifacts
                  ||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDTb[i][j][k]=0;

          }
/*
  FILE *fpvs;
  fpvs=fopen("vseed","r");
  if(fpvs != NULL){
  while(!feof(fpvs))
  {
      int x,y,z;
      fscanf(fpvs,"%d%d%d",&x,&y,&z);
      FDTb[x][y][z]=0;
  }
  fclose(fpvs);
  }
*/

      for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==1)//Vessel Seeds
                  FDTb[i][j][k]=0;

  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              //if(FCv[i][j][k]>0)
              if(out[i][j][k]==5)//core vessel
              {
                  FDTb[i][j][k]=0;
              }



 Flag=2;//1;//2;
 ui->Plabel->setText("Computing FDT for Bone... Iterration:");
 compute_FDT(FDTb);


 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
              pixelValue = vol[i][j][k];

              if(pixelValue <ta || pixelValue > tbp)//Outside Bone and Vessel
                FDT[i][j][k]=0;
              else
                FDT[i][j][k]=65000;

              if(i==0 ||j==0||k<=ui->spinBox_TA->value()//Ignore slices causing Tooth Artifacts
                  ||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDT[i][j][k]=0;

          }

 Flag=2;
 ui->Plabel->setText("Computing FDT for Bone-Vessel Iterration:");
 compute_FDT(FDT);

 ui->checkBox_3->setCheckState(Qt::Checked);


/*
 ///////////////////////////////
 FILE *fpb,*fpv,*fp;
 fpb=fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDTb.dat","w");
 fpv=fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDTv.dat","w");
 fp =fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDT.dat" ,"w");

 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
     fprintf(fpb,"%d ",FDTb[i][j][k]);
     fprintf(fpv,"%d ",FDTv[i][j][k]);
     fprintf(fp ,"%d ",FDT[i][j][k]);
 }
 fclose(fpb);
 fclose(fpv);
 fclose(fp);
 ////////////////////////////////
 */


 on_pushButton_3_clicked();//LM-LS

 GITN=0;

 on_pushButton_4_clicked();//FC-MR



}


void MainWindow::compute_FDT(unsigned short ***lFDT)
{

    float min,temp;
    int i,j,k,d;
    float mu_p,mu_q;
    short pixelValue;

    int tap=ui->spinBox_tap->value();
    int ta=ui->spinBox_ta->value();
    int tb=ui->spinBox_tb->value();

/*
  FILE *fps;
  fps=fopen("sep","r");
  if(fps != NULL){
  while(!feof(fps))
  {
      int x,y,z;
      fscanf(fps,"%d%d%d",&x,&y,&z);
      lFDT[x][y][z]=0;
  }
  fclose(fps);
  }
*/
 //  if(Flag==5)
       {
    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==3)//Sep
                  lFDT[i][j][k]=0;
        }



 int itn=0,F_count=0;
//do
{

     ui->label_itn->setNum(++itn);

F_count=0;
 for(i=1;i<Xdim-1;i++)//Pos FDT
    {
      for(j=1;j<Ydim-1;j++)
          for(k=1;k<=Zdim-1;k++)
          {
            if(lFDT[i][j][k]==0) continue;


    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    if (Flag==0) mu_p=1-mu_p;
    if (Flag==2 && mu_p<0.5) mu_p=1-mu_p;


    if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_p=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_p=(float)(tb-pixelValue)/(tb-ta);
      }

    }


    if (mu_p>1) mu_p=1;
    if (mu_p<0) mu_p=0;

    min= lFDT[i][j][k];
    for(d=0;d<13;d++)
    {
        if(lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]==0)
            mu_q=0;
        else
        {

        mu_q=(float)(vol[i+V[d][0]][j+V[d][1]][k+V[d][2]]-ta)/(tb-ta);
        if (Flag==0) mu_q=1-mu_q;
        if (Flag==2 && mu_q<0.5) mu_q=1-mu_q;


    if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_q=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_q=(float)(tb-pixelValue)/(tb-ta);
      }

    }




        if (mu_q>1) mu_q=1;
        if (mu_q<0) mu_q=0;

        }

        temp=lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];

        temp=ceil(temp-0.5);
        //temp*=100;

        //if(temp<0) temp=0;

    if(temp<min)//>=1.0)
    {
        lFDT[i][j][k]=temp;
        min=temp;
        F_count++;
    }
        }
}
      ui->progressBar->setValue((50*i)/(Xdim-2)+1);
 }//Outer for loop pos FDT


  for(i=Xdim-2;i>0;i--)//Neg FDT
 {
      for(j=Ydim-2;j>0;j--)
          for(k=Zdim-2;k>=0;k--)
          {
            if(lFDT[i][j][k]==0) continue;

    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    if (Flag==0) mu_p=1-mu_p;
    if (Flag==2 && mu_p<0.5) mu_p=1-mu_p;

        if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_p=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_p=(float)(tb-pixelValue)/(tb-ta);
      }

    }


    if (mu_p>1) mu_p=1;
    if (mu_p<0) mu_p=0;

    min= lFDT[i][j][k];

    for(d=13;d<26;d++)
    {
        if(lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]==0)
            mu_q=0;
        else
        {

        mu_q=(float)(vol[i+V[0][0]][j+V[0][1]][k+V[0][2]]-ta)/(tb-ta);
        if (Flag==0) mu_q=1-mu_q;
        if (Flag==2 && mu_q<0.5) mu_q=1-mu_q;

        if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_q=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_q=(float)(tb-pixelValue)/(tb-ta);
      }

    }

        if (mu_q>1) mu_q=1;
        if (mu_q<0) mu_q=0;

    }

        temp=lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];

        temp=ceil(temp-0.5);
        //temp*=100;

        if(temp<0) temp=0;

    if(temp<min)//>=1.0)
    {
        lFDT[i][j][k]=temp;
        min=temp;
        F_count++;
    }

         }
}
      ui->progressBar->setValue(51+((Xdim-2-i)*50/(Xdim-2)));
  }//Outer for loop Neg FDT


  std::cout<<"Iteration:"<<itn<<", Updated points:"<<F_count<<std::endl;
//  ui->label_12->setNum(F_count);
}//while(F_count>0);




}


void MainWindow::on_pushButton_20_clicked()
{
    fstream outf;
    short pointx, pointy, pointz,type=2;
    pointx = ui->spinBox_x->value();
    pointy = ui->spinBox_y->value();
    pointz = ui->spinBox_z->value();
    if(ui->checkBox_14->isChecked()) type=2;
    else if(ui->checkBox_15->isChecked()) type =1;
    else if(ui->checkBox_16->isChecked()) type = 3;
    outf.open("Spine_seed.txt",ios::out | ios::app);

    outf<<curX<<" "<<curY<<" "<<curZ<<" "<<type<<"\n";
}

void MainWindow::on_pushButton_3_clicked()//Local Maxima and Local Scale
{
 /*
 ///////////////////////////////
 FILE *fpb,*fpv,*fp;
 fpb=fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDTb.dat","r");
 fpv=fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDTv.dat","r");
 fp =fopen("/local/vol00/basu/Work/MyPrograms/GUI1/QTITK3/test/FDT.dat" ,"r");

 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
     fscanf(fpb,"%d",&FDTb[i][j][k]);
     fscanf(fpv,"%d",&FDTv[i][j][k]);
     fscanf(fp ,"%d",&FDT[i][j][k]);
 }
 fclose(fpb);
 fclose(fpv);
 fclose(fp);
 ////////////////////////////////
 */


  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
                      tFDT[i][j][k]=65000;
                      LM[i][j][k]=0;
                  }

 ui->Plabel->setText("Computing LM for Vessel and Bone...");
 ui->label_itn->setText("");

 local_maxima();
 draw_image();
////end of local maxima



 ui->Plabel->setText("Computing LS-FDT for Vessel... Iterration:");
 local_scale();
 draw_image();
}



void MainWindow::local_maxima()
{
    unsigned short max=0,temp;
    int l=2,BG=0;
    unsigned long F_count=0,index;


    for(int i=l;i<Xdim-l;i++)
    {
      for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim-l;k++)
          {
            if(FDT[i][j][k]<=0) continue;
            //if(NML[i][j][k]==1) continue;//No LM in No Mans Land

            max=FDT[i-l][j-l][k-l];

            BG=0;
      for(int x=-l;x<=l;x++)
            {
          for(int y=-l;y<=l;y++)
          {
              for(int z=-l;z<=l;z++)

                 {
      //               temp=FDT[i+V[l][0]][j+V[l][1]][k+V[l][2]];
                       if(x==-l && y==-l && z==-l) continue;
                       if(x==0 && y==0 && z==0) continue;

                       temp=FDT[i+x][j+y][k+z];

  //                     if(temp<=0) {BG=1; break;}

                     if(temp>max)
                      max=temp;
                 }
    //          if (BG==1) break;
            }
    //      if (BG==1) break;
         }
               if(abs(FDT[i][j][k]-max)<=10 && BG==0)
               {
                   index=k*Xdim*Ydim+j*Xdim+i;
                   LM[i][j][k]=index;
                   tFDT[i][j][k]=0;
                   //LM[i][j][k]=++F_count;//maxInt;
                   F_count++;
               }

    }
      ui->progressBar->setValue((100*i)/(Xdim-2)+1);
}

std::cout<<"LM points for Bone and Vessel:"<<F_count<<std::endl;


}

void MainWindow::local_scale()
{


    unsigned short min,temp;
    int i,j,k,d;
    float mu_p,mu_q;
    short pixelValue;

    int ta=ui->spinBox_ta->value();
    int tb=ui->spinBox_tb->value();
         int tap=ui->spinBox_tap->value();
         //int tbp=ui->spinBox_tbp->value();


 int itn=0;
 long F_count=0;
//do
{

     ui->label_itn->setNum(++itn);

F_count=0;
 for(i=1;i<Xdim-1;i++)//Pos FDT
    {
      for(j=1;j<Ydim-1;j++)
          for(k=1;k<=Zdim-1;k++)
          {
            if(FDT[i][j][k]==0) continue;
            if(tFDT[i][j][k]==0) continue;

    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    if (Flag==2 && mu_p<0.5) mu_p=1-mu_p;
//    if (Flag==0) mu_p=1-mu_p;

    if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_p=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_p=(float)(tb-pixelValue)/(tb-ta);
      }

    }

    if (mu_p>1) mu_p=1;

    min = tFDT[i][j][k];
    for(d=0;d<13;d++)
    {
        if(FDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]==0) continue;

        mu_q=(float)(vol[i+V[d][0]][j+V[d][1]][k+V[d][2]]-ta)/(tb-ta);
        if (Flag==2 && mu_q<0.5) mu_q=1-mu_q;
//        if (Flag==0) mu_q=1-mu_q;
        if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_q=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_q=(float)(tb-pixelValue)/(tb-ta);
      }

    }

        if (mu_q>1) mu_q=1;

        temp=tFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];

        temp=ceil(temp-0.5);
        //temp*=100;

        if(temp<0) temp=0;


        if(temp<min)
        {tFDT[i][j][k] = temp; min = temp; LM[i][j][k] = LM[i+V[d][0]][j+V[d][1]][k+V[d][2]];F_count++;}
    }

        }
      ui->progressBar->setValue((50*i)/(Xdim-2)+1);
 }//Outer for loop pos FDT


  for(i=Xdim-2;i>0;i--)//Neg FDT
 {
      for(j=Ydim-2;j>0;j--)
          for(k=Zdim-2;k>=0;k--)
          {
            if(FDT[i][j][k]==0) continue;
            if(tFDT[i][j][k]==0) continue;

    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    if (Flag==2 && mu_p<0.5) mu_p=1-mu_p;
//    if (Flag==0) mu_p=1-mu_p;

    if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_p=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_p=(float)(tb-pixelValue)/(tb-ta);
      }

    }


    if (mu_p>1) mu_p=1;

    min = tFDT[i][j][k];
    for(d=13;d<26;d++)
    {
        if(FDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]==0) continue;

        mu_q=(float)(vol[i+V[0][0]][j+V[0][1]][k+V[0][2]]-ta)/(tb-ta);
        if (Flag==2 && mu_q<0.5) mu_q=1-mu_q;
//        if (Flag==0) mu_q=1-mu_q;

        if(Flag==5)
    {
       pixelValue = vol[i][j][k];

      if(pixelValue >=tap && pixelValue <=ta && (ta-tap)>0)
      {
           mu_q=(float)(pixelValue-tap)/(ta-tap);
      }
        else
      if(pixelValue >ta && pixelValue <=tb  && (tb-ta)>0)
      {
           mu_q=(float)(tb-pixelValue)/(tb-ta);
      }

    }

        if (mu_q>1) mu_q=1;

        temp=tFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];

        temp=ceil(temp-0.5);
        //temp*=100;

        if(temp<0) temp=0;

        if(temp<min)
        {tFDT[i][j][k] = temp; min = temp; LM[i][j][k] = LM[i+V[d][0]][j+V[d][1]][k+V[d][2]];F_count++;}
    }

    }

      ui->progressBar->setValue(51+((Xdim-2-i)*50/(Xdim-2)));
  }//Outer for loop Neg FDT


  std::cout<<"LM-FDT Iteration:"<<itn<<", Updated points:"<<F_count<<std::endl;
//  ui->label_12->setNum(F_count);
//  draw_image();
}//while(F_count>0);



}




void MainWindow::on_pushButton_4_clicked()//FC
{
        unsigned short min=0,temp;
        long F_countv=0,F_countb=0;//,max=0;
        //int itn=0;
        int newCnt;

        point tp;
        QQueue<point> VQ;
        QQueue<point> BQ;


if(GITN==0)
{
    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
        for(int k=0;k<Zdim;k++)
          {
        FCb[i][j][k]=FCv[i][j][k]=0;
        MRb[i][j][k]=MRv[i][j][k]=0;
    }

}



//    int tb =ui->spinBox_tb->value();



do//Big Loop
{

ui->checkBox_3->setCheckState(Qt::Unchecked);
ui->checkBox_4->setCheckState(Qt::Checked);



std::cout<<"Computing FC-MR...Itn::"<<GITN<<std::endl;
ui->label_itn->setNum(GITN);

Flag=0;
F_countv=0;
F_countb=0;
if(GITN>0)
{
  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {


                    if(MRv[i][j][k]<MRb[i][j][k])//Vessel Seed
                    //if((FCv[i][j][k]-FCb[i][j][k])>=10)
                    {

                        //if(FCv[i][j][k]!=100)//risky step..not pushing all seeds
                        {

                            tp.x=i;tp.y=j;tp.z=k;
                            VQ.enqueue(tp);

                            FCv[i][j][k]=100;
                            FCb[i][j][k]=0;
                            FDTb[i][j][k]=0;

                            F_countv++;
                        }


                    }
                    else
                    //if((FCb[i][j][k]-FCv[i][j][k])>=10)
                    if(MRb[i][j][k]<MRv[i][j][k])//Bone Seed
                    {

                        //if(FCb[i][j][k]!=100)//risky step
                        {
                        tp.x=i;tp.y=j;tp.z=k;
                        BQ.enqueue(tp);
                        FCb[i][j][k]=100;
                        FCv[i][j][k]=0;
                        FDTv[i][j][k]=0;
                        F_countb++;
                        }
                    }
                    else
                    {
                        if(MRb[i][j][k] < 0) ////// Here: MRb[i][j][k] == MRv[i][j][k]
                        {
                          FDTb[i][j][k]=0;
                          FDTv[i][j][k]=0;
                        }
                        MRb[i][j][k]=MRv[i][j][k]=0;
                        FCb[i][j][k]=FCv[i][j][k]=0;
                    }


                    /*
                    if((FCv[i][j][k]-FCb[i][j][k])>=10)
                        FCv[i][j][k]=100;
                    else
                    if((FCb[i][j][k]-FCv[i][j][k])>=10)
                        FCb[i][j][k]=100;
                    else
                    {
                        FCb[i][j][k]=FCv[i][j][k]=0;
                    }
                    */

          }
}


if(GITN==0)
{
/*
for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(FCv[i][j][k]>0)
              {
                  FCv[i][j][k]=100;
                  tp.x=i;tp.y=j;tp.z=k;
                  VQ.enqueue(tp);
                  FDTb[i][j][k]=0;
                  FCb[i][j][k]=0;

              }

  FILE *fpvs;
  fpvs=fopen("vseed","r");
  if(fpvs != NULL){
  while(!feof(fpvs))
  {
      int x,y,z;
      fscanf(fpvs,"%d%d%d",&x,&y,&z);
      tp.x=x;tp.y=y;tp.z=z;
      VQ.enqueue(tp);
      FCv[x][y][z]=100;
      FDTb[x][y][z]=0;
      FCb[x][y][z]=0;
  }
  fclose(fpvs);
 }
*/

      for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==1)//Vessel Seeds
              {
                  FCv[i][j][k]=100;
                  tp.x=i;tp.y=j;tp.z=k;
                  VQ.enqueue(tp);
                  FDTb[i][j][k]=0;
                  FCb[i][j][k]=0;
              }


  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              //if(FCv[i][j][k]>0)
              if(out[i][j][k]==5)//core vessel
              {
                  FCv[i][j][k]=100;
                  tp.x=i;tp.y=j;tp.z=k;
                  VQ.enqueue(tp);
                  FDTb[i][j][k]=0;
                  FCb[i][j][k]=0;
              }


}

  ui->Plabel->setText("Computing F-C for Vessel...");
  //std::cout<<"Computing F-C for Vessel..."<<std::endl;

  //std::cout<<std::endl;

  //max=VQ.size();
  std::cout<<"FC_Vessel: Q Size:"<<VQ.size()<<std::endl;

  //F_count=0;
  while(!VQ.isEmpty())
  {
      tp=VQ.dequeue();
      int i=tp.x;int j=tp.y;int k=tp.z;
      if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) continue;

      min=FCv[i][j][k];
      for(int l=0;l<26;l++)
      {
        if (FDTv[i+V[l][0]][j+V[l][1]][k+V[l][2]] <=0) continue;
        if(FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]]==100) continue;



        if(NML[i+V[l][0]][j+V[l][1]][k+V[l][2]]==1 && GITN==0) continue;//ui->checkBox_10->isChecked()==TRUE) continue;


                   unsigned long count=LM[i+V[l][0]][j+V[l][1]][k+V[l][2]];
                   int x=count%Xdim;
                   int z=count/(Xdim*Ydim);
                   int y=(count/Xdim)-(z*Ydim);

                   if(FDT[x][y][z]==0) continue;

                     temp=100*FDTv[i+V[l][0]][j+V[l][1]][k+V[l][2]]/FDT[x][y][z];
                     //temp=ceil(temp-0.5);

                     if(temp<min) min=temp;

                     if(min>FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]])
                     {
                      FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]]=min;

                      //if(NML[i+V[l][0]][j+V[l][1]][k+V[l][2]]==1 && GITN==0) continue;

                      tp.x=i+V[l][0];
                      tp.y=j+V[l][1];
                      tp.z=k+V[l][2];
                      VQ.enqueue(tp);

                      //F_count++;
                     }
      }

      //if(VQ.size()>max) max=Q.size();
      //ui->progressBar->setValue(100*(1-(Q.size()/max)));
      //printf("\rFC_Vessel:Updated points: %ld",F_count);
      //std::cout<<"FC_Vessel:"<<"Q Size:"<<Q.size()<<std::endl;
      //fflush(stdout);

  }


  //////////////////////

  std::cout<<"Updated FC points for Vessel::"<<F_countv<<std::endl;

  ui->Plabel->setText("Computing F-C for Bone...");
  //std::cout<<"Computing F-C for Bone..."<<std::endl;




  if(GITN==0)
{
    int tb =ui->spinBox_tb->value();
  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(vol[i][j][k]>tb || SS[i][j][k]==2)//Bone seeds
              {
                  FCb[i][j][k]=100;
                  tp.x=i;tp.y=j;tp.z=k;
                  BQ.enqueue(tp);
                  FDTv[i][j][k]=0;
              }

 /*
  FILE *fpbs;
  fpbs=fopen("bseed","r");
  if(fpbs != NULL){
  while(!feof(fpbs))
  {
      int x,y,z;
      fscanf(fpbs,"%d%d%d",&x,&y,&z);
      tp.x=x;tp.y=y;tp.z=z;
      if(FCb[x][y][z]!=100)
      {
      BQ.enqueue(tp);
      FCb[x][y][z]=100;
      FDTv[x][y][z]=0;
      }
  }
  fclose(fpbs);
}
*/

}


  //std::cout<<"FC_Bone: Q Size:"<<BQ.size()<<std::endl;
  //F_count=0;
  while(!BQ.isEmpty())
  {
      tp=BQ.dequeue();
      int i=tp.x;int j=tp.y;int k=tp.z;
      if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) continue;

      min=FCb[i][j][k];
      for(int l=0;l<26;l++)
      {
        if (FDTb[i+V[l][0]][j+V[l][1]][k+V[l][2]] <=0) continue;
        if(FCb[i+V[l][0]][j+V[l][1]][k+V[l][2]]==100) continue;

        if(NML[i+V[l][0]][j+V[l][1]][k+V[l][2]]==1 && GITN==0) continue;//&& ui->checkBox_10->isChecked()==TRUE) continue;



                   unsigned long count=LM[i+V[l][0]][j+V[l][1]][k+V[l][2]];
                   int x=count%Xdim;
                   int z=count/(Xdim*Ydim);
                   int y=(count/Xdim)-(z*Ydim);

                   if(FDT[x][y][z]==0) continue;

                     temp=100*FDTb[i+V[l][0]][j+V[l][1]][k+V[l][2]]/FDT[x][y][z];
                     //temp=ceil(temp-0.5);

                     if(temp<min) min=temp;

                     if(min>FCb[i+V[l][0]][j+V[l][1]][k+V[l][2]])
                     {
                      FCb[i+V[l][0]][j+V[l][1]][k+V[l][2]]=min;

                      //if(NML[i+V[l][0]][j+V[l][1]][k+V[l][2]]==1 && GITN==0) continue;

                      tp.x=i+V[l][0];
                      tp.y=j+V[l][1];
                      tp.z=k+V[l][2];
                      BQ.enqueue(tp);

                      //F_count++;
                     }
      }

      //ui->progressBar->setValue(100*(Q.size()/)

      //printf("\rFC_Bone:Updated points: %ld",F_count);
      //std::cout<<"FC_Bone:"<<"Q Size:"<<Q.size()<<std::endl;
      //fflush(stdout);
  }

  //NML consideration for GITN==0
/*
  if(ui->checkBox_4->isChecked()==TRUE)//NML consideration
  {
     for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
        for(int k=0;k<Zdim;k++)
           if(NML[i][j][k]==1)
               FCb[i][j][k]=FCv[i][j][k]=0;

  }
*/


  //std::cout<<std::endl;

  /////////////////////




  std::cout<<"Updated FC points for Bone  ::"<<F_countb<<std::endl;

draw_image();

on_pushButton_8_clicked();//MR

GITN++;


//populate output
newCnt=0;
for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(MRv[i][j][k]<MRb[i][j][k])
              {
                if(out[i][j][k]==0)
                {
                  out[i][j][k]=5+(3*GITN);
                  newCnt++;
                }
              }

std::cout<<"New Vessel points in this iteration::"<<newCnt<<std::endl;

////////////////////




if(ui->checkBox_7->isChecked()==FALSE) break;

}while(newCnt>0);
//}while(F_countb>0 && F_countv>0 && GITN<20);

/*
for(int i=0;i<Xdim/2;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=ui->spinBox_TA->value();k<Zdim;k++)
              //if(MRv[i][j][k]>MRb[i][j][k])
              if(vol[i][j][k]>ui->spinBox_tb->value())//for pure bone diplay only
              {
                if(out[i][j][k]==0)
                {
                  out[i][j][k]=2;//Write Bone information
                  //newCnt++;
                }
              }
*/
}


void MainWindow::on_checkBox_4_clicked()
{
    draw_image();
}


void MainWindow::on_checkBox_13_clicked()
{
    if(firstm!=0 || firstc!=0)
    draw_image();
}


void MainWindow::on_pushButton_8_clicked()//Morphological Reconstruction
{
    short temp;
    int d;
    float mu_p,mu_q;
    long F_count=0;//,cnt=0;

    int ta=ui->spinBox_ta->value();
    int tb=ui->spinBox_tb->value();

        point tp;
        QQueue<point> Q;



ui->checkBox_4->setCheckState(Qt::Unchecked);
ui->checkBox_5->setCheckState(Qt::Checked);

 for(int i=1;i<Xdim-1;i++)
      for(int j=1;j<Ydim-1;j++)
          for(int k=1;k<Zdim-1;k++)
          {
              //pixelValue = vol[i][j][k];

              //if(FCv[i][j][k]>FCb[i][j][k])//pixelValue <=tap || pixelValue > tb)//Vessel

/*
            for(d=0,cnt=0;d<26;d++)
                if(FCv[i+V[d][0]][j+V[d][1]][k+V[d][2]]==100) cnt++;
            if(cnt==26) continue;//Working only with the peripheral seeds
*/
              //if(NML[i][j][k]==1 && GITN==0) continue;

              if((FCv[i][j][k]-FCb[i][j][k])>10)//originally 10
                {
                MRv[i][j][k]=(-1)*FDTv[i][j][k];
                tp.x=i;tp.y=j;tp.z=k;
                Q.enqueue(tp);
                   }
              else
                MRv[i][j][k]=0;

//              if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDTv[i][j][k]=0;

          }
/*
  FILE *fpbs;
  fpbs=fopen("bseed","r");
  while(!feof(fpbs))
  {
      int x,y,z;
      fscanf(fpbs,"%d%d%d",&x,&y,&z);
      FDTv[x][y][z]=0;
  }
  fclose(fpbs);
*/

 //Flag=1;
 ui->Plabel->setText("Computing MR for Vessel... Iteration:");
// compute_MR(MRv);
 F_count=0;
  while(!Q.isEmpty())
  {
      tp=Q.dequeue();
      int i=tp.x;int j=tp.y;int k=tp.z;
      if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) continue;

    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    //if (Flag==0) mu_p=1-mu_p;

    if (mu_p<0.5) mu_p=1-mu_p;
    if (mu_p>1) mu_p=1;


    for(d=0;d<26;d++)
    {
        if(FDTv[i][j][k]<=FDTv[i+V[d][0]][j+V[d][1]][k+V[d][2]]) continue;

        if(NML[i+V[d][0]][j+V[d][1]][k+V[d][2]]==1 && GITN==0) continue;//ui->checkBox_10->isChecked()==TRUE) continue;

        mu_q=(float)(vol[i+V[d][0]][j+V[d][1]][k+V[d][2]]-ta)/(tb-ta);

        if (mu_q<0.5) mu_q=1-mu_q;
        if (mu_q>1) mu_q=1;

        //temp=lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];
        temp=MRv[i][j][k]+50*(mu_p+mu_q)*D[d];

        //temp=ceil(temp-0.5);
        //temp*=100;

        //if(temp<0) temp=0;

        //printf("\r Here\n");

    if(MRv[i+V[d][0]][j+V[d][1]][k+V[d][2]]>temp && FCb[i+V[d][0]][j+V[d][1]][k+V[d][2]]!=100)//>=1.0)
    {
        MRv[i+V[d][0]][j+V[d][1]][k+V[d][2]]=temp;

        F_count++;

        tp.x=i+V[d][0];
        tp.y=j+V[d][1];
        tp.z=k+V[d][2];
        Q.enqueue(tp);
    }

    }
    //printf("\rMRv Q Size:%ld",Q.size());
    //printf("\rMRv:Updated points: %ld",F_count);

  }

  std::cout<<"Updated MR points for Vessel  ::"<<F_count<<std::endl;
//std::cout<<std::endl;


 for(int i=1;i<Xdim-1;i++)
      for(int j=1;j<Ydim-1;j++)
          for(int k=1;k<Zdim-1;k++)
          {
 //             pixelValue = vol[i][j][k];

/*
            for(d=0,cnt=0;d<26;d++)
                if(FCb[i+V[d][0]][j+V[d][1]][k+V[d][2]]==100) cnt++;
            if(cnt==26) continue;//Working only with the peripheral seeds
*/

                //if(NML[i][j][k]==1 && GITN==0) continue;

              if((FCb[i][j][k]-FCv[i][j][k])>10)//originally 10  //pixelValue <ta || pixelValue >= tbp)//Bone
                {
                MRb[i][j][k]=(-1)*FDTb[i][j][k];
                tp.x=i;tp.y=j;tp.z=k;
                Q.enqueue(tp);
                }
              else
                MRb[i][j][k]=0;

 //             if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDTb[i][j][k]=0;

          }
/*
  FILE *fpvs;
  fpvs=fopen("vseed","r");
  while(!feof(fpvs))
  {
      int x,y,z;
      fscanf(fpvs,"%d%d%d",&x,&y,&z);
      FDTb[x][y][z]=0;
  }
  fclose(fpvs);
*/

 //Flag=2;
 ui->Plabel->setText("Computing MR for Bone... Iteration:");
 //compute_MR(MRb);
F_count=0;
 while(!Q.isEmpty())
  {
      tp=Q.dequeue();
      int i=tp.x;int j=tp.y;int k=tp.z;

      if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) continue;

    mu_p=(float)(vol[i][j][k]-ta)/(tb-ta);
    //if (Flag==0) mu_p=1-mu_p;

    //if (mu_p<0) mu_p=0;
    if (mu_p<0.5) mu_p=1-mu_p;
    if (mu_p>1) mu_p=1;


    for(d=0;d<26;d++)
    {
        if(FDTb[i][j][k]<=FDTb[i+V[d][0]][j+V[d][1]][k+V[d][2]]) continue;

        if(NML[i+V[d][0]][j+V[d][1]][k+V[d][2]]==1 && GITN==0) continue;//ui->checkBox_10->isChecked()==TRUE) continue;


        mu_q=(float)(vol[i+V[d][0]][j+V[d][1]][k+V[d][2]]-ta)/(tb-ta);

        //if (mu_q<0) mu_q=0;
        if (mu_q>1) mu_q=1;
        if (mu_q<0.5) mu_q=1-mu_q;


        //temp=lFDT[i+V[d][0]][j+V[d][1]][k+V[d][2]]+50*(mu_p+mu_q)*D[d];
        temp=MRb[i][j][k]+50*(mu_p+mu_q)*D[d];

        //temp=ceil(temp-0.5);
        //temp*=100;

        //if(temp<0) temp=0;

        //printf("\r Here\n");

    if(MRb[i+V[d][0]][j+V[d][1]][k+V[d][2]]>temp  && FCv[i+V[d][0]][j+V[d][1]][k+V[d][2]]!=100)//>=1.0)
    {
        MRb[i+V[d][0]][j+V[d][1]][k+V[d][2]]=temp;

        F_count++;

        tp.x=i+V[d][0];
        tp.y=j+V[d][1];
        tp.z=k+V[d][2];
        Q.enqueue(tp);
    }

    }
    //printf("\rMRb Q Size:%ld",Q.size());
    //printf("\rMRb:Updated points: %ld",F_count);
  }

 std::cout<<"Updated MR points for Bone  ::"<<F_count<<std::endl;
 //std::cout<<std::endl;

/*
  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
            if(abs(FCb[i][j][k]-FCv[i][j][k])<=10)
            {
                if(FDTb[i][j][k]<FDTv[i][j][k])
                    FCv[i][j][k]=0;
                else
                    FCb[i][j][k]=0;

            }
        }
*/


draw_image();

}




void MainWindow::on_checkBox_5_clicked()//MR
{
    draw_image();
}

void MainWindow::on_checkBox_6_clicked()
{
    load_seeds();
    draw_image();
}





void MainWindow::on_pushButton_5_clicked()//Vessel seed
{
    //vSeed *newPtr,*temp;


    FILE *fpvs;
    fpvs=fopen("vseed","a");
    fprintf(fpvs,"%d %d %d\n",curX,curY,curZ);
    fclose(fpvs);

}

void MainWindow::on_pushButton_6_clicked()//Bone seed
{
    //vSeed *temp;
    QString str="";

//    temp = (vSeed *) malloc(sizeof(vSeed));

    FILE *fpbs;
    fpbs=fopen("bseed","a");
    fprintf(fpbs,"%d %d %d\n",curX,curY,curZ);
    fclose(fpbs);

}

void MainWindow::on_pushButton_7_clicked()//Seperator
{
    FILE *fps;
    fps=fopen("sep","a");
    fprintf(fps,"%d %d %d\n",curX,curY,curZ);
    fclose(fps);

}

void MainWindow::on_checkBox_8_clicked()
{
    draw_image();
}

void MainWindow::on_pushButton_9_clicked()
{
    //VTKdisplay();
}


//void MainWindow::VTKdisplay()
//{

//    long cnt=0,index=0;

//      for(int i=0;i<Xdim;i++)
//      for(int j=0;j<Ydim;j++)
//          for(int k=0;k<Zdim;k++)
//          {
//            //if(vol[i][j][k]>250 && vol[i][j][k]<300)// && index<50)
//            if(MRv[i][j][k]<MRb[i][j][k]  && Flag!=5) cnt++;


//              if(out[i][j][k]==5 && Flag==5)  cnt++;
//              //if(NML[i][j][k]==3 && Flag==5)  cnt++;
//        }


//      std::cout<<"Cnt :"<<cnt<<std::endl;

//  vtkPoints *newPts = vtkPoints::New();
//  newPts->SetNumberOfPoints(cnt);
//   for(int i=0;i<Xdim;i++)
//      for(int j=0;j<Ydim;j++)
//          for(int k=0;k<Zdim;k++)
//          {
//            //if(vol[i][j][k]>250 && vol[i][j][k]<300)// && index<50)

//            if(MRv[i][j][k]<MRb[i][j][k]  && Flag!=5)
//             {
//                  newPts->SetPoint( index, i,j,k);
//                //std::cout<<"XYZ:"<<i<<"'"<<j<<"'"<<k<<std::endl;
//                index++;
//            }

//            if(out[i][j][k]==5 && Flag==5)
//            //if(NML[i][j][k]==3 && Flag==5)
//            {
//                  newPts->SetPoint( index, i,j,k);
//                //std::cout<<"XYZ:"<<i<<"'"<<j<<"'"<<k<<std::endl;
//                index++;
//            }
//        }


//  vtkCellArray *lines = vtkCellArray::New();

//  //cnt=50;
//  lines->EstimateSize(cnt,1);//10,10);
//  lines->InsertNextCell(cnt);//10);

//  for(int i=0;i<cnt;i++)
//      lines->InsertCellPoint(i);


//  vtkPolyData *pd = vtkPolyData::New();

//  pd->SetPoints(newPts);
//  pd->SetVerts(lines);
//  newPts->Delete();
//  lines->Delete();


//  //////////////new code
//  /*
//  polyData->Initialize();
//  polyData->SetPoints(points);

//  //Then add points  to the vtkPolyData
//  for(int i=0;i<numOfContours;i++){
//    double x=conters.getContour(i)->getX();
//    double y=1.0*i;
//    double z=contours->getContour(i)->getY();

//   points->InsertNextPoint(x,y,z)
//  }
//  */
///*
//  //Generate mesh rest if same as the example)

//  vtkSurfaceReconstructionFilter *surf=vtkSurfaceReconstructionFilter::New();
////  surf->SetInputConnection(newPts->GetOutputPort());
//  surf->SetInput(pd);  // <------ only different with the example

//  vtkContourFilter *cf=vtkContourFilter::New();
//  //cf->SetInputConnection(pd-> GetOutput());
//  cf->SetInput(pd);
//  cf->SetValue(0,0.0);

//  vtkReverseSense *reverse=vtkReverseSense::New();
//  //reverse->SetInputConnection(cf->GetOutputPort());
//  reverse->SetInput(cf);
//  reverse->ReverseCellsOn();
//  reverse->ReverseNormalsOn();

//  //vtkPolyDataMapper *map=vtkPolyDataMapper::New();
//*/



//  /////////////////////

//  vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
//  //mapper->SetInputConnection(reverse->GetOutputPort());
//  mapper->SetInput(pd);
//  mapper->ScalarVisibilityOff();



//  vtkActor *actor = vtkActor::New();
//  actor->SetMapper(mapper);

//  actor->GetProperty()->SetColor(0.75,0,0.0);
//  actor->GetProperty()->SetOpacity(0.25);
//  actor->GetProperty()->SetAmbient(0.2);
//  actor->GetProperty()->SetDiffuse(0.6);
//  actor->GetProperty()->SetSpecular(0.5);
//  actor->GetProperty()->SetSpecularPower(25.0);


//  vtkRenderer *ren = vtkRenderer::New();
//  ren->AddActor(actor);

//  vtkRenderWindow *renWin = vtkRenderWindow::New();
//  renWin->AddRenderer(ren);

//  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
//  iren->SetRenderWindow(renWin);

//  ren->SetBackground(1,1,1);
//  ren->GetActiveCamera()->SetPosition(1,1,1);
//  ren->GetActiveCamera()->SetFocalPoint(0,0,0);
//  ren->ResetCamera();

//  iren->Initialize();
///*
//  while(1)
//  {
//  actor->RotateY(0.1);
//  renWin->Render();
//  }
//*/
//  renWin->Render();

////  int retVal = vtkRegressionTestImage( renWin );
////  if ( retVal == vtkRegressionTester::DO_INTERACTOR)
//    {
//    iren->Start();
//    }

//  // Clean up
//  pd->Delete();
//  //utf->Delete();
//  mapper->Delete();
//  actor->Delete();
//  ren->Delete();
//  renWin->Delete();
//  iren->Delete();

// // return 0;//!retVal;

//}

void MainWindow::on_checkBox_9_clicked()
{
    draw_image();
}

void MainWindow::on_pushButton_12_clicked()
{
    QMessageBox nmsg;
    if(firstm>0)
    on_pushButton_14_clicked();

    findspathforreal(spherearr,Xdim,Ydim,Zdim);

    draw_image();

    allocate_memory();
   // on_pushButton_21_clicked();
   // on_pushButton_test_clicked();
    firstm=1;
}

void MainWindow::load_coreVessel()
{
    QMessageBox qmb;
    int cnt=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            for(int k=0;k<Zdim;k++){
                if(spherearr[i][j][k]==corevessel1 || spherearr[i][j][k]==corevessel2){
                    SS[i][j][k]= 1;
                    cnt=1;
                }
            }
        }
    }
    if(cnt==0){qmb.setText("Core Vessel Seeds not found\n"); qmb.exec();}
}

void MainWindow::load_coreVessel_file()
{
    QMessageBox qmb;
    int cnt=0;
    fstream fout;
    fout.open("vseedfile.txt",ios::in);
    if(!fout){
        cout<<"File Not Found...\n";
    }
    unsigned char val;
    for(int i=0;i<Zdim;i++){
        for(int j=0;j<Ydim;j++){
            for(int k=0;k<Xdim;k++){
                if(!fout.eof()){
                fout.read((char*)&spherearr[k][j][i],sizeof(char));
                if((short)(spherearr[k][j][i])==corevessel2|| (short)(spherearr[k][j][i])==corevessel1){

                    //cout<<(short)spherearr[k][j][i]<<" ";
                    SS[k][j][i]= 1;
                    cnt=1;
                }
            }
        }
    }
  }
    if(cnt==0){qmb.setText("Core Vessel Seeds not found\n"); qmb.exec();}
    fout.close();
}
void MainWindow::on_pushButton_21_clicked(){ //New Core Vessel

    short pixelValue ;
    int ta=ui->spinBox_ta->value();
    int tb=ui->spinBox_tb->value();
    int tap=ui->spinBox_tap->value();
    int tbp=ui->spinBox_tbp->value();

    GITN=0;
    Flag=5;

    for(int i=0;i<Xdim;i++)
        for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
               MRb[i][j][k]=MRv[i][j][k]=out[i][j][k]=0;

    load_seeds();

    load_coreVessel_file();

    if(GITN==0)
    {

 for(int i=0;i<Xdim;i++) //consider only vessel pixels
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
              pixelValue = vol[i][j][k];

              if(pixelValue <tap || pixelValue > tb)//if(spherearr[i][j][k]==corevessel1 || spherearr[i][j][k]==corevessel2)//
                FDTv[i][j][k]=0;
              else
                FDTv[i][j][k]=65000;


              if(i==0 ||j==0||k<=ui->spinBox_TA->value()//Ignore slices causing Tooth Artifacts
                  ||i==Xdim-1||j==Ydim-1||k==Zdim-1) FDTv[i][j][k]=0;

          }

   for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==2)//Bone
                  FDTv[i][j][k]=0;

 int mean=0,scnt=0;
    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==1)//Vessel seed
              {
                  mean+=vol[i][j][k];
                  scnt++;
              }


//    /*
//  FILE *fpvs;

//  fpvs=fopen("vseed","r");
//  if(fpvs != NULL){
//  while(!feof(fpvs))
//  {
//      int x,y,z;
//      fscanf(fpvs,"%d%d%d",&x,&y,&z);
//      mean+=vol[x][y][z];
//      scnt++;
//  }
//  fclose(fpvs);
// }
//  */

  mean=mean/scnt;
  ui->spinBox_ta->setValue(mean);

  ui->spinBox_tap->setValue(mean-175);
  ui->spinBox_tb->setValue(mean+175);


 Flag=5;
 ui->Plabel->setText("Computing FDT for Vessel... Iteration:");
 compute_FDT(FDTv);
on_pushButton_13_clicked();//NML

 ///////////////Only for Vessel FDT
 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              FDT[i][j][k]=FDTv[i][j][k];

 on_pushButton_3_clicked();//LM-LS



}


//core_vessel();

std::cout<<"GITN:"<<GITN<<std::endl;
GITN++;

erosion(FCv,1);
//core_vessel();


 for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
             if(FCv[i][j][k]>0)//if(spherearr[i][j][k]==corevessel1 || spherearr[i][j][k]==corevessel2)
                  out[i][j][k]=5;//write core vessel



 ///////////////////Redefine NML with selected erosion
/*
 ui->Plabel->setText("Re-computing NML:");
    int l=ui->spinBox_NML->value();
    int NMLCnt=0;
    for(int i=l;i<Xdim-l;i++)
    {
       for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim-l;k++)
          {
            if(NML[i][j][k] == 1)//NML voxel

          {

                   for(int x=-l;x<=l;x++)
                    for(int y=-l;y<=l;y++)
                      for(int z=-l;z<=l;z++)
                        {
                        if(FCv[i+x][j+y][k+z] >0)//Core Vessel
                       {
                            NML[i][j][k]=3;//NML
                            NMLCnt++;
                        }
                   }
            }
        }
       ui->progressBar->setValue((100*i)/(Xdim-2)+1);
   }
std::cout<<"NMLCnt:"<<NMLCnt<<std::endl;
/////////////////////////
//*/


}




void MainWindow::core_vessel()
{
    unsigned short min=0,temp,Sth,FCth;
    point tp;
    QQueue<point> VQ;

         //////

        for(int i=0;i<Xdim;i++)
        for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
               FCv[i][j][k]=FCb[i][j][k]=0;
        //////


   // if(GITN==0)

/*
  FILE *fpvs;
  fpvs=fopen("vseed","r");
  if(fpvs != NULL){
  while(!feof(fpvs))
  {
      int x,y,z;
      fscanf(fpvs,"%d%d%d",&x,&y,&z);
      tp.x=x;tp.y=y;tp.z=z;
      VQ.enqueue(tp);
      FCv[x][y][z]=100;
  }
  fclose(fpvs);
 }
*/
    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==1)//Vessel seed
              {
                  tp.x=i;tp.y=j;tp.z=k;
                  VQ.enqueue(tp);
                  FCv[i][j][k]=100;
              }



  ui->Plabel->setText("Computing F-C for Vessel...");
  //std::cout<<"Computing F-C for Vessel..."<<std::endl;

  //std::cout<<std::endl;

  //max=VQ.size();
//  std::cout<<"FC_Vessel: Q Size:"<<VQ.size()<<std::endl;

  //F_count=0;

  Sth=405;//162;

  FCth=2;
 /* while(!VQ.isEmpty())
  {
      tp=VQ.dequeue();
      int i=tp.x;int j=tp.y;int k=tp.z;
      if(i==0 ||j==0||k==0||i==Xdim-1||j==Ydim-1||k==Zdim-1) continue;

      min=FCv[i][j][k];
      for(int l=0;l<26;l++)
      {
        if (FDTv[i+V[l][0]][j+V[l][1]][k+V[l][2]] <=0) continue;//Modification threshold

        if(FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]]==100) continue;
        if(NML[i+V[l][0]][j+V[l][1]][k+V[l][2]]==1) continue;


        //if(FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]]==200) continue;


        //if(GITN>0 && (


                   unsigned long count=LM[i+V[l][0]][j+V[l][1]][k+V[l][2]];
                   int x=count%Xdim;
                   int z=count/(Xdim*Ydim);
                   int y=(count/Xdim)-(z*Ydim);

                   if(FDT[x][y][z]==0) continue;

                   temp=100*FDTv[i+V[l][0]][j+V[l][1]][k+V[l][2]]/FDT[x][y][z];


                     //temp=0.1*FDTv[i+V[l][0]][j+V[l][1]][k+V[l][2]];//using orifinal FDTv. Max FDTv=100.

                     temp=ceil(temp-0.5);
                     if(temp<min) min=temp;

                     if(k<Sth)
                         FCth=10;
                     else
                         FCth=0;

                     if(min>FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]])// && min>FCth)
                     {
                      FCv[i+V[l][0]][j+V[l][1]][k+V[l][2]]=min;
                      tp.x=i+V[l][0];
                      tp.y=j+V[l][1];
                      tp.z=k+V[l][2];
                      VQ.enqueue(tp);

                      //F_count++;
                     }
      }

      //if(VQ.size()>max) max=Q.size();
      //ui->progressBar->setValue(100*(1-(Q.size()/max)));
      //printf("\rFC_Vessel:Updated points: %ld",F_count);
      //std::cout<<"FC_Vessel:"<<"Q Size:"<<Q.size()<<std::endl;
      //fflush(stdout);

  }*/

  std::cout<<"End of Vessel FC"<<std::endl;
  //////////////////////
  draw_image();
  //////////////////////

  //std::cout<<"Updated FC points for Vessel::"<<F_countv<<std::endl;


  erosion(FCv,1);

}






void MainWindow::erosion(unsigned short ***Data, int l)
{
    unsigned short /*max=0,temp,*/Sth;
    int BG=0,eTH=0,voxcnt=0;
    unsigned long F_count=0/*,index*/;
    unsigned short ***copy;

    l=1;//ui->spinBox->value();

    Sth=Zdim;
    //if(GITN>0) Sth=405;//162;

    copy=Data;

    for(int i=l;i<Xdim-l;i++)
    {
      for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim-l;k++)
          //for(int k=l;k<Sth;k++)//Restricted Erosion
          {
            if(copy[i][j][k]<=0) continue;

       //     max=FDT[i-l][j-l][k-l];

            BG=0;

            /*//////////////////////////
                       long count=LM[i][j][k];
                       int xx=count%Xdim;
                       int zz=count/(Xdim*Ydim);
                       int yy=(count/Xdim)-(zz*Ydim);

                       if(FDT[xx][yy][zz]==0) continue;


                       //eTH=FDT[xx][yy][zz]*0.25;
            *//////////////////////////////
           // if(FDTv[i][j][k]<=100)
           //     eTH=0;
           // else
           //     eTH=FDTv[i][j][k]-100;

            //eTH=copy[i][j][k]*.5;///Key Thresold for selection
            eTH=ui->spinBox->value();




            voxcnt=0;
      for(int x=-l;x<=l;x++)
            {
          for(int y=-l;y<=l;y++)
          {
              for(int z=-l;z<=l;z++)

                 {
      //               temp=FDT[i+V[l][0]][j+V[l][1]][k+V[l][2]];
     //                  if(x==-l && y==-l && z==-l) continue;
                       //if(x==0 && y==0 && z==0) continue;

                      if (FDTv[i+x][j+y][k+z] <=eTH) {BG=1; break;}//Subhadip comment for Pigl
                 //if (copy[i+x][j+y][k+z] <=eTH) voxcnt++;//{BG=1; break;}

         //        if(copy[i+x][j+y][k+z] <130
         //        if (copy[i+x][j+y][k+z] <=eTH) voxcnt++;//{BG=1; break;}

/*

                       temp=100*FDTv[i+x][j+y][k+z]/FDT[xx][yy][zz];
                       if(temp<=25)
                       {
                           BG=1; break;
                       }
*/
                       //if(FDT[i+x][j+y][k+z]<=50) {BG=1; break;}



                 }
              if (BG==1) break;
            }
          if (BG==1) break;
         }
               if(BG==1 && SS[i][j][k]!=1)//Not to erode a vessel seed
                //if(voxcnt>0)//.75 voxel erosion
               {
                   Data[i][j][k]=200;
                   //LM[i][j][k]=++F_count;//maxInt;
                   F_count++;
               }

    }
      ui->progressBar->setValue((100*i)/(Xdim-2)+1);
    }


    for(int i=l;i<Xdim-l;i++)
       for(int j=l;j<Ydim-l;j++)
          //for(int k=l;k<Zdim-l;k++)
           for(int k=l;k<Sth;k++)//Restricted Erosion
              if(Data[i][j][k]==200)
              {
                  //  SS[i][j][k]=3;
                  FDTv[i][j][k]=0;
                  Data[i][j][k]=0;
              }

std::cout<<"Eroded points:"<<F_count<<std::endl;


}


void MainWindow::on_pushButton_19_clicked()//Erode PV
{
    unsigned short /*max=0,temp,*/Sth;
    int BG=0,eTH=0,voxcnt=0;
    unsigned long F_count=0/*,index*/;
    short ***copy;

    int l=1;//ui->spinBox->value();

    copy=vol;

    for(int i=l;i<Xdim-l;i++)
    {
      for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim-l;k++)
          {
            if(vol[i][j][k]>1200)
            {
                copy[i][j][k]=1500;
                continue;
            }

            if(vol[i][j][k]<300)
            {
                copy[i][j][k]=20;
                continue;
            }


             BG=0;

      for(int x=-l;x<=l;x++)
            {
          for(int y=-l;y<=l;y++)
          {
              for(int z=-l;z<=l;z++)

                 {

                      if (vol[i+x][j+y][k+z] <300 || vol[i+x][j+y][k+z]>1200)
                      {
                          //if(copy[i][j][k]!=500)
                          {
                              //copy[i][j][k]=20;//eroded pixels
                              BG=1;
                              F_count++;
                          }
                          break;
                      }

                 }
              if (BG==1) break;
            }
          if (BG==1) break;
         }
               if(BG==0)//erosion
               {
                   copy[i][j][k]=500;

               }

    }
      ui->progressBar->setValue((100*i)/(Xdim-2)+1);
    }



    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
            if (i<l||i>=Xdim-l||j<l||j>=Ydim-l||k<l||k>=Zdim-l) {
            copy[i][j][k]=20;
            continue;
            }

              if (copy[i][j][k] >=300 && copy[i][j][k]<=1200)
              {
                  if(copy[i][j][k]!=500)
                    {
                      copy[i][j][k]=20;//eroded pixels
                      F_count++;
                    }
              }

              /*

              if(vol[i][j][k]<300)
              {
                  vol[i][j][k]=20;
              }
              else if( vol[i][j][k]<1200)
            {
                vol[i][j][k]=500;
            }
              else
                  vol[i][j][k]=1500;
            */

           }

    std::cout<<"Eroded points:"<<F_count<<std::endl;

    vol=copy;


  draw_image();





}

void MainWindow::on_pushButton_10_clicked()
{
    erosion(FCv,1);
    draw_image();
}

void MainWindow::dialation(unsigned short ***Data, int l)
{
    unsigned short /*max=0,temp,*/Sth;
    int BG=0;
    long F_count=0/*,index*/;
    unsigned short ***copy;

    copy=Data;

    //l=ui->spinBox->value();


    Sth=Zdim;//405;//162;
    l=2;

    for(int i=l;i<Xdim-l;i++)
    {
      for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim;k++)
          {


          //if(copy[i][j][k]==0) continue;
          if(copy[i][j][k]!=5) continue;//Compute only for CV points

       //     max=FDT[i-l][j-l][k-l];75

            BG=0;
      for(int x=-l;x<=l;x++)
            {
          for(int y=-l;y<=l;y++)
          {
              for(int z=-l;z<=l;z++)

                 {
      //               temp=FDT[i+V[l][0]][j+V[l][1]][k+V[l][2]];
     //                  if(x==-l && y==-l && z==-l) continue;
                       if(x==0 && y==0 && z==0) continue;

                       //if(Data[i+x][j+y][k+z]==2) continue;

                       if(copy[i+x][j+y][k+z]<5)// || copy[i+x][j+y][k+z]==200)
                       {
                           Data[i][j][k]=6;
                           BG=1;
                           F_count++;
                           break;


                       }


                 }
              if(BG==1) break;

            }
          if(BG==1) break;

         }

    }
      ui->progressBar->setValue((100*i)/(Xdim-2)+1);
}


std::cout<<"Dialated points:"<<F_count<<std::endl;


}

void MainWindow::on_pushButton_11_clicked()
{
    dialation(out,2);
    draw_image();
}



void MainWindow::on_pushButton_13_clicked()//Bone NML
{
    //if(ui->checkBox_10->isChecked()==TRUE) return;

    //ui->checkBox_10->setCheckState(Qt::Checked);

    int l=ui->spinBox_NML->value();
    //int Sth=405;//162;
    ui->Plabel->setText("Computing NML:");
    for(int i=l;i<Xdim-l;i++)
    {
       for(int j=l;j<Ydim-l;j++)
          for(int k=l;k<Zdim-l;k++)
          {
            if(vol[i][j][k] >= ui->spinBox_tb->value())//pure bone
                //|| vol[i][j][k] <0)//air
          {

                   for(int x=-l;x<=l;x++)
                    for(int y=-l;y<=l;y++)
                      for(int z=-l;z<=l;z++)
                        {
                        if(vol[i+x][j+y][k+z] <= ui->spinBox_tb->value()
                            && vol[i+x][j+y][k+z] >= ui->spinBox_tap->value())
                        {
                            NML[i+x][j+y][k+z]=1;//NML
                            FDTv[i+x][j+y][k+z]=0;
                        }
                   }
            }
        }
       ui->progressBar->setValue((100*i)/(Xdim-2)+1);
   }
}

void MainWindow::on_pushButton_15_clicked()
{
    fstream outf;
    unsigned short value;

   // outf.open(imagefile2,ios::out  | ios :: binary);
    for(int i = 0 ;i < Zdim ; i++ ){
        for(int j=0;j<Ydim;j++){
            for(int k=0;k<Xdim;k++){
                 bool change=false;
//                if(FCv[k][j][i]>FCb[k][j][i]){
//                    vol[k][j][i] = 50;change=true;
//                //    outf.write((char*)&value,sizeof(thetype));
//                 }
//                else if (FCv[k][j][i]<FCb[k][j][i])
//                 {
//                     vol[k][j][i] = 60;change=true;
//                  //  value=60; outf.write((char*)&value,sizeof(thetype));
//                 }
                if(MRv[k][j][i]<MRb[k][j][i]){
                     vol[k][j][i] = 70;change=true;
                        //  value=60; outf.write((char*)&value,sizeof(thetype));
                     }
                 if(change == false) vol[k][j][i]=0;
                //else  outf.write((char*)&vol[k][j][i],sizeof(thetype));
            }
        }
    }
   // outf.close();
    modifyimagegui(vol);
}

//void MainWindow::on_pushButton_15_clicked()//WRITE IMAGE
//{
//    short value;
//    char name[200];

//  const unsigned int Dimension = 3;

//  //typedef itk::Image< OutputPixelType, Dimension > ImageType;
//  ImageType::Pointer image = ImageType::New();

//  ImageType::SizeType size;
//  size[0]=Xdim;
//  size[1]=Ydim;
//  size[2]=Zdim;


//  ImageType::IndexType start;
//  start[0]=0;
//  start[1]=0;
//  start[2]=0;

//  ImageType::SpacingType spacing;
//  spacing[0]=sp[0];
//  spacing[1]=sp[1];
//  spacing[2]=sp[2];

//  ImageType::RegionType region;
//  region.SetSize(size);
//  region.SetIndex(start);

//  image->SetSpacing(spacing);
//  image->SetRegions(region);
//  image->Allocate();


//  ImageType::IndexType index;


//    for (int row=0; row<Xdim;row++)
//    {
//        for (int column=0;column<Ydim;column++)
//        {
//            for (int slice=0;slice<Zdim;slice++)
//            {
//                index[0] = row;
//                index[1] = column;
//                index[2] = slice;




///*
//                //Makeshift code for testing
//                value=vol[row][column][slice];
//                if(value>ui->spinBox_tb->value() && value<=ui->spinBox_tbp->value())
//                    value=90;
//                else
//                    if(value>=ui->spinBox_tap->value() && value<=ui->spinBox_ta->value())
//                            value=10;
//                    else
//                        value=0;
//*/


///*
//                //ORIGINAL CODE
//                value=out[row][column][slice];

//                if(ui->checkBox_B->isChecked()==FALSE && value ==2)//Donot Create Image with Bone
//                    value=0;
//*/
///*

//                if(Flag==5)
//                {
//                    value=FCv[row][column][slice];

//                    if(FCv[row][column][slice]>0 && slice<=50) value=100;
//                    else
//                    if(FCv[row][column][slice]>0 && slice>50) value=300;
//                    else
//                    value=0;

//                }
//                else
//                    value=MRv[row][column][slice]<MRb[row][column][slice]?100:0;
//*/

//                    //value=MRv[row][column][slice]<MRb[row][column][slice]?0:50;





//                //subhadip for pigl MSTMO

//                value=0;
//                if(MRv[row][column][slice]<MRb[row][column][slice])
//                     {
//                         //int v= (-MRv[curX][i][j]*2.55);
//                         //imagex.setPixel(i,j, qRgb(200,0,0));
//                         value=50;//vessel
//                     }
//                    else
//                        if(MRv[row][column][slice]>MRb[row][column][slice])
//                     {
//                        //int b= (-MRb[curX][i][j]*2.55);
//                         //imagex.setPixel(i,j, qRgb(0,200,0));
//                        value=100;//bone
//                     }


////                 value=vol[row][column][slice];//subhadip for pigl PV erosion

//                image->SetPixel(index,value);
//            }
//        }
//         ui->progressBar->setValue((100*row)/(Xdim-2)+1);
//    }
//    image->Update();


//  typedef itk::ImageFileWriter< ImageType >  WriterType;
//  WriterType::Pointer      writer =  WriterType::New();
//  /*
//  if(Flag==5)
//      sprintf(name,"/home/subhadip/Test3/Results/%s/CV%s_%dx%dx%d.hdr",DataID,DataID,Xdim,Ydim,Zdim);
//  else
//      sprintf(name,"/home/subhadip/Test3/Results/%s/OUT%s_%dx%dx%d.hdr",DataID,DataID,Xdim,Ydim,Zdim);
// */
//  sprintf(name,"testout.hdr");
//  writer->SetFileName(name);
//  writer->SetInput( image );
//  writer->Update();

//}

void MainWindow::on_checkBox_11_clicked()
{
    draw_image();

}

void MainWindow::on_pushButton_16_clicked()
{
    load_GT();
    draw_image();

}

//void MainWindow :: on_pushButton_18_clicked(){

//}
int image_count=0;
void MainWindow::on_pushButton_18_clicked()
{
    int TP=0, TN=0, FP=0, FN=0;

    load_GT();

    for (int row=0; row<Xdim;row++)
    {
        for (int column=0;column<Ydim;column++)
        {
            for (int slice=0;slice<Zdim;slice++)
            {
                short value=vol[row][column][slice];

                if(GT[row][column][slice]==1)
                {
                    if(value==70 || value>=5)
                        TP++;
                    else
                        FN++;
                }
                else
                if(GT[row][column][slice]==2)
                {
                    if(value<=2)
                        TN++;
                    else
                        FP++;
                }

//                if (value > 0  && GT[row][column][slice]==1) TP++;
//               if (value > 0  && GT[row][column][slice]==2) FP++;
//                if (value == 0 && GT[row][column][slice]==1) FN++;
//                if (value == 0 && GT[row][column][slice]==2) TN++;

            }
        }
    }

    std::cout<<"TP:"<<TP<<std::endl;
    std::cout<<"TN:"<<TN<<std::endl;
    std::cout<<"FP:"<<FP<<std::endl;
    std::cout<<"FN:"<<FN<<std::endl;

    ui->acc->setNum((float)((TP+TN)*100)/(TP+FP+TN+FN));

 //   vol is the old_mso data loaded when load_user2 button is clicked
 //   out is the new mso data loaded when load_user1 button is clicked

    if(image_count==2){
     TP=0, TN=0, FP=0, FN=0;
    for(int i=0;i<Xdim;i++){
        for(int j=0;j<Ydim;j++){
            for(int k=0;k<Zdim;k++){
//                if((vol[i][j][k]==8 || vol[i][j][k]==11) && out[i][j][k]==70) TP++;
//                else if((vol[i][j][k]!=8|| vol[i][j][k]!=11) && out[i][j][k]==70) FP++;
//                else  if((vol[i][j][k]==8|| vol[i][j][k]==11) && out[i][j][k]==0) FN++;   //for 2005
//                else  if((vol[i][j][k]!=8 || vol[i][j][k]!=11) && out[i][j][k]==0) TN++;
                if((vol[i][j][k]>=7) && out[i][j][k]==70) TP++;
                else if((vol[i][j][k]==0) && out[i][j][k]==70) FP++;
                else  if((vol[i][j][k]>=7) && out[i][j][k]==0) FN++;    //for 1016
                else  if((vol[i][j][k]==0) && out[i][j][k]==0) TN++;
            }
        }
    }
    cout<<"True Positive : "<<TP<<"\n";
    cout<<"False Positive : "<<FP<<"\n";
    cout<<"True Negetive : "<<TN<<"\n";
    cout<<"false Negetive : "<<FN<<"\n";
    cout<<"Accuracy wrt old data: "<<(((float)((TP+TN))/(TP+FP+TN+FN))*100)<<"\n";
    }

}
string getfilename(string filename){
    string names="";
    int first = filename.find(".");
    int l=first-1;
    while(l>=0 && filename[l]!= '/' ){
        names=filename[l]+names;
        l--;
    }
    return names;
}

int compare=0;
void MainWindow::on_pushButton_LU1_clicked()
{
    string filenames;
    QString temp;
    image_count++;
    if(compare==0){
        allocate_memorycore();
        compare=1;
    }
    temp=QFileDialog::getOpenFileName(this,tr("Open File"), "/local/vol00/basu/Work/CTA-Data01",
                                      tr("Analyze Images (*.hdr)"));

    QByteArray qb=temp.toLatin1();
    filename =  qb.data();
    cout <<"File name : "<<filenames;
    ui->label_19->setText(filename);
    filenames = getfilename((string)filename);

    filenames = filenames +".img";
    cout <<"File name : "<<filenames;
    histogram();//Main function to load image and populate histogram data
    readimage_specific(vol,filenames);
    //OUT matrix is used to load one user's data in reproducability analysis
    out  = (unsigned short ***)calloc(sizeof(unsigned short **) , Xdim);
    for (int i = 0 ;  i < Xdim; i++) {
    out[i] = (unsigned short **)calloc(sizeof(unsigned short *) , Ydim);
    for (int j = 0; j < Ydim; j++){
      out[i][j] = (unsigned short *)calloc(sizeof(unsigned short) , Zdim);
    }
    }

  for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
                out[i][j][k]=vol[i][j][k];//OUT matrix is used to load one user's dat  a

  cout<<"Image One Loaded....\n";
}



void MainWindow::on_pushButton_LU2_clicked()
{
    QString temp;
        image_count++;
    if(compare==0){
        allocate_memorycore();
        compare=1;
    }
    temp=QFileDialog::getOpenFileName(this,tr("Open File"), "/local/vol00/basu/Work/CTA-Data01",
                                      tr("Analyze Images (*.hdr)"));

    QByteArray qb=temp.toLatin1();
    filename =  qb.data();

    ui->label_19->setText(filename);

    string filenames = getfilename((string)filename);
    filenames = filenames +".img";
    cout<<"File name of user 2 : "<<filenames<<"\n";

    histogram();//Main function to load image and populate histogram data
    readimage_specific(vol,filenames);
    cout<<"Image Two Loaded....\n";

}

void MainWindow::on_pushButton_EstAgr_clicked()
{
    long I=0,U=0;

    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
            if(out[i][j][k]==70 && vol[i][j][k]==8) I++;//both agree on vessel:Intersection
            if(out[i][j][k]==70 || vol[i][j][k]==8) U++;//Either of them agree:Union
          }

    std::cout<<"Intersection:"<<I<<std::endl;
    std::cout<<"Union:"<<U<<std::endl;
    ui->label_Agr->setNum((float)I/U);
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    //hmax=hmax/ui->verticalSlider->value();//FORCED
    on_pushButton_clicked();
    //std::cout<<"Hmax Normalization:"<<ui->verticalSlider->value()<<std::endl;
}


void MainWindow::on_pushButton_EvPh_clicked()//evaluate phantom
{
        int TP=0, TN=0, FP=0, FN=0,VS=0, ex=0;//, FPf=0, FNf=0;

    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
              if(SS[i][j][k]==1)//Vessel seed
              {
                  VS=vol[i][j][k];
                  break;
              }


    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=0;k<Zdim;k++)
          {
              if(MRv[i][j][k]<MRb[i][j][k] && vol[i][j][k]==VS)//Vessel pure intensity is 30
                  TP++;
              if(MRv[i][j][k]<MRb[i][j][k] && vol[i][j][k]>VS)//Vessel
                  FP++;

              if(MRb[i][j][k]<MRv[i][j][k] && vol[i][j][k]>VS)//Bone
                  TN++;
              if(MRb[i][j][k]<MRv[i][j][k] && vol[i][j][k]==VS)//Bone
                  FN++;

              if(MRb[i][j][k]==MRv[i][j][k] && MRb[i][j][k]!=0)
                  ex++;

          }

/*
    for(int i=0;i<Xdim;i++)
      for(int j=0;j<Ydim;j++)
          for(int k=Zdim/2;k<Zdim;k++)
          {
              if(MRv[i][j][k]<MRb[i][j][k] && vol[i][j][k]==VS)//Vessel
                  TP++;
              if(MRv[i][j][k]<MRb[i][j][k] && vol[i][j][k]>VS)//Vessel
                  FPl++;

              if(MRb[i][j][k]<MRv[i][j][k] && vol[i][j][k]>VS)//Bone
                  TN++;
              if(MRb[i][j][k]<MRv[i][j][k] && vol[i][j][k]==VS)//Bone
                  FNl++;
          }
          */

    std::cout<<"Equal points:"<<ex<<std::endl;
    std::cout<<"True Positive percentage:"<<((float)((TP+TN)*100)/(TP+FP+TN+FN+ex))<<std::endl;
    std::cout<<"False Negative percentage:"<<((float)((FP+FN)*100)/(TP+FP+TN+FN+ex))<<std::endl;

//    ui->acc->setNum((float)((TP+TN)*100)/(TP+FP+TN+FN));

}
