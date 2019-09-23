//Copyright: My team
//Author: 刘衍涛，陈彦任，陈玥，应飞，马晓雯
//Date：2019-09-04
//Version: 3.1
//Description: Provide a Movie Ticket Sale System for customers and managers
//History: 

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<ctime>
#include<algorithm>
#include<windows.h>
using namespace std;

//global variables
int orderNumber,userNumber,adminNumber,stageNumber,houseNumber,cinemaNumber;
int stageID[15][1005];       //[date][userID] 1 for ordered, 0 for not ordered
int customerStage[15];       //[date] stand for the number of stages
int localTime, localDate;    //system time
int usNum;                   //the login customer ID 
int  languagemark;		     //mark Chinese or English
char movieName[81];          //save the movie's name entered by the customer
char cinemaName[20];         //save the cinema's name entered by the customer
int save[100];				//save the serial numbers of movies filtered after searching name
int save2[100];				//save the serial numbers of movies after filtering language
int save3[100];				//save the serial numbers of movies after filtering type
int  Cnt, Cnt2, Cnt3;		//the number of movies after searching name, filtering language and filtering type
int cusSearchFlag = 0;		//judge whether the customer choose to search names
int L=0,R,C;                // L as a level for judging the seat is good or not, L C for seat
int gender = 0;             //judge the customer's gender
int dateOp;					//record the customer's choice of ticket date


//define struct 
struct Cinema{
	char cinemaName[100];	//cinema name
	int houseNum;			//serial number of house
	int occupyMap[5][37];	//
	int sum[5][160];
	int tag[5][160];
	//attention! 1 ~ 4 house 1 ~ 36 time!
}threatre[10000];	//actually is 'theatre'

struct User{
	char id[11];     //ID 
	char key[80];    //password
	char name[11];   //name
	char mail[20];   //mail
	char phone[20];  //phone number
	char sex[10];    //gender
	double money;    //balance
	int StageNum;    //user's stage
}user[100000];

struct Admin{
	char id[11];     //id
	char key[80];    //password
	char name[80];   //name
	char cinema[80]; //the admin belongs to which cinema
	int cinemaid;    //cinema ID
	char mail[80];   //mail
	char sex[20];    //gender
	char phone[20];  //phone number 
}admin[100000];

struct House{
	char cinema[10]; //cinema name
	int houseID;     //house number
	char type[10];   //movie type e.g. 4D 3D
	int row;         //total rows
	int col;         //total columns
	int totseat;     //total seats
	int available;  //has been used or not used 1 available 0
}house[100000];

struct Order{
	char id[11];     //customer's ID
	char cinema[11]; //cinema
	char movie[20];  //movie name
	int house;       //house number
	int stime;       //movie start time
	int date;        //display date
	int buyDate;     //buy date
	double price;    //price
	int row;         //which row
	int col;         //which column
	int snum;        //which stage
}order[100000];

struct Stage{
	char id[100];		//stage ID
	char cinema[20];	//cinema
	char movie[30];		//movie name
	int date;			//display date
	int house;			//movie house number
	int stime;			//movie start time
	int etime;			//movie end time
	double hours;		//how long
	int rest;			//number of rest seats
	int totseat;		//number of total seats
	double price;		//movie price
	char language[20];	//languange Chinese or English
	char type[10];		//2D,3D,4D,imax
	double discount;	//price discount
	int seat[50][50];	//record the seat
	int row;			//seat row
	int col;			//seat column
	double hot;    //hot extent  
}stage[100000];

//hot extent of stage (used in the function recommendMovie())
struct Sthot {
	double hot;		
	int num;		//save the serial number of stage
}sthot[100000];

//current price of stage  (used in sortMoviePrice())
struct Stprice
{
	double currentprice;
	int num;		//save the serial number of stage
}stprice[100000];

//start time of stage (used in sortMovieTime())
struct Sttime
{
	int stime;
	int num;		//save the serial number of stage
}sttime[100000];

//rest seats of stage (used in sortMovieRest())
struct Strest
{
	int rest;
	int num;		//save the serial number of stage
}strest[100000];

//define function
int getAllAdminData();				//get managers' data from file to memory, return the number of managers
int getAllStageData();				//get stages' data from file to memory, return the number of stages
int getAllOrderData();				//get orders' data from file to memory, return the number of orders
int getAllUserData();				//get users' data from file to memory, return the number of customers
int getAllHouseData();				//get houses' data from file to memory, return the number of houses
void rewriteAllAdminData();			//rewrite manager's data into file
void rewriteAllStageData();			//rewrite stage's data into file
void rewriteAllUserData();			//rewrite user's data into file
void rewriteAllOrderData();			//rewrite order's data into file
int getSystemDate();				//return the local system date
int getSystemTime();				//return the local system time

void customerPart();				//customer first menu
void customer(int);					//customer second menu

void checkMovie();					//check and buy tickets
bool cmphot(Sthot, Sthot);			//used in sorthot
void recommendMovie();				//recommend movies according to hot
void printMovieTable(int, int);		//print movie information in a table
void printTableHeader();			//print the header of table
void searchMovieName();				//customer search movie's name
void searchCinemaName();			//customer search cinema's name
void searchBothName();				//customer search both names
void printMovieName();				//after searching movie's name, print the information for today
void printMovieNameOther();			//after searching movie's name, print the information for tomorrow or the day after tomorrow
void printMovieCinema();			//after searching cinema's name, print the information for today
void printMovieCinemaOther();		//after searching cinema's name, print the information for tomorrow or the day after tomorrow
void printMovieBoth();				//after searching both names, print the information for today
void printMovieBothOther();			//after searching both names, print the information for tomorrow or the day after tomorrow
void filterLanguage();				//filter movie's language
void filterType();					//filter cinema's language
void printMovieLanguage(int);		//after filtering movie's language, print the information
void printMovieType(int);			//after filtering movie's type, print the information
bool cmpprice(Stprice, Stprice);	//called in sortMoviePrice
bool cmptime(Sttime, Sttime);		//called in sortMovieTime
bool cmprest(Strest, Strest);		//called in sortMovieRest
int sortMovie();					//sort movie
int sortMoviePrice();				//sort by price
int sortMovieTime();				//sort by start time	
int sortMovieRest();				//sort by rest seats


void chooseSeat(int, int);
void printColorSeat(int);
bool checkSeat(int [],int [],int ,Stage);
void recommendSeat1(int,int [],int [],int,Stage);
void recommendSeat2(int,int [],int [],int,Stage);
void recommendSeat3(int,int [],int [],int,Stage);
void purchase(int,double,int,int,int [],int[] ); 
void recharge(int,int);
void addOrder(int,int,int [],int [],int,double);

int login();						//customer login, return a number as id of the user

void refund(int);					//cancel orders and refund the customer the ticket price

void Register();					//register
void registerGender();				//record gender when register
void registerPhone();				//record phone number when register
void registerMail();				//record mail address when register
bool isEmail(char *);				//judge whether the format of email is right

void reback();						//find password back

void revise();
void scan();

void build(int ,int ,int ,int ,int); 
void printOrderMessage(int, int, int[], int[], int, double);		//print order information after purchasing for customer to confirm

//define admin funcs
bool AdminLogin();								//adminlogin if success return 1 failed return 0;
void AdminTab(char id[], char cinema[]);		//admin function tab
int  checkDay(int mouth, int day); 				//check day and mouth are consistent or not
void checkUserOrder(char cinema[]);				//check user order of specific cinema
void cntUserOrder(char cinema[]);				//count user order, and sort stages by income, attendence rate
void manageAdmin(char cinema[],char id[]);		//updata the mail or password of admin
void manageStage(char cinema[]);				//query existing stages or add new stage
void manageHouse(char cinema[]);				//query the info of house or put house into use

//define the func of segment tree
int ls(int x);
int rs(int x);
void build(int,int,int,int,int);
void push_up(int p,int c,int h);
void push_down(int p,int c,int h,int l,int r);
void updata(int,int,int,int,int,int,int,int); 
int  query(int,int,int,int,int,int,int); 


int checkInt(int minn,int maxx)
//as function atoi will return 0 when it fails to tranform 
//so we build this func to make sure the num user enter is vaild
{
	printf("Plz enter the num between %d and %d\n",minn,maxx);
	while(1)
	{
		char temp[100];
		scanf("%s",temp);
		if(!atoi(temp) || atoi(temp) > maxx || atoi(temp) < minn)
			printf("The Num You Enter is Wrong,plz try again\n");
		else
			return atoi(temp);
			
	}
}
double checkDouble(double minn,double maxx)
//as function atof will return 0 when it fails to tranform 
//so we build this func to make sure the num user enter is vaild
{
	printf("Plz enter the num between %.2lf and %.2lf\n",minn,maxx);
	while(1)
	{
		char temp[100];
		scanf("%s",temp);
		if(!atof(temp) || atof(temp) > maxx || atof(temp) < minn)
			printf("The Num You Enter is Wrong,plz try again\n");
		else
			return atof(temp);
			
	}
}

int getAllAdminData()
//the return value is the num of Admin
{
	FILE *in;
	in = fopen("Admin.txt","r");
	if (in == NULL)
	{
		printf("Failed to open the file.\n");
		exit(1);
	}
	int cnt = 0;
	while(fscanf(in,"%s %s %s %s %d %s %s %s",
	admin[cnt].id,admin[cnt].key,admin[cnt].name,
	admin[cnt].cinema,
	&admin[cnt].cinemaid,
	admin[cnt].sex,admin[cnt].phone,admin[cnt].mail) > 6)
	{
		cnt ++;
	}
	fclose(in);
	return cnt;
}
int getAllStageData()
//get all stages data save from txt to memory
//the return value is the num of stages
{
	FILE *in;
	in = fopen("Stage.txt","r");
	if (in == NULL)
	{
		printf("Failed to open the file.\n");
		exit(1);
	}
	int i = 0;
	while (fscanf(in, "%s %s %s %d %d %d %d %d %d %lf %s %lf %s %d %d",
		stage[i].id, stage[i].cinema, stage[i].movie, &stage[i].date, &stage[i].house,
		&stage[i].stime, &stage[i].etime, &stage[i].rest, &stage[i].totseat,
		&stage[i].price, stage[i].language, &stage[i].discount, stage[i].type, &stage[i].row, &stage[i].col) > 5)
	{
		for(int j=1;j<=stage[i].row;j++)
		{
			for(int k=1;k<=stage[i].col;k++)
			{
				fscanf(in,"%d",&stage[i].seat[j][k]);
			}
		}
		i++;
		if (i >= 100000) break;
	}
	fclose(in);
	stageNumber = i;
	return i;
}
int getAllCinemaData()
//get all cinemas data save it from txt to memory
//the return value is the num of cinemas
{
	FILE *in;
	in = fopen("threatre.txt","r");
	int cnt = 0;
	char temp[100];
	while(fscanf(in,"%d %s",&threatre[cnt].houseNum,threatre[cnt].cinemaName) == 2)
	{
		fgets(temp,100,in);
		for(int j=0;j<=4;j++)
		{
			fgets(temp,100,in);
			for(int i=1;i<=36;i++)
				threatre[cnt].occupyMap[j][i] = temp[i-1] - '0';
			if(j>0)
				build(1,cnt,j,1,36);
				//build segment tree 
		}
		cnt ++; 
	}
	fclose(in);
	return cnt;
}
int getAllHouseData()
//get all houses data save it from txt to memory
//the return value is the num of houses
{
	FILE *in;
	in = fopen("cinema.txt","r");
	int cnt = 0;
	while(fscanf(in,"%s %d %d %d %d %d %s",
	house[cnt].cinema,&house[cnt].houseID,&house[cnt].row,
	&house[cnt].col,
	&house[cnt].totseat,
	&house[cnt].available,house[cnt].type) > 6)
	{
		cnt ++;
	}
	fclose(in);
	return cnt;
}
int getAllUserData()
//the return value is the num of customer
{
	FILE *in;
	in = fopen("Customer.txt","r");
	if (in == NULL)
	{
		printf("Failed to open the file.\n");
		exit(1);
	}
	int cnt = 0;
	while(fscanf(in,"%s %s %lf %s %s %s %s",
	user[cnt].id,user[cnt].key,&user[cnt].money,
	user[cnt].name,
	user[cnt].sex,user[cnt].phone,user[cnt].mail) > 6)
	{
		cnt ++;
	}
	fclose(in);
	return cnt;
}
int getAllOrderData()
//the return value is the num of orders
{
	FILE *in;
	in = fopen("OrderChanged.txt","r");
	if (in == NULL)
	{
		printf("Failed to open the file.\n");
		exit(1);
	}
	int cnt = 0;
	while(fscanf(in,"%s %s %d %s %d %d %lf %d %d %d",
	order[cnt].id,order[cnt].cinema,&order[cnt].buyDate,
	order[cnt].movie,
	&order[cnt].house,
	&order[cnt].stime,&order[cnt].price,&order[cnt].row,&order[cnt].col,&order[cnt].snum) > 7)
	{
		cnt ++;
	}
	fclose(in);
	return cnt;
}

//return the local system date
int getSystemDate()
{
	struct tm*today;
	time_t t;
	t = time(NULL);
	today = localtime(&t);
	printf("Today is %02d%02d",1 + today->tm_mon, today->tm_mday);
	return (1 + today->tm_mon) * 100 + today->tm_mday;
}

//return the local system time
int getSystemTime()
{
	struct tm *local;
    time_t t; 
    t=time(NULL);
    local=localtime(&t);
    printf("Local time is: %02d:%02d\n",local->tm_hour,local->tm_min);
    return local->tm_hour*100 + local->tm_min;
}

//return the offset of left son and right son
inline int ls(int x){return x<<1;}
inline int rs(int x){return x<<1|1;}
//p is the offset of the sum array and tag array
//c is the offset of threatre
//h is the offset of house
inline void push_up(int p,int c,int h)
{
	threatre[c].sum[h][p] = threatre[c].sum[h][ls(p)] + threatre[c].sum[h][rs(p)];
}
//push up the data to his father
void build(int p,int c,int h,int l,int r)
{
	threatre[c].sum[h][p] = 0;
	if(l == r)
	{
		threatre[c].sum[h][p] = threatre[c].occupyMap[h][l];
		return ;
	}
	int mid = (l+r) >> 1;
	build(ls(p),c,h,l,mid);
	build(rs(p),c,h,mid+1,r);
	push_up(p,c,h);
}
//build the segment tree
inline void push_down(int p,int c,int h,int l,int r)
{
	int mid = (l+r) >> 1;
	threatre[c].sum[h][ls(p)] += (threatre[c].tag[h][p] * (mid-l+1) );
	threatre[c].sum[h][rs(p)] += (threatre[c].tag[h][p] * (r-mid) );
	threatre[c].tag[h][ls(p)] += threatre[c].tag[h][p];
	threatre[c].tag[h][rs(p)] += threatre[c].tag[h][p];
	threatre[c].tag[h][p] = 0;
}
//push down the data and lazyTag to his left son and right son 
inline void updata(int p,int c,int h,int l,int r,int xl,int xr,int v)
{
	if(xl <= l && r <= xr)
	{
		threatre[c].sum[h][p] += v * (r-l+1);
		threatre[c].tag[h][p] += v;
		return ; 
	}
	push_down(p,c,h,l,r);
	int mid = (l+r) >> 1;
	if(xl<=mid)
		updata(ls(p),c,h,l,mid,xl,xr,v);
	if(xr >mid)
		updata(rs(p),c,h,mid+1,r,xl,xr,v);
	push_up(p,c,h);
}
//interval modification
int query(int p,int c,int h,int l,int r,int ql,int qr)
{
	int res = 0;
	if(ql <= l && r <= qr)
		return threatre[c].sum[h][p];
	int mid = (l+r) >> 1;
	push_down(p,c,h,l,r);
	if(ql<=mid)
		res += query(ls(p),c,h,l,mid,ql,qr);
	if(qr >mid)
		res += query(rs(p),c,h,mid+1,r,ql,qr);
	return res;
}
//interval query

//Below is for Customers
void customerPart()
{
	system("cls");
	printf("1.Log in\n");
	printf("2.Register\n");
	printf("3.Exit\n");
	bool flag=1;
	while(flag)
	{
		int op;
		op = checkInt(1,3);
		switch(op)
		{
			case 1:
				usNum=login();  //记录一下编号 
				flag=0;
				break; 
			case 2:
				Register();
				flag=0;
				break;
			case 3:
				rewriteAllOrderData();
				rewriteAllStageData();
				rewriteAllUserData();
				exit(0);
			default:
				printf("Wrong number! Please enter again:\n");
		}
	}
	customer(usNum);
}

//find password back
void reback()
{
	printf("Please enter your ID\n");
	User a;
	scanf("%s",a.id);
	bool flag=1;
	int i;
	for(i=0;i<userNumber;i++)
	{
		if(strcmp(a.id,user[i].id) == 0)
		{
			flag=0;
			break;
		}
	}
	if(flag)
	{
		printf("You are a new customer! Please register at first\n");
		Register();
	}
	else
	{
		printf("Please enter your name:\n");
		scanf("%s",a.name);
		while(1)
		{
			if(strcmp(a.name,user[i].name))
			{
				printf("Wrong name! Please enter again!\n");
				scanf("%s",a.name);
			}
			else break;
		}
		printf("Please enter your phone number\n");
		scanf("%s",a.phone);
		while(1)
		{
			if(strcmp(a.phone,user[i].phone))
			{
				printf("Wrong phone number! Please enter again!\n");
				scanf("%s",a.phone);
			}
			else break;
		}
	}
}

//calculate the num of stages in one day the user bought 
void countStageNumber(int stnum)
{
	memset(stageID,0,sizeof(stageID));
	memset(customerStage,0,sizeof(customerStage));
	for(int i=0;i<orderNumber;i++)
	{
		if(strcmp(order[i].id,user[usNum].id) == 0)
		{
			if(stageID[stage[order[i].snum].date-localDate][order[i].snum] == 0)
			{
				stageID[stage[order[i].snum].date-localDate][order[i].snum]=1;
				customerStage[stage[order[i].snum].date-localDate]+=1;
			}
		}
	}
	if(customerStage[stage[stnum].date-localDate] >= 5)
	{
		printf("Sorry!\nBecause you have ordered 5 stages, you cannot order anymore!!Please choose other options.\n");
		getchar();
		getchar();
		return customer(usNum);
		
	}
}

//login and return a number as id of the user
int login()
{
	system("cls");
	User a;
	printf("Please login at first.\n");
	printf("Please input your ID:\n");
	scanf("%s",a.id);
	bool flag=1;
	int i;
	for(i=0;i<userNumber;i++)
	{
		if(strcmp(a.id,user[i].id) == 0)
		{
			flag=0;
			break;
		}
	}
	if(flag)
	{
		system("cls");
		printf("You are a new customer\n");
		printf("Please register at first\n");
		Register();
	}
	else
	{
		int cnt=3;
		bool flag=1;
		while(cnt--)
		{
			printf("Please input your password:\n");
			scanf("%s",a.key);
			if(strcmp(a.key,user[i].key) == 0)
			{
				printf("Log in successfully! \n");
				printf("Press any button to continue\n");
				getchar();
				getchar();
				break;
			}
			else
			{
				printf("Incorrect password! \n");
				if(cnt == 0)
				{
					flag=0;
				}
			}
		}
		if(!flag)
		{
			printf("Forget your password ? Find it back\n");
			reback();
		}
	}
	return i;
}

//the information of gender when register
void registerGender()
{
	while(1)
	{
		printf("Please input a number as your gender: 1 for male,2 for female\n");
		int gender;
		scanf("%d",&gender);
		if(gender == 1)
		{
			strcpy(user[userNumber].sex,"male");
			break;
		}
		else if(gender == 2)
		{
			strcpy(user[userNumber].sex,"female");
			break;
		}
		else printf("Wrong number!\n");
	 } 
} 

//the information of phone number when register
void registerPhone()
{
	char phone[15];    //输入电话 
	while(1)
	{
		printf("Please enter your phone number\n");
	 	scanf("%s",phone);
	 	int l=strlen(phone);
	 	if(l == 11)
	 	{
	 		int cnt=0;
	 		for(int i=0;i<l;i++)
	 		{
	 			if(phone[i] >='0' && phone[i] <= '9')
	 			{
	 				cnt++;
				}
			}
			if(cnt == 11) break;
		 }
	 }
	 strcpy(user[userNumber].phone,phone); 
}

//check the email format
bool isEmail(char *str)
{
	char a1[32]={0};
    char a2[32]={0};
    char a3[32]={0};
    char a4[32]={0};
    if(sscanf(str,"%[a-zA_Z0-9_-]@%[a-zA-Z0-9._-]",a1,a2) < 2)
    {
        return 0;
    }
    if(sscanf(a2,"%[a-zA_Z0-9_-].%[a-zA_Z0-9_-.]",a3,a4) <2)
    {
        return 0;
    }
    int i = 1;
    char c;
    
    while((c=a4[i])!='\0')
    {
        if(c == '.'&&a4[i-1] == '.')
            return 0;
        ++i;
    }
    
    if(a4[0] == '.' || a4[i-1] =='.')
        return 0;
    return 1;
}

//the information of email when register
void registerMail()
{
	char mail[25];
	printf("Please enter you mail address(***@***.***):\n");
	while(1)
	{
		scanf("%s",mail);
		if(isEmail(mail))
		{
			break;
		}
		else printf("Wrong format!\n");
	}
	strcpy(user[userNumber].mail,mail);
}

//register
void Register()
{
	int x=0,y=0;
	char id[15];
	printf("Weicome to register！\n");
	printf("Please enter your ID: "
			"(6~10 bit combination of letters and digits)\n");
	while(1)      //judge the id format
	{
		scanf("%s",id);
		int l=strlen(id);	//length
		if( l <6 || l >10)		//judge the length
		{
			printf("Wrong format!\n");
			printf("Please enter your ID of correct length:\n");
		}
		else		//judge the combination
		{
			for(int i=0;i<l;i++)
			{
				if( ( ('a' <= id[i]) && (id[i] <= 'z')) ||( ('A' <= id[i]) && (id[i] <= 'Z') ) )
				{
					x++;
				}
				else if( ( '0' <= id[i]) && (id[i] <= '9'))
				{
					y++;
				}
			}
			if ((x == 0) || (y == 0))    //wrong format
			{
				printf("Wrong format!\n");
				printf("Please enter your ID containing both letter and digits:");
			}
			else		//id has been registered before
			{
				bool flag=1;
				for(int i=0;i<userNumber;i++)
				{
					if(strcmp(id,user[i].id) == 0)
					{
						printf("The ID has been registered.\n");
						printf("1.Log in\t2.another ID\n");
						printf("Please enter a number\n");
						int op;
						scanf("%d",&op);
						if(op == 1) return customerPart();
						flag=0;  
						break;                           
					}
				}
				if(flag)
				{
					strcpy(user[userNumber].id,id);
					break;
				}
			}
		}
	}	
	printf("Please input your real name: \n");
	scanf("%s",user[userNumber].name);
	
	 printf("Please input your password:\n");
	 scanf("%s",user[userNumber].key);
	 registerGender();
	 registerPhone();
	 registerMail();
	 
	 userNumber++;
	 printf("Register successfully!\n");
	 getchar();
	 getchar();
	 return customerPart();
	 
}
//print order information after purchasing
void printOrderMessage(int stnum,int tNum,int row[],int col[],int usnum,double money)
{
	system("cls");
	printf("Purchase Successful!\n\n");
	printf("\nCustomer ID: %s\n", user[usnum].id);
	printf("Ticket Number: %d\n", tNum);
	printf("Movie Name: %s\n", stage[stnum].movie);
	printf("Cinema Name: %s\n", stage[stnum].cinema);
	printf("House Number: %d\n", stage[stnum].house);
	printf("Movie Date: 19/%02d/%02d\n", stage[order[usnum].snum].date / 100, stage[order[usnum].snum].date % 100);
	printf("Movie Time: %02d:%02d - %02d:%02d\n", stage[stnum].stime / 100, 
	       stage[stnum].stime % 100, stage[stnum].etime / 100, stage[stnum].etime % 100);
	printf("Seat:\n");
	for (int i = 1; i <= tNum; i++)
		printf("  Row %3d	 Column %3d\n", row[i], col[i]);
	printf("Total Price: %6.2lf\n", money);
	printf("Account Balance: %6.2lf\n", user[usnum].money);
	printf("Buy Date: 19/%02d/%02d\n", localDate / 100, localDate % 100);
	printf("Buy Time: %02d:%02d\n", localTime / 100, localTime % 100);
}


void addOrder(int stnum,int tNum,int row[],int col[],int usnum, double money)
{
	int tmp = tNum;		//save the value of tNum

	for(int i=1;i<=tNum;i++)
	{
		stage[stnum].seat[row[i]][col[i]]=1;
		stage[stnum].rest--;
	}
	while(tNum)
	{
		int temp=orderNumber;
		strcpy(order[temp].movie,stage[stnum].movie);
		strcpy(order[temp].cinema, stage[stnum].cinema);
		order[temp].snum=stnum;
		order[temp].date=stage[stnum].date;
		order[temp].house=stage[stnum].house;
		strcpy(order[temp].id,user[usnum].id);
		order[temp].stime=stage[stnum].stime;
		order[temp].row=row[tNum];
		order[temp].col=col[tNum];
		order[temp].price=stage[stnum].price*stage[stnum].discount;
		order[temp].buyDate=localDate;
		orderNumber++;
		tNum--; 
	}
	printf("%d\n",orderNumber);
	printf("Purchase Successful!\n");
	printOrderMessage(stnum,tmp,row,col,usnum,money);
	getchar();
	getchar();
}



void printColorSeat(int state)
{
	HANDLE hd=GetStdHandle(STD_OUTPUT_HANDLE);
	if(state == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_RED | BACKGROUND_INTENSITY);
		printf("   ");
		SetConsoleTextAttribute(hd, 2 | 8);	
	} 
	else if(state == 0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		printf("   ");
		SetConsoleTextAttribute(hd, 2 | 8);
	}
	else if(state == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		printf("   ");
		SetConsoleTextAttribute(hd, 2 | 8);
	}
	else if(state == 3)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_RED | BACKGROUND_GREEN);
		printf("   ");
		SetConsoleTextAttribute(hd, 2 | 8);
	}
}

void recharge(int usnum,int op)
{
	printf("How much money would you like to recharge?\n");
	printf("Please enter a number:\n");
	double money;
	scanf("%lf",&money);
	user[usnum].money+=money;
	printf("Recharged Successfully! The balance now is: %.2lf \n",user[usnum].money);
	getchar();
	getchar();
	if(op) return customer(usnum);
}

void purchase(int stnum,double money,int usnum,int tNum,int row[],int col[])
{
	int op;
	bool flag=0;
	for(int i=0;i<orderNumber;i++)
	{
		if(strcmp(user[usnum].id,order[i].id) == 0 && 
		   stage[stnum].date == stage[order[i].snum].date)
		{
			if((stage[stnum].stime >= stage[order[i].snum].stime 
					&& stage[stnum].stime <= stage[order[i].snum].etime)
				||(stage[stnum].etime >=stage[order[i].snum].stime
					&& stage[stnum].etime <= stage[order[i].snum].etime))
			{
				flag=1;
				break;
			}
		}
	}
	if(flag)
	{
		printf("You have ordered a movie during this movie's playtime\n");
		printf("Are you going to continue purchase?\n");
		printf("1.continue purchasing\t2.back to main page\n");
		int op;
		scanf("%d",&op);
		if(op == 2)
			return customer(usnum);
	}
	flag=0;
	for(int i=0;i<orderNumber;i++)
	{
		if(strcmp(user[usnum].id,order[i].id) == 0 &&
		   strcmp(order[i].movie,stage[stnum].movie) == 0)
		   {
		   		flag=1;
		   		break;
		   }
	}
	if(flag)
	{
		printf("You have ordered the same movie before\n");
		printf("Are you going to continue purchase?\n");
		printf("1.continue purchasing\t2.back to main page\n");
		int op;
		scanf("%d",&op);
		if(op == 2)
			return customer(usnum);
	}
	if(money<user[usnum].money)     //enough rest money 
	{
		user[usnum].money-=money;
		addOrder(stnum,tNum,row,col,usnum,money);
	}
	else if(money>user[usnum].money)
	{
		while(money > user[usnum].money)
		{
			printf("Total is %.2lf . Your Balance is not enough!\n",money);
			printf("Would you like to recharge?\n1.Yes  2.No\n");
			scanf("%d",&op);
			if(op) recharge(usnum,0);
			else return customer(usnum);
		}
		user[usnum].money-=money,addOrder(stnum,tNum,row,col,usnum,money);
	}
	else
	{
		user[usnum].money-=money;
		addOrder(stnum,tNum,row,col,usnum,money);
		printf("After purchasing, the balance is zero!\n");
		printf("Would you like to recharge?\n1.Yes 2.No\n");
		scanf("%d",&op);
		if(op) recharge(usnum,0);
	}
	return customer(usnum);
}

//query the customer's purchase history
void findRecords(int usnum)     
{	
	int cnt = 0;
	printf("\n");
	for(int i=0;i<orderNumber;i++)
	{
		if(strcmp(user[usnum].id,order[i].id) == 0)
		{
			cnt++;
			if (cnt == 1)
			{
				printf("+------------------------------------------------------------------------------------------------------------+\n");
				printf("|   Customer ID   |   Movie Name  | Movie Date |    Time   | Cinema | House | Row | Col | Payment | Buy Date |\n");
				printf("+------------------------------------------------------------------------------------------------------------+\n");
			}
			printf("|    %-11s  |   %-11s | ",order[i].id,order[i].movie);
			printf(" 19/%02d/%02d  | ",stage[order[i].snum].date/100,stage[order[i].snum].date%100);
			printf(" %02d : %02d  ",order[i].stime/100,order[i].stime%100);
			printf("|    %-3s |   %02d  |",order[i].cinema,order[i].house);
			printf("  %-1d  |  %-1d  | %6.2lf  |",order[i].row,order[i].col,order[i].price);
			printf(" 19/%02d/%02d |\n",order[i].buyDate/100,order[i].buyDate%100);
			printf("+------------------------------------------------------------------------------------------------------------+\n");
		}
	}
	if (cnt == 0)
		printf("You haven't purchased any ticket!\n");
	getchar();
	getchar();
	return customer(usnum);
}


bool checkSeat(int row[],int col[],int tNum,Stage a)
{
	bool flag=0;
	for(int i=1;i<=tNum;i++)
	{
		int r=row[i],c=col[i];
		if(a.seat[r][c] == 1)
		{
			flag=1;
		}
		a.seat[r][c]=1;
	}
	for(int i=1;i<=tNum;i++)
	{
		int r=row[i],c=col[i];
		if(c == 1)
		{
			if(a.seat[r][c+1] == 0 && a.seat[r][c+2]== 1)
			{
				flag=1;
				break;
			}
		}
		else if(c == 2)
		{
			if(a.seat[r][1] == 0)
			{
				flag=1;
				break;
			}
		}
		else if(c == a.col)
		{
			if(a.seat[r][c-1] == 0 && a.seat[r][c-2] == 1)
			{

				flag=1;
				break;
			}
		}
		else if(c == a.col-1)
		{
			if(a.seat[r][c+1] == 0)
			{
				flag=1;
				break;
			}
		}
		else
		{
			if((a.seat[r][c-1] == 0 && a.seat[r][c-2] == 1) 
			|| (a.seat[r][c+1] == 0 && a.seat[r][c+2] == 1))
			{
				flag=1;
				break;
			}
		}
	}
	return flag;
}

void recommendSeat1(int stnum,int row[],int col[],int tNum,Stage a)
{
	int midnumr=(stage[stnum].row+1)/2;
	int midnumc=(stage[stnum].col+1)/2;
	int max=10000;
	for(int i=stage[stnum].row;i>=1;i--)
	{
		for(int j=stage[stnum].col;j>=1;j--)
		{
			if(a.seat[i][j] == 0)
			{
				L=(i-midnumr)*(i-midnumr)+(j-midnumc)*(j-midnumc);
				row[tNum]=i;
				col[tNum]=j;
				if(max>L && checkSeat(row,col,tNum,a) == 0)
				{
					max=L;
					R=i,C=j;
				}
			}
		}
	}
}
void recommendSeat2(int stnum,int row[],int col[],int tNum,Stage a)
{
	int max=10000;
	bool flag=1;
	int midnumr=(stage[stnum].row+1)/2;
	int midnumc=(stage[stnum].col+1)/2;
	for(int i=stage[stnum].row;i>=1;i--)
	{
		for(int j=stage[stnum].col-1;j>=1;j--)
		{
			if(a.seat[i][j] == 0 && a.seat[i][j+1] == 0)
			{
				flag=0;
				L=(i-midnumr)*(i-midnumr)+(j-midnumc)*(j-midnumc)
				  +(i-midnumr)*(i-midnumr)+(j-midnumc+1)*(j-midnumc+1);
				if(max>L )
				{
					max=L;
					R=i,C=j;
				}
			}
		}
	} 
	if(flag)
	{
		printf("Sorry! The rest seats in this stage are not available\n");
		printf("Please try other stages or reduce tickets numbers\n");
		printf("Press any button to continue\n");
		getchar();
		getchar();
		return checkMovie();
	 } 
	while(1)
	{
		row[1]=R,row[2]=R;
		col[1]=C,col[2]=C+1;
		if(checkSeat(row,col,tNum,a))
		{
			a.seat[R][C]=1;
			a.seat[R][C+1]=1;
			flag=1; 
			return recommendSeat2(stnum,row,col,tNum,a);
		}
		else break;
	}
}

void recommendSeat3(int stnum,int row[],int col[],int tNum,Stage a)
{
	int max=10000;
	bool flag=1;
	int midnumr=(stage[stnum].row+1)/2;
	int midnumc=(stage[stnum].col+1)/2;
	for(int i=stage[stnum].row;i>=1;i--)
	{
		for(int j=stage[stnum].col-2;j>=1;j--)
		{
			if(a.seat[i][j] == 0 && a.seat[i][j+1] == 0 && a.seat[i][j+2] == 0)
			{
				flag=0;
				L=(i-midnumr)*(i-midnumr)+(j-midnumc)*(j-midnumc)
				   +(i-midnumr)*(i-midnumr)+(j-midnumc+1)*(j-midnumc+1)
				   +(i-midnumr)*(i-midnumr)+(j-midnumc+2)*(j-midnumc+2);
				if(max>L )
				{
					max=L;
					R=i,C=j;
				}
			}
		}
	}
	if(flag)
	{
		printf("Sorry! The rest seats in this stage are not available\n");
		printf("Please try other stages or reduce tickets numbers\n");
		printf("Press any button to continue\n");
		getchar();
		getchar();
		return checkMovie();
	 } 
	while(1)
	{
		row[1]=R,row[2]=R,row[3]=R;
		col[1]=C,col[2]=C+1,col[3]=C+2;
		if(checkSeat(row,col,tNum,a))
		{
			a.seat[R][C]=1;
			a.seat[R][C+1]=1;
			a.seat[R][C+2]=1;
			flag=1;
			return recommendSeat2(stnum,row,col,tNum,a);
		}
		else break;
	}
}

void printSeat(Stage a)
{
	HANDLE hd=GetStdHandle(STD_OUTPUT_HANDLE);
	for(int i=0;i<=a.col;i++)
	{
		printf("%2d ",i);
		if(i == a.col) printf("\n\n");
		else printf(" "); 
	}
	for(int i=1;i<=a.row;i++)
	{
		for(int j=0;j<=a.col;j++)
		{
			if(j == 0) SetConsoleTextAttribute(hd,7),printf("%2d  ",i);
			else if(j == a.col) printColorSeat(a.seat[i][j]),printf("\n\n");
			else printColorSeat(a.seat[i][j]),printf(" "); 
		}
	}
	SetConsoleTextAttribute(hd,7);
}
 
void chooseSeat(int stnum,int usnum)
{
	Stage a;
	User b;
	memcpy(&a,&stage[stnum],sizeof(stage[stnum]));
	memcpy(&b,&user[usnum],sizeof(user[usnum]));
	HANDLE hd=GetStdHandle(STD_OUTPUT_HANDLE);
	printf("%s(%s)\n",a.movie,a.type);
	printf("Cinema %s House %d\n",a.cinema,a.house);
	printf("%02d:%02d - %02d:%02d\n",a.stime/100,a.stime%100,a.etime/100,a.etime%100);
	countStageNumber(stnum);
	printSeat(a);
	bool flag=1,f=1;
	int tNum=4;
	int row[10],col[10];
	printf("We have an extra discount for customers who buy more than 1 ticket.\n"
		"One-time purchase for 2 tickets will enjoy an extra 5%% off, "
		"and for 3 tickets is 10%% off\n\n");
	while(tNum>3)
	{
		printf("Please enter how many tickets would you like? (NO more than 3)\n\n");
		scanf("%d",&tNum);
	}
	if(stage[stnum].rest <tNum)
	{
		printf("Sorry,this stage does not have so many seats left\n");
		printf("Please choose other stages\n");
		getchar();
		getchar();
		return customer(usnum); 
	}
	system("cls"); 
	while(flag)
	{
		flag=0;
		printf("Green - available Red - Reserved  Yellow - Recommend\n\n");
		if(f)
		{
			switch(tNum)
			{
				case 1:
					recommendSeat1(stnum,row,col,tNum,a);
					a.seat[R][C]=3;
					break;
				case 2:
					recommendSeat2(stnum,row,col,tNum,a);
					a.seat [R][C]=3,a.seat[R][C+1]=3;
					break;
				case 3:
					recommendSeat3(stnum,row,col,tNum,a);
					a.seat [R][C]=3,a.seat[R][C+1]=3,a.seat[R][C+2]=3;
					break;
			}
		}
		f=0;
		printSeat(a);
		printf("We do NOT allow there is an available ticket between two reserved seat in a row!\n\n");
		for(int i=1;i<=tNum;i++)
		{
			printf("\nNo.%d Please enter the row:",i);
			scanf("%d",&row[i]);
			printf("\nNo.%d Please enter the col:",i);
			scanf("%d",&col[i]);
		}
		system("cls");
		if(checkSeat(row,col,tNum,a))
		{
			printf("Your choice is not available! Please try it again!\n\n");
			flag=1;
		}
	}
	for(int i=1;i<=tNum;i++)
		a.seat[row[i]][col[i]] = 2;
	system("cls");
	SetConsoleTextAttribute(hd, 7);
	printf("%s(%s)\n",a.movie,a.type);
	printf("Cinema%s House %d\n",a.cinema,a.house);
	printf("%02d:%02d - %02d:%02d\n",a.stime/100,a.stime%100,a.etime/100,a.etime%100);
	printSeat(a);
	double tot=tNum*(a.price*a.discount);
	printf("The total is %lf\n\n",tot);
	if (tNum == 2)
	{
		tot = tot * 0.95;
		printf("Since you want to buy 2 tickets, you will enjoy an extra 5%% discount.\n"
			"So the current money you need to pay is %lf\n\n", tot);
	}
	if (tNum == 3)
	{
		tot = tot * 0.9;
		printf("Since you want to buy 3 tickets, you will enjoy an extra 10%% discount.\n"
			"So the current money you need to pay is %lf\n\n", tot);
	}
	if (gender)
	{
		tot = tot * 0.9;
		printf("Since today is MOVIE DAY for women, you will enjoy an extra 10%% discount;\n"
			"So the current money you need to pay is %lf\n\n", tot);
	}
	printf("Please confirm payment:\n");
	printf("1.Pay\n2.Cancel\n");
	int op;
	scanf("%d",&op);
	system("cls");
	if(op == 1) purchase(stnum,tot,usnum,tNum,row,col);
	else return customer(usnum);	
} 

//cancel orders and refund ticket
void refund(int usnum)    
{
	int cnt=0;
	int re[10005];
	memset(re,0,sizeof(re));
	for(int i=0;i<orderNumber;i++)
	{
		if(strcmp(order[i].id,user[usnum].id) == 0)
		{
			re[++cnt]=i;
			if (cnt == 1)
			{
				printf("All your orders are as follows：\n");
				printf("\n+-----------------------------------------------------------------------------+\n");
				printf("| No.|   Movie Name   | Cinema | House | Row | Col |   Date   |      Time     |");
				printf("\n+-----------------------------------------------------------------------------+\n");
			}
			printf("| %-2d ",cnt);
			printf("| %-13s | %-6s |   %-3d |  %-2d |  %-2d | 19/%02d/%02d |",
					order[i].movie,order[i].cinema,order[i].house,
					order[i].row,order[i].col,
					stage[order[i].snum].date/100,stage[order[i].snum].date%100);
			printf(" %02d:%02d - %02d:%02d |", stage[order[i].snum].stime / 100, stage[order[i].snum].stime % 100, stage[i].etime / 100, stage[i].etime % 100);
			printf("\n+-----------------------------------------------------------------------------+\n");
		}
	}
	if(cnt == 0)
	{
		printf("\nYou haven't bought any tickets\n");
		getchar();
		getchar();
		return customer(usnum);
	} 
	printf("How many orders would you like to cancel? \n");
	int tnum;
	scanf("%d",&tnum);
	while(tnum)
	{
		tnum--;
		printf("Which one do you want to cancel?\nPlease enter the number:");
		int num;
		scanf("%d",&num);
		user[usnum].money+=order[re[num]].price;
		stage[order[re[num]].snum].rest++;
		stage[order[re[num]].snum].seat[order[re[num]].row][order[re[num]].col]=0;//大概要改一下 
		for(int i=re[num];i<orderNumber;i++)
		{
			memcpy(&order[i],&order[i+1],sizeof(order[i+1]));
		}
		orderNumber--;
	}
	if(tnum!=0)
	{
		printf("Cancel successfully!\n");
		getchar();
		getchar();
	} 
	return customer(usnum);
}

//called in recommendMovie()
bool cmphot(Sthot a, Sthot b)
{
	if (a.hot < b.hot) return 1;
	else return 0;
}

//called in sortMoviePrice()
bool cmpprice(Stprice a, Stprice b)
{
	if (a.currentprice < b.currentprice) return 1;
	else return 0;
}

//called in sortMovieTime()
bool cmptime(Sttime a, Sttime b)
{
	if (a.stime < b.stime) return 1;
	else return 0;
}

//called in sortMovieRest()
bool cmprest(Strest a, Strest b)
{
	if (a.rest < b.rest) return 1;
	else return 0;
}

//print movies' information in a table
void printMovieTable(int i, int num)
{
	double sale;		//discount
	printf("| %-2d | %-13s | %-14s | ", num, stage[i].id, stage[i].movie);
	printf("19/%02d/%02d |", stage[i].date / 100, stage[i].date % 100);
	printf(" %02d:%02d - %02d:%02d ", stage[i].stime / 100, stage[i].stime % 100, stage[i].etime / 100, stage[i].etime % 100);
	printf("| %-8s | %-4s | %-14.2lf | ", stage[i].language, stage[i].type, stage[i].price);
	sale = (1 - stage[i].discount) * 100;
	if (sale == 0) printf("No Discount");
	else printf("%.0lf%% off    ", sale);
	printf("| %-13.2lf | %-6s |   %-3d |", stage[i].price*stage[i].discount, stage[i].cinema, stage[i].house);
	printf("  %3d |\n", stage[i].rest);
	printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
}

//recommend movies according to the popularity of movies (hot)
void recommendMovie()
{
	int i;

	for (i = 0; i < stageNumber; i++)
	{
		stage[i].hot = 1 - (double)stage[i].rest / stage[i].totseat;	//hot is related to seat occupancy rate
		sthot[i].hot = stage[i].hot;
		sthot[i].num = i;
	}
	sort(sthot, sthot + stageNumber, cmphot);		//sort from high to low accoding to hot 
	printTableHeader();

	for (i = 0; i < 10; i++)
	{
		save[i] = sthot[i].num;		//save the serial numbers of recommended movies into save[]
		save3[i] = sthot[i].num;		////save the serial numbers of recommended movies into save3[]
		printMovieTable(sthot[i].num, i + 1);
	}
	Cnt = 10;
	Cnt2 = 10;
	Cnt3 = 10;
}

//print the header of table
void printTableHeader()
{
	printf("\n+---------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
	printf("| No.|  Movie Number  |       Name     |   Date   |      Time     | Language | Type | Original Price |  Discount  | Current Price | Cinema | House | Rest |\n");
	printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
}

//sort accoding to price, return the original number of movie to movieNum 
int sortMoviePrice()
{
	int i;
	int op;		//customer's option

	for (i = 0; i < Cnt3; i++)
	{
		stprice[i].currentprice = stage[save3[i]].price * stage[save3[i]].discount;
		stprice[i].num = save3[i];
	}
	sort(stprice, stprice + Cnt3, cmpprice);		//sort from high to low accoding to price
	printTableHeader();
	for (i = 0; i < Cnt3; i++)
		printMovieTable(stprice[i].num, i + 1);
	printf("\nPlease enter the serial number of the movie you like: (from 1 to %d)\n", Cnt3);
	while (1)
	{
		scanf("%d", &op);
		if (op >= 1 && op <= Cnt3)
			return stprice[op - 1].num;
		else printf("Wrong input! Please enter again:");
	}
	return 0;
}

//sort accoding to start time, return the original number of movie to movieNum 
int sortMovieTime()
{
	int i;
	int op;		//customer's option

	for (i = 0; i < Cnt3; i++)
	{
		sttime[i].stime = stage[save3[i]].stime;
		sttime[i].num = save3[i];
	}
	sort(sttime, sttime + Cnt3, cmptime);
	printTableHeader();
	for (i = 0; i < Cnt3; i++)
	{
		//save[i] = sthot[i].num;		//save the serial numbers of recommended movies into save[]
		printMovieTable(sttime[i].num, i + 1);
	}
	printf("\nPlease enter the serial number of the movie you like: (from 1 to %d)\n", Cnt3);
	while (1)
	{
		scanf("%d", &op);
		if (op >= 1 && op <= Cnt3)
			return sttime[op - 1].num;
		else printf("Wrong input! Please enter again:");
	}
	return 0;
}

//sort accoding to rest seats, return the original number of movie to movieNum 
int sortMovieRest()
{
	int i;
	int op;		//customer's option

	for (i = 0; i < Cnt3; i++)
	{
		strest[i].rest = stage[save3[i]].rest;
		strest[i].num = save3[i];
	}
	sort(strest, strest + Cnt3, cmprest);
	printTableHeader();
	for (i = 0; i < Cnt3; i++)
		printMovieTable(strest[i].num, i + 1);
	printf("\nPlease enter the serial number of the movie you like: (from 1 to %d)\n", Cnt3);
	while (1)
	{
		scanf("%d", &op);
		if (op >= 1 && op <= Cnt3)
			return strest[op - 1].num;
		else printf("Wrong input! Please enter again:");
	}
	return 0;
}

//sort movies, return the original number of movie to chooseSeat()
int sortMovie()
{
	int op;		//customer's option
	int movieNum = 0;
	int flag = 1;

	printf("\nHow do you want to sort the movies?\n");
	printf("1.Ticket price\n");
	printf("2.Start time\n");
	printf("3.Remaining number of seats\n");
	while (1)
	{
		scanf("%d", &op);
		switch (op)
		{
			case 1:		//sort by price
				movieNum = sortMoviePrice();
				break;
			case 2:		//sort by start time
				movieNum = sortMovieTime();
				break;
			case 3:		//sort by rest seats
				movieNum = sortMovieRest();
				break;
			default:	//invalid input
				flag = 0;
				printf("Wrong input! Please enter again:\n");
		}
		if (flag) break;
	}

	return movieNum;
}

//search by entering movie's name 
void searchMovieName()
{
	system("cls");
	printf("Please enter the movie's name to search:\n");
	rewind(stdin);
	scanf("%s", movieName);		//read the movie's name into a global variable movieName
	printMovieName();
}

//search by entering cinema's name 
void searchCinemaName()
{
	system("cls");
	printf("Please enter the cinema's name to search:：\n");
	rewind(stdin);
	scanf("%s", cinemaName);	//read the cinema's name into a global variable cinemaName
	printMovieCinema();
}

//search by entering both movie's name and cinema's name 
void searchBothName()
{
	system("cls");
	printf("Please enter the movie's name and cinema's name to search:\n");
	rewind(stdin);
	scanf("%s %s", movieName, cinemaName);	//read the movie's name into a global variable movieName
											//and the cinema's name into a global variable cinemaName
	printMovieBoth();
}

//print movie's information filtered by entering both movie's and cinema's names (today)
void printMovieBoth()
{
	int i, flag = 1;
	int num = 0;		//count the number of movies satisfying the condition 

	memset(save3, 0, sizeof(save3));	//set the array save3[] to 0
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(movieName, stage[i].movie) == 0 && strcmp(cinemaName, stage[i].cinema) == 0
			&& localDate == stage[i].date && localTime < stage[i].stime - 5)
		{
			save[num] = i;	//save the serial numbers of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial numbers of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				printf("This is information about today's movies\n\n");
				printTableHeader();
			}				
			printMovieTable(i, num);
		}
	}
	Cnt = num;
	Cnt3 = num;
	if (num == 0)		//no movie today
	{
		printf("\nThis movie is not on show in this cinema today. \n\n");
		printf("You can choose to enter:\n"
			"  1.search the same movie on show in this cinema tomorrow\n"
			"  2.search the same movie on show in this cinema the day after tomorrow\n"
			"  3.return to the previous menu\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
			case 1:		//tomorrow
				printMovieBothOther();
				break;
			case 2:		//the day after tomorrow
				printMovieBothOther();
				break;
			case 3:		//return
				return searchBothName();
			default:
				printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
	else		//have movie today
	{
		printf("\nIf you want to buy tickets for the next 2 days, enter:\n"
			"  1.tomorrow\n  2.the day after tomorrow\n  3.just today\n\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
			case 1:		//tomorrow
				printMovieBothOther();
				break;
			case 2:		//the day after tomorrow
				printMovieBothOther();
				break;
			case 3:		//today
				break;
			default:
				printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
}

//print movie's information filtered by entering both movie's and cinema's names (2 other days)
void printMovieBothOther()
{
	int i;
	int num = 0;		//the number of movies satisfying the condition

	memset(save3, 0, sizeof(save3));
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(movieName, stage[i].movie) == 0 
			&& strcmp(cinemaName, stage[i].cinema) == 0 
			&& (localDate + dateOp) == stage[i].date)
		{
			save[num] = i;	//save the serial number of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial numbers of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				if (dateOp == 1)
					printf("This is information about tomorrow's movies\n");
				else printf("This is information about the day after tomorrow's movies\n");
				printTableHeader();
			}
			printMovieTable(i, num);
		}
	}
	if (num == 0)
	{
		if (dateOp == 1)
			printf("\nThis movie is not on show in this cinema tomorrow.\n");
		else printf("\nThis movie is not on show in this cinema the day after tomorrow.\n");
		printf("Press Enter and you'll return to the previous menu.\n");
		getchar();
		getchar();
		return searchBothName();
	}
	Cnt = num;
	Cnt3 = num;
}

//print movie's information filtered by the movie's name (today)
void printMovieName()
{
	int i, flag = 1;
	int num = 0;		//count the number of movies satisfying the condition 

	memset(save3, 0, sizeof(save3));
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(movieName, stage[i].movie) == 0 && localDate == stage[i].date && localTime < stage[i].stime - 5)
		{
			save[num] = i;	//save the serial number of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial numbers of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				printf("This is information about today's movies\n\n");
				printTableHeader();
			}
			printMovieTable(i, num);
		}
	}
	Cnt = num;
	Cnt3 = num;
	if (num == 0)
	{
		printf("\nThis movie is not on show in our cinemas today. \n\n");
		printf("You can choose to enter:\n"
			"  1.search the same movie on show tomorrow\n"
			"  2.search the same movie on show the day after tomorrow\n"
			"  3.return to the previous menu\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
				case 1:		//tomorrow
					printMovieNameOther();
					break;
				case 2:		//the day after tomorrow
					printMovieNameOther();
					break;
				case 3:		//return
					return searchMovieName();
				default:
					printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
	else
	{
		printf("\nIf you want to buy tickets for the next 2 days, enter:\n"
			"  1.tomorrow\n  2.the day after tomorrow\n  3.just today\n\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
			case 1:
				printMovieNameOther();		//tomorrow
				break;
			case 2:		//the day after tomorrow
				printMovieNameOther();
				break;
			case 3:		//today
				break;
			default:
				printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
}

//print movie's information filtered by the movie's name (2 other days)
void printMovieNameOther()
{
	int i;
	int num = 0;		//the number of movies satisfying the condition		

	memset(save3, 0, sizeof(save3));
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(movieName, stage[i].movie) == 0 && (localDate + dateOp)== stage[i].date)
		{
			save[num] = i;	//save the serial number of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial numbers of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				if (dateOp == 1)
					printf("This is information about tomorrow's movies\n");
				else printf("This is information about the day after tomorrow's movies\n");
				printTableHeader();
			}
			printMovieTable(i, num);
		}
	}
	if (num == 0)
	{
		if (dateOp == 1)
			printf("\nThis movie is not on show in our cinemas tomorrow.\n");
		else printf("\nThis movie is not on show in our cinemas the day after tomorrow.\n");
		printf("Press Enter and you'll return to the previous menu.\n");
		getchar();
		getchar();
		return searchMovieName();
	}
	Cnt = num;
	Cnt3 = num;
}

//print movie's information filtered by the cinema's name (today)
void printMovieCinema()
{
	int i, flag = 1;
	int num = 0;		//count the number of movies satisfying the condition 

	memset(save3, 0, sizeof(save3));
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(cinemaName, stage[i].cinema) == 0 && localDate == stage[i].date && localTime < stage[i].stime - 5)
		{
			save[num] = i;		//save the serial number of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial number of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				printf("This is information about today's movies\n\n");
				printTableHeader();
			}
			printMovieTable(i, num);
		}
	}
	Cnt = num;
	Cnt3 = num;
	if (num == 0)
	{
		printf("\nThis cinema does not have any movie on show today. \n\n");
		printf("You can choose to enter:\n"
			"  1.search the same cinema's movies tomorrow\n"
			"  2.search the same cinema's movies the day after tomorrow\n"
			"  3.return to the previous menu\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
			case 1:		//tomorrow
				printMovieCinemaOther();
				break;
			case 2:		//the day after tomorrow
				printMovieCinemaOther();
				break;
			case 3:		//return
				return searchCinemaName();
			default:
				printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
	else
	{
		printf("\nIf you want to buy tickets for the next 2 days, enter:\n"
			"  1.tomorrow\n  2.the day after tomorrow\n  3.just today\n\n");
		while (1)
		{
			scanf("%d", &dateOp);
			switch (dateOp)
			{
			case 1:		//tomorrow
				printMovieCinemaOther();
				break;
			case 2:		//the day after tomorrow
				printMovieCinemaOther();
				break;
			case 3:		//today
				break;
			default:
				printf("Wrong input! Please enter again:\n\n");
			}
			if (flag) break;
		}
	}
}

//print movie's information filtered by the cinema's name (2 other days)
void printMovieCinemaOther()
{
	int i;
	int num = 0;		//the number of movies satisfying the condition

	memset(save3, 0, sizeof(save3));
	printf("\n");
	for (i = 0; i < stageNumber; i++)
	{
		if (strcmp(cinemaName, stage[i].cinema) == 0 && (localDate + dateOp) == stage[i].date)
		{
			save[num] = i;	//save the serial number of movies satisfying the condition into a new array save[]
			save3[num] = i;		//save the serial numbers of movies satisfying the condition into a new array save3[]
			num++;
			if (num == 1)
			{
				if (dateOp == 1)
					printf("This is information about tomorrow's movies in this cinema\n");
				else printf("This is information about the day after tomorrow's movies in this cinema\n");
				printTableHeader();
			}
			printMovieTable(i, num);
		}
	}
	if (num == 0)
	{
		if (dateOp == 1)
			printf("\nThis cinema does not have any movie on show tomorrow.\n");
		else printf("\nThis cinema does not have any movie on show the day after tomorrow.\n");
		printf("Press Enter and you'll return to the previous menu.\n");
		getchar();
		getchar();
		return searchCinemaName();
	}
	Cnt = num;
	Cnt3 = num;
}

//filter the movie's language
void filterLanguage()
{
	char choice;
	int flag = 1;		//whether the input is valid
	printf("\nPlease choose the filter condition：\n\n"
		"Choose the language:\n 1.Chinese\n 2.English\n 3.All\n"
		" 4.Return\n\n");
	while (1)
	{
		rewind(stdin);
		scanf("%c", &choice);
		switch (choice)
		{
		case '1':		//filter Chinese
			printMovieLanguage(1);
			languagemark = 1;
			break;
		case '2':		//filter English
			printMovieLanguage(2);
			languagemark = 2;
			break;
		case '3':		//all
			printMovieLanguage(3);
			languagemark = 3;
			break;
		case '4':		//return
			return checkMovie();
		case '0':		//quit the program
			exit(0);
		default:
			flag = 0;		//invlid input, continue the while loop
			printf("Wrong input! Please enter again:\n\n");
		}
		if (flag) break;		//valid input, quit the loop
	}
}

//filter the movie's type
void filterType()
{
	char choice;
	int flag = 1;		//whether the input is valid

	printf("\n\nChoose the type:\n 1.2D\n 2.3D\n 3.4D\n 4.iMax\n 5.All\n"
		" 6.Return\n\n" );//7.Quit\n\n");
	while (1)
	{
		rewind(stdin);
		scanf("%c", &choice);
		switch (choice)
		{
		case '1':		//filter 2D 
			printMovieType(1);
			break;
		case '2':		//filter 3D 
			printMovieType(2);
			break;
		case '3':		//filter 4D 
			printMovieType(3);
			break;
		case '4':		//filter imax
			printMovieType(4);
			break;
		case '5':		//all
			printMovieType(5);
			break;
		case '6':		//return
			return filterLanguage();
//		case '7':		//quit the program
//			return;
		default:
			flag = 0;
			printf("Invalid input! Please type in again:\n\n");
		}
		if (flag) break;
	}
	//printf("\nSuccess\n");
}

//print movie's information filtered by language
void printMovieLanguage(int choice)
{
	int i;
	int num = 0;		//the number of movies satisfying the conditions
	int flag = 0;		//whether the customer has chosen the language or just want to see all
	const char *temp = "Chinese";

	memset(save3, 0, sizeof(save3));	//set the array save3[] to 0

	//decide temp to be 'Chinese' or 'English'
	switch (choice)
	{
	case 1:		//choose Chinese
		flag = 1;		//has chosen the language
		break;
	case 2:		//choose Chinese
		flag = 1;		//has chosen the language
		temp = "English";
		break;
	case 3:		//all
		break;
	default:
		printf("\nErorr!");
	}

	if (flag)		//if has chosen the language
	{
		for (i = 0; i < Cnt; i++)
		{
			if (strcmp(temp, stage[save[i]].language) == 0)		//if the language chosen is the same as the movie
			{
				save2[num] = save[i];		//save the serial numbers of movies satisfying the condition into save2[]
				save3[num] = save[i];		//save the serial numbers of movies satisfying the condition into save3[]
				num++;
				if (num == 1)
					printTableHeader();
				printMovieTable(save[i], num);
			}
		}	//end of for
		if(num == 0)		//no movie
		{
			printf("\nThere's no movie of this type. Press Enter and return.");
			getchar();
			getchar();
			return checkMovie();
		}
		Cnt2 = num;
		Cnt3 = num;
	}	//end of if

	//if the customer didn't choose any language, just repeat the result before
	else if (cusSearchFlag == 1)		//in checkMovie function the customer choose to search by entering movie's name 
	{
		if (dateOp == 0)	printMovieName();
		else printMovieNameOther();
	}
	else if (cusSearchFlag == 2)		//in checkMovie function the customer choose to search by entering cinema's name 
	{
		if (dateOp == 0)	printMovieCinema();
		else printMovieCinemaOther();
	}
	else if (cusSearchFlag == 3)		//in checkMovie function the customer choose to search by entering both names 
	{
		if (dateOp == 0)	printMovieBoth();
		else printMovieBothOther();
	}
	else		//in checkMovie function the customer choose to filter the recommended movies 
		recommendMovie();
}

//print movie's information filtered by type
void printMovieType(int choice)
{
	int i;
	int num = 0;		//the number of movies satisfying the conditions
	int typeflag = 0;		//whether choose the type
	int langflag = 0;		//whether choose the language
	const char *temp1 = "2D";

	memset(save3, 0, sizeof(save3));

	//decide the content of temp1 (type)
	switch (choice)
	{
	case 1:		//2D
		typeflag = 1;
		break;
	case 2:		//3D
		typeflag = 1;
		temp1 = "3D";
		break;
	case 3:		//4D
		typeflag = 1;
		temp1 = "4D";
		break;
	case 4:		//imax
		typeflag = 1;
		temp1 = "IMAX";
		break;
	case 5:		//all
		break;
	default:
		printf("\nErorr!");
	}

	//decide the content of temp2 (language)
	switch (languagemark)
	{
	case 1:		//Chinese
		langflag = 1;
		break;
	case 2:		//English
		langflag = 1;
		break;
	case 3:		//all
		break;
	default:
		printf("\nErorr!");
	}

	if (langflag)		//if has chosen the language
	{
		if (typeflag)		//if has chosen the type
		{
			for (i = 0; i < Cnt2; i++)
			{
				if (strcmp(temp1, stage[save2[i]].type) == 0)
				{
					save3[num] = save2[i];		//save the serial numbers of movies satisfying the condition into save3[]
					num++;
					if (num == 1)		//print the header for just one time
						printTableHeader();
					printMovieTable(save2[i], num);
				}
			}
			if (num == 0)		//no movie
			{
				printf("\nThere's no movie of this type. Press Enter and return.");
				getchar();
				getchar();
				return checkMovie();
			}
			Cnt3 = num;
		}
		else		//if has chosen language, but hasn't chosen type 
			printMovieLanguage(languagemark);
	}
	else		//if hasn't chosen language
	{
		if (typeflag)		//if has chosen type
		{
			for (i = 0; i < Cnt; i++)
			{
				if (strcmp(temp1, stage[save[i]].type) == 0)
				{
					save3[num] = save[i];		//save the serial numbers of movies satisfying the condition into save3[]
					num++;
					if (num == 1)		//print the header for just one time
						printTableHeader();
					printMovieTable(save[i], num);
				}
			}
			if(num == 0)		//no movie
			{
				printf("\nThere's no movie of this type. Press Enter and return.");
				getchar();
				getchar();
				return checkMovie();
			}
			Cnt2 = num;
			Cnt3 = num;
		}

		//if the customer hasn't chosen any language or type, just repeat the result before
		else if (cusSearchFlag == 1)		//in checkMovie function the customer choose to search by entering movie's name 
			printMovieName();
		else if (cusSearchFlag == 2)		//in checkMovie function the customer choose to search by entering cinema's name 
			printMovieCinema();
		else if (cusSearchFlag == 3)		//in checkMovie function the customer choose to search by entering both names 
			printMovieBoth();
		else		//in checkMovie function the customer choose to filter the recommended movies 
			recommendMovie();
	}
}

//check the movies' information and buy tickets
void checkMovie()
{
	int op;		//customer's first option
	int flag = 1;	//whether valid input
	int ch;		//customer's second option
	localTime = getSystemTime();
	system("cls");
	if (strcmp(user[usNum].sex, "female") == 0 && (localTime % 100) == 5)
	{
		printf("WOW! TODAY is our MOVIE DAY for WOMEN!\n"
			"On the 5th of every month, women will have an extra 10%% off for tickets!\n");
		gender = 1;
	}
	printf("\nBelow are our top 10 movies we recommend:\n");
	recommendMovie();		//print 10 recommended movies
	printf("\nAre you satisfied with our recommendation?"
		"\n  If yes, enter the number of the movie you want to see(1~10)"
		"or enter any other number (except 0~10) to go to filter more conditions"
		"\n  If no, enter '0' to choose other methods to search\n");
	scanf("%d", &op);		//read the first option
	if (op >= 1 && op <= 10)		//if choose one recommended film
		chooseSeat(save[op - 1], usNum);		//go to choose seats and buy tickets directly
	else if (op == 0)		//if other methods to search
	{
		printf("You can also choose to search by entering"
			"\n  1.the movie's name"
			"\n  2.the cinema's name"
			"\n  3.both the movie's name and cinema's name\n");
		getchar();
		while (1)
		{
			scanf("%d", &ch);		//read the second option
			switch (ch)
			{
			case 1:		//search by entering the movie's name
				cusSearchFlag = 1;
				searchMovieName();
				filterLanguage();
				filterType();
				chooseSeat(sortMovie(), usNum);
				system("pause");
				break;
			case 2:
				cusSearchFlag = 2;		//search by entering the cinema's name
				searchCinemaName();
				filterLanguage();
				filterType();
				chooseSeat(sortMovie(), usNum);
				system("pause");
				break;
			case 3:		//search by entering both names
				cusSearchFlag = 3;
				searchBothName();
				filterLanguage();
				filterType();
				chooseSeat(sortMovie(), usNum);
				system("pause");
				break;
			default:		//invalid input
				printf("Wrong input! Please enter again: ");
				flag = 0;
			}
			if (flag) break;
		}
	}
	else		//if want to filter more conditions about the 10 recommended movies
	{
		filterLanguage();
		filterType();
		chooseSeat(sortMovie(), usNum);
		system("pause");
	}
}


void rewriteAllAdminData()
//cover the txt doc to save all admin data 

{
	FILE *out;
	out = fopen("Admin.txt","w");
	
	for(int i=0;i<adminNumber;i++)
	{
		fprintf(out,"%s %s %s %s %d %s %s %s\n",
		admin[i].id,admin[i].key,admin[i].name,
		admin[i].cinema,
		admin[i].cinemaid,
		admin[i].sex,admin[i].phone,admin[i].mail);
	}
	fclose(out);
}

void rewriteAllStageData()
//cover the txt doc to save all stages data 
{
	FILE *out;
	out = fopen("stage.txt","w");
	
	for(int cnt=0;cnt<stageNumber;cnt++)
	{
		fprintf(out,"%s %s %s %04d %d %d %d %d %d %.2lf %s %.2lf %s %d %d\n",
		stage[cnt].id,stage[cnt].cinema,stage[cnt].movie,stage[cnt].date,stage[cnt].house,
		stage[cnt].stime,stage[cnt].etime,stage[cnt].rest,stage[cnt].totseat,
		stage[cnt].price,stage[cnt].language,stage[cnt].discount,stage[cnt].type,stage[cnt].row,stage[cnt].col);
		for(int i=1;i<=stage[cnt].row;i++)
		{
			for(int j=1;j<=stage[cnt].col-1;j++)
				fprintf(out,"%d ",stage[cnt].seat[i][j]);
			fprintf(out,"%d\n",stage[cnt].seat[i][stage[cnt].col-1]);
		}
	}
	fclose(out);
}
void rewriteAllHouseData()
//cover the txt doc to save all house data 

{
	FILE *out;
	out = fopen("cinema.txt","w");
	
	for(int cnt=0;cnt<houseNumber;cnt++)
		fprintf(out,"%s %d %d %d %d %d %s\n",
		house[cnt].cinema,house[cnt].houseID,house[cnt].row,
		house[cnt].col,
		house[cnt].totseat,
		house[cnt].available,house[cnt].type);
		
	fclose(out);
}

void rewriteAllCinemaData()
//cover the txt doc to save all cinema data 
{
	FILE *out;
	out = fopen("threatre.txt","w");
	for(int cnt = 0;cnt < cinemaNumber;cnt ++){
		fprintf(out,"%d %s\n",threatre[cnt].houseNum,threatre[cnt].cinemaName);
		for(int i=0;i<=4;i++)
		{
			if(i>0)
			{
				for(int j=1;j<=36;j++)
					fprintf(out,"%d",query(1,cnt,i,1,36,j,j));
			}
			else
			{
				for(int j=1;j<=36;j++)
					fprintf(out,"%d",threatre[cnt].occupyMap[i][j]);
			}
			
			fprintf(out,"\n");
		}
	}
	fclose(out);
} 

void rewriteAllUserData()
//cover the txt doc to save all user data 
{
	FILE *out;
	out = fopen("Customer.txt","w");
	for(int i=0;i<userNumber;i++)
	{
			fprintf(out,"%s %s %.2lf %s %s %s %s\n",
			user[i].id,user[i].key,user[i].money,
			user[i].name,user[i].sex,user[i].phone,user[i].mail);
	}
	fclose(out);
}
void rewriteAllOrderData()
//cover the txt doc to save all order data 

{
	FILE *out;
	out = fopen("OrderChanged.txt","w");
	
	for(int i=0;i<orderNumber;i++)
	{
//		cout<<"hello"<<endl;
		fprintf(out,"%s %s %d %s %d %d %lf %d %d %d\n",
		order[i].id,order[i].cinema,order[i].buyDate,
		order[i].movie,
		order[i].house,
		order[i].stime,order[i].price,order[i].row,order[i].col,order[i].snum);
	}
	fclose(out);
}


void revise()
{
	int num, flag=1;
	printf("\nRevise\n");
	printf("Please choose which one to modify:\n");
	printf("1.Name\n"
		"2.Gender\n"
		"3.Phone\n"
		"4.Mail\n"
		"5.Password\n");
	while (1)
	{
		scanf("%d", &num);
		switch (num)
		{
		case 1:
			printf("Please input your new name:\n");
			scanf("%s", user[usNum].name);
			break;
		case 2:
			printf("Please input your new gender: \n");
			scanf("%s", user[usNum].sex);
			break;
		case 3:
			printf("Please input your new phone number:\n");
			scanf("%s", user[usNum].phone);
			break;
		case 4:
			printf("Please input your new e-mail address:\n");
			scanf("%s", user[usNum].mail);
			break;
		case 5:
			printf("Please input your new password:\n");
			scanf("%s", user[usNum].key);
			break;
		default:
			printf("Wrong input! Please enter again:");
			flag = 0;
		}
		if (flag) break;
	}
	printf("Revise successfully\n");
	getchar();
	getchar();
	return customer(usNum);
}

void scan()  //显示用户信息 +修改 
{
	char sign = 'y';
	printf("+---------------------------------------------------------------------------+\n");
	printf("|   Name  |   Gender   |    Phone    |         Mail         |    Password   |");
	printf("\n+---------------------------------------------------------------------------+\n");
	printf("|   %-5s |   %-7s  | %-11s | %-20s |    %-10s |\n", user[usNum].name, user[usNum].sex, user[usNum].phone, user[usNum].mail, user[usNum].key);
	printf("+---------------------------------------------------------------------------+\n");
	printf("Wether revise? yes(y)/no(n)\n");
	while(1)
	{
		scanf("\n%c",&sign);
		if (sign == 'y' ||sign == 'Y')
		{
			revise();
		}
		else if(sign == 'n' || sign == 'N')
		{
			return customer(usNum);
		}
		else printf("Wrong input!\n");
	}
}

//customer menu
void customer(int usnum)
{
	system("cls");
	printf("Your balance is %.2lf\n\n",user[usnum].money);
	printf("1.Check the movie and purchase\n");
	printf("2.Modify personal information\n");
	printf("3.Cancel orders\n");
	printf("4.Query purchase records\n");
	printf("5.Add Money\n");
	printf("6.Exit\n"); 
	printf("Please enter a number:\n");
	int op;
	op = checkInt(1,6);
	switch(op)
	{
		case 1:
			checkMovie();
			break;
		case 2:
			scan();
			break;
		case 3:
			refund(usnum);
			break;
		case 4:
			findRecords(usnum);
			break;
		case 5:
			recharge(usNum,1);
			break;
		case 6:
			rewriteAllOrderData();
			rewriteAllStageData();
			rewriteAllUserData(); 
			exit(0);
	}
}

void checkUserOrder(char cinema[])
//check user's order by his id
{
	system("cls");
	rewind(stdin);
	
	printf("Plz enter the id of user that you want to check\n");
	char userID[100];
	scanf("%s",userID);
	for(int i=0;i<orderNumber;i++)
	{
		if(!strcmp(order[i].id,userID) && !strcmp(order[i].cinema,cinema))
		{
			printf("%20s ",order[i].movie);
			printf("The start time is %02d:%02d ",order[i].stime/100,order[i].stime%100);
			printf("Your seat is Row%d Col%d\n",order[i].row,order[i].col);
		} 
	}
	printf("Plz enter 0 to back to last screen\n");
	getchar();
	getchar();
}

//compare functions 
//in order to not change offset of stage 
//we use a new struct cmpStage to save stage data;

struct cmpStage{
	char cinema[20];
	char movie[30];
	char id[30];
	int rest;
	int totseat;
	int stime;
	double price;
	double discount;
}stageTmp[100000];

//compare by attendence rate

bool cmpRate(cmpStage a,cmpStage b){
	return (double)a.rest/(double)a.totseat < (double)b.rest/(double)b.totseat;
}

//compare by total income

bool cmpIncome(cmpStage a,cmpStage b){
	return (a.totseat-a.rest)*a.price*a.discount > (b.totseat-b.rest)*b.price*b.discount;
}
void cntUserOrder(char cinema[])
//count all user order
{
	stageNumber = getAllStageData();
	for(int cnt=0;cnt<stageNumber;cnt++)
	{
		strcpy(stageTmp[cnt].cinema,stage[cnt].cinema);
		strcpy(stageTmp[cnt].movie,stage[cnt].movie);
		strcpy(stageTmp[cnt].id,stage[cnt].id);
		stageTmp[cnt].price = stage[cnt].price;
		stageTmp[cnt].rest = stage[cnt].rest;
		stageTmp[cnt].totseat = stage[cnt].totseat;
		stageTmp[cnt].stime = stage[cnt].stime;
		stageTmp[cnt].discount = stage[cnt].discount; 
	}
	//copy all stage data to cmpStage
	while(1)
	{
		system("cls");
		rewind(stdin);
		int time = getSystemTime(); 
		printf("Plz enter num to choose \n");
		printf("1.sort all finished stages by attendence rate\n");
		printf("2.sort all unfinished stages by attendence rate\n");
		printf("3.sort all finished stages by total expected income\n");
		printf("4.back to last screen\n");
		int choice;
		choice = checkInt(1,4);
		switch(choice)
		{
			case 1:{
				//sort all finished stage by attendence rate
				sort(stageTmp,stageTmp+stageNumber,cmpRate);
				for(int i=0;i<stageNumber;i++)
				{
					if(stageTmp[i].stime < time && !strcmp(stageTmp[i].cinema,cinema))
						printf("%20s %20s the rate is %lf\n",
						stageTmp[i].movie,stageTmp[i].id,1-(double)stageTmp[i].rest/(double)stageTmp[i].totseat); 
				}
				printf("Plz enter 0 to last screen\n");
				getchar();
				getchar();
				break;
			}
			case 2:{
				//sort all unfinished stage by attendence rate 
				sort(stageTmp,stageTmp+stageNumber,cmpRate);
				for(int i=0;i<stageNumber;i++)
				{
					if(stageTmp[i].stime > time && !strcmp(stageTmp[i].cinema,cinema))
						printf("%20s %20s the rate is %lf\n",
						stageTmp[i].movie,stageTmp[i].id,1-(double)stageTmp[i].rest/(double)stageTmp[i].totseat); 
				}
				printf("Plz enter 0 to last screen\n");
				getchar();
				getchar();
				break;
			}
			case 3:{
				//sort all finished stage by total income
				sort(stageTmp,stageTmp+stageNumber,cmpRate);
				for(int i=0;i<stageNumber;i++)
				{
					if(stageTmp[i].stime < time && !strcmp(stageTmp[i].cinema,cinema))
						printf("%20s %20s the total expected income is %.2lf\n",
						stageTmp[i].movie,stageTmp[i].id,stageTmp[i].discount*(stageTmp[i].totseat-stageTmp[i].rest)*stageTmp[i].price ); 
				}
				printf("Plz enter 0 to last screen\n");
				getchar();
				getchar();
				break;
				break;
			}
			case 4:{
				return ; 
				break;
			}
			default:{
				printf("The number you enter is invaild\n");
				getchar();
				break;
			}
		}
	}

}
void manageHouse(char cinema[])
{
	system("cls");
	rewind(stdin);
	
	printf("Plz enter num to choose\n");
	printf("1.check house\n");
	printf("2.add new house\n");
	printf("3.back to last screen\n");
	
	int choice;
	choice = checkInt(1,3);
	switch(choice)
	{
		case 1:{
			//check the house of cinema has been put in used, or not;
			for(int i=0;i<houseNumber;i++)
			{
				if(!strcmp(house[i].cinema,cinema))
				{
					printf("ID %d row %02d col %02d type %-6s",
					house[i].houseID,house[i].row,house[i].col,house[i].type);
					if(house[i].available)
						printf("Has been put into use\n");
					else
						printf("Free\n");
				}		
			}
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
		case 2:{
			//if there exist free house, let admin choose to  
			for(int i=0;i<cinemaNumber;i++)
			{
				if(!strcmp(threatre[i].cinemaName,cinema) && threatre[i].houseNum < 4)
				{
					printf("Now you can add total %d house\n",4-threatre[i].houseNum);
					printf("If you confirm to add 1 house, plz enter 1, otherwise enter 0\n");
					int confirm;
					scanf("%d",&confirm);
					if(confirm)
					{
						threatre[i].houseNum ++;
						for(int k=0;k<houseNumber;k++)
						{
//							cout << house[k].cinema <<' '<<cinema<< ' ' << house[k].houseID << ' '<<threatre[i].houseNum<<endl;
							if(!strcmp(cinema,house[k].cinema) && threatre[i].houseNum == house[k].houseID)
								house[k].available = 1;
						}
						printf("Plz enter 0 to last screen\n");
						getchar();
						getchar();
						rewriteAllHouseData();
						rewriteAllCinemaData();
						break;
					}		
				}
				if(!strcmp(threatre[i].cinemaName,cinema) && threatre[i].houseNum >= 4)
				{
					printf("There is no empty house, Sorry\n");
					printf("Plz enter 0 to last screen\n");
					getchar();
					getchar();
					break;
				}
			}
			break;
		}
		case 3:{
			return ;
			break;
		}
		default:{
			printf("The number you enter is invaild\n");
			system("pause"); 
			break;
		}
	}
}
int checkDay(int mouth,int day)
//check the day is consistent with the month, or not
// consistent return 1 inconsisten return 0;
{
	if(mouth>12||mouth<1)
		return 0;
	if(mouth==1||mouth==3||mouth==5||mouth==7||mouth==8||mouth==10||mouth==12)
 	{
		if(day>31||day<1)
   		return 0;
  	else
   		return 1;
 	}
    if(mouth==4||mouth==6||mouth==9||mouth==11)
    {
        if(day>30||day<1)
            return 0;
        else
            return 1;
    }
    if(mouth==2)
    {
        if(day>28||day<1)
            return 0;
        else
            return 1;
    }
}

void getStageTime(int now,int s,int len)
//get the stage time including starts time and end time
{
	int eMin = 0;
	if(!(s%100)) eMin = 20;
	if(s%100 == 21) eMin = 40;
	if(s%100 == 41) eMin = 59;
	while(1)
	{
		int temp = 0;
		if(eMin == 20)
			temp = 10;
		if(eMin == 40)
			temp = 30;
		if(eMin == 59)
			temp = 50;
		printf("Plz enter the start time(min) which is %d\n",temp);
		
		if(s%100 <=temp && temp <=eMin)
		{
			stage[now].stime = s - s%100 + temp;
			int eH = s/100 + len / 60;
			int eM = temp + len % 60;
			if(eM > 59)
			{
				eM -= 60;
				eH ++;
			}
			stage[now].etime = eH * 100 + eM;
			break;
		}
		else
			printf("The number you enter is wrong\n");
	}
	
	printf("Set the Recommend Ratings\n");
	stage[now].hot = checkInt(0,10);
	
	int mon,day;
	
	printf("Set the data(Month)\n");
	mon = checkInt(1,12);
		
	while(1)
	{
		printf("Set the data(Day)\n");
		day = checkInt(1,31);
		if(!checkDay(mon,day))
			printf("The time you enter is wrong, plz try again\n");
		else
		{
			stage[now].date = mon * 100 + day;	
			break;
		}	
	}
}
int transTime(int i,int isEnd)
//transfrom time from id Format to normal format
{
	int h = 0;
	if(i % 3)
		h = i / 3 + 9;
	else 
		h = i / 3 + 8;
	int m = 0;
	if(isEnd)
	{
		if(i % 3 == 1) m = 20;
		if(i % 3 == 2) m = 40;
		if(i % 3 == 0) m = 59;
	}
	else
	{
		if(i % 3 == 1) m = 00;
		if(i % 3 == 2) m = 21;
		if(i % 3 == 0) m = 41;		
	}
	return h*100 + m;
}
void getStageInfo(int now,int houseID)
//get other stage info including price discount language,etc
{
	printf("The price\n");
	stage[now].price = checkDouble(1.0,1000.0);
	printf("The discount\n");
	stage[now].discount = checkDouble(0.1,1.0);
	printf("The language\n");
	scanf("%s" ,&stage[now].language);
	
	stage[now].id[0] = house[houseID].cinema[0];
	stage[now].id[1] = house[houseID].cinema[1];
	stage[now].id[2] = '1';
	stage[now].id[3] = '9';
	stage[now].id[4] = '0' + stage[now].date/1000;
	stage[now].id[5] = '0' + stage[now].date%1000/100;
	stage[now].id[6] = '0' + stage[now].date%100/10;
	stage[now].id[7] = '0' + stage[now].date%10;
	
	stage[now].id[8] = '0' + stage[now].stime/1000;
	stage[now].id[9] = '0' + stage[now].stime%1000/100;
	stage[now].id[10] = '0' + stage[now].stime%100/10;
	stage[now].id[11] = '0' + stage[now].stime%10;
	
	stage[now].id[12] = '0' + house[houseID].houseID/10;
	stage[now].id[13] = '0' + house[houseID].houseID%10;
	stage[now].id[14] = '\0';
}
void copyStage()
{
	printf("Would you like to arrange the same stage in next 2 days\n");
	printf("According to the rulers, if not, the house will be wasted in next 2 days\n");
	printf("1.No, only the date I enter\n");
	printf("2.Yes, but only next 1 day\n");
	printf("3.Yes, both 2 days\n");
	
	int op = checkInt(1,3);
	switch(op)
	{
		case 1:{
			printf("Only the day you enter");
			break;
		}
		case 2:{
			memcpy(&stage[stageNumber],&stage[stageNumber-1],sizeof(stage[stageNumber-1]));
			stage[stageNumber].date ++;
			stage[stageNumber].id[6] = '0' + stage[stageNumber].date%100/10;
			stage[stageNumber].id[7] = '0' + stage[stageNumber].date%10;
			stageNumber ++; 
			printf("The stage has been arranged in date %02d/%02d\n",stage[stageNumber-1].date/100,stage[stageNumber-1].date%100);
			break;
		}
		case 3:{
			memcpy(&stage[stageNumber],&stage[stageNumber-1],sizeof(stage[stageNumber-1]));
			stage[stageNumber].date ++;
			stage[stageNumber].id[6] = '0' + stage[stageNumber].date%100/10;
			stage[stageNumber].id[7] = '0' + stage[stageNumber].date%10;
			stageNumber ++;
			memcpy(&stage[stageNumber],&stage[stageNumber-1],sizeof(stage[stageNumber-1]));
			stage[stageNumber].date ++;
			stage[stageNumber].id[6] = '0' + stage[stageNumber].date%100/10;
			stage[stageNumber].id[7] = '0' + stage[stageNumber].date%10;
			stageNumber ++;
			printf("The stage has been arranged in date %02d/%02d\n",stage[stageNumber-2].date/100,stage[stageNumber-2].date%100);
			printf("The stage has been arranged in date %02d/%02d\n",stage[stageNumber-1].date/100,stage[stageNumber-1].date%100);
			break;
		}
	}
}
void addStage(char cinema[])
//add a stage
//give advice by searching the empty time of passaway and houses;
{
	int mins = checkInt(30,690);
	int span = mins/20 + 1;
	//we define that 1h equals 3 spans !!
	
	int id = -1;
	for(int i=0;i<cinemaNumber;i++)
		if(!strcmp(threatre[i].cinemaName,cinema))
		{
			id = i;	
		}
	if(id == -1)
	{
		printf("There is some unexpectable wrong\n");
		system("pause");
		return ;	
	}
	int cnt = 1;
	int avaHouse[1000],avaSTime[1000],avaETime[1000];
	int stimeID[1000];
	int etimeID[1000];
	for(int i=1;i<=36-span;i++)
	{
		if(!threatre[id].occupyMap[0][i] && !threatre[id].occupyMap[0][i+span])
		{
			int stime = transTime(i,0);
			int etime = transTime(i+span,1);

			for(int j=1;j<=threatre[id].houseNum;j++)
			{
				int mark = 1;
				if(query(1,id,j,1,36,i,i+span)) mark = 0;
				if(mark)
				{
					avaHouse[cnt] = j;
					avaSTime[cnt] = stime;
					avaETime[cnt] = etime;
					stimeID[cnt] = i;
					etimeID[cnt] = i + span;
					printf("No.%d The available house is %d, The avaible time is between %02d:%02d and %02d:%02d\n",cnt++,j,stime/100,stime%100,etime/100,etime%100);
				}
			}
		}
	}
	if(cnt == 1)
	{
		printf("There is no house to arrange a %d-mins movie, Sorry\n",mins);
		printf("Plz enter 0 to last screen\n");
		getchar();
		getchar();
		return ;	
	}
		
	printf("Plz enter 1 ~ %d to choose arangement\n",cnt-1);
	int choice;
	int houseID = 0;
	while(1)
	{
		choice = checkInt(1,cnt-1);

		for(int i=0;i<houseNumber;i++)
		{
			if(!strcmp(house[i].cinema,cinema) && avaHouse[choice] == house[i].houseID) 
			{
				houseID = i;
				threatre[id].occupyMap[0][stimeID[choice]] = 1;
				threatre[id].occupyMap[0][etimeID[choice]] = 1;
				updata(1,id,house[i].houseID,1,36,stimeID[choice],etimeID[choice],1);

				break;
			}
		} 	
		break;		
	}
	
	int now = stageNumber;
	strcpy(stage[now].cinema,cinema);
	strcpy(stage[now].type,house[houseID].type);
	stage[now].col = house[houseID].col;
	stage[now].row = house[houseID].row;
	stage[now].totseat = stage[now].col * stage[now].row;
	stage[now].rest = stage[now].totseat;
	stage[now].house = avaHouse[choice];
	memset(stage[now].seat,0,sizeof(stage[now].seat));
	
	printf("The movie that you want to add\n");
	scanf("%s",stage[now].movie);
	
	getStageTime(now,avaSTime[choice],mins);	
	getStageInfo(now,houseID); 
	stageNumber ++;
	
	copyStage();
}
void manageStage(char cinema[])
//able admin to check or add stages
{
	system("cls");
	rewind(stdin);
	
	printf("Plz enter num to choose\n");
	printf("1.check stages\n");
	printf("2.add new stages\n");
	printf("3.back to last screen\n");
	int choice;
	choice = checkInt(1,3);
	
	switch(choice)
	{
		case 1:{
			stageNumber = getAllStageData();
			for(int i=0;i<stageNumber;i++)
			{
				if(!strcmp(stage[i].cinema,cinema))
					printf("%20s %s The start time is %02d:%02d the end time is %02d:%02d the price is %5.2lf\n"
					,stage[i].movie,stage[i].id,stage[i].stime/100,stage[i].stime%100,stage[i].etime/100,stage[i].etime%100,stage[i].price);
			}
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
		case 2:{
			addStage(cinema);
			rewriteAllStageData();
			rewriteAllCinemaData();
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
		case 3:{
			return ;
			break;
		}
		default:{
			printf("The number you enter is invaild\n");
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar(); 
			break;
		}
	}
}
bool checkMail(char mail[])
//check the mail format is vaild or not;
//vaild return 1 not vaild return 0
{
	int locAt=-1,locPoi=-1;
	int cntAt = 0,cntPoi = 0;
	int len = strlen(mail);
	for(int i=0;i<len;i++)
	{
		if(mail[i] == '@')
		{
			cntAt ++;
			locAt = i;
		}
		if(mail[i] == '.')
		{
			cntPoi ++;
			locPoi = i;
		}
	}
	if(cntAt != 1 || cntPoi != 1)	return 0;
	//the num of @ or poi is bigger than 1 return 0 
	if(locPoi < locAt || locAt == 0 || locPoi == len - 1)	return 0;
	return 1;
}
void manageAdmin(char cinema[],char id[])
//able admin to change his password or mail address;
{
	system("cls");
	rewind(stdin);
	
	printf("Plz enter num to choose\n");
	printf("1.Change Your Password\n");
	printf("2.Change Your Mail\n");
	printf("3.back to last screen\n");
	
	int choice;
	choice = checkInt(1,3);
	switch(choice)
	{
		case 1:{
			int mark = 0;
			printf("Plz enter your new password\n");
			for(int i=0;i<adminNumber;i++)
				if(!strcmp(admin[i].cinema,cinema) && !strcmp(admin[i].id,id))
					mark = scanf("%s",admin[i].key);
			if(mark)
				printf("Password has been changed\n");
			else
				printf("Fail to change\n");
			rewriteAllAdminData();
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
		case 2:{
			int mark = 0;
			char temp[100];
			printf("Plz enter your new mail\n");
			scanf("%s",temp);
			for(int i=0;i<adminNumber;i++)
			{
				if(!strcmp(admin[i].cinema,cinema) && !strcmp(admin[i].id,id) && checkMail(temp))
				{
					strcpy(admin[i].mail,temp);
					printf("Mail has been changed\n");
					mark ++;
				}							
			}
			if(!mark)
				printf("Fail to change\n");	
			rewriteAllAdminData();
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
		case 3:{
			return ;
			break;
		}
		default:{
			printf("The number you enter is invaild\n");
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
			break;
		}
	}
}
void AdminTab(char name[],char cinema[]);
bool AdminLogin()
//check the key,id and cinema is consistent or not
{
	adminNumber = getAllAdminData();
	char name[100];
	char key[100];
	char cinema[100];
	while(1)
	{
		system("cls");
		rewind(stdin);
			
		printf("Please enter your cinema or enter back to last screen\n");
		scanf("%s",cinema);
		if(cinema[0] == 'b' && cinema[1] == 'a' &&
		   cinema[2] == 'c' && cinema[3] == 'k')
			return 1;
		printf("Please enter your personal id\n");
		scanf("%s",name);
		printf("Please enter your personal key \n");
		scanf("%s",key);
		int mark = 0;
		for(int i=0;i<adminNumber;i++)
		{
			if(!strcmp(admin[i].id,name) && !strcmp(admin[i].key,key) && !strcmp(admin[i].cinema,cinema))
			{
				AdminTab(name,cinema);
				mark ++;
			}	
		}
				
		if(!mark)
		{
			printf("The key, id or cinema you entered may be wrong\n");
			printf("Plz try again\n");
			printf("Plz enter 0 to last screen\n");
			getchar();
			getchar();
		}
	}
}
void AdminTab(char id[],char cinema[])
//the function tab which let admin to choose
{
	userNumber = getAllUserData();
	orderNumber = getAllOrderData();
	stageNumber = getAllStageData();
	houseNumber = getAllHouseData();
	cinemaNumber = getAllCinemaData();	
	while(1)
	{
		system("cls");
		rewind(stdin);
		printf("Welcome Admin %s\n",id);
		printf("Please enter num to choose\n");
		printf("1 check user order\n");
		printf("2 count user order\n");
		printf("3 manage house\n");
		printf("4 manage arangement of house\n");
		printf("5 manage your password or mail address\n");
		printf("6 log out\n");
		
		int choice;
		choice = checkInt(1,6);

		switch(choice)
		{
			case 1:
				checkUserOrder(cinema);
				break;
			case 2:
				cntUserOrder(cinema);
				break;
			case 3:
				manageHouse(cinema);
				break;
			case 4:
				manageStage(cinema);
				break;
			case 5:
				manageAdmin(cinema,id);
				break;
			case 6:
				rewriteAllStageData();
				rewriteAllAdminData();
				rewriteAllHouseData();
				rewriteAllCinemaData();
				return ;
				break;	
			default:
               	printf ("The num you entered is wrong plz try again\n\n");
				break;
		}
	}
		
}




///////
int main()
{
	//adminNumber=getAllAdminData();
	localDate = getSystemDate();
	stageNumber=getAllStageData();
	orderNumber=getAllOrderData();
	userNumber=getAllUserData();
	while(1)
	{
		system("cls");
		printf("Welcome to BUPT BUY BUY BUY movie system!\n");
		printf("1.I am a manager\n");
		printf("2.I am a customer\n");
		printf("3.Exit\n");
		int op;
		op = checkInt(1,3);
		switch(op)
		{
			case 1:
				AdminLogin();
				break;
			case 2:
				customerPart();
				break;
			case 3:
				rewriteAllOrderData();
				rewriteAllStageData();
				rewriteAllUserData(); 
				exit(0);
		}
	}
	
	return 0;
}

