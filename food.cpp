#include <iostream>
using namespace std;

//  GLOBAL VARIABLES:
int booking_id = 0, n;

struct customer
{
    int id, alloted_de;
    char restaruant, destination;
    float time;
} c[1000];

struct delivery_executives
{
    int de_id, trip, de_charge, orders, de_allowance;
    float delivery_time, pickup_time;
} de[1000];

/*
    This initiates the delivery_executives structure and works upon and initiates the interal components of a delivery executive.
*/

void init_delivery_executives(struct delivery_executives de[])
{
    cout << "\n Enter the number of drivers available: \t:";
    cin >> n;
    if (0 < n and n < 1000)
    {
        for (int i = 1; i <= n; i++)
            de[i] = {i, 0, 0, 0, 0, 0.0, 0.0};
    }
}

/*
    This is the booking function:
*/

void init_customer_details(struct customer c[], int id)
{
    cout << "\n Enter the Customer ID: \t";             cin >> c[id].id;
    cout << "\n Enter the Restaruant: \t";              cin >> c[id].restaruant;
    cout << "\n Enter the Destination Point: \t";       cin >> c[id].destination;
    cout << "\n Enter the Time (integer): \t";          cin >> c[id].time;
}

/*  
    Time Arithmetic
*/

float time_management (float time, float minutes)
{
    int sec1 = (time - (int)time)*100;
    int sec2 = (minutes - (int)minutes)*100;
    int sec;

    if(sec1+sec2 >= 60)
    {
        sec = (sec1 + sec2) % 60;
        time += 1;
    }
    
    else
    {
        sec = sec1 + sec2;
    }

    int temp = int(time) % 24;

    if(temp == 0)
    {
        temp = 24;
    }

    return (temp + float(sec)/100);
}
    
int assignment_contigent (struct delivery_executives de[])
{
    for (int i=1; i <= n; i++)
    {
        if (de[i].orders == 0)
            return i;
    }

    int min = 100000, min_id = 0;

    for (int i = 1; i <= n; i++)
    {
        if (min > de[i].de_charge)
        {
            min = de[i].de_charge;
            min_id = i;
        }
    }

    return min_id;
}

int assign_delivery_executives (struct customer c[], struct delivery_executives de[], int cust)
{
    int free, cup = cust;

    free = assignment_contigent(de);
    
    booking_id += 1;
    bool loop = false;

    if (booking_id > 1)
    {
        for (int i = free - 1; i > 0; i--)
        {
            for (int j = cust - 1; j >= 0; j--)
            {
                if ( (abs(c[cust].time - c[j].time) < 0.15) && (c[cust].destination == c[j].destination) )
                {
                    free = i;
                    cup = j;
                    loop = true;
                    break;
                }
            }

            if (loop == true)
                break;
        }
    }

    c[cust].alloted_de = free;
    de[free].orders += 1;
    de[free].pickup_time = time_management(c[cup].time, 0.16);
    de[free].delivery_time = time_management(de[free].pickup_time, 0.30);
    de[free].de_charge += 50;
    de[free].de_allowance += 10;

    if (loop == true)
    {
        de[free].de_charge -= 50;
        de[free].de_charge += 5;
        de[free].de_allowance -= 10;
    }

    return free;
}

void fare_details(struct delivery_executives de[])
{
    cout << "Available Executives: " ;
    cout << "\nEXECUTIVES \t DELIVERY CHARGE EARNED:" << endl;
    for (int i = 1; i <= 5; i++)
        cout << "DE" << de[i].de_id << "\t\t\t" << de[i].de_charge << endl;
    cout << endl;
}

void earn_details(struct delivery_executives de[])
{
    FILE *fptr = fopen("earnings.txt", "w");
    fprintf(fptr, "\nEXECUTIVES \t ALLOWANCE \t DELIVERY CHARGES \t TOTAL \n");
    for (int i = 1; i <= n; i++)
    {
        if (de[i].de_charge == 0)
            break;
        fprintf(fptr, "\nDE%d \t\t %d \t\t %d \t\t\t\t %d", de[i].de_id, de[i].de_allowance, de[i].de_charge, de[i].de_allowance + de[i].de_charge);
    }
}

/*
    File Printing the output
*/

void put_customer(FILE *fp, struct customer c[], struct delivery_executives de[], int id)
{
    int cr = id;
    int dr = id+1;
    fprintf(fp, "\n");
    
    if (de[dr].orders == 0)
        return;

    fprintf(fp, "TRIP \t EXECUTIVE \t RESTAURANT \t DESTINATION \t ORDERS \t PICK-UP TIME \t DELIVERY TIME \t DELIVERY CHARGE \n");
    fprintf(fp, "%d \t\t DE%d \t\t %c \t\t\t\t %c \t\t\t\t %d \t\t\t %.2f \t\t\t %.2f \t\t\t %d \n", c[cr].id, de[dr].de_id, c[cr].restaruant, c[cr].destination, de[dr].orders, de[dr].pickup_time, de[dr].delivery_time, de[dr].de_charge);
    
}

int main()
{
    FILE *fp;
    fp = fopen("orders.txt", "w");

    int exit = 1;

    init_delivery_executives(de);

    while (exit)
    {
        init_customer_details(c, booking_id);
        fare_details(de);
        int deli = assign_delivery_executives(c, de, booking_id);
        
        cout << "Booking id: \t" << booking_id;
        cout << "\n The Assigned Delivery Executive for the Customer " << booking_id << " is DE" << deli;

        cout << "\n\n Click 0 to exit. 1 to continue.\t";
        cin >> exit;
    }

    for (int i=0; i<booking_id; i++)
        put_customer(fp, c, de, i);

    earn_details(de);

    return 0;
}