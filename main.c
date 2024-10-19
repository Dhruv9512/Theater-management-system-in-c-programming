#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// ------------------------- Structure To store movies Data----------------------

// structure for user details
typedef struct
{
    char mobile[11];
    char password[100];
} user;
user new_user;
// Structure to store movie details
typedef struct
{
    char title[40];
    char showtime[6];
    char genre[20];
    char rating[5];
    char price[5];
} Movie;
// structure To store seats details
typedef struct
{
    int seats[5][10];
} seat;

// ------------------------Function Declarations----------------------
void User_history();
void store_invoice(Movie *m, int quantity, char *date, char *time, int Ticket_id);
void Invoice(char title[40], char price[5], int quantity, char *date, char *time, int Ticket_id);
void store_seat_data(int Ticket_id, seat *s);
void print_seat_table(seat *s);
void Book_Ticket();
void store_movies_data(Movie *movie);
void Show_movies_data(int Movie_File_n, Movie *movie);
void update_seat_data(int Ticket_id, seat *s);
int countLinesInFile(const char *filename);
void Reserve_seat(int Ticket_id, int Movie_File_n);
void payment(int quantity, int Ticket_id);
char *getCurrentTime();
char *getCurrentDate();
void clearBuffer();
void clearScreen();
int check_user(char mobile[11]);
int check_login(char mobile[11], char password[100]);
int Login();
void Signup();
void main_login_page();
void main_PVR();

// ------------------------- main function ----------------------------
int main()
{
    main_login_page();
    return 0;
}

// --------------------------------------- Function Definition------------------------------------

// ---------------------- Support Function---------------------
// Function to display the history of user
void User_history()
{
    clearScreen();
    printf("\t\t\t\t\t\t\t    Your Ticket History\n");
    printf("\t\t\t\t\t\t\t---------------------------\n\n");
    printf("\n\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    char filename[30];
    sprintf(filename, "%s.txt", new_user.mobile);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        clearScreen();
        printf("Error opening file or no history found!\n");
        sleep(3);
        return;
    }

    char line[350];
    while (fgets(line, sizeof(line), file))
    {
        int movie_ID, movie_quantity;
        char temp_id[5] = {0};
        char temp_quantity[5] = {0};
        char movie_title[40] = {0};
        char movie_price[5] = {0};
        char movie_date[15] = {0};
        char movie_time[15] = {0};
        int time = 0, ID = 0, title = 0, price = 0, date = 0, temp_time = 0, quantity = 0;

        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ',')
            {
                time++;
                continue;
            }

            // Movie ID
            if (time == 0)
            {
                temp_id[ID++] = line[i];
            }
            // Movie title
            else if (time == 1)
            {
                movie_title[title++] = line[i];
            }
            // Ticket quantity
            else if (time == 2)
            {
                temp_quantity[quantity++] = line[i];
            }
            // Price
            else if (time == 3)
            {
                movie_price[price++] = line[i];
            }
            // Date
            else if (time == 4)
            {
                movie_date[date++] = line[i];
            }
            // Time
            else
            {
                movie_time[temp_time++] = line[i];
            }
        }

        // add '\0' in string
        temp_id[ID] = '\0';
        movie_ID = atoi(temp_id);
        movie_title[title] = '\0';
        temp_quantity[quantity] = '\0';
        movie_quantity = atoi(temp_quantity);
        movie_price[price] = '\0';
        movie_date[date] = '\0';
        movie_time[temp_time] = '\0';

        // Invoice Print
        Invoice(movie_title, movie_price, movie_quantity, &movie_date, &movie_time, movie_ID);

        printf("\n\n\n\n\n\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    fclose(file);

    char ex;
    while (1)
    {
        printf("\n\nIf you want to exit press (y): ");
        scanf(" %c", &ex);

        if (ex == 'y' || ex == 'Y')
        {
            main_PVR();
        }
        else
        {
            printf("Please enter valid operation");
        }
    }
}

// Function to store the invoice in user file
void store_invoice(Movie *m, int quantity, char *date, char *time, int Ticket_id)
{
    char filename[30];
    sprintf(filename, "%s.txt", new_user.mobile);

    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        clearScreen();
        printf("Error opening file!\n");
        sleep(5);
        exit(0);
    }

    fprintf(file, "%d,%s,%d,%s,%s,%s\n", Ticket_id, m[Ticket_id - 1].title, quantity, m[Ticket_id - 1].price, date, time);

    fclose(file);
}

// current time function
char *getCurrentTime()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char *buffer = (char *)malloc(9 * sizeof(char));
    if (buffer != NULL)
    {
        strftime(buffer, 9, "%I:%M %p", tm_info);
    }
    return buffer;
}

// Current date function
char *getCurrentDate()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char *buffer = (char *)malloc(11 * sizeof(char));
    if (buffer != NULL)
    {
        strftime(buffer, 11, "%Y-%m-%d", tm_info);
    }
    return buffer;
}

// Invoice Function
void Invoice(char title[40], char price[5], int quantity, char *date, char *time, int Ticket_id)
{
    printf("\t\t\t\t    PVR Cinemas Invoice\n");
    printf("\t\t\t\t---------------------------\n\n");
    printf("Date: %s\n", date);
    printf("Time: %s\n", time);
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("Movie Id\t\tTitle\t\t\tNumber of Tickets\t\tPrice per Ticket\n");
    printf("---------------------------------------------------------------------------------------------------\n\n");
    printf("%d %30s\t\t%d\t\t\t\t%4s\n", Ticket_id, title, quantity, price);

    double price_per_ticket = atof(price);
    double total = price_per_ticket * quantity;

    printf("---------------------------------------------------------------------------------------------------\n");
    printf("Total Price:\t\t\t\t\t\t\t\t\t%.2f\n", total);

    double discount = total * 0.1;
    printf("Discount @10%% \t\t\t\t\t\t\t\t\t%.2f\n", discount);
    printf("\t\t\t\t\t\t\t\t\t\t---------\n");
    double net_total = total - discount;
    printf("Net Total\t\t\t\t\t\t\t\t\t%.2f\n", net_total);

    double cgst = net_total * 0.09;
    double sgst = net_total * 0.09;
    printf("CGST @9%%\t\t\t\t\t\t\t\t\t%.2f\n", cgst);
    printf("SGST @9%%\t\t\t\t\t\t\t\t\t%.2f\n", sgst);

    double grand_total = net_total + cgst + sgst;
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("Grand Total\t\t\t\t\t\t\t\t\t%.2f\n", grand_total);
    printf("---------------------------------------------------------------------------------------------------\n");
}

// Payment Function
void payment(int quantity, int Ticket_id)
{
    clearScreen();
    Movie m[countLinesInFile("movies.txt")];
    store_movies_data(&m);

    // Print the total bill in an attractive format
    printf("-------------------------------------\n");
    printf("            PAYMENT SCREEN           \n");
    printf("-------------------------------------\n\n");
    printf("Your Total Bill: %d\n", quantity * atoi(m[Ticket_id - 1].price));
    char upi[50];
    char pass[10];
    printf("Please enter your UPI ID: ");
    scanf("%s", upi);
    clearBuffer();
    printf("Please enter your UPI PIN: ");
    scanf("%s", pass);
    clearScreen();
    Invoice(m[Ticket_id - 1].title, m[Ticket_id - 1].price, quantity, getCurrentDate(), getCurrentTime(), Ticket_id);
    char save;
    printf("Do you want to save this Invoice (y/n): ");
    scanf(" %c", &save);
    if (save == 'y' || save == 'Y')
    {
        printf("Your invoice has been saved successfully. (You will be redirected automatically)\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("Thank you for choosing PVR Cinemas!\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        store_invoice(&m, quantity, getCurrentDate(), getCurrentTime(), Ticket_id);
        sleep(5);
        main_PVR();
    }
    else
    {
        printf("Your invoice was not saved. (You will be redirected automatically)\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        printf("Thank you for choosing PVR Cinemas!\n");
        printf("---------------------------------------------------------------------------------------------------\n");
        sleep(5);
        main_PVR();
    }
}

// Function to store seat data
void store_seat_data(int Ticket_id, seat *s)
{
    FILE *file = fopen("seat.txt", "r");
    if (file == NULL)
    {
        clearScreen();
        printf("Error opening file!\n");
        return;
    }

    char line[300];
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        int id = atoi(line);
        if (id == Ticket_id)
        {
            found = 1;

            for (int j = 0; j < 5; j++)
            {
                if (fgets(line, sizeof(line), file))
                {
                    int col = 0;
                    for (int k = 0; line[k] != '\0'; k++)
                    {
                        if (line[k] != ',' && col < 10)
                        {
                            s->seats[j][col] = line[k] - '0';
                            col++;
                        }
                    }
                }
            }
            break;
        }
    }

    if (!found)
    {
        clearScreen();
        printf("Ticket ID %d not found in the file.\n", Ticket_id);
        sleep(2);
        Book_Ticket();
    }

    fclose(file);
}

// Function to show seat layout
void print_seat_table(seat *s)
{
    clearScreen();
    printf("\nSeat Layout:\n\n");
    printf("       ");
    for (int j = 0; j < 10; j++)
    {
        printf(" %d ", j + 1);
    }
    printf("\n       ");
    for (int j = 0; j < 10; j++)
    {
        printf("---");
    }
    printf("\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Row %d  ", i + 1);
        for (int j = 0; j < 10; j++)
        {
            if (s->seats[i][j] == 1)
            {
                printf(" * ");
            }
            else
            {
                printf(" - ");
            }
        }
        printf("\n");
    }

    printf("\nLegend:\n");
    printf(" (*)  Booked Seat\n");
    printf(" (-) Available Seat\n");
}

// Function to count the number of lines in a file
int countLinesInFile(const char *filename)
{
    FILE *file;
    char line[300];
    int lineCount = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return -1;
    }

    while (fgets(line, sizeof(line), file))
    {
        lineCount++;
    }

    fclose(file);

    return lineCount;
}

// Store Movies Data
void store_movies_data(Movie *movie)
{
    FILE *file = fopen("movies.txt", "r");
    if (file == NULL)
    {
        clearScreen();
        printf("Error opening file!\n");
        sleep(3);
        exit(0);
    }

    // Use For Indexes
    int count = 0;

    char line[300];
    while (fgets(line, sizeof(line), file) && count < countLinesInFile("movies.txt"))
    {
        int time = 0, name = 0, movie_time = 0, g = 0, rate = 0, price = 0;

        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++)
        {
            if (line[i] == ',')
            {
                time++;
                continue;
            }
            // Name storing
            else if (time == 0)
            {
                movie[count].title[name++] = line[i];
            }

            // Storing movie time
            else if (time == 1)
            {
                movie[count].showtime[movie_time++] = line[i];
            }

            // Storing genre
            else if (time == 2)
            {
                movie[count].genre[g++] = line[i];
            }
            // Storing rate
            else if (time == 3)
            {
                movie[count].rating[rate++] = line[i];
            }
            // Storing price
            else
            {
                movie[count].price[price++] = line[i];
            }
        }
        // put "\0" at end
        movie[count].genre[g] = '\0';
        movie[count].price[price] = '\0';
        movie[count].rating[rate] = '\0';
        movie[count].showtime[movie_time] = '\0';
        movie[count].title[name] = '\0';
        count++;
    }
    fclose(file);
}

// Function to update the seat Data
void update_seat_data(int Ticket_id, seat *s)
{
    FILE *file = fopen("seat.txt", "r+");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[300];
    int count = 0, position = -1;
    char ID[10];

    while (fgets(line, sizeof(line), file))
    {
        if (count == 6)
        {
            count = 0;
        }

        if (count == 0)
        {

            sscanf(line, "%s", ID);
            if (atoi(ID) == Ticket_id)
            {
                position = ftell(file);
                break;
            }
        }
        count++;
    }

    if (position == -1)
    {
        printf("Ticket ID not found.\n");
        fclose(file);
        return;
    }

    if (fseek(file, position, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < 10; k++)
        {
            fprintf(file, "%d", s->seats[j][k]);
            if (k < 9)
                fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Display movies Data
void Show_movies_data(int Movie_File_n, Movie *movie)
{
    clearScreen();
    printf("\n");
    printf("\t------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("\t\tID\t\t\tTitle\t\t\tShowtime\t\tGenre\t\tRating\t\tPrice\n");
    printf("\t------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < Movie_File_n; i++)
    {
        printf("\t\t%d%30s\t\t\t%5s\t%20s\t\t%3s\t\t%4s\n",
               i + 1,
               movie[i].title,
               movie[i].showtime,
               movie[i].genre,
               movie[i].rating,
               movie[i].price);
    }

    printf("\t------------------------------------------------------------------------------------------------------------------------------------\n");
}

// Reserve Seat
void Reserve_seat(int Ticket_id, int Movie_File_n)
{
    if (Ticket_id > Movie_File_n || Ticket_id <= 0)
    {
        clearScreen();
        printf("Enter a valid movie 'ID'.\n");
        sleep(2);
        return;
    }

    clearScreen();
    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n\n");

    seat s;
    store_seat_data(Ticket_id, &s);
    print_seat_table(&s);

    clearBuffer();
    int n;
    printf("\nEnter Number of tickets you want to book: ");
    scanf("%d", &n);

    // Validate quantity
    if (n <= 0 || n > 5)
    {
        clearScreen();
        printf("Please enter a valid number of tickets (1-5).\n");
        sleep(2);
        Book_Ticket();
    }
    clearBuffer();
    for (int i = 0; i < n; i++)
    {

        clearScreen();
        store_seat_data(Ticket_id, &s);
        print_seat_table(&s);
        printf("\n\nReserve a seat for %d person\n-------------------------------", i + 1);

        int row_number, seat_number;
        printf("\nEnter the row number you want to reserve for (1-5): ");
        scanf("%d", &row_number);

        printf("Enter the seat number in row %d (1-10): ", row_number);
        scanf("%d", &seat_number);

        // Input validation for row and seat number
        if (row_number < 1 || row_number > 5 || seat_number < 1 || seat_number > 10)
        {
            clearScreen();
            printf("Invalid row or seat number. Please try again.\n");
            sleep(2);
            i--;
            continue;
        }

        // Check seat availability
        if (s.seats[row_number - 1][seat_number - 1] == 1)
        {
            clearScreen();
            printf("Sorry, seat %d in row %d is already booked. Please choose a different seat.\n", seat_number, row_number);
            sleep(2);
            i--;
            continue;
        }
        else
        {
            s.seats[row_number - 1][seat_number - 1] = 1;
            clearScreen();
            printf("Seat %d in row %d has been successfully reserved!\n", seat_number, row_number);
            update_seat_data(Ticket_id, &s);
            sleep(2);
        }
    }
    payment(n, Ticket_id);
}

// Clear buffer function
void clearBuffer()
{
    while (getchar() != '\n')
        ;
}

// Clear the screen function
void clearScreen()
{
    system("cls||clear");
}

// Check if user exists
int check_user(char mobile[11])
{
    FILE *file = fopen("signup_info.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        sleep(5);
        exit(0);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char first_word[11] = {0};
        int i = 0;
        while (line[i] != ',' && line[i] != '\0')
        {
            first_word[i] = line[i];
            i++;
        }
        first_word[i] = '\0';

        if (strcmp(first_word, mobile) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// For Login Validation
int check_login(char mobile[11], char password[100])
{
    FILE *file = fopen("signup_info.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        sleep(5);
        exit(0);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char stored_mobile[11] = {0};
        char stored_password[100] = {0};
        int time = 0, mobile_index = 0, password_index = 0;

        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ',')
            {
                time++;
                continue;
            }
            if (time == 0)
            {
                stored_mobile[mobile_index++] = line[i];
            }

            if (time == 1)
            {
                stored_password[password_index++] = line[i];
            }
        }
        stored_mobile[mobile_index] = '\0';
        stored_password[password_index] = '\0';
        if (strcmp(stored_mobile, mobile) == 0 && strcmp(stored_password, password) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// --------------------------------------- Main Login Page Functions------------------------------

// For Login
int Login()
{
    clearScreen();
    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n");
    char mobile_number[11];
    char password[100];

    printf("Enter your Mobile Number: ");
    fgets(mobile_number, sizeof(mobile_number), stdin);
    mobile_number[strcspn(mobile_number, "\n")] = '\0';
    clearBuffer();
    printf("Enter your Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (check_login(mobile_number, password))
    {
        clearScreen();
        printf("You logged in successfully!\n");
        sleep(2);
        strcpy(new_user.mobile, mobile_number);
        strcpy(new_user.password, password);
        main_PVR();
    }
    else
    {
        clearScreen();
        printf("User is not valid, please try again.");
        sleep(3);
        main_login_page();
    }
}

// For Signup
void Signup()
{
    clearScreen();
    char name[100];
    char mobile[11];
    char password[100];

    FILE *file = fopen("signup_info.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return exit(0);
    }

    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n");

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter your mobile number: ");
    fgets(mobile, sizeof(mobile), stdin);
    mobile[strcspn(mobile, "\n")] = '\0';

    if (strlen(mobile) != 10)
    {
        clearScreen();
        printf("Mobile Number is invalid\n");
        sleep(2);
        return main_login_page();
    }

    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(mobile[i]))
        {
            clearScreen();
            printf("Mobile Number is invalid\n");
            sleep(5);
            return main_login_page();
        }
    }
    clearBuffer();

    printf("Enter your password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (check_user(mobile))
    {
        clearScreen();
        printf("User already exists.\n");
        sleep(5);
        return main_login_page();
    }

    fprintf(file, "%s,%s,%s\n", mobile, password, name);
    fclose(file);
    clearScreen();
    printf("Signup successful!...Now try to login.\n");
    sleep(3);
    return main_login_page();
}

// Main Login Page Function
void main_login_page()
{
    int login_choice;
    clearScreen();
    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n");
    printf("1. Login\n2. Signup\n3. Exit\n\n");
    printf("Enter your Choice: ");
    scanf("%d", &login_choice);
    clearBuffer();

    switch (login_choice)
    {
    case 1:
        Login();
        break;
    case 2:
        Signup();
        break;
    case 3:
        clearScreen();
        printf("You successfully exited the program.\n");
        exit(0);
    default:
        printf("Enter a valid operation.\n");
        sleep(2);
        main_login_page();
        break;
    }
}

// -------------------------------------- Main PVR Page Function-------------------------------------

// For Book Ticket
void Book_Ticket()
{
    clearScreen();
    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n\n");

    const char *filename = "movies.txt";
    int Movie_File_n = countLinesInFile(filename);
    Movie movie[Movie_File_n];
    store_movies_data(&movie);

    // show data
    Show_movies_data(Movie_File_n, &movie);
    int Ticket_id;
    printf("\n\tEnter 'ID' of Movie to book a ticket: ");
    scanf("%d", &Ticket_id);

    Reserve_seat(Ticket_id, Movie_File_n);
}

// For main PVR page
void main_PVR()
{
    clearScreen();
    int main_PVR_choice;
    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n");
    printf("1. Book Ticket\n2. Show My Bookings\n3. Exit\n\n");
    printf("Enter your Choice: ");
    scanf("%d", &main_PVR_choice);
    clearBuffer();

    switch (main_PVR_choice)
    {
    case 1:
        Book_Ticket();
        break;
    case 2:
        User_history();
        break;
    case 3:
        clearScreen();
        printf("You successfully exited the program.\n");
        exit(0);
    default:
        printf("Enter a valid operation.\n");
        sleep(2);
        main_PVR();
        break;
    }
}