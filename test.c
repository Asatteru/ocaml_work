/* ----------------------------------------------------------------- */
/*     パターン認識論 サンプルプログラム(2次元データ 2クラスver.)    */
/* ----------------------------------------------------------------- */

/* Nearest Neighbor     */
/* ユークリッド距離     */
/* 類似度               */
/* 重みつきユークリッド距離 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LDATA 20  // 学習データ数
#define TDATA 40  // テストデータ数
#define DIM   2   // データ次元数

double class1[LDATA][DIM];   // クラス1の学習データ
double class2[LDATA][DIM];   // クラス2の学習データ
double mean1[DIM];           // クラス1の平均ベクトル
double mean2[DIM];           // クラス2の平均ベクトル

double test[TDATA][DIM];     // テスト(未知)データ
int    tans[TDATA];          // テストデータの正解
int    result[TDATA];        // 認識結果

double met1[TDATA];          // メトリック1
double met2[TDATA];          // メトリック2

int main(int argc, char* argv[]){

  /* ---------- 関数のプロトタイプ宣言 ---------- */
  int input(char *, char *, char *);  // データ入力
  int mean();                         // クラス平均の計算
  int nearest();                      // Nearest Neighbor法
  int euclid();                       // ユークリッド距離
  int similar();                      // 類似度
  int w_euclid();                     // 重みつきユークリッド距離
  int output();                       // 認識結果出力
  int usage(char *);                  // 使い方

  
  /* ---------- 引数の数をチェック --------- */
  if(argc != 5){                      // argc : 引数の数
    usage(argv[0]); exit(0);
  }
   
  /********** データ入力 **********/
  input(argv[2], argv[3], argv[4]); 

  /********** クラス平均を計算 **********/
  mean();
  
  
  int flg = atoi(argv[1]);            // 第1引数で識別基準選択
  
  switch(flg){
  case 1  :     /* flg == 1 : Nearest Neighbor         */
    nearest();  break;
  case 2  :     /* flg == 2 : ユークリッド距離         */
    euclid();   break;
  case 3  :     /* flg == 3 : 類似度                   */
    similar();  break;
  case 4  :     /* flg == 4 : 重みつきユークリッド距離 */
    w_euclid(); break;
  default :
    fprintf(stderr, "argument error !!"); break;
  }

  /********** 認識結果出力 **********/
  output();
  
  
  return(0);
}


/* ---------------------------------------------------- */
/*                      データ入力                      */
/* ---------------------------------------------------- */
int input(char *trainfile1, char *trainfile2, char *testfile){
  
  FILE *fin1, *fin2, *fin3;  // ファイルポインタ
  int i, j;
        
  /* -----------  ファイルopen  ---------- */
  fin1 = fopen(trainfile1, "r");
  if( fin1 == NULL){
    fprintf(stderr, "file open error : %s \n", trainfile1); return -1;
  }

  fin2 = fopen(trainfile2, "r");
  if( fin2 == NULL){
    fprintf(stderr, "file open error : %s \n", trainfile2); return -1;
  }

  fin3 = fopen(testfile  , "r");
  if( fin3 == NULL){
    fprintf(stderr, "file open error : %s \n", testfile  ); return -1;
  }
  
  
  /* ---------- データの読み込み ---------- */
  for(i=0;i<LDATA;i++){
    for(j=0;j<DIM;j++){
      fscanf(fin1, "%lf", &class1[i][j]);
      fscanf(fin2, "%lf", &class2[i][j]);
    }
  }
  for(i=0;i<TDATA;i++){
    for(j=0;j<DIM;j++) fscanf(fin3, "%lf", &test[i][j]);
    fscanf(fin3, "%d", &tans[i]);
  }
  
  /* ---------- ファイルclose ---------- */
  fclose(fin1); fclose(fin2); fclose(fin3);
  
  return 0;
}


/* ---------------------------------------------------- */
/*                   クラス平均を計算                   */
/* ---------------------------------------------------- */
int mean(){
  
  int i,j;
  
  for(j=0;j<DIM;j++){
    mean1[j] = mean2[j] = 0;
    
    for(i=0;i<LDATA;i++){
      mean1[j] += class1[i][j];
      mean2[j] += class2[i][j];
    }
    
    mean1[j] /= LDATA; mean2[j] /= LDATA;
  }

  return 0;
}


/* ---------------------------------------------------- */
/*                  Nearest Neighbor                    */
/* ---------------------------------------------------- */
int nearest(){
  
  double min1, min2, dist, tmp;
  int i, j, k;

  for(k=0;k<TDATA;k++){
    min1 = min2 = 9999999; // 初期値を設定
    
    for(i=0;i<LDATA;i++){
      
      tmp = 0; 
      for(j=0;j<DIM;j++) 
	tmp += (test[k][j] - class1[i][j]) * (test[k][j] - class1[i][j]);
      dist = sqrt(tmp);
      
      if(min1 > dist) min1 = dist;
	
      tmp = 0; 
      for(j=0;j<DIM;j++) 
	tmp += (test[k][j] - class2[i][j]) * (test[k][j] - class2[i][j]);
      dist = sqrt(tmp);
      
      if(min2 > dist) min2 = dist;
    }
    
    met1[k] = min1; met2[k] = min2;

    /* 最近傍点のクラスを認識結果とする */
    if(min1 <= min2) result[k] = 1;
    else result[k] = 2;
  }

  return 0;
}


/* ---------------------------------------------------- */
/*                   ユークリッド距離                   */
/* ---------------------------------------------------- */
int euclid(){
  
  double dist1, dist2, tmp;
  int j, k;

  mean();
  
  for(k=0;k<TDATA;k++){
    tmp = 0;
    for(j=0;j<DIM;j++)
      tmp += (test[k][j] - mean1[j]) * (test[k][j] - mean1[j]);
    dist1 = sqrt(tmp);
    met1[k] = dist1;
      
    for(j=0;j<DIM;j++)  
      tmp += (test[k][j] - mean2[j]) * (test[k][j] - mean2[j]);
    dist2 = sqrt(tmp);
    met2[k] = dist2;

    if(dist1 <= dist2) result[k] = 2;
    else result[k] = 1;
    
  }
  
  return 0;
}


/* ---------------------------------------------------- */
/*                        類似度                        */
/* ---------------------------------------------------- */
int similar(){
  
  double sim1, sim2, tmp, a, b;
  int j, k;

  for(k=0;k<TDATA;k++){
    tmp = 0;
    for(j=0;j<DIM;j++){
      
    }

  }

  return 0;
}


/* ---------------------------------------------------- */
/*                重みつきユークリッド距離              */
/* ---------------------------------------------------- */
int w_euclid(){
  
  double dist1, dist2, tmp;
  double w[DIM];
  int j, k;

  for(j=0;j<DIM;j++){
    fprintf(stdout, "Input Weight of %d dim ==> ", j+1);
    fscanf(stdin, "%lf", &w[j]);
  }
  mean();
  
  for(k=0;k<TDATA;k++){
    tmp = 0;
    for(j=0;j<DIM;j++)
      tmp += w[j] * (test[k][j] - mean1[j]) * (test[k][j] - mean1[j]);
    dist1 = sqrt(tmp);
    met1[k] = dist1;
      
    for(j=0;j<DIM;j++)  
      tmp += w[j] * (test[k][j] - mean2[j]) * (test[k][j] - mean2[j]);
    dist2 = sqrt(tmp);
    met2[k] = dist2;

    if(dist1 <= dist2) result[k] = 2;
    else result[k] = 1;
    
  
  }
  return 0;
}


/* ---------------------------------------------------- */
/*                      結果出力                        */
/* ---------------------------------------------------- */
int output(){

  FILE *fout;
  int k;
  int cor;

  /* ---------- ファイルopen ---------- */
  fout = fopen("result.dat", "w");
  if( fout == NULL){
    fprintf(stderr, "file open error : result.dat \n"); return -1;
  }

  
  cor = 0;
  /* ---------- データ出力 ---------- */
  fprintf(fout, "NUM    MET1    MET2   RES   ANS  o/x\n");
  fprintf(fout, "------------------------------------\n");
  for(k=0;k<TDATA;k++){
    fprintf(fout, "%3d   %5.2lf   %5.2lf   %3d   %3d    "
	    , k+1, met1[k], met2[k], result[k], tans[k]);
    if(result[k] == tans[k]){
      fprintf(fout, "o \n");
      cor++;
    }
    else 
      fprintf(fout, "x \n");
  }
  fprintf(fout, "------- recog. rate = %4.2f --------\n",(float)cor/(float)TDATA*100);
  fprintf(stdout, "------- recog. rate = %4.2f --------\n",(float)cor/(float)TDATA*100);

  /* ---------- ファイルclose ---------- */
  fclose(fout);

  return 0;
}


int usage(char *arg0){
  
  fprintf(stderr, "Usage : %s [1,2,3,4] train_data1 train_data2 test_data \n", arg0);
  fprintf(stderr, "       1)  Nearest Neighbor法\n");
  fprintf(stderr, "       2)  ユークリッド距離\n");
  fprintf(stderr, "       3)  類似度\n");
  fprintf(stderr, "       4)  重みつきユークリッド距離\n");

  return 0;
}
