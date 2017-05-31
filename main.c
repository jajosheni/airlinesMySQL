#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "mysql.h"

/// AUTHOR : SHENI HAMITAJ
 

    char *server = "localhost";
    char *user = "root";
    char *password = "toor"; /* set me first */
    char *database = "flights";

int ticket_duplicate(char date[20],char plane_number[20],char TCKN[15]);
int pas_duplicate(char date[20],char plane_number[20],char TCKN[15], char origin[50],char destination[50]);
int flight_duplicate(char plane_number[20],char origin[50],char date[10],char time[10],char destination[50]);
int users_duplicate(char TCKN[15]);
int planes_duplicate(char plane_number[20]);
int flight_checking(char plane_number[20],char date[10]);
int get_capacity(char plane_number[20]);
int capacity_reached(char plane_number[20],char date[10],char origin[50],char destination[50]);
int count_tickets(char plane_number[20],char date[10],char origin[50],char destination[50]);

void show_most_attended(int top_five[6][3]);
void leave_no_orphans(char plane_number[20],char origin[50], char date[10], char destination[50]);
void show_details(char plane_number[20],char TC[70][15]);
void add_user(char TCKN[15]);
void add_flight();
void add_plane(char plane_number1[20]);
void add_ticket(char date[10],char plane_number[20],char TCKN[15],char origin[50], char destination[50]);
void buy_ticket();
void your_flights();
void cancel_flight();
void cancel_ticket();
void flight_information();
void most_attended();
void hidden();
void menu();
void selection(int selected);

int sel=404;


int main()
{

  //!START CONNECTION TO MySQL AND CREATE A DATABASE IF IT DOESN'T EXISTS !//
  MYSQL *conn = mysql_init(NULL);

  if (conn == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
  }

  if (mysql_real_connect(conn, "localhost", "root", "toor",
          NULL, 0, NULL, 0) == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
  }

  if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTS flights"))
  {
      fprintf(stderr, "%s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
  }

  mysql_close(conn);

  //! STOP CONNECTION ------------------------------------------------------------ !//

  /// CHECK IF THE DATABASE IS CREATED AND IF YES CONNECT TO IT ///

     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }


        //! ADD REQUIRED TABLES TO THE DATABASE
        ///tables : users, planes, tours, passengers

      mysql_query(conn, "SET SQL_MODE=NO_AUTO_VALUE_ON_ZERO");
      mysql_query(conn, "CREATE TABLE IF NOT EXISTS users (TCKN varchar(15) NOT NULL DEFAULT '',user_name varchar(100) NOT NULL DEFAULT '',user_email varchar(100) NOT NULL DEFAULT '',user_address varchar(200) NOT NULL DEFAULT '',primary key(TCKN)) ENGINE=MyISAM DEFAULT CHARSET=latin1;");
      mysql_query(conn, "CREATE TABLE IF NOT EXISTS planes (plane_number varchar(20) NOT NULL DEFAULT '',model varchar(20) NOT NULL DEFAULT '',capacity INT(20) NOT NULL, primary key(plane_number)) ENGINE=MyISAM DEFAULT CHARSET=latin1;");
      mysql_query(conn, "CREATE TABLE IF NOT EXISTS tours (plane_number varchar(20) NOT NULL DEFAULT '',origin varchar(50) NOT NULL DEFAULT '',destination varchar(50) NOT NULL DEFAULT '',time varchar(20) NOT NULL DEFAULT '',date varchar(20) NOT NULL DEFAULT '',primary key(plane_number,date,destination,origin,time)) ENGINE=MyISAM DEFAULT CHARSET=latin1;");
      mysql_query(conn, "CREATE TABLE IF NOT EXISTS passengers (date varchar(20) NOT NULL DEFAULT '',plane_number varchar(20) NOT NULL DEFAULT '',TCKN varchar(15) NOT NULL DEFAULT '',origin varchar(50) NOT NULL DEFAULT '',destination varchar(50) NOT NULL DEFAULT '',primary key(date,plane_number,TCKN,origin,destination) ) ENGINE=MyISAM DEFAULT CHARSET=latin1;");

        /// Tables added ============================================= //

    if (mysql_query(conn, "show tables")) //!test
        {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
        }

    mysql_close(conn);

        //!START THE MENU
    menu();
}

void leave_no_orphans(char plane_number[20],char origin[50], char date[10], char destination[50])
{
        char q[1024];
        sprintf(q,"DELETE FROM passengers WHERE plane_number='%s' AND origin='%s' AND date='%s' AND destination='%s'",plane_number,origin,date,destination);

        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);


         //! Connect to database
        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            mysql_close(conn);
            getch();
            menu();
        }
        mysql_close(conn);
}

 void add_flight()
{
    printf("\n\t\t\t -- ADD FLIGHT --\n");
    char plane_number1[20];
    char date1[10];
    char time[10];
    char origin1[50];
    char destination1[50];


	printf("\nEnter Plane NUMBER: ");
    scanf(" %20[^\n]s",  plane_number1);

    int flag=0;
    while(1) //! check date for validity
    {
        int i,j=0;
        printf("\nEnter Date in this format ddmm20yy(31122018): ");
        scanf(" %10[^\n]s",  date1);
        flag=0;

        for(i=0;i<10;i++)
            {
                if(date1[i]=='\0')
                    break;
               if(date1[i]<48 || date1[i]>57)
                    flag=1;

                j++;
            }
        if(j!=8)
            flag=1;

        if(date1[0]>51 || (date1[0]==51 && date1[1]>49) || date1[2]>49 || (date1[2]==49 && date1[3]>50) || date1[4]!=50 || date1[5]!=48)
            flag=1;

        if(flag==0)
            break;
    }


    while(1) //! check time for validity
    {
        int i;
        printf("\nEnter Time in this format HHMM(2359): ");
        scanf(" %10[^\n]s",  time);
        flag=0;

        for(i=0;i<10;i++) //check if there is an illegal character
            {
                if(time[i]=='\0')
                    break;
                if(time[i]<48 || time[i]>57)
                     flag=1;
            }

        if((time[0]==50 && time[1]>51) || time[0]>50 || time[2]>53)
            flag=1;

        if(flag==0)
            break;
    }

    printf("\nEnter origin: ");
    scanf(" %50[^\n]s",  origin1);

    printf("\nEnter Destination: ");
    scanf(" %50[^\n]s",  destination1);

    /// check for flight duplicate

    int duplicate=flight_duplicate(plane_number1,origin1,date1,time,destination1);

    if(duplicate)//! 0(false) if there is a duplicate
	{
	    //! If there is no duplicate just continue
	}
	else
    {
        //! If there is a duplicate then return to menu;

		printf("\n\nFlight Tour EXISTS\n");
		printf("\npress any key to continue");
        getch();
        menu();
    }

    /// check for plane registered or not
	  duplicate=planes_duplicate(plane_number1);

	if(duplicate)//! 0(false) if there is a duplicate
	{
		add_plane(plane_number1);

		//!after plane added
		MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
            }


        char q[1024];
        sprintf(q,"INSERT INTO tours(plane_number,date,origin,destination,time) VALUES('%s','%s','%s','%s','%s')",plane_number1,date1,origin1,destination1,time);

	        if(mysql_query(conn, q))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				mysql_close(conn);
				printf("\npress any key to continue");
				getch();
				menu();
			}

		mysql_close(conn);
		printf("\n\nFlight Tour ADDED\n");
		printf("\npress any key to continue");
        getch();
        menu();
	}
	else
	{
		//!if plane exists add the flight
		MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
            }


        char q[1024];
        sprintf(q,"INSERT INTO tours(plane_number,date,origin,destination) VALUES('%s','%s','%s','%s')",plane_number1,date1,origin1,destination1);

	        if(mysql_query(conn, q))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				mysql_close(conn);
				printf("\npress any key to continue");
				getch();
				menu();
			}

		mysql_close(conn);
		printf("\n\nFlight Tour ADDED\n");
		printf("\npress any key to continue");
        getch();
        menu();
	}
}

void buy_ticket()
{
     printf("\n\t\t\t -- BUY TICKET --\n");
    int tickets=0,counter=0;
    char date[10];
    char origin[50];
    char destination[50];
	char plane_index[10][20];
	char plane_number[20];
    char TCKN[15];

	printf("\nEnter your origin: ");
    scanf(" %50[^\n]s",  origin);

    printf("\nEnter your Destination: ");
    scanf(" %50[^\n]s",  destination);
	int flag=0;
    while(1) //! check date for validity
    {
        int i,j=0;
        printf("\nEnter your Date in this format ddmm20yy(31122018): ");
        scanf(" %10[^\n]s",  date);
        flag=0;

        for(i=0;i<10;i++)
            {
                if(date[i]=='\0')
                    break;
               if(date[i]<48 || date[i]>57)
                    flag=1;

                j++;
            }
        if(j!=8)
            flag=1;

        if(date[0]>51 || (date[0]==51 && date[1]>49) || date[2]>49 || (date[2]==49 && date[3]>50) || date[4]!=50 || date[5]!=48)
            flag=1;

        if(flag==0)
            break;
    }

	printf("\nHow many tickets do you want to buy?");
    scanf(" %d", &tickets);


	//! Get Connection & SHOW THE FLIGHTS
        MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
            }

        char q[1024];
        sprintf(q,"SELECT * FROM tours WHERE tours.date='%s' AND tours.origin='%s' AND tours.destination='%s'",date,origin,destination);

        if (mysql_query(conn, q))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
            }

        res = mysql_use_result(conn);
	printf("\n-----------------------------------------------------------------\n");
        //! output flights
        printf("\nAvailable Direct Flights:\nIndex\t Plane Number\t Time\n");
        int i=1;

        while ((coloumn = mysql_fetch_row(res)) != NULL)
		{
			printf("%d.\t %s\t\t%s\n",i, coloumn[0],coloumn[3]);
			sprintf(plane_index[i],"%s",coloumn[0]);
			i++;
		}
		mysql_free_result(res);

        if(i==1)
		{
			printf("\n No flights available(press any key to continue)\n");
			getch();
			mysql_close(conn);
			menu();
		}

		mysql_close(conn);

	//! flights shown & Connection Terminated

	printf("\n-----------------------------------------------------------------\n");

	//! get flight index;
		printf("\n\n Enter the flight index(404 to return to menu): ");
		int flight = 0;
        scanf(" %d", &flight);

		if(flight==404)
		{
			menu();
		}
    //! get Plane number
        int j=1;
        while (j<10)
        {
            if(flight==j)
            {
                sprintf(plane_number,"%s",plane_index[j]);
				break;
            }
			j++;
        }



for(counter=0; counter<tickets; counter++)
{
    if(capacity_reached(plane_number,date,origin,destination))
        {
            printf("\nPlane capacity is reached! No free seats left.\npress any key...");
            getch();
            menu();
        }
    printf("\nEnter your %d. TCKN: ", counter+1);
        scanf(" %15[^\n]s",  TCKN);

		int duplicate = users_duplicate(TCKN);
        if(duplicate)
            add_user(TCKN);


	//! ADDING A NEW TICKET TO passengers TABLE
        ///check for duplicate in tickets  (passengers table)
		duplicate = pas_duplicate(date,plane_number,TCKN,origin,destination);
		if(duplicate) //! 0 (false) when there is a duplicate
			add_ticket(date,plane_number,TCKN,origin,destination);

		else
			{
				printf("\n DUPLICATE ENTRY FOUND (press any key to continue)\n");
				getch();
				menu();
			}
}
        printf("\npress any key...\n");
        getch();
        menu();
}

void your_flights()
{
    printf("\n\t\t\t -- Your Flights --\n\n");

    char date1[10];
    char origin1[50];
    char destination1[50];
    char TCKN[15];
    char plane_number[20];

    printf("Enter your TCKN to see your flights: ");
    scanf(" %15[^\n]s",  TCKN);

    MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
    sprintf(q,"SELECT * FROM passengers WHERE TCKN='%s'", TCKN);

    if (mysql_query(conn, q))
        {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
        }

    res = mysql_use_result(conn);

     //! output table name
    printf("\nYour Flights (date is on ddmmyyyy format):\n\n\n");
    printf("\tPlane Number\tOrigin\t\tDestination\t\tdate\n");
    while ((coloumn = mysql_fetch_row(res)) != NULL)
        {
            printf("%16.16s%16.16s%16.16s%24.24s \n", coloumn[1],coloumn[3],coloumn[4],coloumn[0]);
        }
    printf("\npress any key...\n");
    getch();

    /** close connection */
    mysql_free_result(res);
    mysql_close(conn);
    menu();
}

void cancel_flight()
{
    printf("\n\t\t\t -- Cancel a FLIGHT --\n");
    char plane_number[20];
    char origin[50];
    char destination[50];
    char time[10];
    char date[10];

    printf("\nEnter Plane NUMBER: ");
    scanf(" %20[^\n]s",  plane_number);

    printf("\Confirm origin: ");
    scanf(" %50[^\n]s",  origin);

    printf("\Confirm destination: ");
    scanf(" %50[^\n]s",  destination);

    int flag=0;
    while(1) //! check date for validity
    {
        int i,j=0;
        printf("\nEnter your Date in this format ddmm20yy(31122018): ");
        scanf(" %10[^\n]s",  date);
        flag=0;

        for(i=0;i<10;i++)
            {
                if(date[i]=='\0')
                    break;
               if(date[i]<48 || date[i]>57)
                    flag=1;

                j++;
            }
        if(j!=8)
            flag=1;

        if(date[0]>51 || (date[0]==51 && date[1]>49) || date[2]>49 || (date[2]==49 && date[3]>50) || date[4]!=50 || date[5]!=48)
            flag=1;

        if(flag==0)
            break;
    }

    while(1) //! check time for validity
    {
        int i;
        printf("\nEnter Time in this format HHMM(2359): ");
        scanf(" %10[^\n]s",  time);
        flag=0;

        for(i=0;i<10;i++) //check if there is an illegal character
            {
                if(time[i]=='\0')
                    break;
               if(time[i]<48 || time[i]>57)
                    flag=1;

            }


        if((time[0]==50 && time[1]>51) || time[0]>50 || time[2]>53)
            flag=1;

        if(flag==0)
            break;
    }

	int duplicate=flight_duplicate(plane_number,origin, date,time,destination); //! check if there is a flight with these details

	if(duplicate==0)
    {
        printf("\nFlight found and selected:\n");
        printf("\n date is on ddmmyyyy format and time on HH:MM\n\n\n");

        printf("\tPlane Number\tOrigin\t\tDestination\t\tdate\t\ttime\n");

            printf("%16.16s%16.16s%16.16s%24.24s%16.16s \n", plane_number, origin,destination,date,time);

        char q[1024];
        sprintf(q,"DELETE FROM tours WHERE plane_number='%s' AND origin='%s' AND date='%s' AND destination='%s' AND time='%s' ",plane_number, origin,date,destination,time);

        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);


         //! Connect to database
        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));

            mysql_close(conn);
            printf("\npress any key to return to menu\n");
            getch();
            menu();
        }
        mysql_close(conn);
        printf("\nFlight deleted(press any key to return to menu)\n");
        /// DELETE ALL THE TICKETS OF THIS FLIGHT!!!!!
        leave_no_orphans(plane_number,origin,date,destination);
        getch();
        menu();

    }
    else if(duplicate==1)
    {
        printf("\nNo such flight Found ...");

        printf("\npress any key to return to menu");
        getch();
        menu();
    }
}

void cancel_ticket()
{
     printf("\n\t\t\t -- Cancel a TICKET --\n");
     char TCKN[15];
     char date[10];
     char plane_number[20];

    printf("\nEnter TCKN: ");
    scanf(" %15[^\n]s",  TCKN);

    printf("\nEnter Plane NUMBER: ");
    scanf(" %20[^\n]s",  plane_number);
    int flag;
    while(1) //! check date for validity
    {
        int i,j=0;
        printf("\nEnter your Date in this format ddmm20yy(31122018): ");
        scanf(" %10[^\n]s",  date);
        flag=0;

        for(i=0;i<10;i++)
            {
                if(date[i]=='\0')
                    break;
               if(date[i]<48 || date[i]>57)
                    flag=1;

                j++;
            }
        if(j!=8)
            flag=1;

        if(date[0]>51 || (date[0]==51 && date[1]>49) || date[2]>49 || (date[2]==49 && date[3]>50) || date[4]!=50 || date[5]!=48)
            flag=1;

        if(flag==0)
            break;
    }

    int duplicate=ticket_duplicate(date,plane_number,TCKN);
    if(duplicate==0)
    {
        printf("\nTicket found and selected:\n TCKN: %s\t Date: %s\t Plane Number: %s", TCKN,date,plane_number);

        char q[1024];
        sprintf(q,"DELETE FROM passengers WHERE TCKN='%s' AND date='%s' AND plane_number='%s'",TCKN, date,plane_number );

        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);


         //! Connect to database
        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));

            mysql_close(conn);
            printf("\npress any key to return to menu\n");
            getch();
            menu();
        }
        mysql_close(conn);
        printf("\nTicket deleted(press any key to return to menu)\n");
        getch();
        menu();

    }
    else if(duplicate==1)
    {
        printf("\nNo such ticket Found ...");

        printf("\npress any key to return to menu");
        getch();
        menu();
    }

}

void flight_information()
{
    printf("\n\t\t\t -- FLIGHT's Information --\n\n");

    char date[10];
    char plane_number[20];
	char TC[70][15];

   int flag=0;
    while(1) //! check date for validity
    {
        int i,j=0;
        printf("\nEnter Date in this format ddmm20yy(31122018): ");
        scanf(" %10[^\n]s",  date);
        flag=0;

        for(i=0;i<10;i++)
            {
                if(date[i]=='\0')
                    break;
               if(date[i]<48 || date[i]>57)
                    flag=1;

                j++;
            }
        if(j!=8)
            flag=1;

        if(date[0]>51 || (date[0]==51 && date[1]>49) || date[2]>49 || (date[2]==49 && date[3]>50) || date[4]!=50 || date[5]!=48)
            flag=1;

        if(flag==0)
            break;
    }


	printf("Enter plane number: ");
    scanf(" %20[^\n]s",  plane_number);


//! check if the flight exists
	int duplicate = flight_checking( plane_number, date);

	if(!duplicate)//0 (false) when there is a duplicate
	{
		MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW coloumn;
		conn = mysql_init(NULL);


		 //! Connect to database
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		char q[1024];
		sprintf(q,"SELECT * FROM passengers WHERE plane_number='%s' AND date='%s'", plane_number, date);

		if (mysql_query(conn, q))
			{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
			}

		res = mysql_use_result(conn);
        int i=1;
		 //! get tckn's
		while ((coloumn = mysql_fetch_row(res)) != NULL)
		{
			sprintf(TC[i],"%s",coloumn[2]);
			i++;
		}



		/** close connection */
		mysql_free_result(res);
		mysql_close(conn);
		show_details(plane_number,TC);
		menu();
	}
	else
	{
	    printf("\nNo such flight\npress any key...");
	    getch();
	    menu();
	}

}

void most_attended()
{
	int tour_index[100][3];/// tickets, capacity , index
	int j;

	for(j=0;j<100;j++)
	{
	    tour_index[j][0]=0;
	    tour_index[j][1]=0;
	    tour_index[j][2]=0;
	}


	MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
            }


        if (mysql_query(conn, "SELECT * FROM tours"))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
            }

        res = mysql_use_result(conn);

        //! count

        int i=1;

        while ((coloumn = mysql_fetch_row(res)) != NULL)
		{
			tour_index[i][0]=count_tickets(coloumn[0],coloumn[4],coloumn[1],coloumn[2]);
			tour_index[i][1]=get_capacity(coloumn[0]);
			tour_index[i][2]=i;
			i++;
		}
		mysql_free_result(res);

        if(i==1)
		{
			printf("\n No flights available(press any key to continue)\n");
			getch();
			mysql_close(conn);
			menu();
		}

    ///BUBBLE SORT :

    int c,d,swap,three;
    float ratio,ratio1;


    for (c = 0 ; c < 100; c++)
          {
            for (d = 0 ; d < 100 - c - 1; d++)
            {
                if(tour_index[d][1]!=0)
                {
                    ratio=(float)(tour_index[d][0])/(float)(tour_index[d][1]);
                    ratio1=(float)(tour_index[d+1][0])/(float)(tour_index[d+1][1]);
                }
                else
                    continue;


              if (ratio < ratio1) /// decreasing order
              {
                  for(three=0;three<3;three++)
                  {
                    swap = tour_index[d][three];
                    tour_index[d][three]   = tour_index[d+1][three];
                    tour_index[d+1][three] = swap;
                  }

              }
            }
          }
        int top_five[6][3]; /// tickets , capacity , index

        for ( c = 1 ; c < 6; c++ )
        {
            top_five[c][0]=tour_index[c][0];
            top_five[c][1]=tour_index[c][1];
            top_five[c][2]=tour_index[c][2];
        }

/// END OF BUBBLE SORT ; top_five holds the tickets and capacity and indexes in a ratio based sorting

        mysql_close(conn);
        show_most_attended(top_five);
		printf("\n  press any key to continue\n");
        getch();
        menu();
}

void show_most_attended(int top_five[6][3])
{
    printf("\n\t\t\t--- Most Attended Flights ----\n\n");
    char rows[100][150];
    MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
            }


        if (mysql_query(conn, "SELECT * FROM tours"))
            {
				fprintf(stderr, "%s\n", mysql_error(conn));
                exit(1);
            }

        res = mysql_use_result(conn);

        //! count

        int i=1;
        while ((coloumn = mysql_fetch_row(res)) != NULL)
		{
			 sprintf(rows[i],"Plane Number: %s\tOrigin: %s\tDestination: %s\tTime: %s\tDate: %s", coloumn[0],coloumn[1],coloumn[2],coloumn[3],coloumn[4]);
			 i++;
		}
		mysql_free_result(res);
		mysql_close(conn);

		for(i=1;i<6;i++)
        {
            printf("%d. %s Ratio: %d / %d\n",i,rows[top_five[i][2]],top_five[i][0], top_five[i][1]);
        }
}

void add_plane(char plane_number1[20])
{
	char model[20];
	int capacity;		///no duplicate
		printf("Plane not found\n\n");
		printf("\nEnter Plane model: ");
		scanf(" %20[^\n]s",  model);

		printf("\nEnter Plane capacity: ");
		scanf(" %d",  &capacity);

		MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
            }


        char q[1024];
        sprintf(q,"INSERT INTO flights.planes(plane_number,model,capacity) VALUES('%s','%s',%d)",plane_number1,model,capacity);

	        if(mysql_query(conn, q))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				mysql_close(conn);
				printf("\npress any key to continue");
				getch();
				menu();
			}

        mysql_close(conn);
}


void add_ticket(char date[10],char plane_number[20],char TCKN[15],char origin[50], char destination[50])
{
	int duplicate = pas_duplicate(date,plane_number,TCKN,origin,destination);

	if(duplicate) //! 0(false) when there is a duplicate
	{
		MYSQL *conn = mysql_init(NULL);
		MYSQL_RES *res;
		MYSQL_ROW coloumn;
		conn = mysql_init(NULL);

			if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
				{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
				}

		char q[1024];
		sprintf(q,"INSERT INTO flights.passengers(date, plane_number, TCKN, origin, destination) VALUES('%s','%s','%s','%s','%s')",date,plane_number,TCKN,origin,destination);

		if(mysql_query(conn, q))
            {
                fprintf(stderr, "%s\n", mysql_error(conn));
				mysql_close(conn);
                printf("\npress any key to continue");
                getch();
				menu();
            }

        printf("\n Ticket SUCCESSFULLY ADDEDD\n");
        mysql_close(conn);
	}
}

void add_user(char TCKN[15])
{

    char name[20];
    char email[20];
    char address[99];
    printf("\n\n * TCKN not Found, adding new entry...\n\n");
    printf("Enter Name: ");
    scanf(" %20[^\n]s",  name);

    printf("\nEnter email: ");
    scanf(" %20[^\n]s",  email);

    printf("\nEnter Address: ");
    scanf(" %99[^\n]s",  address);

//! ADDING A NEW TCKN TO users TABLE

    MYSQL *conn = mysql_init(NULL);
        MYSQL_RES *res;
        MYSQL_ROW coloumn;
        conn = mysql_init(NULL);

        if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))
            {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
            }
    char q[1024];
    sprintf(q,"INSERT INTO flights.users(TCKN, user_name, user_email, user_address) VALUES('%s','%s','%s','%s')",TCKN,name,email,address);

            if(mysql_query(conn, q))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				printf("\npress any key to continue");
				mysql_close(conn);
				getch();
				menu();
			}
            printf("\n TCKN SUCCESSFULLY ADDEDD\n");
            mysql_close(conn);
/// END of TCKN control
}

int pas_duplicate(char date[20],char plane_number[20],char TCKN[15], char origin[50],char destination[50])
{
    MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.passengers WHERE date='%s' AND plane_number='%s' AND TCKN='%s' AND origin='%s' AND destination='%s'",date,plane_number,TCKN,origin,destination);

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF PASSENGER FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}
int ticket_duplicate(char date[20],char plane_number[20],char TCKN[15])
{
    MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.passengers WHERE date='%s' AND plane_number='%s' AND TCKN='%s' ",date,plane_number,TCKN );

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF TICKET FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

int users_duplicate(char TCKN[15])
{
    MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.users WHERE users.TCKN='%s'",TCKN);

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF TICKET FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}


int planes_duplicate(char plane_number[20])
{
    MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.planes WHERE planes.plane_number='%s'",plane_number);

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF PLANE FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

int flight_duplicate(char plane_number[20],char origin[50],char date[10],char time[10],char destination[50])
{
	MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.tours WHERE plane_number='%s' AND origin='%s' " ,plane_number,origin );

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF TICKET FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

int flight_checking(char plane_number[20],char date[10])
{
	MYSQL *conn;
     MYSQL_RES *res;
     MYSQL_ROW coloumn;
     conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    char q[1024];
       ///CHECK FOR DUPLICATES
            sprintf(q,"SELECT * FROM flights.tours WHERE plane_number='%s' AND date='%s' " ,plane_number,date );

            if (mysql_query(conn, q))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }

            res = mysql_use_result(conn);


    ///IF TICKET FOUND SAY DUPLICATE !!!!
        if((coloumn = mysql_fetch_row(res)) != NULL)
        {
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }
    mysql_free_result(res);
    mysql_close(conn);
    return 1;
}

int get_capacity(char plane_number[20])
{

    int i;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW coloumn;
    conn = mysql_init(NULL);

     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    char q[1024];
    sprintf(q,"SELECT capacity FROM flights.planes WHERE plane_number='%s'",plane_number);


    if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

    res = mysql_use_result(conn);
    coloumn = mysql_fetch_row(res);

    i=atoi(coloumn[0]);


    /** close connection */
    mysql_free_result(res);
    mysql_close(conn);

    return i;
}

int capacity_reached(char plane_number[20],char date[10],char origin[50],char destination[50])
{

    int compare=get_capacity(plane_number);

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW coloumn;
    conn = mysql_init(NULL);

     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    char q[1024];
    sprintf(q,"SELECT * FROM flights.passengers WHERE plane_number='%s' AND date='%s' AND origin='%s' AND destination='%s'",plane_number,date,origin,destination);

    if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

    res = mysql_use_result(conn);
    coloumn = mysql_fetch_row(res);

    int i=0;
     while ((i=i+1)!=-1 && (coloumn = mysql_fetch_row(res)) != NULL )
     {
        //! count
        int z=0; //! prevent loop from being thrashed
     }

     /** close connection */
    mysql_free_result(res);
    mysql_close(conn);

    if(i>=compare) /// return 1 (true) if capacity is reached
        return 1;
    else
        return 0;
}


int count_tickets(char plane_number[20],char date[10],char origin[50],char destination[50])
{


    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW coloumn;
    conn = mysql_init(NULL);

     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    char q[1024];
    sprintf(q,"SELECT * FROM flights.passengers WHERE plane_number='%s' AND date='%s' AND origin='%s' AND destination='%s'",plane_number,date,origin,destination);

    if (mysql_query(conn, q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

    res = mysql_use_result(conn);
    coloumn = mysql_fetch_row(res);

    int i=0;
     while ((i=i+1)!=-1 && (coloumn = mysql_fetch_row(res)) != NULL )
     {
        //! count
        int j=0; //! prevent loop from being thrashed
     }

     /** close connection */
    mysql_free_result(res);
    mysql_close(conn);

    return i; //!return tickets
}

void show_details(char plane_number[20],char TC[70][15])
{
    int i;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW coloumn;
    conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }


    if (mysql_query(conn, "SELECT * FROM flights.users"))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

    res = mysql_use_result(conn);

     //! output table name
    printf("\nFlight: %s\n\n", plane_number);
    printf("%15.15s %20.20s %20.20s %20.20s \n","TCKN","Name","Email","Address");
    while ((coloumn = mysql_fetch_row(res)) != NULL)
        {
            for(i=0;i<70;i++)
            {
                if(!strcmp(coloumn[0],TC[i]))
                    printf("%15.15s %20.20s %20.20s %20.20s \n",coloumn[0],coloumn[1],coloumn[2],coloumn[3]);

            }
        }
    printf("\npress any key...\n");
    getch();

    /** close connection */
    mysql_free_result(res);
    mysql_close(conn);
    menu();

}

void hidden()
{
    printf("\n\t\t--run a query--\n\n");
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW coloumn;
    conn = mysql_init(NULL);


     //! Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    char q[1024];
    printf("\nEnter query: ");
    scanf(" %1024[^\n]s",  q);

    if (mysql_query(conn,q))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

    res = mysql_use_result(conn);

     //! output table name
    printf("\Results: \n\n");

    while ((coloumn = mysql_fetch_row(res)) != NULL)
    {
       printf("%20.20s %20.20s %20.20s  \n",coloumn[0],coloumn[1],coloumn[2] );
    }
        printf("\npress any key...\n");
    getch();

    /** close connection */
    mysql_free_result(res);
    mysql_close(conn);
    menu();

}

void menu()
{
    system("cls");
    printf("** Select an option and press ENTER**\n\n");
    printf("==========================================\n");
    printf("|| 0. Add a flight (Admin)\t\t||\n");
    printf("|| 1. Buy a ticket\t\t\t||\n");
    printf("|| 2. Your flights\t\t\t||\n");
    printf("|| 3. Cancel a flight(Admin)\t\t||\n");
    printf("|| 4. Cancel a ticket\t\t\t||\n");
    printf("|| 5. Flight Information (Admin)\t||\n");
    printf("|| 6. Most attended flights (Admin)\t||\n");
    printf("==========================================\n\n\n");
//!plane:
    printf("\n                             |\n                       --====|====--\n                             |  \n\n                         .-'''''-. \n                       .'_________'. \n                      /_/_|__|__|_\_\ '\n                     ;'-._       _.-';\n\\--------------------|    `-. .-'    |--------------------/\n ``''--..__    ___   ;       '       ;   ___    __..--''``\n           `'-// \\.._\               ..// \\-'` \n              \\\\_//    '._       _.'  \\\\_//\n               `'`       ``-----``     `'`\n\n");

    scanf(" %d", &sel);
    selection(sel);
}

void selection(int selected)
{
    switch(selected)
    {
    case 0:
        add_flight();
        break;
    case 1:
        buy_ticket();
        break;
    case 2:
        your_flights();
        break;
    case 3:
        cancel_flight();
        break;
    case 4:
        cancel_ticket();
        break;
    case 5:
        flight_information();
        break;
    case 6:
        most_attended();
        break;
    case 1696:
        hidden();
        break;
    default:
        printf("The selection you made is not available");
        Sleep(1000);
        menu();
        break;
    }

}

//! As there is no time left i am just posting the query(MySQL) that can be used for flights with 1 transfer

/**


 DROP TABLE IF EXISTS table2;

 CREATE TEMPORARY TABLE IF NOT EXISTS table2 AS
 (
	select origin from tours WHERE
	 (

		(origin IN(select destination from tours where origin = '1') AND destination='5')

	 )
 );


	 select * from tours WHERE
	 (

		(origin IN(select destination from tours where origin = '1') AND destination='5')
        OR
        (origin='1' AND destination IN(select origin from table2))

	 )
 */
