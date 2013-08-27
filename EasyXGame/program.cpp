#include"game.h"
#include"films.h"
#include<conio.h>
#include<stdio.h>
#include<time.h>
#include<graphics.h>
#include<string.h>
#pragma comment(lib,"Winmm.lib")
#define NEEDED 1
#define WINDOWX 640
#define WINDOWY 480
#define RUN 0
#define LIMIT 30;
#define ADDFB(vtype,vfilm,vx) for(i=0;(feedback[i].film>0)&&(i<10);++i);feedback[i].type=2;feedback[i].film=80;feedback[i].x=point->x/10-118;
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ?1:0)
#define PUTUSER(z,x,y,number) putimage(x,y,z##W,z##H,&z##b,z##W*number,0,SRCAND);putimage(x,y,z##W,z##H,&z,z##W*number,0,SRCPAINT)
#define PUTITEM(z,x,y,number) putimage(x,y,z##W,z##H,&z##b,z##W*number,0,SRCAND);putimage(x,y,z##W,z##H,&z,z##W*number,0,SRCPAINT);point->film++;if(point->film>=z##N)point->film=0;
struct effect
{
	int film;
	int type;
	int x;
};
struct catcher
{
	float x;
	float y;
	float speed;
	float xspeed;
	int film;
	float hp;
	bool left;
	bool move;
} ;
bool mousem=FALSE,gameon=FALSE,soundon=FALSE;
IMAGE pu,pub,ba,bab,bas,basb,oa,oab,wa,wab,bo,bob,bg,bomb,got,lost,dr,drb,dl,dlb;
MOUSEMSG mouse={0,0,0,0,0,0,0,0,0};
target *head=NULL,*point=NULL,*pre=NULL;
DWORD WINAPI getsound(void* FileName)
{
	PlaySound((char*)FileName,NULL, SND_FILENAME|SND_ASYNC);
	return 1;
}
void fadeout(bool selection,int width = WINDOWX,int height = WINDOWY)
{
	static bool needed;
	if(selection==1)
	{
		needed=TRUE;
		return;
	}
	if(needed==FALSE)
		return;
	IMAGE temp;
	getimage(&temp,0,0,width,height);
	DWORD*	pBuffer;	
	pBuffer = GetImageBuffer(NULL);
	for(double j=0;j<40;++j)
	{
		putimage(0,0,&temp);
		for(int i = width*height - 1; i >= 0; i--)
			if (pBuffer[i] != 0)
			{
				pBuffer[i] = RGB(GetRValue(pBuffer[i])*(j/40), GetGValue(pBuffer[i])*(j/40), GetBValue(pBuffer[i])*(j/40));
			}
			FlushBatchDraw();
			Sleep(5);
	}
	needed=FALSE;
	return;
}
target* fruitcreater(target* head,int width = WINDOWX)
{
	int i=rand();
	target* point;
	point=head;
	head=(targets*)malloc(sizeof(targets));
	head->film=0;
	head->speed=(9+i%9);
	head->type=(i%6);
	head->x=(i%(width*10));
	head->y=-10;
	head->node=point;
	return head;
}
int control(MOUSEMSG* mouse)
{
	if(MouseHit()==1)
		*mouse=GetMouseMsg();
	FlushMouseMsgBuffer();
	if(kbhit()==1)
		return(getch());
	return 0;
}
void fade(bool selection,int width = WINDOWX,int height = WINDOWY)
{
	static bool needed;
	if(selection==1)
	{
		needed=TRUE;
		return;
	}
	if(needed==FALSE)
		return;
	DWORD*	pBuffer;	
	pBuffer = GetImageBuffer(NULL);
	for(int j=0;j<40;++j)
	{
		for(int i = width*height - 1; i >= 0; i--)
			if (pBuffer[i] != 0)
			{
				pBuffer[i] = RGB(GetRValue(pBuffer[i])*0.9, GetGValue(pBuffer[i])*0.9, GetBValue(pBuffer[i])*0.9);
			}
			FlushBatchDraw();
			Sleep(6);
	}
	needed=FALSE;
	return;
}
void loadimages(void)
{
	loadimage(&dlb,"IMAGE","dlb");
	loadimage(&dlb,"IMAGE","dlb");
	loadimage(&dl,"IMAGE","dl");
	loadimage(&drb,"IMAGE","drb");
	loadimage(&dr,"IMAGE","dr");
	loadimage(&bg,"IMAGE","bg");
	loadimage(&pu,"IMAGE","pu45");
	loadimage(&got,"IMAGE","got");
	loadimage(&lost,"IMAGE","lost");
	loadimage(&bomb,"IMAGE","bmb");
	loadimage(&pub,"IMAGE","pub90");
	loadimage(&ba,"IMAGE","ba52");
	loadimage(&bab,"IMAGE","bab60");
	loadimage(&bas,"IMAGE","bas59");
	loadimage(&basb,"IMAGE","basb65");
	loadimage(&oa,"IMAGE","oa760");
	loadimage(&oab,"IMAGE","oab35");
	loadimage(&wa,"IMAGE","wa52");
	loadimage(&wab,"IMAGE","wab85");
	loadimage(&bo,"IMAGE","bo80");
	loadimage(&bob,"IMAGE","bob75");
}
void setrecordtime(char *timechar)
{
	time_t timep;
	time(&timep);
	struct tm *temptime=localtime(&timep);
	sprintf(timechar,"%4d-%2d-%2d %2d:%2d",1900+temptime->tm_year,temptime->tm_mon+1,temptime->tm_mday,temptime->tm_hour,temptime->tm_min);
}
void initialization(void)
{
	initgraph (WINDOWX,WINDOWY);
	BeginBatchDraw();
	setbkmode(TRANSPARENT);
	setcolor(WHITE);
	setfillcolor(WHITE);
}
bool freepoint()
{
	if(head==point)
	{
		head=point->node;
		free(point);
		point=head;
		if(point==NULL)
			return 1;
		return 0;
	}
	pre->node=point->node;
	free(point);
	point=pre;
	return 0;
}
int  playercontrol(catcher* catcher)
{			
	if(KEYDOWN(VK_ESCAPE)||KEYDOWN(VK_MBUTTON)||KEYDOWN(VK_SPACE))
	{
		gameon=TRUE;
		return 10;
	}
	if(mousem==TRUE)
	{
		if(MouseHit()==TRUE)
			mouse=GetMouseMsg();
		if(KEYDOWN(VK_LBUTTON)==TRUE&&catcher->y==3800)
			catcher->speed=30;
		if ((mouse.x-10)>catcher->x/10)
		{
			catcher->move=TRUE;
			catcher->left=FALSE;
			if(KEYDOWN(VK_RBUTTON))
			{
				if((catcher->film+=2)>=80)
					catcher->film=1;
				catcher->x+=39;
			}
			else
			{
				if(++catcher->film>=80)
					catcher->film=1;
				catcher->x+=19;
			}
		}
		if((mouse.x+10)<catcher->x/10)
		{
			catcher->move=TRUE;
			catcher->left=TRUE;
			if(KEYDOWN(VK_RBUTTON))
			{
				if((catcher->film+=2)>=80)
					catcher->film=1;
				catcher->x-=39;
			}
			else
			{
				if(++catcher->film>=80)
					catcher->film=1;
				catcher->x-=19;
			}
		}
	}
	else
	{
		if(KEYDOWN(VK_UP)&&catcher->y==3800)
			catcher->speed=30;
		if(KEYDOWN(VK_LEFT))
		{
			catcher->move=TRUE;
			catcher->left=TRUE;
			if(KEYDOWN(VK_SHIFT))
			{
				if((catcher->film+=2)>=80)
					catcher->film=1;
				catcher->x-=39;
			}
			else
			{
				if(++catcher->film>=80)
					catcher->film=1;
				catcher->x-=19;
			}
		}

		else
			if(KEYDOWN(VK_RIGHT))
			{
				catcher->move=TRUE;
				catcher->left=FALSE;
				if(KEYDOWN(VK_SHIFT))
				{
					if((catcher->film+=2)>=80)
						catcher->film=1;
					catcher->x+=39;
				}
				else
				{
					if(++catcher->film>=80)
						catcher->film=1;
					catcher->x+=19;
				}
			}
	}
	if(catcher->x>6000)
		catcher->x=6000;
	else
		if(catcher->x<400)
			catcher->x=400;
	catcher->y-=catcher->speed;
	catcher->x+=catcher->xspeed;
	if(catcher->xspeed!=0)
	{
		if(catcher->xspeed>=9)
			catcher->xspeed-=5;
		if(catcher->xspeed<=-9)
			catcher->xspeed+=5;
		if(catcher->xspeed>-9&&catcher->xspeed<9)
			catcher->xspeed=0;
	}
	if(catcher->y>=3800)
	{
		catcher->y=3800;
		catcher->speed=0;
	}
	else
		catcher->speed--;
	return 3;
}
bool fruitcomputendraw(catcher* catcher,effect* feedback,scoreset* cscore)
{
	int i;
	bool deleted=FALSE;
	for(pre=point=head;point!=NULL;pre=point,point=point->node)
	{
		if(point->y>catcher->y+100&&(catcher->x+200>(point->x-200)&&(point->x+200)>catcher->x-200)&&point->y<catcher->y+1000)
		{
			switch(point->type)
			{
			case(0):
				catcher->hp+=180;
				cscore->score+=10;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=0;
				feedback[i].film=LIMIT;
				feedback[i].x=catcher->x/10;
				break;
			case(1):
				catcher->hp+=80;
				cscore->score+=5;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=0;
				feedback[i].film=LIMIT;
				feedback[i].x=catcher->x/10;
				break;
			case(2):
				catcher->hp+=250;
				cscore->score+=15;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=0;
				feedback[i].film=LIMIT;
				feedback[i].x=catcher->x/10;
				break;
			case(3):
				catcher->hp+=120;
				cscore->score+=7;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=0;
				feedback[i].film=LIMIT;
				feedback[i].x=catcher->x/10;
				break;
			case(4):
				catcher->hp+=170;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=0;
				feedback[i].film=LIMIT;
				feedback[i].x=catcher->x/10;
				cscore->score+=10;
				break;
			case(5):
				catcher->hp-=2000;
				cscore->score-=30;
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=2;
				feedback[i].film=80;
				feedback[i].x=point->x/10-118;
				if(point->x+100>=catcher->x)
					catcher->xspeed=-230;
				else
					catcher->xspeed=230;
				if(!soundon)
				{
					CreateThread (NULL,0,getsound,"explode.wav",0,NULL);
				}
				break;
			default:
				break;
			}
			if(point->type!=5&&!soundon)
				{
					CreateThread (NULL,0,getsound,"get.wav",0,NULL);
				}
			deleted=TRUE;
			if(freepoint())
				break;
			continue;
		}
		if(point->y>5400)
		{
			if(point->type!=5)
			{
				for(i=0;(feedback[i].film>0)&&(i<10);++i);
				feedback[i].type=1;
				feedback[i].film=LIMIT;
				switch(point->type)
				{
				case(0):
					feedback[i].x=point->x/10-35;
					break;
				case(1):
					feedback[i].x=point->x/10-50;
					break;
				case(2):
					feedback[i].x=point->x/10-47;
					break;
				case(3):
					feedback[i].x=point->x/10-63;
					break;
				case(4):
					feedback[i].x=point->x/10-37;
					break;
				default:
					break;
				}
			}
			deleted=TRUE;
			if(freepoint())
				break;
			continue;
		}
		switch(point->type)
		{
		case(0):
			PUTITEM(pu,point->x/10-45,point->y/10-70,point->film)
				break;
		case(1):
			PUTITEM(ba,point->x/10-30,point->y/10-45,point->film)
				break;
		case(2):
			PUTITEM(bas,point->x/10-32,point->y/10-55,point->film)
				break;
		case(3):
			PUTITEM(oa,point->x/10-17,point->y/10-33,point->film)
				break;
		case(4):
			PUTITEM(wa,point->x/10-42,point->y/10-65,point->film)
				break;
		case(5):
			PUTITEM(bo,point->x/10-37,point->y/10-80,point->film);
			break;
		}
		(point->y)+=(point->speed);
	}
	return deleted;
}
int main()
{
	int i=0,count=0,mode=10;
	long timer;
	struct effect feedback[10];
	memset(feedback,0,sizeof(feedback));
	struct catcher catcher={0,3800,0,0,0,6000,0,0};
	scoreset cscore={0,0,0},highscore=reader(0),temp[10];
	// intro
	initialization();
	char buffer[30];
	loadimages();
	setrecordtime(cscore.time);
	sprintf(buffer,"%.5d",highscore.score);
	srand((unsigned int)time(NULL));
	if(!InputBox(cscore.name,10,"请输入新玩家名称(最多10字)\n按\"取消\"使用现有用户名","请输入新玩家名称",cscore.name,300,120,FALSE))
		mode=5;
	while(mode!=0)
	{	
		for (int i=0;i<10;i++)
			temp[i]=reader(i);
		highscore=reader(0);
		sprintf(buffer,"%.5d",highscore.score);
		setfont(18, 0,"黑体");
		fadeout(1);
		fade(NEEDED);
		fade(RUN);
		while(mode==10)
		{
			i=control(&mouse);
			mode=menu(i,mouse.x,mouse.y,KEYDOWN(VK_LBUTTON),gameon);
			setcolor(WHITE);
			outtextxy(435,425,"最高得分：");
			outtextxy(515,427,buffer);
			outtextxy(435,445,"玩家名称：");
			outtextxy(515,447,cscore.name);
			fadeout(0);
			FlushBatchDraw();
			Sleep(10);
		}
		fadeout(NEEDED);
		fade(NEEDED);
		fade(RUN);
		fade(NEEDED);
		while(mode==1)
		{
			cleardevice();
			i=control(&mouse);
			mode=scorelist(i,mouse.x,mouse.y,KEYDOWN(VK_LBUTTON));
			for(int i=0;i<10;i++)
			{
				setcolor(BLACK);
				temp[0]=reader(i);
				sprintf(buffer,"%.5d",temp[0].score);
				outtextxy(40,145+25*i,buffer);
				outtextxy(130,145+25*i,temp[0].name);
				outtextxy(320,145+25*i,temp[0].time);
			}
			fadeout(0);
			FlushBatchDraw();
			Sleep(10);
		}
		while(mode==2)
		{
			cleardevice();
			i=control(&mouse);
			mode=options(i,mouse.x,mouse.y,KEYDOWN(VK_LBUTTON),&mousem,&soundon,cscore.name);
			fadeout(0);
			FlushBatchDraw();
			Sleep(10);
		}
		setcolor(WHITE);
		while(mode==5)
		{
			cleardevice();
			i=control(&mouse);
			outtextxy(40,40,"请用鼠标选择要使用的用户名,按ESC取消");
			if(KEYDOWN(VK_ESCAPE))
			{
				mode=10;
				break;
			}
			for(int j=0;j<10;j++)

			{	
				for(i=0;i<j;i++)
					if(!strcmp(temp[j].name,temp[i].name))
						i=16;
				if(i>j)
					continue;
				if(mouse.y>145+25*j&&mouse.y<165+25*j&&KEYDOWN(VK_LBUTTON))
				{
					if(reader(j).score==0)
						continue;
					cscore=reader(j);
					cscore.score=0;
					setrecordtime(cscore.time);
					mode=10;
				}
				outtextxy(290,145+25*j,temp[j].name);
			}
			fadeout(0);
			FlushBatchDraw();
			Sleep(10);
		}
		if(mode==4)
		{
			recoder(&cscore);
			//清空数据
			if(head!=NULL)
			{
				setrecordtime(cscore.time);
				catcher.x=3200,catcher.speed=0,catcher.y=3800,catcher.xspeed=0,catcher.film=0,catcher.hp=6000;
				point=head->node;
				free(head);
				head=NULL;
				while(point!=NULL)
				{
					pre=point;
					point=point->node;
					free(pre);
				}
			}
			cscore.score=0;
			count=0;
			mode=3;
		}
		//游戏模式
		setrecordtime(cscore.time);
		setfont(35, 0,"宋体");
		fadeout(NEEDED);
		fade(NEEDED);
		setcolor(WHITE);
		timer=clock();
		while(mode==3)
		{
			if(((clock()-timer))<10)
			{
				continue;
			}
			timer=clock();
			catcher.move=FALSE;
			if((catcher.hp-=2)>6000)
				catcher.hp=6000;
			if(catcher.hp<0)
			{
				recoder(&cscore);
				fade(RUN);
				setcolor(WHITE);
				outtextxy(250,210,"游戏结束");
				outtextxy(250,240,"得分");
				sprintf(buffer,"%.5d",cscore.score);
				outtextxy(340,240,buffer);
				FlushBatchDraw();
				Sleep(3000);
				cscore.score=0;
				gameon=FALSE;
				mode=10;
				break;
			}
			putimage(0,0,&bg);
			//接操作&结束或暂停游戏判定
			mode=playercontrol(&catcher);
			//接绘图
			if(catcher.move==FALSE)
				catcher.film=0;
			if(catcher.left==TRUE)
			{PUTUSER(dl,catcher.x/10-29,catcher.y/10,(19-(int)(catcher.film/4)));}
			else
			{PUTUSER(dr,catcher.x/10-29,catcher.y/10,(int)(catcher.film/4));}
			//链表产生
			if((rand()%100)<=3&&count<10)
			{
				head=fruitcreater(head);
				++count;
			}

			//水果绘图与判定
			if(fruitcomputendraw(&catcher,feedback,&cscore)==TRUE)
				--count;
			//对勾叉子绘制
			for(i=0;i<10;++i)
			{
				if(feedback[i].film==0)
					continue;
				switch(feedback[i].type)
				{
				case 0:
					putimage(feedback[i].x-40,300,80,80,&got,0,0,SRCPAINT);
					--feedback[i].film;
					break;
				case 1:
					putimage(feedback[i].x,400,80,80,&lost,0,0,SRCPAINT);
					--feedback[i].film;
					break;
				case 2:
					putimage(feedback[i].x,277,320,133,&bomb,302*(20-feedback[i].film/4),0,SRCPAINT);
					--feedback[i].film;
					break;
				}
			}
			//score
			sprintf(buffer,"%.10d",cscore.score);
			outtextxy(400,10,buffer);
			//catcher.hp bar;
			setfillcolor(RGB(255,(catcher.hp*255)/6000,0));
			solidrectangle(0,10,--catcher.hp/20,30);
			fadeout(RUN);
			FlushBatchDraw();
			FlushMouseMsgBuffer();
		}
		if(cscore.score>highscore.score)
			highscore.score=cscore.score;
		sprintf(buffer,"%.10d",highscore.score);
		while(_kbhit())
			_getch();
	}
	recoder(&cscore);
	return 1;
}
