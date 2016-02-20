#include <stdio.h>
#include <math.h>
#include <stdlib.h>
# define Nmod 7
# define BegA 8
# define StartM 5
# define Step 3
# define TIME 2
# define Nmerc 8
struct sldr{																			//структура солдат: принадлежность и кол-во
	char blng;
	int q;	
};
struct mdf{																				//модификация: 
	int pres;
	int var;
	int key;
};
struct mapcell{																			//элементарная единица карты:солдаты на ней, обнуление модицикаций на ней и солдат
	sldr def;
	mdf fld[Nmod];
	void mdfzero(){
		int i;
		fld[0].key=0;
		fld[0].pres=0;
		fld[0].var=-1;
		for(i=1;i<Nmod;i++){
			fld[i].key=1;
			fld[i].pres=0;
			fld[i].var=-1;
		};
	}
	void sldrzero(){
		def.blng='0';
		def.q=0;
	}
	void zero(int k){																	//доп. функция позволяющаяя насильно обнулить клетку, либо сделать это, если войск нет
		if (k>0) return;
		sldrzero();
		int i;
		if (fld[0].key!=0) return;
		for(i=1;i<Nmod;i++){
			if (fld[i].pres==0){
				fld[i].key=1;
				fld[i].var=-1;
				};
			};
	};
};
void incor(int *k,int *i){															//используется при некорректных введённых данных
		(*i)--;
		*k=0;
		printf("Incorrect data\n");
}
void checka(int j1,int j2,int ar,int *i, mapcell l,char o,int *k,int m,int n){				//проверяет корректность движений солдат
	if ((j1>m*n)||(j2>m*n)||(j1<=0)||(j2<=0)||(ar<=0)) {								//координаты принадлежат карте, солдат использовано положительное число
		incor(k,i);
		return;
	};
	if (l.def.blng!=o){																	//солдаты управлются корректным игроком
		incor(k,i);
		return;
	};
	if (ar>l.def.q) {																	//солдат используется не больше, чем на клетке
		incor(k,i);
		return;
	};
	if ((abs(j1-j2)!=m)&&(abs(j1-j2)!=1)){												//солдаты ходят в корректные клетки (вверх, вниз, влево, вправо)
		incor(k,i);
		return;
	};
	if (((j1%m==0)&&((j2-j1)==1))||((j2%m==0)&&(j1-j2==1))){							//доп. проверка, чтобы исключить переход "по диагонали"
		incor(k,i);
		return;
	};
	return;
}
void move(mapcell **mp,int j1,int j2,int ar,int **s){									//передвижение солдат
	j1--;
	j2--;
	if ((*mp)[j2].def.blng==(*mp)[j1].def.blng) (*mp)[j2].def.q+=ar;					//объединение со своими войсками
		else {
			if ((*mp)[j2].def.blng!='0'){												//атака, от всего числа войск сторон отнимается кол-во нападавших солдат 
				if ((*s)[(*mp)[j2].def.blng-'a']<ar){ 
					(*s)[(*mp)[j2].def.blng-'a']-=(*mp)[j2].def.q;
					(*s)[(*mp)[j1].def.blng-'a']-=(*mp)[j2].def.q;
					
				} 
				else {
					(*s)[(*mp)[j2].def.blng-'a']-=ar;
					(*s)[(*mp)[j1].def.blng-'a']-=ar;
					};
			};
			(*mp)[j2].def.q-=ar;
		};
	if ((*mp)[j2].def.q<0) {															//переподчинение клетки
		(*mp)[j2].def.q=abs((*mp)[j2].def.q);
		(*mp)[j2].def.blng=(*mp)[j1].def.blng;
	};
	(*mp)[j1].def.q-=ar;																//аналогично, но отнимается для частной клетки
	(*mp)[j1].zero((*mp)[j1].def.q);													//если войск нет в клетке, то она становится ничьей
	(*mp)[j2].zero((*mp)[j2].def.q);
}
void winner(int **s,int rt,char p,int *STOP){
	int x,sum=0,u=0;
	if (p!='0') (*s)[p-'a']=0;															//кол-во войск сдавшегося игрока обнуляется, сами солдаты остаются на поле
	for (x=0;x<rt;x++)																	//кол-во оставшихся игроков
		if ((*s)[x]!=0){
			sum++;
			u=x+1;
			if (sum>1) return;															//больше одного игрока
		}
	if (u==0) u=rt;
	if (sum==1) printf("Winner is player %d\n",u);									//остался лишь один игрок с войсками
	if (sum==0) printf("Draw\n");														//все выбыли
	*STOP=1;
}
void camicadze(mapcell **mp,int j,int m,int n){
	int o7=(*mp)[j].def.q/4;
	if (j<=m*(n-1)){
		(*mp)[j+m-1].def.q-=o7;
		(*mp)[j+m-1].zero((*mp)[j+m-1].def.q);
		};	
	if (j>m){
		(*mp)[j-m-1].def.q-=o7;
		(*mp)[j-m-1].zero((*mp)[j-m-1].def.q);
		};	
	if (j%m!=0){
		(*mp)[j].def.q-=o7;
		(*mp)[j].zero((*mp)[j].def.q);
		};
	if (j%m!=1){
		(*mp)[j-2].def.q-=o7;
		(*mp)[j-2].zero((*mp)[j].def.q);
		};	
	(*mp)[j-1].zero(0);
}
int main(){
	mdf mount;																			//библиотека модификаций
	mount.key=0;
	mount.pres=0;
	mount.var=-1;
	mdf swamp;
	swamp.key=0;
	swamp.pres=0;
	swamp.var=-1;
	mdf art;
	art.key=1;
	art.pres=1;
	art.var=1;																			//кол-во пушек
	mdf zero;
	zero.key=0;
	zero.pres=0;
	zero.var=-1;
	mdf tun;
	tun.key=2;
	tun.pres=0;
	tun.var=-2;																			//координата второго конца
	mdf fact;
	fact.key=0;
	fact.pres=0;
	fact.var=0;																			//мощность, наращивается со временем, проведенным войском на ней
	mdf wall;
	wall.key=3;
	wall.pres=0;
	wall.var=0;																			//стены возможны со всех сторон, где они - опр. произведением (-1)вв,2вн,3вл,5вп
	mdf trap;
	trap.key=4;
	trap.pres=1;
	trap.var=-1;
	mdf membr;
	membr.key=5;
	membr.pres=0;
	membr.var=TIME;																		//кол-во времени на поле
	mdf mine;
	mine.key=6;
	mine.pres=0;
	mine.var=1;																			//мощность
	mdf hired;
	hired.key=0;
	hired.pres=0;
	hired.var=Nmerc;																	//кол-во наёмников
	mdf river;
	river.key=0;
	river.pres=0;
	river.var=0;																		//наличие моста и его пропускная способность
	int m,n,numpl=0,i=0,j,j1,j2,ar,k,STOP=0,str=0,pos;
	char o='a',br='0',w='0';
	printf("Enter number of columns and lines\n");										//ввод параметров таблицы
	while (i==0){
		i=1;
		scanf("%d %d",&m,&n);
		if ((m<=0)||(n<=0)||(m*n==1)) incor(&i,&i);
		};
	mapcell *map=(mapcell *)malloc(sizeof(mapcell)*m*n);
	for (i=0;i<m*n;i++){																//обнуление всех клеток
	map[i].mdfzero();
	map[i].sldrzero();
	};
	printf("Enter locations of the objects\n");
	scanf("%d",&k);
	k=1;
	printf("Enter number of players\n");
	i=0;
	while (i==0){																		//ввод кол-ва игроков, которое не больше размеров таблицы
		i=1;
		scanf("%d",&numpl);
		if ((numpl>m*n-str)||(numpl<=1)) incor(&i,&i);
	};
	printf("Enter their initial positions\n");											//ввод первоначальных позиций, два игрока на одном месте не могут быть
	for (i=0;i<numpl;i++){
	scanf("%d",&pos);
	pos--;
	if ((pos<0)||(pos>m*n-1)) incor(&k,&i);
		else if ((map[pos].def.blng=='0')&&(map[pos].fld[0].key==0)) {
					map[pos].def.blng=o;
					map[pos].def.q=BegA;
					map[pos].mdfzero();
					o++;
				} else incor(&k,&i);
	};
	//for (i=0;i<m*n;i++)
	//	printf("%d %c",map[i].def.q,map[i].def.blng);	
	k=1;
	int *s=(int *)malloc(sizeof(int)*numpl);											//задание массивов с кол-вом войск и денег игроков
	int *mon=(int *)malloc(sizeof(int)*numpl);
	for	(i=0;i<numpl;i++){
		s[i]=BegA;
		mon[i]=StartM;
	};
	while (STOP!=1){
		for (o='a';o<'a'+numpl;){
			if (s[o-'a']!=0){
				for (i=0;i<Step;){
					printf("Player %c, you have %d step(s)\n",o,Step-i);
					i++;
					printf("Enter a-attack,b-bonus,c-capitulate\n");
					br='0';
					while ((br!='a')&&(br!='b')&&(br!='c'))
						br=getchar();
					if (br=='a'){
						scanf("%d %d %d",&j1,&j2,&ar);
						checka(j1,j2,ar,&i,map[j1-1],o,&k,m,n);						//проверка корректности движения
						if (k==1)														//ошибки нет - движение совершается
							move(&map,j1,j2,ar,&s);
						//	for (j=0;j<m*n;j++)
						//	printf("+%d%c+",map[j].def.q,map[j].def.blng);
						//	printf("ss%d %dss",s[0],s[1]);
					}
					else if (br=='b'){													//игрок тратит деньги на бонус
				
						}
						else if (br=='c') {
								winner(&s,numpl,o,&STOP);
								break;
							};
					k=1;
					winner(&s,numpl,0,&STOP);														//проверка на корректность продолжения игры
					if (STOP==1) break;
				};
			o++;
			if (STOP==1) break;
			};
		};
	};
	return 0;
}
