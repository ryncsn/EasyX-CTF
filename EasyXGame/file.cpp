#include"game.h"
#include<stdio.h>
scoreset reader(int num)
{
	scoreset buffer={0,0,0};
	FILE* saved;
	saved=fopen("save.dat","rb");
	if(saved==NULL)
		return buffer;
	fseek(saved,num*sizeof(scoreset),SEEK_SET);
	fread(&buffer,sizeof(scoreset),1,saved);
	fclose(saved);
	return buffer;
}
int recoder(scoreset* score)
{
	if (score->score==0)
		return 0;
	int i;
	scoreset bunch[10],buffer[10];
	for(i=0;i<10;++i)
		buffer[i]=reader(i);
	FILE* saved;
	saved=fopen("save.dat","wb");
	if(saved==NULL)
		return NULL;
	if(score==NULL)
		return NULL;
	for(i=0;i<10;++i)
	{
		if(score->score>(buffer[i].score))
		{
			bunch[i]=*score;
			bunch[i+1]=buffer[i];
			++i;
			break;
		}
		else
			bunch[i]=buffer[i];
	}
	while(i<9)
	{
		bunch[i+1]=buffer[i];
		i++;
	}
	fwrite(bunch,sizeof(scoreset),10,saved);
	fclose(saved);
	return 1;
}
void deleterecord(void)
{
	scoreset buffer={0,0,0};
	int i;
	FILE* saved;
	saved=fopen("save.dat","wb");
	for(i=0;i<10;++i)
		fwrite(&buffer,sizeof(scoreset),1,saved);
	fclose(saved);
}