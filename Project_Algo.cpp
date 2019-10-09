#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include <time.h>
#include<string.h>

//Struct Data
struct Topping
{
	char topping[255];
	int stock;
	//int harga;
} listTopp[255];

struct User
{
	char username[255];
	char fullName[255];
	char password[255];
	char userType[255];

} listUser[255], *loginUser;

struct Transaction
{
	char name[255];
	char code[255];
	long long int grandtotal;
	int jlhporsi;
} listTrans[255], tempListTrans[255];

struct TransactionDetail
{
	char code[255];
	char topping[255];
	int jlhtopp;
} listTransDetail[255], tempListTransDetail[255];

//
int index=1,totalStock,banyakBeli=0,hargaTotal,jumlahPorsi;
char namaCust[200],idCust[20];
struct Item
{
	char topping[255];
	int stock,harga,beli,tempBeli;
}list[255];
//


//Variable Declaration
int inputLogin;
int inputMainMenu;
int inputAdmin;
int inputDataUser;
int inputDataTopp;
int inputDataTrans;
int inputDataSearch;
int countUser;
int countTopp;
int countTrans;
int countTransDetil;
int flagStatus;
int flag;
int tempStock;
char tempLoginUsername[255];
char tempLoginPassword[255];
char tempUsername[255];
char tempPassword[255];
char tempUserType[255];
char tempFullName[255];
char tempTopping[255];
char searchNama[255];
FILE *Userfp;
FILE *Toppfp;
FILE *Transfp;


void printEnter(char type, int jlh)
{
	for(int i = 0; i < jlh; i++)
	{
		printf("%c", type);
	} 
}

//
void cls()
{
	for(int i=0;i<30;i++)
	{
		printf("\n");
	}
}
void reset()
{
	for(int i=0;i<255;i++)
	{
		list[i].beli=0;
	}
	banyakBeli=0;
	jumlahPorsi=0;
	hargaTotal=0;
	totalStock=0;
}
void done()
{
	for(int i=1;i<index;i++)
	{
		list[i].beli=0;
	}
	reset();
}
void getID()
{
	char alnum[]="1234567890abcdefghijklmnopqrstuvqxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	srand(time(NULL));
	for(int i=0;i<5;i++)
	{
		idCust[i]=alnum[rand()%62];
	}
}
int read()
{
	char tempStr[50];
	int tempInt;
	FILE *fp=fopen("topping.txt","a+");
	if(fp)
	{
		index = 1;
		while(fscanf(fp,"%[^#]#%d\n",tempStr,&tempInt)!=EOF)
		{			
//			fscanf(fp,"%[^#]#%d\n",tempStr,&tempInt);
//			printf("%s %d\n",tempStr,tempInt);
			strcpy(list[index].topping,tempStr);
			list[index].stock=tempInt;
			totalStock+=tempInt;
			index++;
		}
		fclose(fp);
		if(totalStock==0)
		{
			printf("Maaf, Topping belum tersedia\n"); //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			getchar();
			return 0;
		}
		return 1;
	}
	else
	{
		printf("Maaf, Topping belum tersedia\n");//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
		fclose(fp);
		getchar();
		return 0;
	}
}
void save()
{
	FILE *fp=fopen("transaksi.txt","a+");
	fprintf(fp,"%s#%s#%d#%d\n",namaCust,idCust,hargaTotal,jumlahPorsi);
	fclose(fp);
	
	fp=fopen("transaksidetil.txt","a+");
	for(int i=1;i<index;i++)
	{
		if(list[i].beli>0)
			fprintf(fp,"%s#%s#%d\n",idCust,list[i].topping,list[i].beli);
	}
	fclose(fp);
}
void updateStock()
{
	FILE *fp=fopen("topping.txt","w");
	for(int i=1;i<index;i++)
	{
		fprintf(fp,"%s#%d\n",list[i].topping,list[i].stock);
	}
	fclose(fp);
}
int minn(int a,int b){
	if(a>b)
		return b;
	return a;
}
void view()
{
	printf("=========================================================\n");
	printf("| No. |            Topping             | Stock | Jumlah |\n");
	printf("=========================================================\n");	
	banyakBeli=0;
	for(int i=1;i<index;i++)
	{
		printf("| %2d. | %-30s | %5d | %6d |\n",i,list[i].topping,list[i].stock,list[i].beli);
		banyakBeli+=list[i].beli;
	}
	printf("=========================================================\n");
}
void add()
{
	int banyak,indexTopping,pilihBanyak,porsi;
	char confirm;
	do
	{
		cls();
		do
		{	
			porsi=0;
			printf("Beli berapa porsi? (1 porsi isi 3 @15000)\n");
			printf(">> ");
			scanf("%d",&porsi);
			getchar();
			if(porsi*3 > totalStock)
			{
				printf("Maaf, Stock kami hanya tersisa %d porsi\n",totalStock/3);
				porsi=0;
			}
		}while(porsi<1);
		do
		{	
			confirm='a';
			printf("Beli %d porsi?[y|n]: ",porsi);
			scanf("%c",&confirm);
			//getchar();
		}while(confirm!='y' && confirm!='n');
	}while(confirm!='y');
	banyak=porsi*3;
	jumlahPorsi+=porsi;
	do
	{
		do
		{
			cls();
			view();
			printf("Masih %d minion ball yang belum dipilih\n",banyak);
			do
			{
				indexTopping=0;
				printf("Pilih topping [1..%d]: ",index-1);
				scanf("%d",&indexTopping);
				getchar();
				if(list[indexTopping].stock<1 && (indexTopping>=1 && indexTopping<=index-1 ))
				{
					printf("Maaf, topping %s sudah habis\n",list[indexTopping].topping);
					indexTopping=0;
				}
			}while(indexTopping<1 || indexTopping>index-1);
			do
			{
				pilihBanyak=0;
				printf("Rasa %s, berapa banyak? ",list[indexTopping].topping);
				scanf("%d",&pilihBanyak);
				getchar();
				if(pilihBanyak>banyak)
				{
					printf("Maaf, anda hanya memesan %d porsi\n",porsi);
					pilihBanyak=-1;
				}
				else if(pilihBanyak>list[indexTopping].stock)
				{
					printf("Maaf, stock %s hanya tersisa %d\n",list[indexTopping].topping,list[indexTopping].stock);
					pilihBanyak=-1;
				}
			}while(pilihBanyak<0);
			list[indexTopping].stock-=pilihBanyak;
			list[indexTopping].tempBeli+=pilihBanyak;
			list[indexTopping].beli+=pilihBanyak;
			banyak-=pilihBanyak;
		}while(banyak>0);
		cls();
		do
		{
			confirm='a';
			printf("Anda membeli\n");
			for(int i=1;i<=index-1;i++)
			{
				if(list[i].tempBeli>0)
					printf("%d topping %s\n",list[i].tempBeli,list[i].topping);
			}
			printf("Confirm?[y|n]: ");
			scanf("%c",&confirm);
		//	getchar();
		}while(confirm!='y' && confirm!='n');
		if(confirm=='n')
		{
			for(int i=1;i<=index-1;i++)
			{
				banyak+=list[i].tempBeli;	
				list[i].stock+=list[i].tempBeli;
				list[i].beli-=list[i].tempBeli;
				list[i].tempBeli=0;
			}
		}
	}while(confirm!='y');
	for(int i=1;i<=index-1;i++)
	{
		list[i].tempBeli=0;
	}
	totalStock-=porsi*3;
}
void edit()
{
	int idxTukar1,banyakPerluTukar,idxTukar2,banyakSudahTukar;
	char confirm;
	cls();
	if(banyakBeli<1)
	{
		printf("Maaf, anda belum membeli apapun\n");
		getchar();
		return;
	}
	do
	{
		view();
		idxTukar1=0;
		printf("Ganti topping apa?[1..%d] ",index-1);
		scanf("%d",&idxTukar1);
		getchar();
		if(list[idxTukar1].beli<1 && !(idxTukar1<1 || idxTukar1>index-1))
		{
			printf("Anda belum membeli topping ini\n");
		}
	}while(idxTukar1<1 || idxTukar1>index-1);
	do
	{
		banyakPerluTukar=0;
		printf("Berapa?[1..%d] ",list[idxTukar1].beli);
		scanf("%d",&banyakPerluTukar);
		getchar();
	}while(banyakPerluTukar<0||banyakPerluTukar>list[idxTukar1].beli);
	do
	{
		cls();
		view();	
		printf("Anda masih punya %d yang perlu ditukar\n",banyakPerluTukar);
		do
		{
			printf("Ganti dengan topping apa?[1..%d] ",index-1);
			scanf("%d",&idxTukar2);
			getchar();
			if(idxTukar2==idxTukar1)
			{
				do
				{
					confirm='a';
					printf("Anda memilih rasa yang sama, yakin?[y|n] ");
					scanf("%c",&confirm);
				//	getchar();	
				}while(confirm!='y' && confirm!='n');
				if(confirm=='n')
				{
					idxTukar2=0;
				}
			}
			else if(list[idxTukar2].stock<1)
			{
				printf("Maaf, topping %s sudah habis\n",list[idxTukar2].topping);
				idxTukar2=0;
			}
		}while(idxTukar2<1 || idxTukar2>index-1);
		do
		{
			banyakSudahTukar=-1;
			printf("Berapa?[1..%d] ",minn(banyakPerluTukar,list[idxTukar1].beli));
			scanf("%d",&banyakSudahTukar);
			getchar();
			if(banyakSudahTukar>banyakPerluTukar)
			{
				printf("Anda hanya bisa menukar %d lagi\n",banyakPerluTukar);
				banyakSudahTukar=-1;
			}
		}while(banyakSudahTukar<0 ||banyakSudahTukar>list[idxTukar1].beli);
		printf("%d %s berhasil diganti dengan %s\n",banyakSudahTukar,list[idxTukar1].topping,list[idxTukar2].topping);
		list[idxTukar1].beli-=banyakSudahTukar;
		list[idxTukar2].beli+=banyakSudahTukar;
		banyakPerluTukar-=banyakSudahTukar;
	}while(banyakPerluTukar>0);
}
void remove()
{
	int idxHapus,banyakHarusHapus, banyakSudahHapus;
	cls();
	view();
	if(banyakBeli<1)
	{
		printf("Maaf, anda belum membeli apapun\n");
		getchar();
		return;
	}
	do
	{
		banyakHarusHapus=0;
		printf("Hapus berapa porsi?[1..%d]",banyakBeli/3);
		scanf("%d",&banyakHarusHapus);
		getchar();	
	}while(banyakHarusHapus<1 || banyakHarusHapus>banyakBeli/3);
	jumlahPorsi-=banyakHarusHapus;
	banyakHarusHapus*=3;
	banyakBeli-=banyakHarusHapus;
	do
	{
		printf("Hapus %d lagi\n",banyakHarusHapus);
		do
		{
			idxHapus=0;
			printf("Hapus pilihan yang mana?[1..%d]: ",index-1);
			scanf("%d",&idxHapus);
			getchar();
		}while(idxHapus<1 || idxHapus >index-1);
		if(list[idxHapus].beli==0)
		{
			printf("Maaf, anda belum membeli menu dengan topping tersebut\n");
		}
		else
		{
			do
			{
				banyakSudahHapus=0;
				printf("Berapa?[1..%d]: ",minn(list[idxHapus].beli,banyakHarusHapus));
				scanf("%d",&banyakSudahHapus);
				getchar();
				if(banyakSudahHapus>banyakHarusHapus)
				{
					printf("Anda hanya perlu menghapus %d lagi",banyakHarusHapus);
					banyakSudahHapus=0;
				}
			}while(banyakSudahHapus<1 || banyakSudahHapus>list[idxHapus].beli);
			list[idxHapus].stock+=banyakSudahHapus;
			list[idxHapus].beli-=banyakSudahHapus;
			printf("Topping %s sudah dihapus sebanyak %d\n",list[idxHapus].topping,banyakSudahHapus);
			totalStock+=banyakSudahHapus;
			banyakHarusHapus-=banyakSudahHapus;
			cls();
			view();		
		}	
	}while(banyakHarusHapus>0);
}
void removeAll()
{
	if(banyakBeli<1)
	{
		printf("Maaf, anda belum membeli apapun\n");
		getchar();
		return;
	}
	for(int i=1;i<index;i++)
	{
		list[i].stock+=list[i].beli;
		list[i].beli=0;
	}
	jumlahPorsi=0;
}
void bayar()
{
	int bayar;
	do
	{
		bayar=0;
		printf("Masukkan uang: ");
		scanf("%d",&bayar);
		getchar();
		if(bayar<hargaTotal)
		{
			printf("Uang anda tidak cukup\n");
			bayar=0;
		}
	}while(bayar<=0);
	if(bayar==hargaTotal)
	{
		printf("Anda membayar dengan uang pas\n");
	}
	else
	{
		printf("Kembalian anda Rp. %d\n",bayar-hargaTotal);
	}
	getchar();
}
void finish()
{
	char confirm;
	cls();
	printf("Anda membeli:\n");
	printf("=============\n");
	for(int i=1;i<=index-1;i++)
	{
		if(list[i].beli>0)
		{
			printf("%d topping %s\n",list[i].beli,list[i].topping);
//			jumlahPorsi+=list[i].beli;
		}
	}
//	jumlahPorsi/=3;
	hargaTotal=jumlahPorsi*15000;
	printf("Total %d porsi @15.000 = %d\n",jumlahPorsi,hargaTotal);
	bayar();
	cls();
	printf("Atas nama siapa?\n");
	scanf("%[^\n]s",namaCust);
	getID();
	cls();
	printf("Silakan menunggu sampai nama anda dipanggil\n");
	printf("Nama: %s\n",namaCust);
	printf("Kode pembelian: %s",idCust);
	getchar();
}
void user()
{
	int menu_input;
	if(read()==1)
	{
		reset();
		do
		{
			cls();
			view();
			printf("\n\n");
			printf("1. Beli\n");
			printf("2. Ganti Topping\n");
			printf("3. Hapus Pesanan\n");
			printf("4. Hapus Semua Pesanan\n");
			printf("5. Bayar\n");
			printf("6. Keluar\n");
			do
			{
				menu_input=0;
				printf("Choose >> ");	
				scanf("%d",&menu_input);
				getchar();
			}while(menu_input<1||menu_input>6);
			switch(menu_input)
			{
				case 1:
				{
					if(totalStock/3>0)
						add();
					else
					{
						printf("Maaf, stock kami sudah habis\n");
						getchar();
					}
					break;
				}
				case 2:
				{
					edit();
					break;
				}
				case 3:
				{
					remove();
					break;
				}	
				case 4:
				{
					removeAll();
					break;
				}
				case 5:
				{
					if(jumlahPorsi>0)
					{
						finish();
						save();
						done();
						updateStock();
					}
					else
					{
						printf("Anda belum memesan apapun\n");
					}
					getchar();
					break;
				}
			}
	
		}while(menu_input!=6);		
	}	
}
//

int checkLogin(int jlh, char* usernamekey, char* passwordkey, User listusr[])
{
	for(int i = 0; i < jlh; i++)
	{
		//printf("%s", listusr[i].password);
		if(strcmp(passwordkey, listusr[i].password) == 0 && strcmp(usernamekey, listusr[i].username) == 0)
		{
			return i;
		}
	}

	return -1;
}

void printLoadScreen()
{
	
	printf("   _____  .__       .__                __________        .__  .__          \n");
	_sleep(100);
	printf("  /     \\ |__| ____ |__| ____   ____   \\______   \\_____  |  | |  |   ______\n");
	_sleep(100);
	printf(" /  \\ /  \\|  |/    \\|  |/  _ \\ /    \\   |    |  _/\\__  \\ |  | |  |  /  ___/\n");
	_sleep(100);
	printf("/    Y    \\  |   |  \\  (  <_> )   |  \\  |    |   \\ / __ \\|  |_|  |__\\___ \\ \n");
	_sleep(100);
	printf("\\____|__  /__|___|  /__|\\____/|___|  /  |______  /(____  /____/____/____  >\n");
	_sleep(100);
	printf("        \\/        \\/               \\/          \\/      \\/               \\/ \n");
	_sleep(100);

}

void printExitScreen()
{
	
	printf("___________.__                   __     _____.___.             \n");
	_sleep(100);
	printf("\\__    ___/|  |__ _____    ____ |  | __ \\__  |   | ____  __ __ \n");
	_sleep(100);
	printf("  |    |   |  |  \\\\__  \\  /    \\|  |/ /  /   |   |/  _ \\|  |  \\\n");
	_sleep(100);
	printf("  |    |   |   Y  \\/ __ \\|   |  \\    <   \\____   (  <_> )  |  /\n");
	_sleep(100);
	printf("  |____|   |___|  (____  /___|  /__|_ \\  / ______|\\____/|____/ \n");
	_sleep(100);
	printf("                \\/     \\/     \\/     \\/  \\/                    \n");
	_sleep(500);

}

void setColor(WORD w){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), w);
}

int binarySearch(int left, int right, char key[], struct User tmpUsr[], int jlh)
{
	int mid = (int)(left + right)/2;
	if(left <= right){
		if(strcmp(key, tmpUsr[mid].username) == 0)
		{
			//printf("a\n");
			return mid;
		}
		else
		{
		
			if(strcmp(key, tmpUsr[mid].username) < 0)
			{
				right = mid - 1;
				return binarySearch(left, right, key, tmpUsr, jlh);
			}
			else
			{
				left = mid + 1;
				return binarySearch(left, right, key, tmpUsr, jlh);
			}
		}
	}
	return -1;
}

int binarySearchTopp(int left, int right, char key[], struct Topping tmpTopp[], int jlh)
{
	int mid = (int)(left + right)/2;
	if(left <= right){
		if(strcmp(key, tmpTopp[mid].topping) == 0)
		{
			//printf("a\n");
			return mid;
		}
		else
		{
			//printf("b\n");
			if(strcmp(key, tmpTopp[mid].topping) < 0)
			{
				right = mid - 1;
				return binarySearchTopp(left, right, key, tmpTopp, jlh);
			}
			else
			{
				left = mid + 1;
				return binarySearchTopp(left, right, key, tmpTopp, jlh);
			}
		}
	}
	return -1;
}

void viewUser(char* type)
{
	if(countUser == 0)
	{
		printf("No Data Available...");
		flagStatus = 0;
		getchar();
	}
	else
	{
		flagStatus = 1;
		printEnter('\n', 40); 
		printf("Data User\n");
		printf("=====================\n");
		printf("Sorted by username\n");
		printf("=====================================================================================\n");
		printf("| No | Username             | Full Name                                | Type       |\n");
		printf("=====================================================================================\n");
		
		struct User tempListUser[255];
		for(int i = 0; i < countUser; i++)
		{
			tempListUser[i] = listUser[i];
		}
		if(strcmp(type, "sort") == 0)
		{
			
			for(int i = 0; i < countUser - 1; i++)
			{
				for(int j = 0; j < countUser - 1 - i; j++)
				{
					if(strcmp(tempListUser[j].username, tempListUser[j+1].username) > 0)
					{
						struct User tempUser = tempListUser[j];
						tempListUser[j] = tempListUser[j + 1];
						tempListUser[j + 1] = tempUser;
					}
				}
			}
			
		}
		for(int i = 0; i < countUser; i++)
		{
			printf("| %-2d | %-20s | %-40s | %-10s | \n", i + 1, tempListUser[i].username, tempListUser[i].fullName, tempListUser[i].userType);
		}
		printf("=====================================================================================\n");

	}
}

void inputUserInfo(char* type)
{
	do
	{
		flag = 1;
		strcpy(tempUsername, "");
		printf("Input Username [first letter must be lowercase | may not contain space | max 20 characters] : ");
		scanf("%s", tempUsername);
		while(getchar() != '\n');

		if(strlen(tempUsername) > 0)
		{
			for(int i = 0; i < strlen(tempUsername); i++)
			{
				if(i == 0)
				{
					if(!islower(tempUsername[i]))
					{
						flag = 0;
						break;
					}
				}
				else
				{
					if(!isalnum(tempUsername[i]))
					{
						flag = 0;
						break;
					}
				}
			}
			if(strcmp(type, "insert") == 0)
			{
				int idx = binarySearch(0, countUser - 1, tempUsername, listUser, countUser);
				if(idx != -1)
				{
					flag = 0;
					printf("Username Already Exists!\n");
				}
			}
		}
		else
		{
			flag = 0;
			
		}

	}while(flag == 0);

	do
	{
		strcpy(tempPassword, "");
		printf("Input Password [6 - 20 characters]:\n");
		char ch;
		int a = 0;
		while (1) {
			ch = getch();
 
			if (ch == 13)
				break;
			if (ch == 8) /*ASCII value of BACKSPACE*/
			{
				putch('\b');
				putch(NULL);
				putch('\b');
				a--;
				continue;
			}
 
			tempPassword[a++] = ch;
			ch = '*';
			putch(ch);
		}
		tempPassword[a] = '\0';
	}while(strlen(tempPassword) < 5 || strlen(tempPassword) > 20);

	do
	{
		strcpy(tempFullName, "");
		flag = 1;
		printf("\nInput Your Full Name [may not contain special characters | max 40 characters | alphabet only] : ");
		scanf("%[^\n]", tempFullName);
		while(getchar () != '\n');

		if(strlen(tempFullName) < 1)
		{
			flag = 0;
			
		}
		else
		{
			for(int i = 0; i < strlen(tempFullName); i++)
			{
				if(!isspace(tempFullName[i]))
				{
					if(!isalpha(tempFullName[i]))
					{
						flag = 0;
						break;
					}
				}
			}
		}

	}while(flag == 0);

	do
	{
		strcpy(tempUserType, "");
		printf("Input User Type ['Admin' or 'Worker'] : ");
		scanf("%s", tempUserType);
		while(getchar () != '\n');

	}while(strcmp(tempUserType, "Admin") != 0 && strcmp(tempUserType, "Worker") != 0);

}

void insertUser()
{
	printEnter('\n', 40);
	inputUserInfo("insert");
	strcpy(listUser[countUser].username, tempUsername);
	strcpy(listUser[countUser].password, tempPassword);
	strcpy(listUser[countUser].fullName, tempFullName);
	strcpy(listUser[countUser].userType, tempUserType);
	countUser++;
	printf("Success...\n");
	printf("Press Enter to continue...");
	getchar();


}

void updateUser()
{
	viewUser("sort");
	if(flagStatus == 1)
	{
		int idx;
		char tmpUsernameUpdate[255];
		do
		{
			flag = 1;
			
			printf("Input Username that you want to update : ");
			scanf("%s", tmpUsernameUpdate);
			while(getchar() != '\n');

			idx = binarySearch(0, countUser - 1, tmpUsernameUpdate, listUser, countUser);
			if(idx == -1)
			{
				flag = 0;
				printf("Username Not Found!\n");
			}

		}while(flag == 0);

		inputUserInfo("update");
		strcpy(listUser[idx].username, tempUsername);
		strcpy(listUser[idx].password, tempPassword);
		strcpy(listUser[idx].fullName, tempFullName);
		strcpy(listUser[idx].userType, tempUserType);
		printf("Success...\n");
		printf("Press Enter to continue...");
		getchar();
	}
}

void saveAllUser()
{
	printf("Saving All User...\n");
	printf("Please wait...\n");
	
	Userfp = fopen("User.txt", "w+");
	for(int i = 0; i < countUser; i++)
	{
		fprintf(Userfp, "%s#%s#%s#%s\n", listUser[i].username, listUser[i].password, listUser[i].fullName, listUser[i].userType);
	}
	fclose(Userfp);
	_sleep(2000);
}

void deleteUser()
{
	viewUser("sort");
	if(flagStatus == 1)
	{
		int idx;
		char tmpUsernameDelete[255];
		do
		{
			flag = 1;
			
			printf("Input Username that you want to delete : ");
			scanf("%s", tmpUsernameDelete);
			while(getchar() != '\n');

			idx = binarySearch(0, countUser - 1, tmpUsernameDelete, listUser, countUser);
			if(idx == -1)
			{
				flag = 0;
				printf("Username Not Found!\n");
			}

		}while(flag == 0);

		for(int i = idx; i < countUser; i++)
		{
			strcpy(listUser[i].username, listUser[i + 1].username);
			strcpy(listUser[i].password, listUser[i + 1].password);
			strcpy(listUser[i].fullName, listUser[i + 1].fullName);
			strcpy(listUser[i].userType, listUser[i + 1].userType);
		}
		countUser--;
		printf("Success...\n");
		printf("Press Enter to continue...");
		getchar();
	}
}

void dataUser()
{
	Userfp = fopen("User.txt", "a+");
	countUser = 0;
	while(fscanf(Userfp, "%[^#]#%[^#]#%[^#]#%s\n", listUser[countUser].username, listUser[countUser].password, listUser[countUser].fullName, listUser[countUser].userType) != EOF)
	{

		countUser++;
	}
	fclose(Userfp);
	
	

	do
	{	
		printEnter('\n', 40);   
		printLoadScreen();
		for(int i = 0; i < countUser - 1; i++)
		{
			for(int j = 0; j < countUser - 1 - i; j++)
			{
				if(strcmp(listUser[j].username, listUser[j+1].username) > 0)
				{
					struct User tempUser = listUser[j];
					listUser[j] = listUser[j + 1];
					listUser[j + 1] = tempUser;
				}
			}
		}
		setColor(0x0B);
		printf("\n\nWelcome, %s\n", loginUser->fullName);
		setColor(0x0F); 
		printf("Data User\n");
		printf("=====================\n");
		printf("1. Insert new User\n");
		printf("2. Update User\n");
		printf("3. View User\n");
		printf("4. Delete User\n");
		printf("5. Save All & Back\n");
		printf("Choose>> ");
		scanf("%d", &inputDataUser);
		while(getchar() != '\n');

		switch(inputDataUser)
		{
			case 1 : 
				insertUser();

				break;

			case 2 : 
				updateUser();

				break;

			case 3 : 
				viewUser("sort");
				if(flagStatus == 1)
				getchar();

				break;

			case 4 : 
				deleteUser();

				break;

			case 5 : 
				saveAllUser();

				break;
		}

	}while(inputDataUser != 5);
}

void viewTopp(char* type)
{
	if(countTopp == 0)
	{
		printf("No Data Available...");
		flagStatus = 0;
		getchar();
	}
	else
	{
		flagStatus = 1;
		printEnter('\n', 40); 
		printf("Data Topping\n");
		printf("=====================\n");
		printf("Sorted by Topping\n");
		printf("===============================================\n");
		printf("| No | Topping                        | Stock |\n");
		printf("===============================================\n");
		
		struct Topping tempListTopp[255];
		for(int i = 0; i < countTopp; i++)
		{
			tempListTopp[i] = listTopp[i];
		}
		if(strcmp(type, "sort") == 0)
		{
			
			for(int i = 0; i < countTopp - 1; i++)
			{
				for(int j = 0; j < countTopp - 1 - i; j++)
				{
					if(strcmp(tempListTopp[j].topping, tempListTopp[j+1].topping) > 0)
					{
						struct Topping tempTopp = tempListTopp[j];
						tempListTopp[j] = tempListTopp[j + 1];
						tempListTopp[j + 1] = tempTopp;
					}
				}
			}
			
		}
		for(int i = 0; i < countTopp; i++)
		{
			printf("| %-2d | %-30s | %-5d |\n", i + 1, tempListTopp[i].topping, tempListTopp[i].stock);
		}
		printf("===============================================\n");

	}
}

void inputToppInfo(char *type)
{
	do
	{
		flag = 1;
		strcpy(tempTopping, "");
		printf("Input Topping [first letter must be Uppercase | may not contain space and special characters | max 30 characters] : \n");
		scanf("%[^\n]", tempTopping);
		while(getchar() != '\n');

		if(strlen(tempTopping) > 0)
		{
			for(int i = 0; i < strlen(tempTopping); i++)
			{
				if(i == 0)
				{
					if(!isupper(tempTopping[i]))
					{
						flag = 0;
						break;
					}
				}
				else
				{
					if(!isspace(tempTopping[i]))
					{
						if(!isalnum(tempTopping[i]))
						{
							flag = 0;
							break;
						}
					}
				}
			}
			if(strcmp(type,"insert") == 0)
			{
				int idx = binarySearchTopp(0, countTopp - 1, tempTopping, listTopp, countTopp);
				if(idx != -1)
				{
					flag = 0;
					printf("%d", idx);
					printf("Topping Already Exists!\n");
				}
			}
		}
		else
		{
			flag = 0;
			
		}

	}while(flag == 0);

	do
	{
		printf("Input Stock [more than 0] : ");
		scanf("%d", &tempStock);
		while(getchar () != '\n');

	}while(tempStock < 1);

}

void insertTopp()
{
	printEnter('\n', 40);
	inputToppInfo("insert");
	strcpy(listTopp[countTopp].topping, tempTopping);
	listTopp[countTopp].stock = tempStock;
	countTopp++;
	printf("Success...\n");
	printf("Press Enter to continue...");
	getchar();


}

void updateTopp()
{
	viewTopp("sort");
	
	if(flagStatus == 1)
	{
		int idx;
		char tmpToppUpdate[255];
		struct Topping tempListTopp[255];
		for(int i = 0; i < countTopp; i++)
		{
			tempListTopp[i] = listTopp[i];
		}
			
		for(int i = 0; i < countTopp - 1; i++)
		{
			for(int j = 0; j < countTopp - 1 - i; j++)
			{
				if(strcmp(tempListTopp[j].topping, tempListTopp[j+1].topping) > 0)
				{
					struct Topping tempTopp = tempListTopp[j];
					tempListTopp[j] = tempListTopp[j + 1];
					tempListTopp[j + 1] = tempTopp;
				}
			}
		}
		do
		{
			flag = 1;
			
			printf("Input Topping Name that you want to update : ", countTopp);
			scanf("%[^\n]", tmpToppUpdate);
			while(getchar() != '\n');

			idx = binarySearchTopp(0, countTopp - 1, tmpToppUpdate, listTopp, countTopp);
			if(idx == -1)
			{
				flag = 0;
				printf("Topping Not Found!\n");
			}

		}while(flag == 0);

		inputToppInfo("update");
		strcpy(listTopp[idx].topping, tempTopping);
		listTopp[idx].stock = tempStock;
		printf("Success...\n");
		printf("Press Enter to continue...");
		getchar();
	}
}

void deleteTopp()
{
	viewTopp("sort");
	
	if(flagStatus == 1)
	{
		int idx;
		char tmpToppDelete[255];
		struct Topping tempListTopp[255];
		for(int i = 0; i < countTopp; i++)
		{
			tempListTopp[i] = listTopp[i];
		}
			
		for(int i = 0; i < countTopp - 1; i++)
		{
			for(int j = 0; j < countTopp - 1 - i; j++)
			{
				if(strcmp(tempListTopp[j].topping, tempListTopp[j+1].topping) > 0)
				{
					struct Topping tempTopp = tempListTopp[j];
					tempListTopp[j] = tempListTopp[j + 1];
					tempListTopp[j + 1] = tempTopp;
				}
			}
		}
			
		
		do
		{
			flag = 1;
			
			printf("Input Topping Name that you want to delete : ", countTopp);
			scanf("%[^\n]", tmpToppDelete);
			while(getchar() != '\n');

			idx = binarySearchTopp(0, countTopp - 1, tmpToppDelete, tempListTopp, countTopp);
			if(idx == -1)
			{
				flag = 0;
				printf("%d", idx);
				printf("Topping Not Found!\n");
			}

		}while(flag == 0);

		for(int i = idx; i < countTopp; i++)
		{
			strcpy(listTopp[i].topping, listTopp[i + 1].topping);
			listTopp[i].stock = listTopp[i + 1].stock;
		}
		countTopp--;
		printf("Success...\n");
		printf("Press Enter to continue...");
		getchar();
	}
}

void saveAllTopp()
{
	printf("Saving All Topping...\n");
	printf("Please wait...\n");
	
	Toppfp = fopen("topping.txt", "w+");
	for(int i = 0; i < countTopp; i++)
	{
		fprintf(Toppfp, "%s#%d\n", listTopp[i].topping, listTopp[i].stock);
	}
	fclose(Toppfp);
	_sleep(2000);
	
}

void dataTopping()
{
	Toppfp = fopen("topping.txt", "a+");
	countTopp = 0;
	while(fscanf(Toppfp, "%[^#]#%d\n", listTopp[countTopp].topping, &listTopp[countTopp].stock) != EOF)
	{

		countTopp++;
	}
	fclose(Toppfp);

	

	do
	{
		printEnter('\n', 40);   
		printLoadScreen();
		for(int i = 0; i < countTopp - 1; i++)
		{
			for(int j = 0; j < countTopp - 1 - i; j++)
			{
				if(strcmp(listTopp[j].topping, listTopp[j+1].topping) > 0)
				{
					struct Topping tempTopp = listTopp[j];
					listTopp[j] = listTopp[j + 1];
					listTopp[j + 1] = tempTopp;
				}
			}
		}
		setColor(0x0B);
		printf("\n\nWelcome, %s\n", loginUser->fullName);
		setColor(0x0F); 
		printf("Data Topping\n");
		printf("=====================\n");
		printf("1. Insert new Topping\n");
		printf("2. Update Topping\n");
		printf("3. View Topping\n");
		printf("4. Delete Topping\n");
		printf("5. Save All & Back\n");
		printf("Choose>> ");
		scanf("%d", &inputDataTopp);
		while(getchar() != '\n');

		switch(inputDataTopp)
		{
			case 1 : 
				insertTopp();

				break;

			case 2 : 
				updateTopp();

				break;

			case 3 : 
				viewTopp("sort");
				if(flagStatus == 1)
				getchar();

				break;

			case 4 : 
				deleteTopp();

				break;

			case 5 : 
				saveAllTopp();

				break;
		}

	}while(inputDataTopp != 5);
}

void saveAllData()
{
	printf("Saving All Topping...\n");
	printf("Please wait...\n");
	Toppfp = fopen("topping.txt", "w+");
	for(int i = 0; i < countTopp; i++)
	{
		fprintf(Toppfp, "%s#%d\n", listTopp[i].topping, listTopp[i].stock);
	}
	fclose(Toppfp);
	_sleep(1000);
	printf("Saving All User...\n");
	printf("Please wait...\n");
	
	Userfp = fopen("User.txt", "w+");
	for(int i = 0; i < countUser; i++)
	{
		fprintf(Userfp, "%s#%s#%s#%s\n", listUser[i].username, listUser[i].password, listUser[i].fullName, listUser[i].userType);
	}
	fclose(Userfp);
	_sleep(1000);
}

void showTransaksi()
{
	printEnter('\n', 40); 
	printf("Data Transaksi\n");
	printf("=====================\n");
	printf("Sorted by name\n");
	printf("=======================================================================================================================\n");
	printf("| No | Name                           | Code  | Grand Total          | Jumlah Porsi | List Rasa            | Jlh Rasa |\n");
	printf("=======================================================================================================================\n");
		
	struct Transaction tmpListTrans[255];
	for(int i = 0; i < countTrans; i++)
	{
		tmpListTrans[i] = listTrans[i];
	}
	for(int i = 0; i < countTrans - 1; i++)
	{
		for(int j = 0; j < countTrans - 1 - i; j++)
		{
			if(strcmp(tmpListTrans[j].name, tmpListTrans[j+1].name) > 0)
			{
				struct Transaction tempTrans = tmpListTrans[j];
				tmpListTrans[j] = tmpListTrans[j + 1];
				tmpListTrans[j + 1] = tempTrans;
			}
		}
	}
		
		
	for(int i = 0; i < countTrans; i++)
	{
		printf("| %-2d | %-30s | %-5s | %-20lld | %-12d | ", i + 1, tmpListTrans[i].name, tmpListTrans[i].code, tmpListTrans[i].grandtotal, tmpListTrans[i].jlhporsi);
			
		int countTotalDetail = 0;
			
		for(int j = 0; j < countTransDetil; j++)
		{
			if(strcmp(tmpListTrans[i].code, listTransDetail[j].code) == 0)
			{
				strcpy(tempListTransDetail[countTotalDetail].code , listTransDetail[j].code);
				strcpy(tempListTransDetail[countTotalDetail].topping , listTransDetail[j].topping);
				tempListTransDetail[countTotalDetail].jlhtopp = listTransDetail[j].jlhtopp;
				countTotalDetail++;
			}
		}
		///printf("%d", countTotalDetail);
		printf("%-20s | %-8d |\n", tempListTransDetail[0].topping, tempListTransDetail[0].jlhtopp);
		for(int i = 1; i < countTotalDetail; i++)
		{
			printf("|    |                                |       |                      |              | %-20s | %-8d |\n", tempListTransDetail[i]. topping, tempListTransDetail[i].jlhtopp);
		}

	}
	printf("=======================================================================================================================\n");
}

void viewSearchTrans(char* key)
{
	printEnter('\n', 40); 
	printf("Data Transaksi\n");
	printf("=====================\n");
	printf("Search by %s\n", key);
	printf("=======================================================================================================================\n");
	printf("| No | Name                           | Code  | Grand Total          | Jumlah Porsi | List Rasa            | Jlh Rasa |\n");
	printf("=======================================================================================================================\n");
		
	struct Transaction tmpListTrans[255];
	for(int i = 0; i < countTrans; i++)
	{
		tmpListTrans[i] = listTrans[i];
	}
	for(int i = 0; i < countTrans - 1; i++)
	{
		for(int j = 0; j < countTrans - 1 - i; j++)
		{
			if(strcmp(tmpListTrans[j].name, tmpListTrans[j+1].name) > 0)
			{
				struct Transaction tempTrans = tmpListTrans[j];
				tmpListTrans[j] = tmpListTrans[j + 1];
				tmpListTrans[j + 1] = tempTrans;
			}
		}
	}
		
	int a = 0;
	for(int i = 0; i < countTrans; i++)
	{
		if(strcmp(key, tmpListTrans[i].name) == 0)
		{
			printf("| %-2d | %-30s | %-5s | %-20lld | %-12d | ", a + 1, tmpListTrans[i].name, tmpListTrans[i].code, tmpListTrans[i].grandtotal, tmpListTrans[i].jlhporsi);
			a++;
			int countTotalDetail = 0;
			
			for(int j = 0; j < countTransDetil; j++)
			{
				if(strcmp(tmpListTrans[i].code, listTransDetail[j].code) == 0)
				{
					strcpy(tempListTransDetail[countTotalDetail].code , listTransDetail[j].code);
					strcpy(tempListTransDetail[countTotalDetail].topping , listTransDetail[j].topping);
					tempListTransDetail[countTotalDetail].jlhtopp = listTransDetail[j].jlhtopp;
					countTotalDetail++;
				}
			}
			///printf("%d", countTotalDetail);
			printf("%-20s | %-8d |\n", tempListTransDetail[0].topping, tempListTransDetail[0].jlhtopp);
			for(int i = 1; i < countTotalDetail; i++)
			{
				printf("|    |                                |       |                      |              | %-20s | %-8d |\n", tempListTransDetail[i]. topping, tempListTransDetail[i].jlhtopp);
			}
		}
	}
	printf("=======================================================================================================================\n");
}

void searchData()
{
	do
	{
		flag = 1;
		setColor(0x0C); 
		printf("*will display all transaction with this name\n");
		setColor(0x0F); 
		printf("Input name to search ['0' to exit] : ");
		scanf("%[^\n]", searchNama);
		while(getchar() != '\n');

		if(strlen(searchNama) > 0)
		{
			if(searchNama[0] == '0')
			{
				break;
			}
			else
			{
				int idx = -1;
				for(int i = 0; i < countTrans; i++)
				{
					if(strcmp(searchNama, listTrans[i].name) == 0)
					{
						idx = i;
						break;
					}
				}
				if(idx == -1)
				{
					printf("Transaction with this name not found!\n");
					flag = 0;
				}
				else
				{
					viewSearchTrans(searchNama);
					getchar();
				}
			}

		}

	}while(flag == 0);
}

void listTransaksi()
{
	if(countTrans == 0)
	{
		printf("No Data Available...");
		flagStatus = 0;
		getchar();
	}
	else
	{
		flagStatus = 1;
		
		do
		{
			showTransaksi();
			printf("1. Search Data\n");
			printf("2. Exit\n");
			printf("Choose>> ");
			scanf("%d", &inputDataSearch);
			while(getchar() != '\n');

			switch(inputDataSearch)
			{
				case 1 :
					searchData();

					break;
			}

		}while(inputDataSearch != 2);
	}
}

void dataTransaction()
{
	Transfp = fopen("transaksi.txt", "a+");
	countTrans = 0;
	while(fscanf(Transfp, "%[^#]#%[^#]#%lld#%d\n", listTrans[countTrans].name, listTrans[countTrans].code, &listTrans[countTrans].grandtotal, &listTrans[countTrans].jlhporsi) != EOF)
	{

		countTrans++;
	}
	fclose(Transfp);
	Transfp = fopen("transaksidetil.txt", "a+");
	countTransDetil = 0;
	while(fscanf(Transfp, "%[^#]#%[^#]#%d\n", listTransDetail[countTransDetil].code, listTransDetail[countTransDetil].topping, &listTransDetail[countTransDetil].jlhtopp) != EOF)
	{

		countTransDetil++;
	}
	fclose(Transfp);

	do
	{
		printEnter('\n', 40);   
		printLoadScreen();
		for(int i = 0; i < countTrans - 1; i++)
		{
			for(int j = 0; j < countTrans - 1 - i; j++)
			{
				if(strcmp(listTrans[j].name, listTrans[j+1].name) > 0)
				{
					struct Transaction tempTrans = listTrans[j];
					listTrans[j] = listTrans[j + 1];
					listTrans[j + 1] = tempTrans;
				}
			}
		}
		setColor(0x0B);
		printf("\n\nWelcome, %s\n", loginUser->fullName);
		setColor(0x0F); 
		printf("Data Transaksi\n");
		printf("=====================\n");
		printf("1. List All Transaksi\n");
		printf("2. Back\n");
		printf("Choose>> ");
		scanf("%d", &inputDataTrans);
		while(getchar() != '\n');

		switch(inputDataTrans)
		{
			case 1 : 
				listTransaksi();

				break;

			case 2 : 
				//updateTopp();

				break;
		}

	}while(inputDataTrans != 2);
}

void adminMenu()
{
	do
	{
		printEnter('\n', 40);   
		printLoadScreen();
		setColor(0x0B);
		printf("\n\nWelcome, %s\n", loginUser->fullName);
		setColor(0x0F); 
		printf("Admin Menu\n");
		printf("=====================\n");
		printf("1. Data Topping\n");
		printf("2. Data User\n");
		printf("3. Data Transaksi\n");
		printf("4. Save All & Back\n");
		printf("Choose>> ");
		scanf("%d", &inputAdmin);
		while(getchar() != '\n');

		switch(inputAdmin)
		{
			case 1 : 
				dataTopping();

				break;

			case 2 : 
				dataUser();

				break;

			case 3 : 
				dataTransaction();

				break;

			case 4 : 
				saveAllData();

				break;
		}

	}while(inputAdmin != 4);
}

void mainMenu()
{
	int exit1;
	do
	{
		printEnter('\n', 40);   
		printLoadScreen();
		if(!loginUser)
		{
			break;
		}
		else
		{
			setColor(0x0B);
			printf("\n\nWelcome, %s\n", loginUser->fullName);
			setColor(0x0F); 
			printf("Main Menu\n");
			printf("=====================\n");
			if(strcmp(loginUser->userType, "Admin") == 0)
			{
				printf("1. Cashier\n");
				printf("2. Admin Menu\n");
				printf("3. Exit\n");
				printf("Choose>> ");
			}
			else
			{
				printf("1. Cashier\n");
				printf("2. Exit\n");
				printf("Choose>> ");
			}
		}
		scanf("%d", &inputMainMenu);
		while(getchar() != '\n');

		if(strcmp(loginUser->userType, "Admin") == 0)
		{
			exit1 = 3;
		}
		else
		{
			exit1 = 2;
		}

		switch(inputMainMenu)
		{
			case 1 :
				user();

				break;

			case 2 :
				if(strcmp(loginUser->userType, "Admin") == 0)
				{
					adminMenu();
				}

				break;

			case 3 :
				

				break;

		}

	}while(inputMainMenu != exit1);

}

void mainApp()
{
	do
	{
		Userfp = fopen("User.txt", "a+");
		countUser = 0;
		while(fscanf(Userfp, "%[^#]#%[^#]#%[^#]#%s\n", listUser[countUser].username, listUser[countUser].password, listUser[countUser].fullName, listUser[countUser].userType) != EOF)
		{

			countUser++;
		}
		
		fclose(Userfp);
		printEnter('\n', 40);   
		printLoadScreen();
		//printf("%d", countUser);
		printf("1. Login\n");
		printf("2. Exit\n");
		if(countUser == 0)
		{
			printf("3. Read this\n");
		}
		
	
	
		printf("Choose>> ");
		scanf("%d", &inputLogin);
		while(getchar() != '\n');

		switch(inputLogin)
		{
			case 1 :
				if(countUser == 0)
				{
					printf("There still no account created\nUse the \"Read this\" menu first\n");
					printf("Press Enter to continue...");
					getchar();
				}
				else
				{
					do
					{
						flag = 1;
						printf("Input Username [May not contain special characters and space] : ");
						scanf("%s", tempLoginUsername);
						while(getchar() != '\n');
						for(int i = 0; i < strlen(tempLoginUsername); i++)
						{
							if(!isalnum(tempLoginUsername[i]))
							{
								flag = 0;
								break;
							}
						}
						if(strlen(tempLoginUsername) < 1)
						{
							flag = 0;
						}
						else
						{
							if(strlen(tempLoginUsername) == 1)
							{
								if(tempLoginUsername[0] == '0')
									break;
							}
						}
					}while(flag == 0);

					do
					{
						flag = 1;
						//printf("Input Password ['0' to exit] : ");
						//scanf("%s", tempLoginPassword);
						//while(getchar() != '\n');
						char ch;
						printf("Input Password : \n");
						int a = 0;
						while (1) {
							ch = getch();
							
							if (ch == 13)
								break;
							if (ch == 8) /*ASCII value of BACKSPACE*/
							{
								putch('\b');
								putch(NULL);
								putch('\b');
								a--;
								continue;
							}
							
 
							tempLoginPassword[a++] = ch;
							ch = '*';
							putch(ch);
						}
						tempLoginPassword[a] = '\0';
						printf("%d", a);
					}while(flag == 0);
					int checkloginstatus = checkLogin(countUser, tempLoginUsername, tempLoginPassword, listUser);
					printf("\n\n");
					if(checkloginstatus == -1)
					{
						printf("\nLogin Failed, Username or Password wrong!\n");
						printf("Press enter to continue...");
						getchar();
					}
					else
					{
						printf("Login Success\n");
						loginUser = (User*)malloc(sizeof(User));
						strcpy(loginUser->username, listUser[checkloginstatus].username);
						strcpy(loginUser->password, listUser[checkloginstatus].password);
						strcpy(loginUser->fullName, listUser[checkloginstatus].fullName);
						strcpy(loginUser->userType, listUser[checkloginstatus].userType);
						printf("Welcome, %s\n", loginUser->fullName);
						printf("Press enter to continue...");
						getchar();

						mainMenu();

					}

				}

				break;

			case 2 :
				printEnter('\n', 40);  
				printExitScreen();
				

				break;

			case 3 :
				if(countUser == 0)
				{
					printf("User created..\n");
					printf("The default user for login is : \n");
					printf("username : admin\n");
					printf("password : admin\n");
					printf("Please change it later\n");
				
					Userfp = fopen("User.txt", "w+");
					fprintf(Userfp, "admin#admin#Admin Minions Balls#Admin\n");

					fclose(Userfp);

					
					printf("Press enter to continue...");
					getchar();
				}
				

				break;


		}

	}while(inputLogin != 2);

	

}

int main()
{
	system("mode 150, 35");
	mainApp();

	return 0;
}
