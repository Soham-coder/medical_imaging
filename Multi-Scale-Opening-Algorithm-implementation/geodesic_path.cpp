
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <string.h>

#include "global.h"

//#include "sp.h"

//using namespace std;


#include<iostream>
#include<fstream>
#include<list>
#include<cmath>
#include<map>
#include<ctime>
#include<algorithm>


#define intenl 1
#define dividendop 1
#define intenu 450
#define fixedmodinten 200
#define threshold 0.01
#define INT_MAX 999999
#define imagefile "newAnalyze2005_130-450_dt.img"//"OUTbinarydt_240x240x240.img"//"newAnalyze2005_130-450_dt.img"//"RFDT.img"//"dtbinary1016.img"////"OUT1_1016_512x512x406.img"
#define imagefile2 "newAnalyze2005_130-450_dt1.img"//"OUTbinarydt_240x240x2401.img"//"newAnalyze2005_130-450_dt1.img"//"RFDT1.img"// "dtbinary10161.img"//OUT1_1016_512x512x406.img"
#define thetype   unsigned  short //unsigned char//
#define no 1
#define PI 3.14159265
#define multiplier 100
#define precision 0.65
#define spinten 200
#define roundfact 20
void readimage(short***,char***);
/*#define height 77//190//406
#define width  22//200//512
#define depth  11//100//512
#define intenl 1
#define intenu 450
#define modifiedinten 90
#define INT_MAX 9999
#define imagefile "ACA_dt_11x22x77_char.img"
#define imagefile2 "ACA_dt_11x22x77_char2.img"
#define thetype char //unsigned short
#define no 2
#define multiplier 100
*/
int multp=0;
int modifiedinten=fixedmodinten;
int sphereintensity=spinten ;
using namespace std;
int height;
int width ;
int depth;
 fstream fout;

long long int imagedimension = height*width*depth;

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

        Size=imagedimension;
        free(array);
        array=new minheapnode[Size];   //allocating memory for the minheap

    }
    ~minheap(){free(array);array=NULL;}
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

   cout<<xc<<" "<<yc<<" "<<zc<<" : radius : "<<r<<"\n";
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

/*****************************************************NEW SPHERE CODE*******************************************************/
 int xendp,yendp,zendp,xendn,yendn,zendn;
// void detec_xend(short*** arr,short xs,short ys, short zs)
// {
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int x=xs;x<height;x++)
//     {
//         if(arr[x][ys][zs]>max)
//         {
//             max=arr[x][ys][zs];
//         }
//         else if(arr[x][ys][zs]<min)
//         {
//             min=arr[x][ys][zs];
//         }
//         int dif = abs(max-min);
//         if(arr[x][ys][zs]<=0) {xendp=x-1;break;}
//         if((1/dif)<threshold)
//         {
//             xendp=x-1;
//             break;
//         }
//     }
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int x=xs;x>=0;x--)
//     {
//         if(arr[x][ys][zs]>max)
//         {
//             max=arr[x][ys][zs];
//         }
//         else if(arr[x][ys][zs]<min)
//         {
//             min=arr[x][ys][zs];
//         }
//         int dif = abs(max-min);
//         if(arr[x][ys][zs]<=0) {xendp=x+1;break;}
//         if((1/dif)<threshold)
//         {
//             xendn=x+1;
//             break;
//         }
//     }

// }
// void detec_yend(short*** arr,short xs,short ys, short zs)
// {
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int y=ys;y<height;y++)
//     {
//         if(arr[xs][y][zs]>max)
//         {
//             max=arr[xs][y][zs];
//         }
//         else if(arr[xs][y][zs]<min)
//         {
//             min=arr[xs][y][zs];
//         }
//         int dif = abs(max-min);
//         if(arr[xs][y][zs]<=0) {yendp=y-1;break;}
//         if((1/dif)<threshold)
//         {
//             yendp=y-1;
//             break;
//         }
//     }
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int y=ys;y>=0;y--)
//     {
//         if(arr[xs][y][zs]>max)
//         {
//             max=arr[xs][y][zs];
//         }
//         else if(arr[xs][y][zs]<min)
//         {
//             min=arr[xs][y][zs];
//         }
//         int dif = abs(max-min);
//         if(arr[xs][y][zs]<=0) {yendn=y+1;break;}
//         if((1/dif)<threshold)
//         {
//             yendn=y+1;
//             break;
//         }
//     }

// }
// void detec_zend(short*** arr,short xs,short ys, short zs)
// {
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int z=zs;z<height;z++)
//     {
//         if(arr[xs][ys][z]>max)
//         {
//             max=arr[xs][ys][z];
//         }
//         else if(arr[xs][ys][z]<min)
//         {
//             min=arr[xs][ys][z];
//         }
//         int dif = abs(max-min);
//         if(arr[xs][ys][z]<=0) {zendp=z-1;break;}
//         if((1/dif)<threshold)
//         {
//             zendp=z-1;
//             break;
//         }
//     }
//     int max=arr[xs][ys][zs];int min = arr[xs][ys][zs];
//     for(int z=zs;z>=0;z--)
//     {
//         if(arr[xs][ys][z]>max)
//         {
//             max=arr[xs][ys][z];
//         }
//         else if(arr[xs][ys][z]<min)
//         {
//             min=arr[xs][ys][z];
//         }
//         int dif = abs(max-min);
//         if(arr[xs][ys][z]<=0) {zendn=z+1;break;}
//         if((1/dif)<threshold)
//         {
//             zendn=z+1;
//             break;
//         }
//     }
// }

// void detect_end(short*** arr,short xs,short ys, short zs)
// {
//     detec_xend(arr,xs,ys,zs);
//     detec_yend(arr,xs,ys,zs);
//     detec_zend(arr,xs,ys,zs);
// }

/***************************************************************************************************************************/


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
    int i,j,k;
    for( i=0;i<height;i++)
    {
        for( j=0;j<width;j++)
        {
            for(k=0;k<depth;k++)
            {
                if(arr[i][j][k]>=sphereintensity)
                spherearr[i][j][k]=5;
            }
        }
    }
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
      distmap.erase(inten);
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
                   //  fmsg.setText("S "+xst+" "+yst+" "+zst);
                  //   fmsg.exec();
                     xst.setNum(xe);
                     yst.setNum(ye);
            zst.setNum(ze);
                 //   fmsg.setText("E "+xst+" "+yst+" "+zst);
                   // fmsg.exec();
   // fmsg.setText("Entering Read And Modify Image ");
    //fmsg.exec();
    }

    short d=2;
    int haha=1;
   cout<<"modified Inten  : "<<modifiedinten<<"\n";
   short x=xe;short y=ye; short z=ze;
    int ecnt=0;
    int pint;
  //  sfp.read((char*)&sintensitymap,sizeof(map<long long int , short>));

    while(1)
    {
      //  sfp.open("Spherefileinten.txt",ios::in);
      //  sfp.read((char*)&sintensitymap,sizeof(map<long long int , short>));

        if(arr[x][y][z]>=fixedmodinten  && arr[x][y][z]==modifiedinten)
        {

            visited[arr[x][y][z]-fixedmodinten]=true;
            QMessageBox qm;
           QString rads;
           d = spherearr[x][y][z];
           sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
          // spherearr[x][y][z]=1;
         //  d=intensitymap[calculateindex(x,y,z)];
            pint = d;
            d=round(d);
            if(d<=0 || d>9)
            {
                d=2;
            }
           // d=4;//omitt this one for actual artery

          //  intensitymap = drawspherex(arr,intenlist2,d,x,y,z);
             intensitymap = drawsphere(arr,intenlist2,d,x,y,z);

        }

        else if(arr[x][y][z]>=fixedmodinten  && arr[x][y][z]!=modifiedinten && visited[arr[x][y][z]-fixedmodinten] == false)
        {
           // cout<<"Storing in greater map : "<<x<<" "<<y<<" "<<z<< " : \n";
            //readf<<"Adding edge : "<<arr[x][y][z]-fixedmodinten<<","<<modifiedinten-fixedmodinten<<"\n";
           // cout<<"Adding edge : "<<arr[x][y][z]-fixedmodinten<<","<<modifiedinten-fixedmodinten<<"\n";
            QMessageBox qm;
            QString rads;

            cg.addedge(arr[x][y][z]-fixedmodinten,modifiedinten-fixedmodinten);
            visited[arr[x][y][z]-fixedmodinten]=true;
            d =spherearr[x][y][z];
            sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
       //     d=intensitymap[calculateindex(x,y,z)];
            pint = d;
            d=round(d);

            if(d<=0 || d>9)
            {

                d=2;
            }

          //  qm.setText("second if "+rads.setNum(d)+" "+rads.setNum(d));
          //  if(modifiedinten>238)
            //    cout<<"second if "<<x<<" "<<y<<" "<<z<<" "<<d<<" "<<pint<<"\n";
           // qm.exec();
         //   d=4;//omitt this one for actual artery

         //    intensitymap = drawspherex(arr,intenlist2,d,x,y,z);
            intensitymap = drawsphere(arr,intenlist2,d,x,y,z);
        }

        else if(arr[x][y][z]<fixedmodinten )
        {
            QMessageBox qm;
            QString rads;
       // cout<<"Storing in intensityMap : "<<x<<" "<<y<<" "<<z<< " : \n";
            sfp<<x<<" "<<y<<" "<<z<<" "<<"\n";
        d=round(arr[x][y][z]);
        pint = arr[x][y][z];
        //d=10;
       // d=intensitymap[calculateindex(x,y,z)];
        if(d<=0 || d>9)
        {
            d=2;
        }
      //  qm.setText("normal if "+rads.setNum(d)+" "+rads.setNum(pint));
      //  if(modifiedinten>238)
         //      cout<<"normal if "<<x<<" "<<y<<" "<<z<<" "<<d<<" "<<pint<<"\n";
        //qm.exec();

        intensitymap.insert(std :: make_pair(calculateindex(x,y,z),arr[x][y][z]));
       //  intensitymap = drawspherex(arr,intenlist2,d,x,y,z);
         intensitymap = drawsphere(arr,intenlist2,d,x,y,z);
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
   //
    }

    pathmap.insert(std::make_pair(modifiedinten-fixedmodinten,intenlist));
  //  sfp.close();
    outfile.close();
    infile.close();
    if(modifiedinten>=205){
  //   fmsg.setText("Read And Modify Operation COmplete");
    // fmsg.exec();
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
    cout<<"Entering ReadImage\n";
    long long int zncnt=0;
    fstream infile;
    QMessageBox rmsg;
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
                    spherearr[i][j][k]=(char)(arr[i][j][k]);
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
      cout<<"Image File read:......\n";
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

    spathfind spthobj;
   // map<long long int , short> intensitymap;
    ifstream fin;
    fstream fout;
    fin.open("ShortestPath", ios :: in);
    float weight;
  //  fout.open("IntensityFile", ios :: in | ios :: out);
  //  fout.read((char*)&intensitymap,sizeof(intensitymap));
    fin.read((char*)&maps,sizeof(maps));
    fin.close();

        {
            if(x<height && y-1>=0 && z<depth  && arr[x][y-1][z]>=intenl &&arr[x][y-1][z]<=intenu && mhp.isinminheap(pos,calculateindex(x,y-1,z)) && dist[x][y][z]!=INT_MAX)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y-1,z);
                if(dist[x][y][z]+weight<dist[x][y-1][z])
                {

                dist[x][y-1][z]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y-1,z,dist[x][y-1][z]);
                spthobj=spathfind(arr[x][y-1][z],x,y,z,x,y-1,z);

                if(maps.count(calculateindex(x,y-1,z))==1)
                {
                    maps.erase(calculateindex(x,y-1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x,y-1,z),spthobj));
                }
            }
        }


        {
            if(x<height && y+1<width && z<depth  && mhp.isinminheap(pos,calculateindex(x,y+1,z)) && arr[x][y+1][z]>=intenl &&arr[x][y+1][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y+1,z);
                if(dist[x][y][z]+weight<dist[x][y+1][z])
                {

                dist[x][y+1][z]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y+1,z,dist[x][y+1][z]);
                spthobj=spathfind(arr[x][y+1][z],x,y,z,x,y+1,z);
                 if(maps.count(calculateindex(x,y+1,z))==1)
                {
                    maps.erase(calculateindex(x,y+1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x,y+1,z),spthobj));
                }
            }
        }

        {
            if(x<height && y<width && z+1<depth&&mhp.isinminheap(pos,calculateindex(x,y,1+z))&& arr[x][y][z+1]>=intenl && arr[x][y][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y,z+1);
                if(dist[x][y][z]+weight<dist[x][y][z+1])
                {

                dist[x][y][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y,z+1,dist[x][y][z+1]);
                spthobj=spathfind(arr[x][y][z+1],x,y,z,x,y,z+1);
                 if(maps.count(calculateindex(x,y,z+1))==1)
                {
                    maps.erase(calculateindex(x,y,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y,z+1),spthobj));
                }
            }
        }

        {
            if(x<height && y<width && z-1>=0 &&mhp.isinminheap(pos,calculateindex(x,y,z-1))&& arr[x][y][z-1]>=intenl && arr[x][y][z-1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y,z-1);
                if(dist[x][y][z]+weight<dist[x][y][z-1]){
                dist[x][y][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y,z-1,dist[x][y][z-1]);
                spthobj=spathfind(arr[x][y][z-1],x,y,z,x,y,z-1);
                 if(maps.count(calculateindex(x,y,z-1))==1)
                {
                    maps.erase(calculateindex(x,y,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y,z-1),spthobj));}

            }
        }


        {
            if(x<height && y+1<width && z+1<depth  &&mhp.isinminheap(pos,calculateindex(x,y+1,z+1))&& arr[x][y+1][z+1]>=intenl &&arr[x][y+1][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y+1,z+1);
                if(dist[x][y][z]+weight<dist[x][y+1][z+1]){
                dist[x][y+1][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y+1,z+1,dist[x][y+1][z+1]);
                spthobj=spathfind(arr[x][y+1][z+1],x,y,z,x,y+1,z+1);
                   if(maps.count(calculateindex(x,y+1,z+1))==1)
                {
                    maps.erase(calculateindex(x,y+1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y+1,z+1),spthobj));}

            }
        }


        {
            if(x<height && y-1>=0 && z+1<depth  &&mhp.isinminheap(pos,calculateindex(x,y-1,z+1))&&arr[x][y-1][z+1]>=intenl &&arr[x][y-1][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y-1,z+1);
                if(dist[x][y][z]+weight<dist[x][y-1][z+1]){
                dist[x][y-1][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y-1,z+1,dist[x][y-1][z+1]);
                spthobj=spathfind(arr[x][y-1][z+1],x,y,z,x,y-1,z+1);
                if(maps.count(calculateindex(x,y-1,z+1))==1)
                {
                    maps.erase(calculateindex(x,y-1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y-1,z+1),spthobj));}

            }
        }


        {
            if(x<height && y+1<depth && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x,y+1,z-1))&&arr[x][y+1][z-1]>=intenl && arr[x][y+1][z-1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y+1,z-1);
                if(dist[x][y][z]+weight<dist[x][y+1][z-1]){
                dist[x][y+1][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y+1,z-1,dist[x][y+1][z-1]);
                spthobj=spathfind(arr[x][y+1][z-1],x,y,z,x,y+1,z-1);
                 if(maps.count(calculateindex(x,y+1,z-1))==1)
                {
                    maps.erase(calculateindex(x,y+1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y+1,z-1),spthobj));}

            }
        }

        {
            if(x<height && y-1>=0 && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x,y-1,z-1))&& arr[x][y-1][z-1]>=intenl && arr[x][y-1][z-1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x,y-1,z-1);
                if(dist[x][y][z]+weight<dist[x][y-1][z-1]){
                dist[x][y-1][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x,y-1,z-1,dist[x][y-1][z-1]);
                spthobj=spathfind(arr[x][y-1][z-1],x,y,z,x,y-1,z-1);
                if(maps.count(calculateindex(x,y-1,z-1))==1)
                {
                    maps.erase(calculateindex(x,y-1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x,y-1,z-1),spthobj));}

            }
        }

        {
            if(x+1<height && y-1>=0 && z<depth  &&mhp.isinminheap(pos,calculateindex(x+1,y-1,z))&&arr[x+1][y-1][z]>=intenl &&arr[x+1][y-1][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y-1,z);
                if(dist[x][y][z]+weight<dist[x+1][y-1][z]){
                dist[x+1][y-1][z]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y-1,z,dist[x+1][y-1][z]);
                spthobj=spathfind(arr[x+1][y-1][z],x,y,z,x+1,y-1,z);
                 if(maps.count(calculateindex(x+1,y-1,z))==1)
                {
                    maps.erase(calculateindex(x+1,y-1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y-1,z),spthobj));}

            }
        }

        {
            if(x+1<height && y+1<width && z<depth  &&mhp.isinminheap(pos,calculateindex(x+1,y+1,z))&& arr[x+1][y+1][z]>=intenl && arr[x+1][y+1][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y+1,z);
                if(dist[x][y][z]+weight<dist[x+1][y+1][z]){
                dist[x+1][y+1][z]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y+1,z,dist[x+1][y+1][z]);
                spthobj=spathfind(arr[x+1][y+1][z],x,y,z,x+1,y+1,z);
                 if(maps.count(calculateindex(x+1,y+1,z))==1)
                {
                    maps.erase(calculateindex(x+1,y+1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y+1,z),spthobj));}

            }
        }
        {
            if(x+1<height && y<width && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x+1,y,z-1))&& arr[x+1][y][z-1]>=intenl && arr[x+1][y][z-1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y,z-1);
                if(dist[x][y][z]+weight<dist[x+1][y][z-1]){
                dist[x+1][y][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y,z-1,dist[x+1][y][z-1]);
                spthobj=spathfind(arr[x+1][y][z-1],x,y,z,x+1,y,z-1);
                 if(maps.count(calculateindex(x+1,y,z-1))==1)
                {
                    maps.erase(calculateindex(x+1,y,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y,z-1),spthobj));}

            }
        }

        {
            if(x+1<height && y+1<width && z+1<depth  &&mhp.isinminheap(pos,calculateindex(x+1,y+1,z+1))&&arr[x+1][y+1][z+1]>=intenl &&arr[x+1][y+1][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y+1,z+1);
                if(dist[x][y][z]+weight<dist[x+1][y+1][z+1]){
                dist[x+1][y+1][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y+1,z+1,dist[x+1][y+1][z+1]);
                spthobj=spathfind(arr[x+1][y+1][z+1],x,y,z,x+1,y+1,z+1);
                if(maps.count(calculateindex(x+1,y+1,z+1))==1)
                {
                    maps.erase(calculateindex(x+1,y+1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y+1,z+1),spthobj));}

            }
        }

        {
            if(x+1<height && y+1<width && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x+1,y+1,z-1))&& arr[x+1][y+1][z-1]>=intenl && arr[x+1][y+1][z-1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y+1,z-1);
                if(dist[x][y][z]+weight<dist[x+1][y+1][z-1]){
                dist[x+1][y+1][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y+1,z-1,dist[x+1][y+1][z-1]);
                spthobj=spathfind(arr[x+1][y+1][z-1],x,y,z,x+1,y+1,z-1);
                if(maps.count(calculateindex(x+1,y+1,z-1))==1)
                {
                    maps.erase(calculateindex(x+1,y+1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y+1,z-1),spthobj));}

            }
        }

        {
            if(x+1<height && y-1>=0 && z+1<height  &&mhp.isinminheap(pos,calculateindex(x+1,y-1,z+1))&& arr[x+1][y-1][z+1]>=intenl && arr[x+1][y-1][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y-1,z+1);
                if(dist[x][y][z]+weight<dist[x+1][y-1][z+1]){
                dist[x+1][y-1][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y-1,z+1,dist[x+1][y-1][z+1]);
                spthobj=spathfind(arr[x+1][y-1][z+1],x,y,z,x+1,y-1,z+1);
                if(maps.count(calculateindex(x+1,y-1,z+1))==1)
                {
                    maps.erase(calculateindex(x+1,y-1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y-1,z+1),spthobj));}

            }
        }

        {
            if(x+1<height && y-1>=0 && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x+1,y-1,z-1))&&arr[x+1][y-1][z-1]>=intenl &&arr[x+1][y-1][z-1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y-1,z-1);
                if (dist[x][y][z]+weight<dist[x+1][y-1][z-1]){
                dist[x+1][y-1][z-1]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x+1,y-1,z-1,dist[x+1][y-1][z-1]);
                spthobj=spathfind(arr[x+1][y-1][z-1],x,y,z,x+1,y-1,z-1);
                if(maps.count(calculateindex(x+1,y-1,z-1))==1)
                {
                    maps.erase(calculateindex(x+1,y-1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y-1,z-1),spthobj));}

            }
        }

        {
            if(x-1>=0 && y+1<width && z+1<depth &&mhp.isinminheap(pos,calculateindex(x-1,y+1,z+1))&& arr[x-1][y+1][z+1]>=intenl &&  arr[x-1][y+1][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y+1,z+1);
                if(dist[x][y][z]+weight<dist[x-1][y+1][z+1]){
                dist[x-1][y+1][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x-1,y+1,z+1,dist[x-1][y+1][z+1]);
                spthobj=spathfind(arr[x-1][y+1][z+1],x,y,z,x-1,y+1,z+1);
                if(maps.count(calculateindex(x-1,y+1,z+1))==1)
                {
                    maps.erase(calculateindex(x-1,y+1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y+1,z+1),spthobj));}

            }
        }

        {
            if(x-1>=0 && y+1<width && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x-1,y+1,z-1))&&arr[x-1][y+1][z-1]>=intenl && arr[x-1][y+1][z-1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y+1,z-1);
                if  (dist[x][y][z]+weight<dist[x-1][y+1][z-1])
                {

                dist[x-1][y+1][z-1]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x-1,y+1,z-1,dist[x-1][y+1][z-1]);
                spthobj=spathfind(arr[x-1][y+1][z-1],x,y,z,x-1,y+1,z-1);
                 if(maps.count(calculateindex(x-1,y+1,z-1))==1)
                {
                    maps.erase(calculateindex(x-1,y+1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y+1,z-1),spthobj));}

            }
        }

        {
            if(x-1>=0 && y-1>=0 && z<depth&&mhp.isinminheap(pos,calculateindex(x-1,y-1,z))&&arr[x-1][y-1][z]>=intenl && arr[x-1][y-1][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y-1,z);
                if(dist[x][y][z]+weight<dist[x-1][y-1][z]){
                dist[x-1][y-1][z]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x-1,y-1,z,dist[x-1][y-1][z]);
                spthobj=spathfind(arr[x-1][y-1][z],x,y,z,x-1,y-1,z);
                  if(maps.count(calculateindex(x-1,y-1,z))==1)
                {
                    maps.erase(calculateindex(x-1,y-1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y-1,z),spthobj));
                }
            }
        }

        {
            if(x-1>=0 && y-1>=0 && z+1<depth &&mhp.isinminheap(pos,calculateindex(x-1,y-1,z+1))&&arr[x-1][y-1][z+1]>=intenl &&arr[x-1][y-1][z+1]<=intenu)
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y-1,1+z);
                if (dist[x][y][z]+weight<dist[x-1][y-1][z+1]){
                dist[x-1][y-1][z+1]=dist[x][y][z]+ weight;
                 mhp.update(dist,pos,x-1,y-1,z+1,dist[x-1][y-1][z+1]);
                spthobj=spathfind(arr[x-1][y-1][z+1],x,y,z,x-1,y-1,1+z);
                  if(maps.count(calculateindex(x-1,y-1,z+1))==1)
                {
                    maps.erase(calculateindex(x-1,y-1,z+1));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y-1,z+1),spthobj));
                }
            }
        }

        {
            if(x-1>=0 && y-1>=0 && z-1>=0 &&mhp.isinminheap(pos,calculateindex(x-1,y-1,z-1))&&arr[x-1][y-1][z-1]>=intenl && arr[x-1][y-1][z-1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y-1,z-1);
                if (dist[x][y][z]+weight<dist[x-1][y-1][z-1]){
                dist[x-1][y-1][z-1]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x-1,y-1,z-1,dist[x-1][y-1][z-1]);
                spthobj=spathfind(arr[x-1][y-1][z-1],x,y,z,x-1,y-1,z-1);
                  if(maps.count(calculateindex(x-1,y-1,z-1))==1)
                {
                    maps.erase(calculateindex(x-1,y-1,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y-1,z-1),spthobj));
                }
            }
        }
        {
            if(x-1>=0 && y<width && z+1<depth  &&mhp.isinminheap(pos,calculateindex(x-1,y,z+1))&&arr[x-1][y][z+1]>=intenl &&arr[x-1][y][z+1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y,1+z);
                if (dist[x][y][z]+weight<dist[x-1][y][z+1]){
                dist[x-1][y][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x-1,y,z+1,dist[x-1][y][z+1]);
                spthobj=spathfind(arr[x-1][y][z+1],x,y,z,x-1,y,1+z);
                   if(maps.count(calculateindex(x-1,y,1+z))==1)
                {
                    maps.erase(calculateindex(x-1,y,1+z));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y,1+z),spthobj));}

            }
        }

        {
            if(x-1>=0 && y<width && z-1>=0  &&mhp.isinminheap(pos,calculateindex(x-1,y,z-1))&&arr[x-1][y][z-1]>=intenl &&arr[x-1][y][z-1]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y,z-1);
                if (dist[x][y][z]+weight<dist[x-1][y][z-1]){
                dist[x-1][y][z-1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x-1,y,z-1,dist[x-1][y][z-1]);
                spthobj=spathfind(arr[x-1][y][z-1],x,y,z,x-1,y,z-1);
                   if(maps.count(calculateindex(x-1,y,z-1))==1)
                {
                    maps.erase(calculateindex(x-1,y,z-1));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y,z-1),spthobj));
                }

            }
        }

        {
            if(x-1>=0 && y+1<width && z<depth  &&mhp.isinminheap(pos,calculateindex(x-1,y+1,z))&&arr[x-1][y+1][z]>=intenl &&arr[x-1][y+1][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y+1,z);
                if (dist[x][y][z]+weight<dist[x-1][y+1][z])
                {

                dist[x-1][y+1][z]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x-1,y+1,z,dist[x-1][y+1][z]);
                spthobj=spathfind(arr[x-1][y+1][z],x,y,z,x-1,y+1,z);
                   if(maps.count(calculateindex(x-1,y+1,z))==1)
                {
                    maps.erase(calculateindex(x-1,y+1,z));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y+1,z),spthobj));
                }
            }
        }

        {
            if(x+1<height && y<width && z<depth  &&mhp.isinminheap(pos,calculateindex(x+1,y,z))&&arr[x+1][y][z]>=intenl && arr[x+1][y][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y,z);
                if(dist[x][y][z]+weight<dist[x+1][y][z]){
                dist[x+1][y][z]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x+1,y,z,dist[x+1][y][z]);
                spthobj=spathfind(arr[x+1][y][z],x,y,z,x+1,y,z);
                   if(maps.count(calculateindex(x+1,y,z))==1)
                {
                    maps.erase(calculateindex(x+1,y,z));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y,z),spthobj));

            }
            }
        }

        {
            if(x-1>=0 && y<width && z<depth  &&mhp.isinminheap(pos,calculateindex(x-1,y,z))&&arr[x-1][y][z]>=intenl && arr[x-1][y][z]<=intenu )
            {
                weight = setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x-1,y,z);
                if (dist[x][y][z]+weight<dist[x-1][y][z]){
                dist[x-1][y][z]=dist[x][y][z]+weight;
                 mhp.update(dist,pos,x-1,y,z,dist[x-1][y][z]);
                spthobj=spathfind(arr[x-1][y][z],x,y,z,x-1,y,z);
                   if(maps.count(calculateindex(x-1,y,z))==1)
                {
                    maps.erase(calculateindex(x-1,y,z));
                }
                maps.insert(std :: make_pair(calculateindex(x-1,y,z),spthobj));
                }
            }
        }

        {
            if(x+1<height && y<width && z+1<depth  &&mhp.isinminheap(pos,calculateindex(x+1,y,z+1))&&arr[x+1][y][z+1]>=intenl && arr[x+1][y][z+1]<=intenu )
            {
                weight =setfdtweight(intlist,visited,cg,arr,intensitymap,types,intype1,intype2,x,y,z,x+1,y,z+1);
                if (dist[x][y][z]+weight<dist[x+1][y][z+1]){
                dist[x+1][y][z+1]=dist[x][y][z]+weight;
                mhp.update(dist,pos,x+1,y,z+1,dist[x+1][y][z+1]);
                spthobj=spathfind(arr[x+1][y][z+1],x,y,z,x+1,y,z+1);
                   if(maps.count(calculateindex(x+1,y,1+z))==1)
                {
                    maps.erase(calculateindex(x+1,y,1+z));
                }
                maps.insert(std :: make_pair(calculateindex(x+1,y,1+z),spthobj));
                }
            }
        }

    ofstream out;
    out.open("ShortestPath",ios :: out);
    out.write((char*)&maps,sizeof(maps));
  //  fout.write((char*)&intensitymap,sizeof(intensitymap));
    out.close();
  //  fout.close();


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
   /* intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoint.returnx(),fpoint.returny(),fpoint.returnz(),intensitymap);
    if(confirmation==0)
    {
        QMessageBox nqmsg;
         if(dist[fpoint.returnx()][fpoint.returny()][fpoint.returnz()]>0){
         //    nqmsg.setText("Intermediate modification");
           //  nqmsg.exec();
        intensitymap = readandmodifyimage(cg,arr,xs,ys,zs,fpoint.returnx(),fpoint.returny(),fpoint.returnz(),intensitymap);
         }
        change=1;
        //modinten--;
        xs=xf;ys=yf;zs=zf;
         cvisited[arr[xs][ys][zs]-fixedmodinten]=true;
        xf=xn;yf=yn;zf=zn;
       // modifiedinten--;
      //  xs=xn;ys=yn;zs=zn;
       goto begining;
    }*/

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

    int type,intype1,intype2;
    short prevdist;
    list<int> intlist;
    bool cvisited[100];
    for(int i=0;i<100;i++)
    {
        cvisited[i]=false; //keeps track of all the segment that are in the path between start and end point
    }

    storemin fpoint;

    /*
     *  based on the intensity of start and end points queries are given types(0,1,2,etc).
     *  main reason is to minimize the execution time.Suppose for joining point if algorithm reaches a point on a segment
     *  which is in turn connected to the desired segment then serch immediately stopped.
    */

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
    qmstr.setNum(type);
  //  qmsb.setText("Type Of query : "+qmstr);
    //qmsb.exec();
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
    cout<<"Type : "<<type <<"Query\n";
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



void findspathforreal(char*** spherearr, int xdim, int ydim, int zdim) //Main function that reads the points from the file and finds shortest path
{
    QMessageBox nmsg;
    static int sinti=0;
    nmsg.setText("find shortest path\n");
    nmsg.exec();
    /**
     * process_input() reads points from Spine_seed.txt(file where points given by the user is stored)
     * and converts it into object and writes those objects into PointsFile.txt
     */
    height = zdim;
    width = ydim;
    depth=xdim;
    latest_start_end lstartend =  process_input();  //lstartend keeps track of first segment whose both end points are given
    nmsg.setText("input processed\n");
    nmsg.exec();
    cgraph cg(1000);
    short xs,ys,zs,xe,ye,ze;
    short *** arr,***dist;

    long int* pos;
     minheap mhp;


    arr=new short**[height];
    dist=new short**[height];

    delete pos;
    pos=new  long int[imagedimension]; //needed for shortestpath mean heap implementation

    nmsg.setText("arr dist pos allocated\n");
    nmsg.exec();
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

    cout<<"Memories allocated\n";


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

    // return; //comment out this line only to inspect the validity of points

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
           // fpoints=findspath3dsingle(cg,mhp,pos,arr,dist,xs,ys,zs,xe,ye,ze,intensitymap); //finds shortestpath in case of joining function

            //  fmsg.setText("Will draw points");
          //  fmsg.exec();
           // if(dist[fpoints.returnx()][fpoints.returny()][fpoints.returnz()]>0)
           // intensitymap=readandmodifyimage(cg,arr,xs,ys,zs,fpoints.returnx(),fpoints.returny(),fpoints.returnz(),intensitymap);
          //  fmsg.setText("Joining point processed");
          //  fmsg.exec();
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
           // fmsg.setText("After While Loop");
           // fmsg.exec();

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
         //   msgb.setText("Last InterMediate Point is Processed");
         //   msgb.exec();
        }
     //   msgb.setText("Process Next Set of Points");
     //   msgb.exec();
    }

    int choice;

    update_spherearr(arr,spherearr);
    //showsphere(arr);
    msgb.setText("Shortest Paths Found");
    msgb.exec();
    free(dist); dist=NULL;
    free(arr); arr=NULL;
    free(pos); pos=NULL;
    outf.close();
    fin.close();

}

