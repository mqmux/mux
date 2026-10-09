#include <stdio.h>
/* #include <stdlib.h> */

/// <summary>
/// 比较两个文件内容是否相同
/// </summary>
/// <param name="file1">文件1的路径</param>
/// <param name="file2">文件2的路径</param>
/// <returns>是否相同，1相同，0不相同，有一个文件不同返回2 </returns>
int Compare(const char * file1, const char* file2)
{
	FILE* f1, * f2;
	int size1, size2;
	unsigned char buffer1[1024], buffer2[1024];
	f1 = fopen(file1, "r");
	f2 = fopen(file2, "r");
	int isTheSame = 1;
	if (!f1){
		isTheSame = 2;
		goto end;
	}
	if (!f2){
		isTheSame = 2;
		goto end;
	}
	while (1)
	{
		size1 = fread(buffer1, 1, 1024, f1);
		size2 = fread(buffer2, 1, 1024, f2);
		if (size1 != size2)
		{
			goto end;
		}
		if (size1 < 1)
			break;
		for (int i = 0; i < size1; i++)
		{
			if (buffer1[i] != buffer2[i])
			{
				goto end;
			}
		}
	}
	isTheSame = 0;
end:
	if (f1)
		fclose(f1);
	if (f2)
		fclose(f2);
	return isTheSame;
}

int main(int argc,char *argv[])
{
	if( argc==3 ) 
		return Compare(argv[1], argv[2]);
	else printf("Usage: %s [file1] [file2]\n",argv[0]);
}

/* #include<stdio.h>
#include <windows.h>
 
int main()
{
	FILE *f1,*f2;
	char ch1,ch2;
	char fileName1[100],fileName2[100];
 
 	printf("Enter first filename :\n");
 	scanf("%s",fileName1);
 
	printf("Enter second filename :\n");
 	scanf("%s",fileName2);
 
	if(((f1=fopen(fileName1,"r"))==0) || ((f2=fopen(fileName2,"r"))==0))
	{
		printf("Files do not exist!!\n");	
		return 0;	
	}
	do
	{
		ch1=fgetc(f1);
		ch2=fgetc(f2);
		if(ch1!=ch2)
		{
			printf("different！\n") ;
			return 0;
		}
	}while(ch1!=EOF || ch2!=EOF);
	printf("same！\n");
 
	system("pause");
 
	return 0;
} */

/* #include<iostream>
#include<string>
 
using namespace std;
int main()
{
	string t,ans,ans2;
	int i;
	freopen("in1.txt","r",stdin);
	char c;
	while(scanf("%c",&c)!=EOF) ans+=c;
	fclose(stdin);
	freopen("in2.txt","r",stdin);
	while(scanf("%c",&c)!=EOF) ans2+=c;;
	fclose(stdin);
	if(ans.size()!=ans2.size()){cout<<"NO\n";return 0;}
	for(i=0;i<ans.size();i++)
		if(ans[i]!=ans2[i]){cout<<"NO\n";return 0;}
	cout<<"YES\n";
	return 0;
} */