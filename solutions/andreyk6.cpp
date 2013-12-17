#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;

int DetectLine(int **&field,int n){
        int pos,tempLength,maxLength=0,tempColor,i,j=0,totalLines=0;

        for (i=0;i<n;i++){
                //Поиск самой длинной линии по горизонтали                
                maxLength=0;
                tempLength=0;
                tempColor=field[i][0];
                for (j=0;j<n;j++){
                        if (tempColor==0 && field[i][j]!=0){
                                tempColor=field[i][j];
                                tempLength=1;
                        } else if(tempColor==field[i][j] && tempColor!=0){
                                tempLength++;
                                if(maxLength<tempLength) maxLength=tempLength;
                        } else if(tempColor!=field[i][j] && tempColor!=0){
                                tempColor=field[i][j];
                                tempLength=1;
                        }
                }
                
                if(maxLength>=5) totalLines++;

                //Поиск самой длинной линии по вертикали
                maxLength=0;
                tempLength=0;
                tempColor=field[0][i];
                for (j=0;j<n;j++){
                        if (tempColor==0 && field[j][i]!=0){
                                tempColor=field[j][i];
                                tempLength=1;
                        } else if(tempColor==field[j][i] && tempColor!=0){
                                tempLength++;
                                if(maxLength<tempLength && tempColor!=0) maxLength=tempLength;
                        } else if(tempColor!=field[j][i]){
                                tempColor=field[j][i];
                                tempLength=1;
                        }
                }

                if(maxLength>=5) totalLines++;

        }
        
        //Поиск в диагонали
        for (i=0;i<n;i++){
                //Поиск самой длинной линии по горизонтали                
                pos=0;
                maxLength=0;
                tempLength=0;
                tempColor=field[i][j];
                for (j=i;j>0,pos<i;j--){
                        if (tempColor==0 && field[pos][j]!=0){
                                tempColor=field[pos][j];
                                tempLength=1;
                        } else if(tempColor==field[pos][j] && tempColor!=0){
                                tempLength++;
                                if(maxLength<tempLength) maxLength=tempLength;
                        } else if(tempColor!=field[pos][j] && tempColor!=0){
                                tempColor=field[pos][j];
                                tempLength=1;
                        }
                        pos++;
                }
                
                if(maxLength>=5) totalLines++;
                
                pos=0;
                maxLength=0;
                tempLength=0;
                tempColor=0;
                for (j=i;j<n;j++){
                        if (tempColor==0 && field[j-i][j]!=0){
                                tempColor=field[j-i][j];
                                tempLength=1;
                        } else if(tempColor==field[j-i][j] && tempColor!=0){
                                tempLength++;
                                if(maxLength<tempLength) maxLength=tempLength;
                        } else if(tempColor!=field[j-i][j] && tempColor!=0){
                                tempColor=field[j-i][j];
                                tempLength=1;
                        }
                        pos++;
                }
                
                if(maxLength>=5) totalLines++;
        }
        
        return totalLines;
}

int ReadAndMove(int **&field,int n){
        int x,y,x2,y2,result;
        string strParam,strData,strColor,strTemp;

        //Читаем начальные параметры
        cin >> strParam;
        cin >> strData;
        while(strParam=="BALL"){
                //Оторвали X
                strTemp=strData.substr(0,strData.find(","));
                x=atoi(strTemp.c_str());

                //Отрезали лишние данные
                strData=strData.substr(strData.find(",")+1,strData.length());

                //Оторвали Y
                strTemp=strData.substr(0,strData.find(","));
                y=atoi(strTemp.c_str());

                //Достали цвет
                strColor=strData.substr(strData.find(",")+1,strData.length());

                if(x>=0 && x<n && y>=0 && y<n)
                        if (strColor=="red") field[y][x]=1; else
                                if (strColor=="green") field[y][x]=2; else
                                        if (strColor=="blue") field[y][x]=3; else return 0;

                cin >> strParam;
                cin >> strData;

        }

        //Двигаем шарики

        while(strParam=="MOVE"){
                //Оторвали X
                strTemp=strData.substr(0,strData.find(","));
                x=atoi(strTemp.c_str());

                //Отрезали лишние данные
                strData=strData.substr(strData.find(",")+1,strData.length());

                //Оторвали Y
                strTemp=strData.substr(0,strData.find(","));
                y=atoi(strTemp.c_str());

                //Отрезали лишние данные
                strData=strData.substr(strData.find(",")+1,strData.length());

                //Оторвали X2
                strTemp=strData.substr(0,strData.find(","));
                x2=atoi(strTemp.c_str());

                //Оторвали Y2
                strTemp=strData.substr(strData.find(",")+1,strData.length());
                y2=atoi(strTemp.c_str());

                if(x>=0 && x<n && y>=0 && y<n && x2>=0 && x2<n && y2>=0 && y2<n) 
                        if(field[y2][x2]==0 && field[y][x]!=0){
                                field[y2][x2]=field[y][x];
                                field[y][x]=0;
                        } else return 0;


                result = DetectLine(field,n);
                if (result>=1) return result;

                if (cin.eof()) break;
                cin >> strParam;
                cin >> strData;
        }

        return 0;
} 

int main(){
        int n; 
        string strParam;
        
        freopen("input.txt","r",stdin);
//        freopen("output.txt","w",stdout);

        //Читаем размер
        cin >> strParam >> n;

        //Создали массив
                int **field = new int *[n];
                for (int i=0;i<n;i++) 
                        field[i]=new int[n];
        for (int i=0;i<n;i++)
                for(int j=0;j<n;j++)
                        field[i][j]=0;

        //Заполняем массив входными данными и двигаем шарики
        cout << ReadAndMove(field,n);

        delete []field;
        return 0;
}
