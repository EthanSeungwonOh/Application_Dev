//Form.h
#include <stdio.h>

#ifndef _Form_H
#define _Form_H
#include "ticket.h"
typedef struct _ticket Ticket; //front declaration
void DisplayMenu();
void FormForReservation(Ticket*(*ticketBook));
void FormForCancelReservation(Ticket*(*ticketBook));
void FormForDisplaySeat(Ticket*(*ticketBook), int intTime, int intMovie, int intDay);


#endif//_Form_H
