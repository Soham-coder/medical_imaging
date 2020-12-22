#include<QCoreApplication>
#include<stdio.h>
#include<stdlib.h>
#define sizex 101
#define sizey 101
#define sizez 101
unsigned short a[sizex][sizey][sizez];
void drawconjointtube(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void drawsphere(unsigned short[sizex][sizey][sizez],int ,int,int,int);
void drawYshape(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void drawTshape(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void jointtjunction(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void drawUshape(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void drawtubeshape(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void drawconjointtube(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void irregular_halfcircle(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void irregulartube(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void ring(unsigned short[sizex][sizey][sizez],int,int,int,int);
void umbrella(unsigned short[sizex][sizey][sizez],int,int,int,int,int,int);
void smoothsphere(unsigned short[sizex][sizey][sizez],int ,int ,int ,int);
void create_image(unsigned short[sizex][sizey][sizez]);
void draw_circle(unsigned short[sizex][sizey][sizez],int,int,int,int);
void draw_cone(unsigned short[sizex][sizey][sizez], int, int, int, int);
void smoothcircle(unsigned short[sizex][sizey][sizez],int,int,int,int);
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    //drawYshape(a,70,70,70,20,20,20);
    //drawTshape(a,40,40,40,40,40,40);
     //drawUshape(a,70,70,70,30,30,30);
    //drawhalfcircle(a,30,20,20,20,60,60,60);
    //drawsphere(a,18,30,30,30);
    //drawtubeshape(a,70,70,70,20,20,20);
    //umbrella(a,70,70,70,20,20,20);
    //irregular_halfcircle(a,70,70,70,42,42,42);
   // irregulartube(a,70,70,70,20,20,20);
   // printf("xxxxxxxx");
   // drawconjointtube(a,70,70,70,20,20,20);
    //jointtjunction(a,40,40,40,40,40,40);
    //drawconjointtube(a,70,70,70,20,20,20);
    draw_cone(a,25,30,30,30);
   //ring(a,30,50,50,50);
    create_image(a);
    printf("\n done!!");
    return app.exec();
}

void drawYshape(unsigned short a[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int j=ys;
    for(int i=xs;i<=xs+xlen/2;i++)
    {

            //for(int k=zs;k<=zs+klen;k++)
            {
                drawsphere(a,8,i,j,zs);
                j++;
            }

    }
    j=ys;

    for(int i=xs+xlen;i>=xs+xlen/2;i--)
    {

            //for(int k=zs;k<=zs+klen;k++)
            {
                drawsphere(a,8,i,j,zs);
                j++;
            }

    }

    for(int i=xs+xlen/2;i<=xs+xlen;i++)
    {
        drawsphere(a,8,xs+xlen/2,j,zs);
        j++;
    }
}

void drawsphere(unsigned short a[sizex][sizey][sizez],int r,int xc,int yc,int zc)
{
    printf("Draw sphere : %d %d %d \n",xc,yc,zc);
    unsigned short b[sizez];
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
           int y=c;
           int p=1-c;
            a[xc+x][yc-y][k]=100;
            a[xc-x][yc-y][k]=100;
            a[xc+x][yc+y][k]=100;
            a[xc-x][yc+y][k]=100;
            a[xc+y][yc-x][k]=100;
            a[xc-y][yc-x][k]=100;
            a[xc+y][yc+x][k]=100;
            a[xc-y][yc+x][k]=100;

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
            a[xc+x][yc-y][k]=100;
            a[xc-x][yc-y][k]=100;
            a[xc+x][yc+y][k]=100;
            a[xc-x][yc+y][k]=100;
            a[xc+y][yc-x][k]=100;
            a[xc-y][yc-x][k]=100;
            a[xc+y][yc+x][k]=100;
            a[xc-y][yc+x][k]=100;
         }

        }
         smoothsphere(a,r1,xc,yc,k);
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
            a[xc+x][yc-y][k]=100;
            a[xc-x][yc-y][k]=100;
            a[xc+x][yc+y][k]=100;
            a[xc-x][yc+y][k]=100;
            a[xc+y][yc-x][k]=100;
            a[xc-y][yc-x][k]=100;
            a[xc+y][yc+x][k]=100;
            a[xc-y][yc+x][k]=100;
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
            a[xc+x][yc-y][k]=100;
            a[xc-x][yc-y][k]=100;
            a[xc+x][yc+y][k]=100;
            a[xc-x][yc+y][k]=100;
            a[xc+y][yc-x][k]=100;
            a[xc-y][yc-x][k]=100;
            a[xc+y][yc+x][k]=100;
            a[xc-y][yc+x][k]=100;
      }

        }
        smoothsphere(a,r1,xc,yc,k);
 }

    printf("Sphere with Centre at %d %d %d is created\n",xc,yc,zc);

}

void create_image(unsigned short im[sizex][sizey][sizez])
{
    FILE *ft;
    int i,j,k;
    unsigned short pixel;
    ft=fopen("cone30.img" ,"wb");

    if(ft==NULL)
    {
        printf("\n Unable to open o/p file");
        exit(0);
    }
   for(k=1;k<sizez;k++)
   {
    for(j=1;j<sizey;j++)
    {
     for(i=1;i<sizex;i++)
     {
       pixel=im[i][j][k];
       fwrite(&pixel,sizeof(unsigned short),1,ft);
     }
    }
   }
fclose(ft);
}

void smoothsphere(unsigned short a[sizex][sizey][sizez],int r, int xc,int yc,int zc)
{
    int k=zc;
    for(int i=xc-r;i<xc+r;i++)
    {
        for(int j=yc;j<yc+r;j++)
        {
            if(a[i][j][k]!=100 && a[i][j-1][k]!=0 && a[i-1][j][k]!=0 && a[i+1][j][k]!=0 && a[i][j+1][k]!=0)
            {
                a[i][j][k]=100;
            }
        }
    }

    for(int i=xc-r;i<xc+r;i++)
    {
        for(int j=yc;j>=yc-r;j--)
        {
            if(a[i][j][k]!=100 && a[i][j-1][k]!=0 && a[i-1][j][k]!=0 && a[i+1][j][k]!=0 && a[i][j+1][k]!=0)
            {
                a[i][j][k]=100;
            }
        }
    }
}

void drawTshape(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs,i=xs;
    for(k=zs;k<=zs+klen;k++)
    {
        drawsphere(a,8,i,ys,k);
    }
    for(i=xs;i<=(xs+xlen-10);i++)
    {
        drawsphere(a,8,i,ys,zs);
    }
    for(i=xs;i>=(xs-xlen+10);i--)
    {
      drawsphere(a,8,i,ys,zs);
    }
}

void jointtjunction(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs,i=xs;
    for(k=zs;k<=zs+klen;k++)
    {
        drawsphere(a,4,i,ys,k);
    }
    for(i=xs;i<=(xs+xlen-10);i++)
    {
        drawsphere(a,4,i,ys,zs);
    }
    for(k=zs+9;k<=zs+klen;k++)
    {
        drawsphere(a,4,xs+9,ys,k);
    }
    for(i=xs+9;i<=(xs+xlen-1);i++)
    {
        drawsphere(a,4,i,ys,zs+9);
    }

}

void drawUshape(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int i,j,j1;
    i=xs-10;
    for(j=ys;j<=(ys+ylen-15);j++)
    {
       drawsphere(a,4,i,j,zs);
    }
    i=xs+10;
    for(j=ys;j<=(ys+ylen-15);j++)
    {
       drawsphere(a,4,i,j,zs);
    }
    j=j-1;
    i=0;
    j1=10;
    drawsphere(a,4,xs,j+j1,zs);
    int p=-9;
    while(i<=j1)
    {
       if (p<0)
        {
           p=(p+(2*i)+3);
           j1=j1;
           i=i+1;
        }
       else
       {
           p=p+(2*(i-j1)+5);
           j1=j1-1;
           i=i+1;
       }

    if(i>0)
    {
     drawsphere(a,4,xs+i,j+j1,zs);
     drawsphere(a,4,xs-i,j+j1,zs);
     drawsphere(a,4,xs+j1,j+i,zs);
     drawsphere(a,4,xs-j1,j+i,zs);
    }
   }
}

void drawtubeshape(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs;
    for(k=zs;k<=zs+klen;k++)
    {
        drawsphere(a,12,xs,ys,k);
    }
}

void umbrella(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs,k1,i;
    for(k=zs;k<=zs+klen-40;k++)
    {
        drawsphere(a,4,xs,ys,k);
    }
    k=k-14;
    i=0;
    k1=14;
    drawsphere(a,4,xs,ys,k+k1);
    int p=-13;
    while(i<=k1)
    {
       if (p<0)
        {
           p=(p+(2*i)+3);
           k1=k1;
           i=i+1;
        }
       else
       {
           p=p+(2*(i-k1)+5);
           k1=k1-1;
           i=i+1;
       }

    if(i>0)
    {
     drawsphere(a,4,xs+i,ys,k+k1);
     drawsphere(a,4,xs-i,ys,k+k1);
     drawsphere(a,4,xs+k1,ys,k+i);
     drawsphere(a,4,xs-k1,ys,k+i);
    }
   }
}

void irregular_halfcircle(unsigned short[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs,k1,i;
    i=0;
    k1=35;
    drawsphere(a,4,xs,ys,k+k1);
    int p=-34;
    while(i<=k1)
    {
       if (p<0)
        {
           p=(p+(2*i)+3);
           k1=k1;
           i=i+1;
        }
       else
       {
           p=p+(2*(i-k1)+5);
           k1=k1-1;
           i=i+1;
       }

    if(i>0)
    {
     drawsphere(a,4,xs+i,ys,k+k1);
     drawsphere(a,4,xs-i,ys,k+k1);
     drawsphere(a,4,xs+k1,ys,k+i);
     drawsphere(a,4,xs-k1,ys,k+i);
    }
   }
   drawsphere(a,7,xs+15,ys,zs+43);
}

void irregulartube(unsigned short a[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k;
    for(k=zs;k<zs+klen;k++)
    {
        drawsphere(a,4,xs,ys,k);
    }
}

void drawconjointtube(unsigned short a[sizex][sizey][sizez],int xlen,int ylen,int klen,int xs,int ys,int zs)
{
    int k=zs;
    for(k=zs;k<=zs+klen;k++)
    {
        drawsphere(a,4,xs,ys,k);
    }
    for(k=zs;k<=zs+klen;k++)
    {
        drawsphere(a,4,xs+20,ys+20,k);
    }
}

void ring(unsigned short a[sizex][sizey][sizez], int r,int xs, int ys,int zs)
{
    unsigned short b[sizez],r1,k;
    int z=0;
    int x=r;
    b[zs]=x;
    b[zs+x]=z;
    b[zs-x]=z;
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
     b[zs+z]=x;
     b[zs-z]=x;
     b[zs+x]=z;
     b[zs-x]=z;
    }
   }
   for(k=zs;k>=zs-r;k--)
    {
        r1=b[k];
        drawsphere(a,10,xs+r1,ys,k);
        drawsphere(a,10,xs-r1,ys,k);
        //drawsphere(a,4,r1,ys,k+r1);
//        a[xc+x][yc-y][k]=100;
//        a[xc-x][yc-y][k]=100;
//        a[xc+x][yc+y][k]=100;
//        a[xc-x][yc+y][k]=100;
//        a[xc+y][yc-x][k]=100;
//        a[xc-y][yc-x][k]=100;
//        a[xc+y][yc+x][k]=100;
//        a[xc-y][yc+x][k]=100;
    }
    for(k=zs;k<=zs+r;k++)
    {
        r1=b[k];
        drawsphere(a,10,xs+r1,ys,k);
        drawsphere(a,10,xs-r1,ys,k);
    }
}
void draw_circle(unsigned short a[sizex][sizey][sizez],int r,int xc,int yc,int zc)
{
    int x=0,y=r,c;
    for(int c=0;c<=r;c++)
    {
        int x=0;
        int y=c;
        int p=1-c;
        a[xc+x][yc-y][zc]=100;
        a[xc-x][yc-y][zc]=100;
        a[xc+x][yc+y][zc]=100;
        a[xc-x][yc+y][zc]=100;
        a[xc+y][yc-x][zc]=100;
        a[xc-y][yc-x][zc]=100;
        a[xc+y][yc+x][zc]=100;
        a[xc-y][yc+x][zc]=100;
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
        a[xc+x][yc-y][zc]=100;
        a[xc-x][yc-y][zc]=100;
        a[xc+x][yc+y][zc]=100;
        a[xc-x][yc+y][zc]=100;
        a[xc+y][yc-x][zc]=100;
        a[xc-y][yc-x][zc]=100;
        a[xc+y][yc+x][zc]=100;
        a[xc-y][yc+x][zc]=100;
  }

}}

void draw_cone(unsigned short a[sizex][sizey][sizez], int r, int xc, int yc, int zc)
{
    int c,k,r1=r;
    for(k=zc;k<=zc+r;k++)
    {
    for(c=1;c<=r1;c++)
    {
        draw_circle(a,c,xc,yc,k);
    }
    smoothcircle(a,r1,xc,yc,k);
    r1--;
    }
    draw_circle(a,r1,xc,yc,k);

}
void smoothcircle(unsigned short a[sizex][sizey][sizez],int r,int xc,int yc,int zc)
{
    int k=zc;
    for(int i=xc-r;i<=xc+r;i++)
    {
        for(int j=yc-r;j<=yc+r;j++)
        {
            if(a[i][j][k]!=100 && a[i][j-1][k]!=0 && a[i-1][j][k]!=0 && a[i+1][j][k]!=0 && a[i][j+1][k]!=0)
            {
                a[i][j][k]=100;
            }
        }
    }
}


