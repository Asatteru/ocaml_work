/* ----------------------------------------------------------------- */
/*     �ѥ�����ǧ���� ����ץ�ץ����(2�����ǡ��� 2���饹ver.)    */
/* ----------------------------------------------------------------- */

/* Nearest Neighbor     */
/* �桼����åɵ�Υ     */
/* �����               */
/* �ŤߤĤ��桼����åɵ�Υ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LDATA 20  // �ؽ��ǡ�����
#define TDATA 40  // �ƥ��ȥǡ�����
#define DIM   2   // �ǡ���������

double class1[LDATA][DIM];   // ���饹1�γؽ��ǡ���
double class2[LDATA][DIM];   // ���饹2�γؽ��ǡ���
double mean1[DIM];           // ���饹1��ʿ�ѥ٥��ȥ�
double mean2[DIM];           // ���饹2��ʿ�ѥ٥��ȥ�

double test[TDATA][DIM];     // �ƥ���(̤��)�ǡ���
int    tans[TDATA];          // �ƥ��ȥǡ���������
int    result[TDATA];        // ǧ�����

double met1[TDATA];          // ��ȥ�å�1
double met2[TDATA];          // ��ȥ�å�2

int main(int argc, char* argv[]){

  /* ---------- �ؿ��Υץ�ȥ�������� ---------- */
  int input(char *, char *, char *);  // �ǡ�������
  int mean();                         // ���饹ʿ�Ѥη׻�
  int nearest();                      // Nearest Neighborˡ
  int euclid();                       // �桼����åɵ�Υ
  int similar();                      // �����
  int w_euclid();                     // �ŤߤĤ��桼����åɵ�Υ
  int output();                       // ǧ����̽���
  int usage(char *);                  // �Ȥ���

  
  /* ---------- �����ο�������å� --------- */
  if(argc != 5){                      // argc : �����ο�
    usage(argv[0]); exit(0);
  }
   
  /********** �ǡ������� **********/
  input(argv[2], argv[3], argv[4]); 

  /********** ���饹ʿ�Ѥ�׻� **********/
  mean();
  
  
  int flg = atoi(argv[1]);            // ��1�����Ǽ��̴������
  
  switch(flg){
  case 1  :     /* flg == 1 : Nearest Neighbor         */
    nearest();  break;
  case 2  :     /* flg == 2 : �桼����åɵ�Υ         */
    euclid();   break;
  case 3  :     /* flg == 3 : �����                   */
    similar();  break;
  case 4  :     /* flg == 4 : �ŤߤĤ��桼����åɵ�Υ */
    w_euclid(); break;
  default :
    fprintf(stderr, "argument error !!"); break;
  }

  /********** ǧ����̽��� **********/
  output();
  
  
  return(0);
}


/* ---------------------------------------------------- */
/*                      �ǡ�������                      */
/* ---------------------------------------------------- */
int input(char *trainfile1, char *trainfile2, char *testfile){
  
  FILE *fin1, *fin2, *fin3;  // �ե�����ݥ���
  int i, j;
        
  /* -----------  �ե�����open  ---------- */
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
  
  
  /* ---------- �ǡ������ɤ߹��� ---------- */
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
  
  /* ---------- �ե�����close ---------- */
  fclose(fin1); fclose(fin2); fclose(fin3);
  
  return 0;
}


/* ---------------------------------------------------- */
/*                   ���饹ʿ�Ѥ�׻�                   */
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
    min1 = min2 = 9999999; // ����ͤ�����
    
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

    /* �Ƕ�˵���Υ��饹��ǧ����̤Ȥ��� */
    if(min1 <= min2) result[k] = 1;
    else result[k] = 2;
  }

  return 0;
}


/* ---------------------------------------------------- */
/*                   �桼����åɵ�Υ                   */
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
/*                        �����                        */
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
/*                �ŤߤĤ��桼����åɵ�Υ              */
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
/*                      ��̽���                        */
/* ---------------------------------------------------- */
int output(){

  FILE *fout;
  int k;
  int cor;

  /* ---------- �ե�����open ---------- */
  fout = fopen("result.dat", "w");
  if( fout == NULL){
    fprintf(stderr, "file open error : result.dat \n"); return -1;
  }

  
  cor = 0;
  /* ---------- �ǡ������� ---------- */
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

  /* ---------- �ե�����close ---------- */
  fclose(fout);

  return 0;
}


int usage(char *arg0){
  
  fprintf(stderr, "Usage : %s [1,2,3,4] train_data1 train_data2 test_data \n", arg0);
  fprintf(stderr, "       1)  Nearest Neighborˡ\n");
  fprintf(stderr, "       2)  �桼����åɵ�Υ\n");
  fprintf(stderr, "       3)  �����\n");
  fprintf(stderr, "       4)  �ŤߤĤ��桼����åɵ�Υ\n");

  return 0;
}
