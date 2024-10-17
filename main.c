#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// ------------------------- Structure To store movies Data----------------------

// Structure to store movie details
typedef struct
{
    char title[40];
    char showtime[5];
    char genre[20];
    char rating[5];
    char price[5];
    int seats[5][10];
} Movie;
// structure To store seats details
typedef struct
{
    int seats[5][10];
} seat; 

// ------------------------Function Declarations----------------------
void store_seat_data(int Ticket_id,seat*s);
void print_seat_table(seat*s);
void Book_Ticket();
void store_movies_data();
void Show_movies_data(int Movie_File_n, Movie *movie);
int countLinesInFile(const char *filename);
void Reserve_seat(int Ticket_id, int Movie_File_n);
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

// ---------------------------------- Function Definition----------------------------

// ---------------------------------- Support Function-----------------------------------

// Function to store seat data
void store_seat_data(int Ticket_id,seat*s) {
    FILE *file = fopen("seat.txt", "r");
    if (file == NULL) {
        clearScreen();
        printf("Error opening file!\n");
        sleep(3);
        exit(0);
    }

    char line[300];
    int count = 0;
    char ID[4];  // Buffer to store the movie ID

    while (fgets(line, sizeof(line), file)) {   
      
        if (count == 6) {
            count = 0;
        }
        
       
        if (count == 0) {
            int i;
           
            for (i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
                ID[i] = line[i];
            }
            ID[i] = '\0';  

            if (atoi(ID) == Ticket_id) {
                
                for (int j = 0; j < 5; j++) {
                    if (fgets(line, sizeof(line), file)) {
                        int col = 0;
                        for (int i = 0; line[i] != '\0'; i++) {
                            if (line[i] != ',') {
                                s->seats[j][col] =  line[i] - '0';  
                                col++;
                            }
                        }
                        
                    }
                }
            }
        }

        count++;  
    }
    
    fclose(file);
}

// Function to show seat layout
void print_seat_table(seat*s) {
    clearScreen();
    printf("\nSeat Layout:\n\n");
    printf("       ");
    for (int j = 0; j < 10; j++) {
        printf(" %d ", j + 1);  
    }
    printf("\n       ");
    for (int j = 0; j < 10; j++) {
        printf("---");  
    }
    printf("\n");
    for (int i = 0; i < 5; i++) {
        printf("Row %d  ", i + 1);  
        for (int j = 0; j < 10; j++) {
            if (s->seats[i][j] == 1) {
                printf(" * ");  
            } else {
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
    while (fgets(line, sizeof(line), file))
    {
        int time = 0, name = 0, movie_time = 0, genre = 0, rate = 0, price = 0;

        for (int i = 0; line[i] != '\0'; i++)
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
                movie[count].genre[genre++] = line[i];
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

            // put "\0" at end
            movie[count].genre[genre] = '\0';
            movie[count].price[price] = '\0';
            movie[count].rating[rate] = '\0';
            movie[count].showtime[movie_time] = '\0';
            movie[count].title[name] = '\0';
        }

        count++;
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

    if (Ticket_id > Movie_File_n || Ticket_id == 0)
    {
        printf("Enter a valid movie 'ID'.\n");
        sleep(5);
        Book_Ticket();
    }
    clearScreen();

    printf("\t    Welcome to PVR Cinemas\n\t------------------------------\n\n");
    seat s;
    store_seat_data(Ticket_id,&s);
    print_seat_table(&s);
     
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
        main_PVR();
    }
    else
    {
        clearScreen();
        printf("User is not valid, please try again.");
        sleep(5);
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