#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int i;
	int j;
}Node;

int main()
{
	Node n,m;
	n.i = 1;
	n.j = 2;
	
	FILE *fp;
	if ((fp = fopen("1.txt","w+")) == NULL)
	{
		perror("open error");
		exit(0);
	}
	fwrite(&n,sizeof(Node),1,fp);
	
	fseek(fp,0,SEEK_SET);
	//rewind(fp);直接到头
	fread(&m,sizeof(Node),1,fp);
	printf("%d,%d\n",m.i,m.j);
	fclose(fp);
	return 0;
}
