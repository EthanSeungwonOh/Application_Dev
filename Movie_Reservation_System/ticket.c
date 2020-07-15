#define _CRT_SECURE_NO_WARNINGS

#include "ticket.h"
#include "form.h"

#if 0
int main(int argc, char *argv[]) {
	
	int menu;
	Ticket* dayList[CountOfWeekDay];
	CreateTicketBook(&dayList);

	DisplayMenu();
	fflush(stdin);
	scanf("%d", &menu);

	while (menu != 0) {
		switch (menu) {
		case 1: FormForReservation(&dayList); break;
		case 2: FormForCancelReservation(&dayList); break;
		default: break;
		}
		DisplayMenu(); fflush(stdin); scanf("%d", &menu);
	}

	/*
	Data data;
	data.cId = 100;
	data.seatId = 100;
	Data data1;
	data1.cId = 200;
	data1.seatId = 200;
	Data data2;
	data2.cId = 150;
	data2.seatId = 150;
	Data data3;
	data3.cId = 130;
	data3.seatId = 130;
	Data data4;
	data4.cId = 120;
	data4.seatId = 120;
	Data data5;
	data5.cId = 110;
	data5.seatId = 110;
	Data data6;
	data6.cId = 190;
	data6.seatId = 190;
	Data data7;
	data7.cId = 60;
	data7.seatId = 60;

	int result;
	result = Reservation(&dayList, morning, HarryPoter, twoday, data);
	result = Reservation(&dayList, morning, HarryPoter, twoday, data1);
	result = Reservation(&dayList, morning, HarryPoter, twoday, data2);
	result = Reservation(&dayList, morning, HarryPoter, twoday, data3);
	result = Reservation(&dayList, morning, LoardOfRings, today, data4);
	result = Reservation(&dayList, afternoon, LoardOfRings, today, data5);
	result = Reservation(&dayList, evening, LoardOfRings, today, data6); 
	result = Reservation(&dayList, morning, LoardOfRings, today, data7);

	DisplaySeat(&dayList);
	*/

	return 0;
}
#endif

/*
Create:
*/
void CreateTicketBook(Ticket*(*ticketBook)) {
	int i;
	int j;
	int k;
	int m;
	time_t t;
	int p;
	int y;
	int z;
	int cId;
	int movieIndexes[15] = { 0, 1, 2, 0, 1, 1, 0, 2, 2, 3, 3 ,3, 4, 4, 4 };
	int seatId;
	Data data;
	int temp;
	int randomIndex;
	int randomIndexes[MAX_SEAT];
	for (i = 0; i < MAX_SEAT; i++) {
		randomIndexes[i] = i;
	}
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	for (i = 0; i < CountOfWeekDay; i++) {
		//three times per movie
		ticketBook[i] = (Ticket*)calloc(CountOfRoom*CountOfTime, sizeof(Ticket));
		//For the random movie show time 
		for (m = 0; m < CountOfMovieList * CountOfTime; m++) {
			temp = movieIndexes[m];
			randomIndex = rand() % (CountOfMovieList * CountOfTime);
			movieIndexes[m] = movieIndexes[randomIndex];
			movieIndexes[randomIndex] = temp;
		}
		z = 0;
		for (j = 0; j < CountOfRoom; j++) {
			for (k = 0; k < CountOfTime; k++) {
				//�� ��¥�� ��� �ð��� ������
				(ticketBook[i])[z].room = j;
				(ticketBook[i])[z].time = k;
				(ticketBook[i])[z].movie = movieIndexes[z];
				//���¼����� 70%�� ������Ʈ�� �Ҵ��ϱ�
				/*�� �뺰 �¼��� �����ϰ� ����*/
				for (p = 0; p < MAX_SEAT; p++) {
					temp = randomIndexes[p];
					randomIndex = rand() % MAX_SEAT;
					randomIndexes[p] = randomIndexes[randomIndex];
					randomIndexes[randomIndex] = temp;
				}
				//insert 60%, delete 20%, insert 40%, delete 10%
				int firstIndexes[MAX_SEAT];
				int firstIndexCount = 0;
				for (y = 0; y < (MAX_SEAT * 6) / 10; y++) {
					cId = y;
					seatId = randomIndexes[y];
					data.cId = cId;
					data.seatId = seatId;
					Insert(&((ticketBook[i])[z].btree), data);
					firstIndexes[y] = seatId;
					firstIndexCount++;
				}
				int secondIndexCount = 0;
				for (y = 0; y < (MAX_SEAT * 2) / 10; y++) {
					Delete(&(ticketBook[i])[z].btree, firstIndexes[y]);
					secondIndexCount++;
				}
				int ThirdIndex = firstIndexCount;
				for (y = 0; y < (MAX_SEAT * 4) / 10; y++) {
					cId = ThirdIndex;
					seatId = randomIndexes[ThirdIndex];
					data.cId = cId;
					data.seatId = seatId;
					Insert(&(ticketBook[i])[z].btree, data);
					ThirdIndex++;
				}
			
				int fourthIndex = secondIndexCount;
				for (y = 0; y < (MAX_SEAT * 1) / 10; y++) {
					Delete(&(ticketBook[i])[z].btree, firstIndexes[fourthIndex]);
					fourthIndex++;
				}
				
				z++;
			}
		}
	}
}

BTree* Findticket(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day) {
	int i;
	
	/* pass pointer address of the input day to tmp */
	Ticket* tmp = ticketBook[day];

	/* search each element of particular day's ticketbook that matches time and the movie */
	for (i = 0; i<CountOfMovieList*CountOfTime; i++) {
		if (time == tmp[i].time && movieList == tmp[i].movie)
			return &(tmp[i].btree); /* if found, return the address of btree */
	}

	return NULL; /* Findticket failure */
}

int Reservation(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day, Data resinfo) {
	int flag = 0;

	/* pass pointer address to Bdummy using ticketbook */
	BTree* Bdummy = Findticket(ticketBook, time, movieList, day);

	/* execute func insert */
	flag = Insert(Bdummy, resinfo);
	PrintAttribute(Bdummy); //print attribute of inserted binary search tree
	/* print and return value according to insert result */
	if (flag == 1) {
		//printf("reservation successful.\n");
		return 1;
	}
	else {
		//printf("reservation failed - seat occupied.");
		return 0;
	}
}

int CancelReservation(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day, int seatId){
	int flag = 0;
	BTree* Bdummy = Findticket(ticketBook, time, movieList, day); /* pass pointer address to Bdummy */

	flag = Delete(Bdummy, seatId); /* execute func insert */
	PrintAttribute(Bdummy); //print attribute of deleted binary search tree
	if (flag == 1) { /* print and return value according to insert result */
		//printf("cancellation successful.");
		return 1;
	}
	else {
		//printf("cancellation failed - no seat was found with your cId.");
		return 0;
	}
}


void DisplaySeat(Ticket*(*ticketBook)) {
	int i;
	int j;
	int m;
	for (i = 0; i < CountOfWeekDay; i++) {
		printf("day: %d\n", i);
		m = 0;for (j = 0; j < CountOfRoom*CountOfTime; j++) {
			printf("room: %d ", (ticketBook[i])[j].room);
			printf("time: %d ", (ticketBook[i])[j].time);
			printf("movie: %d ", (ticketBook[i])[j].movie);
			printf("================================\n");
			printf("seat status\n");
			Print((ticketBook[i])[j].btree.root);
		}
		printf("printed count: %d\n", j);
	}
}

/*
FindMovieShowtimes: find movie show time at the day and the time
*/
void FindMovieShowTimes(Ticket*(*ticketBook), Weekday day, MovieList movie, int *times){
	/*
	1. Ƽ�ϸ���Ʈ�� �Է¹޴´�
	2. Ư�� ��¥�� �Է¹޴´�. / Ư�� ��ȭ�� �Է¹޴´�
	3. �ش� ��¥�� Ƽ�ϸ���Ʈ��ŭ �ݺ��Ѵ�
	4. Ư�� ��ȭ�� ã��, �ش� ��ȭ�� �ð��� �Է��Ѵ�
	5. �Է��� ��ȭ�� ����Ѵ�
	*/
	int i = 0;
	int j = 0;
	while (i < CountOfRoom*CountOfTime) {
		if ((ticketBook[day])[i].movie == movie) {
			times[j] = (ticketBook[day])[i].time;
			j++;
		}
		i++;
	}
}
void MakeSeatChart(int(*tempSeatArray), BTree *printTree) {
	int index = 0;
	
	ConvertArray(printTree->root, tempSeatArray, &index);

	//sorting the converted array
	int smallestIndex;
	int temp;
	int i, j;

	for (i = 0; i < (printTree->length); i++) {
		smallestIndex = i;
		for (j = i + 1; j < (printTree->length); j++) {
			if (tempSeatArray[j] < tempSeatArray[smallestIndex]) {
				smallestIndex = j;
			}
		}
		temp = tempSeatArray[i];
		tempSeatArray[i] = tempSeatArray[smallestIndex];
		tempSeatArray[smallestIndex] = temp;
	}
}