#include"game.h"
#include<conio.h>
#include<graphics.h>
int menu(int input,int mx,int my,bool click,bool mode)
{
	IMAGE title;
	char buffer[30];
	static int point;
	loadimage(&title,"IMAGE","title");
	// draw menu
	putimage(0,0,&title);
	if(mode==0)
	{
		setfillcolor(WHITE);
		bar(440,165,570,210);
	}
	if(mx>445&&my>115&&mx<570&&my<160)
		point=0;
	if(mx>445&&my>160&&mx<570&&my<203&&mode==TRUE)
		point=1;
	if(mx>445&&my>203&&mx<570&&my<247)
		point=2;
	if(mx>445&&my>247&&mx<570&&my<290)
		point=3;
	if(mx>445&&my>290&&mx<570&&my<333)
		point=4;
	if(input==0xE0)
	{
		switch(getch())
		{
		case(72):
			point--;
			if(mode==0&&point==1)
				point=0;
			if(point==-1)
				point=4;
			break;
		case(80):
			point++;
			if(mode==0&&point==1)
				point=2;
			if(point==5)
				point=0;
			break;
		}
	}
	setcolor(BLACK);
	line(445,155+point*44,570,155+point*44);
	if(input==27)
		return 0;
	if(input=='\r'||click==TRUE)
	{
		return 4-point;
	}
	return 10;
}
int scorelist(int input,int mx,int my,bool click)
{
	IMAGE bg;
	scoreset temp;
	char buffer[30];
	loadimage(&bg,"IMAGE","scorelist");
	putimage(0,0,&bg);
	if(mx>540&&my>420&&mx<600&&my<460&&click==TRUE)
		return 10;
	if(input==27)
		return 10;
	return 1;
}
int options(int input,int mx,int my,bool click,bool* mouse,bool* sound,char* name)
{
	IMAGE bg;
	loadimage(&bg,"IMAGE","option");
	putimage(0,0,&bg);
	setfillcolor(WHITE);
	if(*mouse==TRUE)
	{
		bar(400,180,440,210);
	}
	else
	{
		bar(210,180,250,210);
	}
	if(*sound==TRUE)
	{
		bar(210,216,250,258);
	}
	if(mx>90&&my>175&&mx<260&&my<216&&click==TRUE)
		*mouse=TRUE;
	if(mx>280&&my>175&&mx<450&&my<216&&click==TRUE)
		*mouse=FALSE;	
	if(mx>90&&my>216&&mx<260&&my<258&&click==TRUE)
	{
		(*sound==TRUE)?*sound=FALSE:*sound=TRUE;
		Sleep(50);
	}
	if(mx>90&&my>258&&mx<210&&my<300&&click==TRUE)
	{
		deleterecord();
		setcolor(BLACK);
		outtextxy(300,300,"删除成功！");
		FlushBatchDraw();
		Sleep(2000);
		setcolor(WHITE);
	}
	if(mx>540&&my>420&&mx<600&&my<460&&click==TRUE)
		return 10;
	if(mx>90&&my>300&&mx<210&&my<340&&click==TRUE)
		if(!InputBox(name,10,"请输入新玩家名称(最多10字)\n按\"取消\"使用现有用户名","请输入新玩家名称",name,300,120,FALSE))
			return 5;
	if(input==27)
		return 10;
	return 2;
}