//Ticket.h
#ifndef _Ticket_H
#define _Ticket_H

#include <stdio.h>
#include <stdlib.h>
#include "binarySearchTree.h"
#define MAX_SEAT 150
#define MIN_SHOW_NUMBER 3
typedef enum {
	HarryPoter,
	Mission,
	LoardOfRings,
	Notebook,
	SocialNetwork,
	CountOfMovieList
} MovieList;

typedef enum {
	today,
	oneday,
	twoday,
	threeday,
	fourday,
	fiveday,
	sixday,
	CountOfWeekDay
} Weekday;

typedef enum {
	morning,
	afternoon,
	evening,
	CountOfTime
} Time;
typedef enum {
	firstRoom,
	secondRoom,
	thirdRoom,
	fourthRoom,
	fifthRoom,
	CountOfRoom
} Room;

typedef struct _ticket {
	BTree btree;
	Room room;
	MovieList movie;
	Time time;
} Ticket;

int main(int argc, char *argv[]);
void CreateTicketBook(Ticket*(*ticketBook));
BTree* Findticket(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day);
int Reservation(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day, Data resinfo);
int CancelReservation(Ticket*(*ticketBook), Time time, MovieList movieList, Weekday day, int seatId);
void DisplaySeat(Ticket*(*ticketBook));
void FindMovieShowTimes(Ticket*(*ticketBook), Weekday day, MovieList movie, int *times);
void MakeSeatChart(int(*tempSeatArray), BTree *printTree);

#endif//_Ticket_H
