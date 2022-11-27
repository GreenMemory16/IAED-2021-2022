/* Diogo Pires - 99475*/

/* This is the main file that takes care of all the operations
   and algorithms. It handles all the instructions through the
   Command_Handler that calls the main functions for each command */

#include "proj1.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main()
{
    int i;

    airport_num = 0;
    flight_num = 0;
    flight_output_num = 0;
    global_date.day = 1;
    global_date.month = 1;
    global_date.year = 2022;

    HEAD = NULL;
    list_num = 0;


    for (i=0; i < HASHTABLE_SIZE; i++) {
        hashtable[i] = NULL;
    }

    while (Command_Handler()) {
    }

    return 0;
}

int Command_Handler()
{
    char c = getchar();

    switch (c) {
        case 'a':
            Add_Airport_Handler();      /* Adds an airport */
            return 1;
        case 'l':
            Airport_List_Handler();     /* Lists an airport */
            return 1;
        case 'v':
            Add_Flight_Handler();       /* Adds a flight or shows flights */
            return 1;
        case 'p':
            Flight_List_Handler_p();      /* Shows departures for an airport */
            return 1;
        case 'c':
            Flight_List_Handler_c();     /* Shows arrivals for an airport */
            return 1;
        case 't':
            Advances_Time_Handler();    /* Sets a new date */
            return 1;
        case 'r':
            Reservation_Handler();
            return 1;
        case 'e':
            Eliminate_Handler();
            return 1;
        case 'q':                       /* Stops the program */
            Free_All_Memory();
            return 0;
        default:                        /* Ignores unknown commands */
            return 1;
    }
}

int Is_Upper(char c) {
    /*Checks if a caracter is an uppercase letter*/
    return c >= 'A' && c <= 'Z';
}

int Is_Digit(char c) {
    return c >= '0' && c <= '9';
}

int Same_Date(date d1, date d2) {
    /* Cheks if two dates are the same*/

    if (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year) {
        return 1;
    }

    return 0;
}

int Validade_31Days(int day, int month) {
    /* Checks if we are at th end of a month with 31 days */
    if (day > 31 && (month==1 || month==3 || month==5 ||
        month==7 || month==8 || month==12 || month == 10)) {
        return 0;
    }
    return 1;
}

int Validade_30Days(int day, int month) {
    /* Checks if we are at the end of a month with 30 days */
    if (day > 30 && (month==4 || month==6 || month==9 || month==11)) {
        return 0;
    }
    return 1;
}

int Valid_Month_Days(int day, int month) {
    /* Checks if we are at th end of a month */

    if (Validade_30Days(day, month) == 0) {
        return 0;
    }
    else if (Validade_31Days(day, month) == 0) {
        return 0;
    }
    else if (day > 28 && month == 2) {
        return 0;
    }

    return 1;
}

void Sums_Time(flight_output *fo, time t) {
    /* Recieves a flight_output and a time and sums the time given to 
       the flight_output time */ 

    int new_m = t.minutes + fo->hours.minutes;
    int new_h = t.hour + fo->hours.hour;
    int day = fo->date.day;
    int month = fo->date.month;
    int year = fo->date.year;
    
    if (new_m >= 60) { new_h += 1; new_m -= 60;
    }
    if (new_h >= 24) { new_h -= 24; day += 1;}

    if (Valid_Month_Days(day, month) == 0) { month += 1; day = 1;}

    if (month == 13) { year += 1; day = 1; month = 1;}

    fo->date.day = day;
    fo->date.month = month;
    fo->date.year = year;
    fo->hours.hour = new_h;
    fo->hours.minutes = new_m;
}

int Checks_ID(char id[]) {
    /* Cheks if an airport ID is valid or not */

    int i;

    for (i=0; i < IdentifierDIM-1; i++) {
        if (!Is_Upper(id[i])) {
            return 0;
        }
    }

    return 1;
}

int Checks_ID_Existance(char id[]) {
    /* Checks if an airport ID exists */

    int i;

    for (i=0; i < airport_num; i++) {
        if (strcmp(id,airports[i].identif) == 0) {
            return 1;
        }
    }
    return 0;
}

int Check_Valid_Date(date d) {
    /* Cheks if a given date if valid for insetion */

    if(d.year> 2023 || d.year < 2022) { return 0; }

    if(d.year<global_date.year) { return 0; }
    else if (d.year == global_date.year) {
        if (d.month < global_date.month) { return 0; }
        else if (d.month == global_date.month) {
            if (d.day < global_date.day) { return 0; }
        }
    }
    else if (d.year > global_date.year) {
        if (d.month > global_date.month) { return 0; }
        else if (d.month == global_date.month) {
            if (d.day > global_date.day) { return 0; }
        }
    }

    return 1;
}

void Add_Airport_Handler() {
    /* Handles the command 'a' */

    airport ap;
    int decider;
 
    scanf("%s %s %[^\n]", ap.identif, ap.country, ap.city);
    
    /* decides which action to tale */
    decider = Airport_Handler_Action_Decider(&ap);  
    
    if (decider == 1) {
        printf("%s", INVALID_AIRPORT_ID);
    }
    else if (decider == 2) {
        printf("%s", TOO_MANY_AIRPORTS);
    }
    else if (decider == 3) {
        printf("%s", DUPLICATE_AIRPORT);
    }
    else {
        Insert_Airport_Alphabeticaly(&ap);
        airport_num += 1;
        printf("airport %s\n", ap.identif);
    }
}

void Insert_Airport_Alphabeticaly(airport *ap) {
    /* Inserts and airport inside the array in alphabetical order */

    int i = 0;
    int j;
    airport save;
    airport helper;

    for (i=0; i < airport_num; i++) {
        if (strcmp(airports[i].identif, ap->identif) > 0) {
            break;
        }
    }
    
    if (i != airport_num) {
        save = airports[i];
    }

    airports[i] = *ap;

    for (j=i+1; j < airport_num+1; j++) {
        helper = airports[j];
        airports[j] = save;
        save = helper;
    }

}

int Airport_Handler_Action_Decider(airport *ap) {
    /* Checks if there are any errors in the command 'a' */

    if (Checks_ID(ap->identif) == 0) {
        return 1;
    }

    if (airport_num >= 40) {
        return 2;
    }
    else if (Checks_ID_Existance(ap->identif) == 1) {
        return 3;
    }
    else {
        return 0;
    }
}

void Airport_List_Handler() {
    /* Handles de command l*/

    char c = getchar();

    if (c == '\n') {
        Show_All_Airports();
    }
    else {
        Show_Some_Airports();
    }
}

void Show_All_Airports() {
    /* Shows every listed airport */

    int i, j;
    int counter = 0;

    c_names names[AirportDIM];

    for (i=0; i < airport_num; i++) {
        strcpy(names[i].nome, airports[i].city);
    }

    for (i=0; i < airport_num; i++) {
        counter = 0;
        for (j=0; j < flight_num; j++) {
            if (strcmp(airports[i].identif, flights[j].departure.identif)==0) {
                counter += 1;
            }
        }
        printf("%s %s %s %d\n", airports[i].identif,
                airports[i].city, airports[i].country, counter);
        counter = 0;
    }
}

int Finds_Airport(char code[]) {
    /* Finds the ID of a given airport */
    int i;

    for (i=0; i < airport_num; i++) {
        /* Looks for the given ID */
        if (strcmp(code, airports[i].identif) == 0) {
            return i;
        }
    }

    return 0;
}

int Counts_Airports(char code[]) {
    /* Counts the number of flights of a given airport */

    int i;
    int counter = 0;

    for (i=0; i < flight_num; i++) {
        /* Id there's a flight with an airport departure in the given ID, increases counter*/
        if (strcmp(code, flights[i].departure.identif) == 0) {
             counter += 1; 
        }
    }

    return counter;
}

int Show_Some_Airports() {
    /* Shows only the airport chosen by the user */

    int j;
    char c;
    int counter = 0;
    char code[IdentifierDIM];

    while ((c=getchar()) != EOF && c != '\n') {
        counter = 0;
        if (c != ' ') {
            code[0] = c;
            code[1] = getchar();    /* Saves the ID */ 
            code[2] = getchar();
            code[3] = '\0';
            counter = 0;    

            /* If there's no such airport ID */
            if (Checks_ID_Existance(code) == 0) { 
                printf("%s: %s\n",code, NO_SUCH_AIRPORT_ID);
                continue;
            }
            
            j = Finds_Airport(code);
            counter = Counts_Airports(code);

            printf("%s %s %s %d\n", airports[j].identif, 
                    airports[j].city, airports[j].country, counter);
        }
    }

    return 1;
}

void Add_Flight_Handler() {
    /* Decides if the program adds a flight or shows all the flights */

    char c = getchar();

    if (c == '\n') {
        Show_Flights();
    }
    else {
        Add_Flight();
    }
}

void Add_Flight() {
    /* Adds a flight to the flights list */

    int decider;
    flight f;
    
    scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", 
        f.code, f.departure.identif, f.arrival.identif, &f.date.day,
         &f.date.month, &f.date.year, &f.time.hour, &f.time.minutes,
          &f.duration.hour, &f.duration.minutes, &f.n_passangers);
    
    decider = Add_Flight_Decider(f);

    /* Prints the right error if any */
    switch (decider) {
        case 1:
            printf("%s", INVALID_FLIGHT_CODE);
            break;
        case 2:
            printf("%s", FLIGHT_ALREADY_EXISTS);
            break;
        case 3:
            printf("%s: %s\n", f.departure.identif, NO_SUCH_AIRPORT_ID);
            break;
        case 4:
            printf("%s: %s\n", f.arrival.identif, NO_SUCH_AIRPORT_ID);
            break;
        case 5:
            printf("%s", TOO_MANY_FLIGHTS);
            break;
        case 6:
            printf("%s", INVALID_DATE);
            break;
        case 7:
            printf("%s", INVALID_DURATION);
            break;
        case 8:
            printf("%s", INVALID_CAPACITY);
            break;
        case 0:
            flights[flight_num] = f;
            flight_num += 1;
    }
}

int Check_ID_Format(flight f, int len) {
    /* Checks if the flight code is invalid */

    int i;

    if (!Is_Upper(f.code[0]) || !Is_Upper(f.code[1])) { return 0; }

    for (i=2; i < len; i++) {
        if(!Is_Digit(f.code[i])) 
            return 0;
    }

    if (f.code[2] == '0') { return 0; }

    return 1;
}

int Check_Departure_Existence(flight f) {
    /* Checks if a given airport exists on Departures */

    int i = 0;
    int switcher = 0;

    for (i=0; i < airport_num; i++) {
        if (strcmp(f.departure.identif, airports[i].identif) == 0) {
            switcher = 1;
            break;
        }
    }

    return switcher;
}

int Check_Arrivals_Existence(flight f) {
    /* Checks if a given airport exists on Arrivals */

    int i = 0;
    int switcher = 0;

    for (i=0; i < airport_num; i++) {
        if (strcmp(f.arrival.identif, airports[i].identif) == 0) {
            switcher = 1;
            break;
        }
    }

    return switcher;
}

int Add_Flight_Decider(flight f) {
    /* Checks for errors in flight input and acts accordingly */

    int len_code = strlen(f.code);

    if (Check_ID_Format(f, len_code) == 0) { return 1; }
    
    if (Check_Duplicate_Flight(f) == 1) { return 2; }

    if (Check_Departure_Existence(f) == 0) { return 3; }

    if (Check_Arrivals_Existence(f) == 0) { return 4; }

    if (flight_num >= 30000) { return 5; } /* If there are 30000 flights */

    if (Check_Valid_Date(f.date) == 0) { return 6; } /* If there's an invalid date */

    /* If the duration is greater than 12:00 hours */
    if (f.duration.hour>12 || (f.duration.hour==12 && f.duration.minutes>0)) 
        return 7;

    /* If there are less than 10 passengers */
    if (f.n_passangers < 10) { return 8; }

    return 0;
}

int Check_Duplicate_Flight(flight f) {
    /* Cheks if a given flight already exists on flight lists, which means if it
       has the same code and the same date */

    int i;

    for (i=0; i < flight_num; i++) {
        if ((strcmp(f.code, flights[i].code) == 0) &&
             Same_Date(f.date, flights[i].date) == 1) {
            return 1;
        }
    }

    return 0;
}

void Show_Flights() {
    /* Prints every flight registered */

    int i;

    for (i=0; i < flight_num; i++) {
        printf("%s %s %s %02d-%02d-%d %02d:%02d\n", flights[i].code,
             flights[i].departure.identif, flights[i].arrival.identif,
              flights[i].date.day, flights[i].date.month, flights[i].date.year,
               flights[i].time.hour, flights[i].time.minutes);
    }
}

void Prints_Flight_Outputs() {
    /* Prints the flights from the global array flights_output */
    int i;

    for (i=0; i < flight_output_num; i++) {
            printf("%s %s %02d-%02d-%d %02d:%02d\n", flights_out[i].code,
                     flights_out[i].identif, flights_out[i].date.day, 
                    flights_out[i].date.month, flights_out[i].date.year,
                     flights_out[i].hours.hour, flights_out[i].hours.minutes);
        }
}

void Flight_List_Handler_p(){
    /* Handles de p command
       It populates the global flights_output list with
       flight_output objects that contain the necessary 
       information to print and sort them after */

    char id[IdentifierDIM];
    int i;
    flight_output fo;
    scanf("%s", id);

    if (Checks_ID_Existance(id) == 0) 
        printf("%s: %s\n", id, NO_SUCH_AIRPORT_ID);

    else {
        for (i=0; i < flight_num; i++) {
            if (strcmp(id, flights[i].departure.identif) == 0) {
                /* It transfer the data from Flight to FLights_Output */

                strcpy(fo.code, flights[i].code);
                strcpy(fo.identif, flights[i].arrival.identif);
                fo.date.day = flights[i].date.day;
                fo.date.month = flights[i].date.month;
                fo.date.year = flights[i].date.year;
                fo.hours.hour = flights[i].time.hour;
                fo.hours.minutes = flights[i].time.minutes;

                flights_out[flight_output_num] = fo;
                flight_output_num += 1;
            }
        }
        
        Sort_Flight_Output();

        Prints_Flight_Outputs();
    }
    flight_output_num = 0;
}

void Flight_List_Handler_c() {
    /* Handles de c command
       It populates the global flights_output list with
       flight_output objects that contain the necessary 
       information to print and sort them after */

    char id[IdentifierDIM];
    int i;
    flight_output fo;
    scanf("%s", id);

    if (Checks_ID_Existance(id) == 0) {
        printf("%s: %s\n", id, NO_SUCH_AIRPORT_ID);
    }
    else {
        for (i=0; i < flight_num; i++) {
            if (strcmp(id, flights[i].arrival.identif) == 0) {
                /* It transfer the data from Flight to FLights_Output */

                strcpy(fo.code, flights[i].code);
                strcpy(fo.identif, flights[i].departure.identif);
                fo.date.day = flights[i].date.day;
                fo.date.month = flights[i].date.month;
                fo.date.year = flights[i].date.year;
                fo.hours.hour = flights[i].time.hour;
                fo.hours.minutes = flights[i].time.minutes;

                Sums_Time(&fo, flights[i].duration);

                flights_out[flight_output_num] = fo;
                flight_output_num += 1;
            }
        }

        Sort_Flight_Output();

        Prints_Flight_Outputs();
    }
    flight_output_num = 0;
}

void Sort_Flight_Output() {
    /* Used Bubble Sort to sort flights output
     by their date and time */

    int i, j;
    int flag = 0;
    flight_output aux;

    for (i=0; i < flight_output_num; i++) {
        for (j=0; j < flight_output_num-i-1; j++) {
            flag = Checks_Date_Switch(j);

            if (flag == 1) {
                aux = flights_out[j];
                flights_out[j] = flights_out[j+1];
                flights_out[j+1] = aux;
            }
        }
    }
}

int Checks_Date_Switch(int j) {
    /* Checks if two flight_outputs should be switched in the array or not */

    int flag = 0;

    if (flights_out[j+1].date.year < flights_out[j].date.year) 
        flag = 1;
    else if (flights_out[j+1].date.year == flights_out[j].date.year) {
        if (flights_out[j+1].date.month < flights_out[j].date.month) 
            flag = 1;
        else if (flights_out[j+1].date.month == flights_out[j].date.month) {
            if (flights_out[j+1].date.day < flights_out[j].date.day) 
                flag = 1;
            else if (flights_out[j+1].date.day == flights_out[j].date.day) {
                if (flights_out[j+1].hours.hour < flights_out[j].hours.hour) 
                    flag = 1;
                else if(flights_out[j+1].hours.hour==flights_out[j].hours.hour)
                {
                    if (flights_out[j+1].hours.minutes
                         < flights_out[j].hours.minutes) 
                        flag = 1;
                }
            }
        }
    }

    return flag;
}

int Advances_Time() {
    /* Advances Time to the date set by the user. 
       If the date is invalid, it returns 0 */

    date d;

    scanf("%d-%d-%d", &d.day, &d.month, &d.year);

    if (Check_Valid_Date(d) == 0) {
        return 0;
    }

    global_date.day = d.day;
    global_date.month = d.month;
    global_date.year = d.year;

    return 1;
}

void Advances_Time_Handler() {
    /* Handles the t command*/

    if (Advances_Time() == 0) {
        printf("%s", INVALID_DATE);
    }
    else {
        printf("%02d-%02d-%02d\n",
         global_date.day, global_date.month, global_date.year);
    }

}

void Reservation_Handler() {
    /* Decides if the command adds a booking or shows all bookings */

    char fcode[FlightCodeDIM];
    date d;
    char c;

    scanf("%s %d-%d-%d", fcode, &d.day, &d.month, &d.year);
    c = getchar();

    if (c == '\n') {
        if (Check_FLight_Existence(fcode, d) == 0) {
            printf("%s: %s", fcode, FLIGHT_DOES_NOT_EXIST);
        }
        else if (Check_Valid_Date(d) == 0) {
            printf("%s", INVALID_DATE);
        }
        else {
            Print_Reservarions(fcode, d);
        }
    }
    else {
        Adds_Reservation(fcode, d);
    }
    
}

flight *Flight_Finder(char fcode[], date d) {
    /* Shearches for the specific flight we want and returns the pointer to it */

    int i;

    for (i=0; i < flight_num; i++) {
        if((strcmp(fcode,flights[i].code) == 0) && Same_Date(d,flights[i].date)) {
            return &flights[i];
        }
    }

    return NULL;
}

node *NEW(char *buffer, int pass, flight *f_pt) {
    /* Creates a new node and allocates three memory slots for the node,
       for the reservation and for the code. */

    node *n = (node*)malloc(sizeof(struct node));
    n->res = (reservation*)malloc(sizeof(reservation));
    n->res->code = (char*)malloc(sizeof(char) * (strlen(buffer)+1));

    /* If there's no more memory, the program ends, freeing all the memory */
    if (n == NULL || n->res == NULL || n->res->code == NULL) {
        printf("%s", NO_MEMORY);
        Free_All_Memory();
        exit(0);
    }

    strcpy(n->res->code, buffer);
    n->f_pt = f_pt;
    n->res->passengers = pass;
    n->next = NULL;

    return n;
}

void Free_All_Memory() {
    /* Frees all allocated memory */

    node *tmp = HEAD;

    while(HEAD != NULL) {
        tmp = HEAD->next;
        free(HEAD->res->code);
        free(HEAD->res);
        free(HEAD);
        HEAD = tmp;
    }
}

node *Serach_For_Place(node *n, node* head) {
    /* Searches the linked list for the right place to insert a node alphabeticaly */

    node *tmp = head;
    node *previous = NULL;

    while(tmp != NULL) {
        if (strcmp(n->res->code, tmp->res->code) < 0) {
            return previous;
        }
        previous = tmp;
        tmp = tmp->next;
    }

    return previous;
}

node *InsertBegin(char *buffer, int pass, flight *f_pt, node** head) {
    /* Inserts a node in the linked list */

    node *n = NEW(buffer, pass, f_pt); 
    node *previous_n = Serach_For_Place(n, *head);

    /* If there are no elements in the list */
    /*
    if (list_num == 0) {             
        n->next = NULL;
        head = n;
        list_num += 1;
    }
    */
    /* If it's inserted at the beggining of the list */
    if (previous_n == NULL) {    
        n->next = *head;
        *head = n;
        list_num += 1;
    }
    /* If it's inserted in the middle or at the end of the list */
    else {                        
        n->next = previous_n->next;
        previous_n->next = n;
        list_num += 1;
    }

    return n;
}

void Print_Reservarions(char code[], date d) {
    /* Prints every reservation with the given flight code and date */

    node *tmp = HEAD;

    if (Check_Valid_Date(d) == 0) {
        printf("%s", INVALID_DATE);
        return;
    }

    while (tmp != NULL) {
        if (strcmp(code, tmp->f_pt->code) == 0 && Same_Date(d, tmp->f_pt->date)) {
            printf("%s %d\n", tmp->res->code, tmp->res->passengers);
        }
        tmp = tmp->next;
    }
}

void Adds_Reservation(char fcode[], date d) {
    /* Main function for adding a reservation to the list */

    char buffer[MAX_CAR];
    int pass, error, id_hashed;
    flight *f_pt;

    scanf("%s %d",buffer ,&pass);

    /* Checks if there are any errors */
    error = Reservation_Error_Handler(fcode, d, pass, buffer);

    if (error == 1) {
        f_pt = Flight_Finder(fcode, d);
        InsertBegin(buffer, pass, f_pt, &HEAD);
        id_hashed = Hash(buffer);
        InsertBegin(buffer, pass, f_pt, &hashtable[id_hashed]);
    }
}

int Reservation_Error_Handler(char code[], date d, int pass, char buffer[]) {
    /* Function that searches for all possible errors in the user input */

    /* Checks if reservation code is valid */
    if (Verifies_Reservation_Code(buffer) == 0) {
        printf("%s", INVALID_RESERVATION_CODE);
        return 0;
    }
    /* Checks if the inserted flight exists */
    else if (Check_FLight_Existence(code, d) == 0) {
        printf("%s: %s", code, FLIGHT_DOES_NOT_EXIST);
        return 0;
    }
    /* Checks if the reservation code inserted already exists */
    else if (Check_Reservation_Existence(buffer) == 0) {
        printf("%s: %s", buffer, FLIGHT_RESERVATION_ALREADY_USED);
        return 0;
    }
    /* Checks if the passenger number is valid */
    else if (Check_Valid_Passenger_Number(code, d, pass) == 0) {
        printf("%s", TOO_MANY_RESERVATIONS);
        return 0;
    }
    /* Checks if the date is valid */
    else if (Check_Valid_Date(d) == 0) {
        printf("%s", INVALID_DATE);
        return 0;
    }
    /* Checks if the passenger number is a positive digit different from zero */
    else if (Invalid_Passenger_Number(pass) == 0) {
        printf("%s", INVALID_PASSENGER_NUMBER);
        return 0;
    }

    return 1; 
}

int Verifies_Reservation_Code(char rcode[]) {
    /* Verifies if the reservation code only has uppercase letter and digits */

    int i, counter = 0;
    int c_len = strlen(rcode);

    for (i=0; i < c_len; i++) {
        /* Checks if it's a digit or an upper case letter */
        if (Is_Upper(rcode[i]) == 1 || Is_Digit(rcode[i]) == 1) {
            counter += 1;
        }
        else {
            return 0;
        }
    }
    
    if (counter < 10) {
        return 0;
    }

    return 1;
}

int Check_FLight_Existence(char fcode[], date d) {
    /* Checks if a given flight exists */

    int i;

    for (i=0; i < flight_num; i++) {
        if ((strcmp(fcode, flights[i].code) == 0) && Same_Date(d, flights[i].date)) {
            return 1;
        }
    }

    return 0;
}

int Check_Valid_Passenger_Number(char fcode[], date d, int pass) {
    /* Checks if the number of passengers of a given reservation is valid for
       that fligth. So, it checks all reservations for that flight to see if
       it's valid */

    int i, n_pass = 0, counter = 0;
    node *tmp = HEAD;

    /* finds the indix of the flight we want to analize */
    for (i=0; i < flight_num; i++) {
        if (strcmp(flights[i].code, fcode) == 0 && Same_Date(d, flights[i].date)) {
            n_pass = flights[i].n_passangers;
            break;
        }
    }

    /* runs through the list */
    while (tmp != NULL) {
        /* If there's a reservation for that flight, we add the number of people to the counter */
        if (strcmp(tmp->f_pt->code, flights[i].code) == 0 && Same_Date(tmp->f_pt->date, d)) {
            counter = counter + tmp->res->passengers;
        }
        tmp = tmp->next;
    }

    counter += pass;

    /* If the counter is bigger than the flight limit, it returns 0*/
    if (counter > n_pass) {
        return 0;
    }

    return 1;
}

int Check_Reservation_Existence(char buffer[]) {
    /* Checks if a given reservation code exists */

    /*node *tmp = HEAD;*/

    int id_hashed = Hash(buffer);
    node *tmp2 = hashtable[id_hashed];

    while (tmp2 != NULL) {
        if (strcmp(buffer, tmp2->res->code) == 0) {
            return 0;
        }
        tmp2 = tmp2->next;
    }
    
    /*
    while (tmp != NULL) {
        if (strcmp(buffer, tmp->res->code) == 0) {
            return 0;
        }
        tmp = tmp->next;
    }
    */

    return 1;
}

int Invalid_Passenger_Number(int num) {
    /* Checks if the passenger number is a positive integer */
    if (num <= 0) {
        return 0;
    }

    return 1;
}

void Eliminate_Handler() {
    /* Handles all the elimination commands */

    char fcode[MAX_CAR];
 
    scanf("%s", fcode);

    if (strlen(fcode) <= 7) {
        Eliminates_Reservations(fcode, &HEAD);
        Eliminates_Flights(fcode);
    }
    else {
        Eliminates_Single_Reservation(fcode, &HEAD);
    }
}

int Check_Flight_Existence_NoDate(char fcode[]) {
    /* Checks if a flight exists without checking the date */

    int i;

    for (i=0; i < flight_num; i++) {
        if (strcmp(flights[i].code, fcode) == 0) {
            return 1;
        }
    }

    return 0;
}

void Eliminates_Flights(char fcode[]) {
    /* Eliminates all flights with the given code from the flights array */

    int i, m = 0;   
    
    for (i=0; i < flight_num; i++) {
        if (strcmp(fcode, flights[i].code) == 0) {
            m += 1;
        }
        else {
            flights[i-m] = flights[i];
        }
    }

    flight_num -= m;

    /* If the given flight didn't exist, prints an error*/
    if (m == 0) {
        printf("%s", NOT_FOUND);
    }
}

void Eliminates_Reservations(char fcode[], node **head) {
    /* Eliminates all reservations with a given flight code */

    node *tmp = *head;
    node *previous = *head;

    while (tmp != NULL) {
        if (strcmp(tmp->f_pt->code, fcode) == 0) {

            /* if we are trying to eliminate from the list's HEAD */
            if (tmp == *head) {
                *head = (*head)->next;
                free(tmp->res->code);
                free(tmp->res);
                free(tmp);
                tmp = *head;
                previous = *head;
                continue;
            }
            /* if we are trying to eliminate from the middle of end */
            else {
                previous->next = tmp->next;
                free(tmp->res->code);
                free(tmp->res);
                free(tmp);
                tmp = previous->next;
                continue;
            }
        }
        previous = tmp;
        tmp = tmp->next;
    }
}

void Eliminates_Single_Reservation(char fcode[], node **head) {
    node *tmp = *head;
    node *previous = *head;
    int flag = 0;

    while (tmp != NULL) {
        if (strcmp(fcode, tmp->res->code) == 0) {
            flag = 1;

            /* If we are trying to eliminate from the HEAD */
            if (tmp == *head) {
                *head = (*head)->next;
                free(tmp->res->code);
                free(tmp->res);
                free(tmp);
                return;
            }
            /* If we are trying to eliminate from the middle or the end */
            else {
                previous->next = tmp->next;
                free(tmp->res->code);
                free(tmp->res);
                free(tmp);
                return;
            }
        }
        previous = tmp;
        tmp = tmp->next;
    }

    if (flag == 0) {
        printf("%s", NOT_FOUND);
    }
}

int Hash(char *v) {

    int h, a = 31415, b = 27183;
    
    for (h=0; *v != '\0'; v++, a = a*b % (HASHTABLE_SIZE-1)) {
        h = (a*h + *v) % HASHTABLE_SIZE;
    }

    return h;
}

