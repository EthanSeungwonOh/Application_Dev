//Form.c
#include "form.h"
#include <string.h>
#include <time.h>
const char* movieNames[] = { "HarryPotter", "Mission", "LordOfRings", "Notebook", "SocialNetwork" };
const char* dayNames[] = { "today", "oneday", "twoday", "threeday", "fourday", "fiveday", "sixday" };
const char* roomNames[] = { "firstRoom", "secondRoom", "thirdRoom", "fourthRoom", "fifthRoom" };
const char* timeNames[] = { "Morning", "Afternoon", "Evening"};

//#if 0
int main(int argc, char *argv[]) {
	int menu;
	Ticket* ticketBook[CountOfWeekDay];
	CreateTicketBook(&ticketBook);

	DisplayMenu();
	fflush(stdin);
	printf("\t\t\t     ");
	scanf("%d", &menu);

	while (menu != 0) {
		switch (menu) {
		case 1: FormForReservation(&ticketBook); break;
		case 2: FormForCancelReservation(&ticketBook); break;
		default: break;
		}
		DisplayMenu(); fflush(stdin); printf("\t\t\t     "); scanf("%d", &menu);
	}

	return 0;
}
//#endif

/*DisplayMenu: display Main UI menu*/
void DisplayMenu() {
	system("cls");
	printf("\n\n\n\n\n");
	printf("\t\t\t     Ticketing Service V0.1\n");
	printf("\t\t\t     ======================\n");
	printf("\t\t\t     [1]Reservation\n");
	printf("\t\t\t     [2]Cancel Reservation\n");
	printf("\t\t\t     Select Menu!\n");
}

/*FormForReservation: When user select 'Reservation' in menu, this UI will activate*/
void FormForReservation(Ticket*(*ticketBook)) {
	/*
	1. ��¥�� ����(�����̳� ������ Ŭ�� �� ���ο� ������ �ٽ� ���
	2. ��ȭ�� ����
	3. �ð��� ����
	4. �ο��� ����
	5. ���� �ڸ��� ���÷���
	6. �ο��� ��ŭ �ڸ��� ����
	7. �����ϸ� ���õ� �� �ڸ��� ���÷���
	*/
	char day[255] = "today";
	int intDay = 0;
	char movie[255];
	int intMovie;
	char movieTime[255];
	int intTime;
	int seatId;
	int seatIdList[6];
	int cidList[6];
	int numOfBooker;
	int i;
	int j;
	char finish[25] = "N";
	bool selectDay = false;

	while (selectDay != true) {

		system("cls");
		printf("\n\n\n\n\n");
		printf("\t\t\t     Reservation\n");
		printf("\t\t\t     ======================\n");
		printf("\t\t\t     <<%s>> \n\n", dayNames[intDay]);

		for (i = 0; i < CountOfMovieList; i++) {
			printf("\t\t\t     ");
			printf(" %s ", movieNames[i]);//because movie list is fixed, all movies will be shown
			/*choose time that correspond with the day and movie*/
			int times[3] = { -1 };
			FindMovieShowTimes(ticketBook, intDay, i, times);
			for (j = 0; j < 3; j++) {
				printf(" %s ||", timeNames[times[j]]);
			}
			printf("\n");
		}
		printf("\t\t\t     ======================\n");
		printf("\t\t\t     Do you want to change the date?(prev/next/ok)\n");
		printf("\t\t\t    ");
		scanf("%s", &day);

		if (strcmp(&day, "ok") == 0) {
			selectDay = true;
			printf("\t\t\t     ======================\n");
			printf("\t\t\t     Select the movie. Write the movie title. ex.HarryPotter\n");
			printf("\t\t\t    ");
			scanf("%s", &movie);
			printf("\t\t\t     ======================\n");
			printf("\t\t\t     Select the time. Write the time of the movie. ex)Morning\n");
			printf("\t\t\t    ");
			scanf("%s", &movieTime);
			printf("\t\t\t     ======================\n");
			printf("\t\t\t     Choose the number of ticket.(Max number is 6) \n");
			printf("\t\t\t    ");
			scanf("%d", &numOfBooker);
			//Applied FormForDisplaySeat hear
			system("cls");
			printf("\n\n\n\n\n");
			printf("\t\t\t     Reservation\n");
			printf("\t\t\t     ======================\n");
			
			int index = 0;
			//change string movie to int number
			for (j = 0; j < CountOfMovieList; j++) {
				if (strcmp(movieNames[j], movie) == 0) {
					intMovie = j;
				}
			}
			//change string time to int number
			for (j = 0; j < CountOfTime; j++) {
				if (strcmp(timeNames[j], movieTime) == 0) {
					intTime = j;
				}
			}
			printf("\t\t\t     day:%s movie:%s time:%s\n", dayNames[intDay], movieNames[intMovie], timeNames[intTime]);
			printf("\t\t\t     ======================\n");
			FormForDisplaySeat(ticketBook, intTime, intMovie, intDay);
			i = numOfBooker;
			int cid; //unique random num
			Data data;
			int result = 0;

			i = 0;
			while (i < numOfBooker) {
				printf("\t\t\t     Choose the seat\n");
				printf("\t\t\t     ======================\n");
				printf("\t\t\t     ");
				scanf("%d", &seatId);
				seatId--; //beacuse system based on 0

				time_t rawtime;
				struct tm * timeinfo;
				
				rawtime = time(NULL);
				timeinfo = localtime(&rawtime);
				//printf("[%d %d %d %d:%d:%d]", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

				cid = (timeinfo->tm_hour) * 10000 + (timeinfo->tm_min) * 100 + timeinfo->tm_sec;
				cidList[i] = cid;
				data.cId = cid;
				data.seatId = seatId;
				result = Reservation(ticketBook, intTime, intMovie, intDay, data);
				if (result == 0)
					printf("\t\t\t     already seat is occupied. Choose another seat.\n");
				else {
					seatIdList[i] = seatId;
					i++;
					printf("\t\t\t     ======================\n");
					FormForDisplaySeat(ticketBook, intTime, intMovie, intDay);
				}
			}

			printf("\t\t\t     Reservation complete.\n");
			for (i = 0; i < numOfBooker; i++) {
				printf("\t\t\t     ======================\n");
				printf("\t\t\t     cid:%d seatId:%d day:%s movie:%s time:%s\n", cidList[i], seatIdList[i] + 1, dayNames[intDay], movieNames[intMovie], timeNames[intTime]);
			}
			printf("\n");
			printf("\t\t\t     Press any key.\n");
			scanf("%s", &finish);
		}
		else {
			if (strcmp(&day, "prev") == 0 && intDay > 0) {
				intDay--;
			}
			else if (strcmp(&day, "next") == 0 && intDay < 6) {
				intDay++;
			}

		}

	}


}

void FormForCancelReservation(Ticket*(*ticketBook)) {
	/* FormForCancleReservation: When user select 'Cancel Reservation' in menu, this UI will activate */
	int seatId, movieTime, movieList, day;
	char isCancel[20];
	int result = 0;

	while (1) {
		printf("\t\t     cancel reservation \n");
		printf("\t\t     ======================\n");

		printf("\t\t     please type your date. [in numbers]\n");
		printf("\t\t     (0 - today, 1 - oneday, 2 - twoday, 3 - threeday, 4 - fourday, 5 - fiveday, 6 - sixday)\n");
		fflush(stdin); printf("\t\t     "); scanf("%d", &day); /* insert your movie */
		printf("\t\t     please type your movie. [in numbers]\n");
		printf("\t\t     (0 - HarryPotter, 1 - Mission, 2 - LordOfRings, 3 - Notebook, 4 - SocialNetwork)\n");
		fflush(stdin);  printf("\t\t     "); scanf("%d", &movieList); /* insert your movie */
		printf("\t\t     please type your time. [in numbers] (0 - Morning, 1 - Afternoon, 2 - Evening)\n");
		fflush(stdin); printf("\t\t     "); scanf("%d", &movieTime); /* insert your day */
		printf("\t\t     please type your reservation number(seat id). \n");
		fflush(stdin); printf("\t\t     "); scanf("%d", &seatId); /* insert your seatId number */

		printf("\t\t     day : %s, movie : %s, time : %s, Reservation number : %d \n", dayNames[day], movieNames[movieList], timeNames[movieTime], seatId);
		printf("\t\t     do you really want to cancel your reservation? (type 'yes' or 'no')\n");
		fflush(stdin); printf("\t\t     "); scanf("%s", isCancel);

		if (!strncmp(isCancel, "yes", 3)) {
			result = CancelReservation(ticketBook, movieTime, movieList, day, seatId-1);
			if (result == 1) {
				FormForDisplaySeat(ticketBook, movieTime, movieList, day);
			}
			printf("\t\t     Do you want to return to the main menu? (type 'yes' or 'no') \n");
			fflush(stdin); scanf("%s", isCancel);

			if (!strncmp(isCancel, "yes", 2))break;
			else if (!strncmp(isCancel, "no", 2)) continue;
			else
				printf("\t\t     you must type either 'yes' or 'no'. \n");
		}

		else if (!strncmp(isCancel, "no", 2)) {
			printf("\t\t     your reservation has not been canceled. \n");
			printf("\t\t     Do you want to return to the main menu? (type 'yes' or 'no') \n");
			fflush(stdin); printf("\t\t     "); scanf("%s", isCancel);

			if (!strncmp(isCancel, "yes", 2)) break;
			else if (!strncmp(isCancel, "no", 2)) continue;
			else
				printf("\t\t     you must type either 'yes' or 'no'. \n");
		}

		else
			printf("\t\t     you must type either 'yes' or 'no'. \n");
	}
}

void FormForDisplaySeat(Ticket*(*ticketBook), int intTime, int intMovie, int intDay) {

	int tempSeatArray[MAX_SEAT];
	BTree *printTree = Findticket(ticketBook, intTime, intMovie, intDay);

	MakeSeatChart(&tempSeatArray, printTree);

	int occupiedSeat;
	bool occupied = false;
	int i, j;
	int m = 0;
	int n = 0;
	printf("\n\n\t\t\t     ====================��ũ��========================\n\n");
	for (i = 0; i < 15; i++) {
		printf("\t\t\t     ");
		for (j = 0; j < 10; j++) {
			if (occupied == false && n < (printTree->length)) {
				occupiedSeat = tempSeatArray[n];
				n++;
				occupied = true;
			}
			if (occupiedSeat == m) {
				printf("  X  ");
				occupied = false;
			}
			else {
				printf(" %3d ", m + 1);
			}
			m++;
		}
		printf("\n\n\n");
	}
}
