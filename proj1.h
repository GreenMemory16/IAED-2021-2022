/* Diogo Pires - 99475*/

/* This is the Header file that contains all the constants
   and all the function prototypes. It also has three global
   lists and a variable to handle their lenghts */

#ifndef MAIN_H
#define MAIN_H

/* Constants */

#define StateIN 1
#define StateOUT 0

#define AirportDIM 40
#define FlightDIM 30000
#define nPassengersMIN 10

#define IdentifierDIM 4
#define CountryDIM 31
#define CityDIM 51
#define FlightCodeDIM 7

#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define TOO_MANY_AIRPORTS "too many airports\n"
#define DUPLICATE_AIRPORT "duplicate airport\n"

#define NO_SUCH_AIRPORT_ID "no such airport ID"

#define INVALID_FLIGHT_CODE "invalid flight code\n"
#define FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define TOO_MANY_FLIGHTS "too many flights\n"
#define INVALID_DATE "invalid date\n"
#define INVALID_DURATION "invalid duration\n"
#define INVALID_CAPACITY "invalid capacity\n"

#define MAX_CAR 65535
#define NO_MEMORY "No memory\n"
#define INVALID_RESERVATION_CODE "invalid reservation code\n"
#define FLIGHT_DOES_NOT_EXIST "flight does not exist\n"
#define INVALID_PASSENGER_NUMBER "invalid passenger number\n"
#define FLIGHT_RESERVATION_ALREADY_USED "flight reservation already used\n"
#define TOO_MANY_RESERVATIONS "too many reservations\n"
#define NOT_FOUND "not found\n"

#define HASHTABLE_SIZE 30071

/* STRUCTURES */   

typedef struct {
    char identif[IdentifierDIM];
    char country[CountryDIM];
    char city[CityDIM];
} airport;

typedef struct {
    int hour;
    int minutes;
} time;

typedef struct {
    int year;
    int month;
    int day;
} date;

typedef struct {
    char code[FlightCodeDIM];
    date date;
    time time;
    airport departure;
    airport arrival;
    time duration;
    int n_passangers;
} flight;

typedef struct {
    char nome[CountryDIM];
} c_names;

typedef struct {
    char code[FlightCodeDIM];
    char identif[IdentifierDIM];
    date date;
    time hours;
} flight_output;

typedef struct {
    int passengers;
    char *code;
} reservation;

typedef struct node {
    struct node *next;
    reservation *res;
    flight *f_pt;
} node;


int Same_Date(date d1, date d2);

int Validade_31Days(int day, int month);

int Validade_30Days(int day, int month);

int Check_Valid_Date(date d);

int Valid_Month_Days(int day, int month);

void Sums_Time(flight_output *fo, time t);

int Checks_ID(char id[]);

int Checks_ID_Existance_Arrivals(char id[]);

int Checks_ID_Existance_Departure(char id[]);

int Command_Handler();

void Add_Airport_Handler();

void Insert_Airport_Alphabeticaly(airport *ap);

int Airport_Handler_Action_Decider(airport *ap);

int Check_Duplicate_Airport(char id1[]);

void Airport_List_Handler();

void Show_All_Airports();

int Finds_Airport(char code[]);

int Counts_Airports(char code[]);

int Show_Some_Airports();

void Add_Flight_Handler();

void Add_Flight();

int Check_ID_Format(flight f, int len);

int Check_Departure_Existence(flight f);

int Check_Arrivals_Existence(flight f);

int Add_Flight_Decider(flight f);

int Check_Duplicate_Flight(flight f);

void Show_Flights();

void Flight_List_Handler_p();

void Flight_List_Handler_c();

void Sort_Flight_Output();

int Checks_Date_Switch(int j);

void Advances_Time_Handler();

int Advances_Time();

void Reservation_Handler();

flight *Flight_Finder(char fcode[], date d);

node *NEW(char *buffer, int pass, flight *f_pt);

void Free_All_Memory();

node *Serach_For_Place(node *n, node *head);

node *InsertBegin(char *buffer, int pass, flight *f_pt, node** head);

void Print_Reservarions(char code[], date d);

void Adds_Reservation(char fcode[], date d);

int Reservation_Error_Handler(char code[], date d, int pass, char buffer[]);

int Check_FLight_Existence(char code[], date d);

int Check_Valid_Passenger_Number(char fcode[], date d, int pass);

int Verifies_Reservation_Code(char code[]);

int Check_Reservation_Existence(char buffer[]);

int Invalid_Passenger_Number(int num);

void Eliminate_Handler();

int Check_Flight_Existence_NoDate(char fcode[]);

void Eliminates_Flights(char fcode[]);

void Eliminates_Reservations(char fcode[], node **head);

void Eliminates_Single_Reservation(char fcode[], node **head);

int Hash(char *v);


node *HEAD;

int list_num;

/* Global Array that stores all airports */
airport airports[AirportDIM] = {0};

/* Global variable that has the airports array size */
int airport_num;

/* Global Array that sores all flights */
flight flights[FlightDIM];

/* Global variable that has the flights array size */
int flight_num;

/* Global array that has the flights output array size.
   These are used for the p and c commands */
flight_output flights_out[FlightDIM];

/* Global variable that has de flights output array size*/
int flight_output_num;

/* Global date that stores the current time */
date global_date;


node* hashtable[HASHTABLE_SIZE];








#endif