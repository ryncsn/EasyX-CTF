typedef struct targets
{
	short type;
	int film;
	float x;
	float y;
	float speed;
	struct targets* node;
}target;

typedef struct scores
{
	long score;
	char time[17];
	char name[10];
}scoreset;
int menu(int input,int x,int y,bool click,bool gameon);
int scorelist(int input,int x,int y,bool click);
int options(int input,int x,int y,bool click,bool* mouse,bool* sound,char* name);
int recoder(scoreset* score);
void deleterecord(void);
scoreset reader(int number);