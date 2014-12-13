#include "gkdtree.h"
#include "../nifticlib-2.0.0/niftilib/nifti1_io.h"


#define LENGTHFEA 18
#define NUMCLASS 20

//incremental_knn21.exe C:\cygwin64\home\Yuhang\visceral\visceral-dataset\trainingset\volumes\temp\rawimage\10000021_1_CT_wb.nii C:\cygwin64\home\Yuhang\visceral\visceral-dataset\trainingset\volumes\temp\segment\size8\10000021_1_CT_wb.nii_label246719.txt C:\cygwin64\home\Yuhang\visceral\visceral-dataset\trainingset\volumes\temp\features\size8\10000021_1_CT_wb.nii_svfeature.txt C:\cygwin64\home\Yuhang\visceral\visceral-dataset\trainingset\volumes\temp\voxellabel\21-voxellabel.txt 1

//exe id
int main(int argc, char *argv[])//test sort
{
	gkdtree prototype(LENGTHFEA+4);
	char *fname = new char[500];
	sprintf(fname,"C:\\cygwin64\\home\\Yuhang\\visceral\\visceral-dataset\\trainingset\\volumes\\temp\\rawimage\\100000%d_1_CT_wb.nii",atoi(argv[1]));
	nifti_image * nim =  nifti_image_read(fname,1);
	float * mydata = (float*)nim->data;
	sprintf(fname,"C:\\cygwin64\\home\\Yuhang\\visceral\\visceral-dataset\\trainingset\\volumes\\temp\\segment\\size8\\100000%d_1_CT_wb.nii.txt",atoi(argv[1]));
	FILE *fp_seg = fopen(fname,"r");
	sprintf(fname,"C:\\cygwin64\\home\\Yuhang\\visceral\\visceral-dataset\\trainingset\\volumes\\temp\\features\\size8\\100000%d_1_CT_wb.nii_svfeature.txt",atoi(argv[1]));
	FILE *fp_fea = fopen(fname,"r");
	sprintf(fname,"C:\\cygwin64\\home\\Yuhang\\visceral\\visceral-dataset\\trainingset\\volumes\\temp\\voxellabel\\%d-voxellabel.txt",atoi(argv[1]));
	FILE *fp_lab = fopen(fname,"r");


	

	int tempi;

	fscanf(fp_seg,"%d",&tempi);


	vector<vector<double> > features;
	features.push_back(vector<double>(0));
	
	while(fscanf(fp_fea,"%d",&tempi)==1)
	{
		vector<double> tempv(0);
		for(int i=0;i<LENGTHFEA;i++)
		{
			double tempd;
			fscanf(fp_fea,"%lf",&tempd);
			tempv.push_back(tempd);
		}
		features.push_back(tempv);
	}
	fclose(fp_fea);



	float minvox,maxvox;

	minvox = mydata[0];
	maxvox = mydata[0];

	for(int i=0; i<nim->nvox ;i++)
	{
		if(minvox>=mydata[i]) minvox=mydata[i];
		if(maxvox<=mydata[i]) maxvox=mydata[i];
	}

	float maxgap = maxvox-minvox;

	for(int i=0;i<nim->dim[1];i++)
	{
		printf("i=%d / %d %d\n",i,nim->dim[1],prototype.getsize());
		for(int j=0;j<nim->dim[2];j++)
		{
			//if(i==82) printf("j=%d / %d\n",j,nim->dim[2]);
			for(int k=0;k<nim->dim[3];k++)
			{

				if(i==82&&j==220&&k==396)printf("ijk = %d %d %d\n",i,j,k);
				vector<double> feature(0);
				feature.push_back(double(i)/nim->dim[1]);
				feature.push_back(double(j)/nim->dim[2]);
				feature.push_back(double(k)/nim->dim[3]);
				feature.push_back(double(mydata[k*nim->dim[1]*nim->dim[2]+i*nim->dim[2]+j]-minvox)/maxgap);

				fscanf(fp_seg,"%d",&tempi);
				//printf("seg=%d\n",tempi);

				for(int ii=0;ii<LENGTHFEA;ii++)
				{				
					feature.push_back(features[tempi][ii]);
					//printf("push%f\n",features[tempi][ii]);getchar();
				}


				//printf("featuresize=%d\n",feature.size());getchar();

				fscanf(fp_lab,"%d",&tempi);
				//if(i==82&&j==220&&k==396)printf("check in\n");
				prototype.checkandadd(feature,tempi);
				//if(i==82&&j==220&&k==396)printf("check out\n");
				if(i%50==0&&j==0&&k==0) 
				{
					sprintf(fname,"tree_%d_%d.txt",atoi(argv[1]),prototype.getsize());
					prototype.savekdtree(fname);
					//delete fname;
				}

				feature.clear();

			}
		}
	}
	prototype.rebuild();

	sprintf(fname,"tree_%d_%d_final.txt",atoi(argv[1]),prototype.getsize());
	prototype.savekdtree(fname);


	return 0;
}