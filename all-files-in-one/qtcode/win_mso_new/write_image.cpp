void MainWindow::on_pushButton_15_clicked()
{
    for(int i = 0 ;i < Xdim ; i++ ){
        for(int j=0;j<Ydim;j++){
            for(int k=0;k<Zdim;k++){
                if(FCv[i][j][k]>FCb[i][i][j]){
                     arr[i][j][k]= 50;
                 }
                else if (FCv[i][j][k]<FCb[i][i][j])
                 {
                    arr[i][j][k]=60;
                 }
                if(MRv[curX][i][j]<MRb[curX][i][j]){
                          arr[i][j][k]=70;
                     }
            }
        }
    }
}

void write_vessel_file(unsigned char*** spherearr)
{
    fstream outf;
    outf.open("vseedfile.txt",ios::in);
    for(int i=0; i<depths; i++){
        for(int j=0;j<widths;j++){
            for(int k=0;k<heights;k++){
                outf.write((char*)&spherearr[k][j][i],sizeof(unsigned char));
            }
        }
    }
    outf.close();
}
