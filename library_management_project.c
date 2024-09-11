#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>

// System Operation Functions 
void password(void);
void menu(void);
void user_menu();
void userPanel(void);
void adminPanel(void);
void password_menu();
void Register();
void Login();
void forget_password();

// user operation function
void modifyUser();
void listUser();
int searchUser();
void view_books();
void list_books();
void searchByName();
void searchByID();
void view_issueinfo(char *);

// Staff Operation Function
void addUser();
void deleteUser();
void search_book();
void issue_book();
void return_book();
void remove_book();
void admin_menu();
void user_search();
void add_book();
void issueinfo();

// Supporting Function 
void due_date(int, struct tm *);

//Stores the ID of current User
char uid2[11];

//Library Book Structure
struct library{
    char Book_Name[50];
    char Author_Name[20];
    char Book_Number[10];
    char edition[5];
    char rack[8];
    char price[8];
    char category[20];
    char year_publication[6];
    char issue_to[20];
    char issue_date[20];
    char duedate[20];

};
struct library book;

//User Structure
struct user{
    char username[20];
    char gender[2];
    char phone[11];
    char userId[11];
    char Password[12];
};
struct user user;

int main(){
    system("cls");
    menu();
}

//Password Interface for Admin
void password(){
    char user[8];
    char userid[8]={"Admin"};
    char pwd[255];
    char code[255]={"Admin"};

    printf("-------------------\n");
    printf(">>> ADMIN LOGIN <<<\n");
    printf("-------------------\n\n");

    //Takes Admin Credentials
    printf("> Enter your username: ");
    scanf(" %[^\n]", user);
	printf("> Enter your password: ");
    scanf(" %[^\n]", pwd);

    // verifies the password 
    if(!strcmp(user, userid))
    {
        if(!strcmp(pwd, code)){
            admin_menu();
        }else{
            //If invalid password is given
            printf("-------------------------\n");  
            printf(">>> Invaild Password! <<<\n");
            printf("-------------------------\n\n");
            printf("Press any key to continue...");
            getch();
            password();
        }
    }
    else
    {
        //if invalid username is given
        printf("-------------------------\n");  
        printf(">>> Invaild Username! <<<\n");
        printf("-------------------------\n\n");
        printf("Press any key to continue...");
        getch();
        password();
    }
}

//Password Interface for User
void password_menu(){

    int choice;

    printf("---------------------\n");
    printf(">>> PASSWORD MENU <<<\n");
    printf("---------------------\n\n");
    printf("---------------------\n");
    printf("> 1. Register  \n");
    printf("> 2. Login  \n");
    printf("> 3. Forgot Password \n");
    printf("> 4. Main Menu\n");
    printf("---------------------\n\n");
    printf("> Enter Choice : ");
    scanf("%d",&choice);
    switch(choice){
        case 1:
        Register();
        break;
        case 2:
        Login();
        break;
        case 3:
        forget_password();
        break;
        case 4:
        menu();
        default:
        printf("\n\n");

        //If invalid choice is given
        printf("----------------------\n");
        printf(">>> INVALID CHOICE <<<\n");
        printf("----------------------\n\n");
        printf("> Try Agian\n\n");
        printf("Press any key to conitnue...");
        getch();
        system("cls");
        password_menu();
    }
}

//Register User
void Register(){
    //Clears the screen
    system("cls");

    struct user user;

    printf("-------------------------\n");
    printf(">>> Register Yourself <<<\n");
    printf("-------------------------\n\n");
    FILE *pF = fopen("user_Records.txt", "ab");
    if(pF != NULL)
    {
        //Takes new user credentials
        fflush(stdin);
        printf("> Enter the Username: ");
        scanf(" %[^\n]", user.username);

        printf("> Enter Gender [M/F]: ");
        scanf(" %[^\n]", user.gender);

        printf("> Enter Phone Number: ");
        scanf(" %[^\n]", user.phone);

        printf("> Enter User ID: ");
        scanf(" %[^\n]", user.userId);

        printf("> Enter password : ");
        scanf(" %[^\n]", user.Password);

        //Store credentials into the file
        fwrite(&user,sizeof(user),1,pF);

        printf("\n\n");
        printf("-------------------------------\n");
        printf(">>> SUCCESSFUL REGISTRATION <<< \n");
        printf("-------------------------------\n\n");
    }
    else{
        //If files doesn't opens
        printf("\n\n");
        printf("-------------------------------\n");
        printf(">>> Error in opening file <<< \n");
        printf("-------------------------------\n\n");
    }
    fclose(pF);
    printf("Press any key to continue....");
    getch();
    password_menu();

}

void Login(){
    //Clears the screen
    system("cls");

    struct user user;
    char uid[11];
    char pass[12];
    int found=0;

    printf("-----------------------\n");
    printf(">>> LOGIN INTERFACE <<<\n");
    printf("-----------------------\n\n");

    //Takes User Credentials
    printf("> USERID : ");
    scanf(" %[^\n]", uid);

    printf("> PASSWORD : ");
    scanf(" %[^\n]", pass);

    //Open users.txt file
    FILE *pF = fopen("user_Records.txt", "rb");
    if(pF == NULL){
        printf("\n\n");
        printf("-----------------------------\n");
        printf(">>> ERROR IN OPENING FILE <<<");
        printf("-----------------------------\n\n");
        password_menu();
    }else{
        while(fread(&user,sizeof(user),1,pF)==1){
            if(strcmp(user.userId,uid)==0 && strcmp(user.Password,pass)==0){
                //If user is found
                printf(" \n\n >> you login succesfully << \n");
                found=1;
                strcpy(uid2,uid);
            }

        }
        if(!found){
            //If the user doesn't exist
            printf("\n\n");
            printf("----------------------------------\n");
            printf(">>> WRONG USERNAME OR PASSWORD <<<\n");
            printf("----------------------------------\n\n");
            fclose(pF);
            printf("Press any key to continue...");
            getch();
            password_menu();
        }
        else{
            //Successful Login
            fclose(pF);
            printf("\n\n");
            printf("------------------------\n");
            printf(">>> LOGIN SUCCESSFUL <<<\n");
            printf("------------------------\n\n");

            printf("Press any key to continue...");
            getch();
            user_menu(); 
        }
    }
    
}

//Forget password for users
void forget_password(){
    system("cls");

    //Set seed of rand() to current time
    srand(time(NULL));

    printf("-----------------------\n");
    printf(">>> FORGOT PASSWORD <<<\n");
    printf("-----------------------\n\n");

    struct user user;
    char  pass[12] ,pass1[12];
    int flag=0;
    char find[11];
    int captcha;

    //Generate Captcha code
    int n = 1000 + rand()%100;
    
    printf("> CAPTCHA CODE: %d\n",n);
    printf("> Enter captcha code : ");
    scanf("%d",&captcha);

    if(captcha == n){
        //Takes User id
        printf("\n> Enter the user ID: ");
        scanf(" %[^\n]", find);

        FILE *pF = fopen("user_Records.txt", "rb");
        FILE *pT = fopen("temporary.txt", "ab");

        while(fread(&user,sizeof(struct user),1,pF)){
            if(!strcmp(find,user.userId)){
                //takes new password
                printf("> Enetr password : ");
                scanf(" %[^\n]", pass);

                printf("> Confirm password : ");
                scanf(" %[^\n]", pass1);

                if(!strcmp(pass , pass1)){
                    strcpy(user.Password,pass);
                    fwrite(&user,sizeof(user),1,pT);
                    printf("\n\n");
                    printf("-------------------------------------\n");
                    printf(">>> PASSWORD CHANGED SUCCESSFULLY <<<\n");
                    printf("-------------------------------------\n\n");
                }
                else{
                    printf("\n\n");
                    printf("-------------------------------\n");
                    printf(">>> PASSWORD DOES NOT MATCH <<<\n");
                    printf("-------------------------------\n\n");
                    printf("> Try Again\n\n");
                    printf("Press any key to continue...");
                    getch();
                    forget_password();
                }
                flag = 1;
            }
            else{
                fwrite(&user,sizeof(user),1,pT);
            }
        }
        fclose(pF);
        fclose(pT);

        if(!flag){
            printf("\n\n");
            printf("------------------------\n");
            printf(">>> RECORD NOT FOUND <<<\n");
            printf("------------------------\n\n");
            pT = fopen("temporary.txt", "wb");
            fclose(pT);
            printf("Press key to continue...");
            getch();
            password_menu();
        }else{
            pF = fopen("user_Records.txt", "wb");
            pT = fopen("temporary.txt", "rb");

            while(fread(&user,sizeof(user),1,pT)){
                fwrite(&user,sizeof(user),1,pF);
            }
            fclose(pF);
            fclose(pT);
            pT = fopen("temporary.txt", "wb");
            fclose(pT);
            printf("Press any key to continue...");
            getch();
            password_menu();
        }
    }
    else{
        printf("\n\n");
        printf("----------------------------\n");
        printf(">>> INVALID CAPTCHA CODE <<<\n\n");
        printf("----------------------------\n\n");
        printf("> Try Again\n\n");
        printf("Press any key to continue...");
        getch();
        forget_password();
    }
}

// Main Selection menu between User & Book Panel
void menu(){
    system("cls");

    int number;

    printf("---------------------------------\n");
    printf(">>> LIBRARY MANAGEMENT SYSTEM <<< \n");
    printf("---------------------------------\n\n");
    printf("> 1. User Management\n");
    printf("> 2. Login as Admin\n");
    printf("> 3. Exit programme\n\n");
    printf("> Enter the number: ");
    scanf("%d",&number);

    switch(number)
    {
        case 1:
            userPanel();
            break;
        case 2:
            adminPanel();
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("\n\n");
            printf("----------------------\n");
            printf(">>> INVLALID INPUT <<<\n");
            printf("----------------------\n\n");
            
            printf("Press any key to continue...");
            getch();

            menu();
        
    }
}

//Prints header for admin password interface
void adminPanel(){
    system("cls");
    printf("-----------------------------------------------\n");
    printf(">>> LIBRARY MANAGEMENT SYSTEM - ADMIN PANEL <<< \n");
    printf("-----------------------------------------------\n\n");
    password();  
}

//Prints header for User password interface
void userPanel(){
    system("cls");
    printf("----------------------------------------------\n");
    printf(">>> LIBRARY MANAGEMENT SYSTEM - USER PANEL <<< \n");
    printf("----------------------------------------------\n\n");
    password_menu();      
}

//User menu
void user_menu(){
    char uid1[11];
    strcpy(uid1,uid2);
    int number;
menu:
    system("cls");

    printf("---------------------------------------------\n");
    printf(">>> LIBRARY MANAGEMENT SYSTEM - USER MENU <<<\n");
    printf("---------------------------------------------\n\n");
    printf("> 1. Search book \n");
    printf("> 2. List of issued book\n");
    printf("> 3. List of Books\n");
    printf("> 4. Open Main Menu \n");
    printf("> 5. Close the Program \n\n");

    printf("> Enter the number & hit ENTER: ");
    scanf("%d",&number);

    switch(number)
    {
        case 1:
            search_book();
            break;
        case 2:
            view_issueinfo(uid1);
            break;
        case 3:
            list_books();
            break;
        case 4:
            menu();
            break;
        case 5:
            exit(0);
            break;
        default:
            printf("\n\n");
            printf("----------------------\n");
            printf(">>> INVALID CHOICE <<<\n");
            printf("----------------------\n\n");
            printf("> Try Again\n\n");
            printf("Press any key to continue...");
            getch();
            goto menu;
    }
}

//Addition of new user
void addUser(){
    
label1:
    
    system("cls");

    FILE *pF = fopen("user_Records.txt", "ab");

    if(pF != NULL)
    {
        //Takes credentials for new user
        fflush(stdin);
        printf("> Enter the Username: ");
        scanf(" %[^\n]", user.username);

        printf("> Enter Gender [M/F]: ");
        scanf(" %[^\n]", user.gender);

        printf("> Enter Phone Number: ");
        scanf(" %[^\n]", user.phone);

        printf("> Enter User ID: ");
        scanf(" %[^\n]", user.userId);

        printf("> Enter the Password: ");
        scanf(" %[^\n]", user.Password);

        fwrite(&user,sizeof(user),1,pF);

        printf("\n\n");
        printf("--------------------------------------\n");
        printf(">>> User Record Added Successfully <<<\n");
        printf("--------------------------------------\n\n");

    }
    else
    {
        //Error in opening/locating file
        printf("\n\n");
        printf("----------------------------------\n");
        printf(">>> UNABLE TO LOCATE/OPEN FILE <<<\n");
        printf("----------------------------------\n");
        printf("> Redirecting to Admin Menu\n\n");
        printf("Press any key to continue...");
        getch();
        system("cls");
        admin_menu();
    }

    fclose(pF); 
    char input;

retry:
    //retry screen

    printf("\n> Do you wanna enter more records [Y/N]: ");
    scanf(" %c",&input);

    if(input == 'y' || input=='Y')
    {
        goto label1;
    }
    else if(input=='n' || input=='N')
    {
        printf("> Redirecting to Admin Menu\n\n");
        printf("Press any key to continue...");
        getch();
        system("cls");
        admin_menu();
    }
    else
    {
        printf("> Invalid Input\n");
        printf("> Try Again\n\n");
        goto retry;
    }
}

//Modify existing user credentials
void modifyUser(char uid[]){
    system("cls");

    char name1[20], gender1[2];
    char  uid1[11];
    char  phone1[11];
    int flag=0;

    strcpy(uid1,uid);

    FILE *pF = fopen("user_Records.txt", "rb");
    FILE *pT = fopen("temporary.txt", "ab");

    while(fread(&user,sizeof(struct user),1,pF)==1)
    {
        if(strcmp(uid1,user.userId)==0)
        {
            //If user already exists
            printf("\n\n");
            printf("------------------------------------------\n");
            printf(">>> RECORD FOUND, ALLOWING MODIFICATON <<<\n");
            printf("------------------------------------------\n\n");

            //takes user's new credentials
            printf("> Enter Username: ");
            scanf(" %[^\n]", user.username);

            printf("> Enter Gender [M/F]: ");
            scanf(" %[^\n]", user.gender);

            printf("> Enter User ID: ");
            scanf(" %[^\n]", user.userId);

            printf("> Enter Phone Number: ");
            scanf(" %[^\n]", user.phone);

            fwrite(&user,sizeof(user),1,pT);
            
            printf("\n\n");
            printf("----------------------------------\n");
            printf(">>> USER MODIFIED SUCCESSFULLY <<<\n");
            printf("----------------------------------\n\n");

            flag = 1;
        }
        else fwrite(&user,sizeof(user),1,pT);
    }

    fclose(pF);
    fclose(pT);

    if(!flag)
    {
        printf("\n\n");
        printf("------------------------\n");
        printf(">>> RECORD NOT FOUND <<<\n");
        printf("------------------------\n\n");
        printf("> Redirecting to User Menu\n\n");
        pT = fopen("temporary.txt", "wb");
        fclose(pT);
        printf("Press any key to continue...");
        getch();
        system("cls");
        admin_menu();
    }else{
        pF = fopen("user_Records.txt", "wb");
        pT = fopen("temporary.txt", "rb");

        while(fread(&user,sizeof(user),1,pT))
        {
            fwrite(&user,sizeof(user),1,pF);
        }

        fclose(pF);
        fclose(pT);

        pT = fopen("temporary.txt", "wb");
        fclose(pT);

        admin_menu();
    }
}

//list existing users
void listUser(){
    system("cls");
    
    FILE *pF = fopen("user_Records.txt", "rb");
    int empty=1;

    printf("\n\n");
    printf("--------------------\n");
    printf(">>> USER RECORDS <<<\n");
    printf("--------------------\n\n");
    
    //reads data for user.txt
    while(fread(&user,sizeof(user),1,pF)==1){
        printf("-------------------------------\n");
        printf("> Full Name: %s \n", user.username);
        printf("> Gender: %s \n", user.gender);
        printf("> Student-ID: %s \n", user.userId);
        printf("> Phone No.: %s \n", user.phone);
        printf("-------------------------------\n\n\n");
        empty=0; 
    }

    fclose(pF);

    if(empty)
    {
        printf("---------------------------------------\n");
        printf("> There is no user records added yet...\n");
        printf("---------------------------------------\n\n");
    }

    printf("Press any key to get back to User Panel.\n");
    getch();
    admin_menu();
}

//Search for user by user id
int searchUser(){

label2:
    system("cls");

    int flag=0;   
    char find[11];

    printf("Search by User ID of the student: ");
    scanf(" %[^\n]", find);

    FILE *pF = fopen("user_Records.txt", "rb");

     while(fread(&user,sizeof(user),1,pF))
    {

        if(!strcmp(find, user.userId))
        {
            printf("\n\n");
            printf("--------------------\n");
            printf(">>> RECORD FOUND <<<\n");
            printf("--------------------\n\n");

            printf("-------------------------------\n");
            printf("> Full Name: %s \n", user.username);
            printf("> Gender: %s \n", user.gender);
            printf("> Student-ID: %s \n", user.userId);
            printf("> Phone Number: %s \n", user.phone);
            printf("-------------------------------\n\n");
            flag=1;
        }
    }
    
    fclose(pF);

    if(!flag)
    {
        printf("\n\n");
        printf("------------------------\n");
        printf(">>> RECORD NOT FOUND <<<\n");
        printf("------------------------\n\n");
    }
    
    printf("Press any key to redirect back to Panel.");
    getch();
    user_menu();
}

//list books for admin
void view_books(){
    system("cls");

    //opens book.txt file
    FILE *fptr;
    fptr=fopen("book.txt","rb");

    printf("\n\n");
    printf("----------------------\n");
    printf(">>> VIEW BOOK DATA <<<\n");
    printf("----------------------\n\n");
    while(fread(&book,sizeof(struct library),1,fptr)){
        //prints book data
        printf("\n-------------------------------------------\n\n");
        printf("> Book Name: %s\n",book.Book_Name);
        printf("> Author Name: %s\n",book.Author_Name);
        printf("> Book Published: %s \n",book.year_publication);
        printf("> Book Number: %s \n",book.Book_Number); 
        printf("> Book Edition: %s \n",book.edition);
        printf("> Book Price: %s \n",book.price);
        printf("> Rack No: %s \n",book.rack);
        printf("\n-------------------------------------------\n\n");
    }
    fclose(fptr);
    printf("Press any key to continue....");
    getch();
    admin_menu();
}

//delete existing user
void deleteUser(char uid[]){

    system("cls");

    int flag=0;
    char find[11];
    strcpy(find,uid);

    //opens user records file and a temporary file
    FILE *pF = fopen("user_Records.txt", "rb");
    FILE *pT = fopen("temporary.txt", "ab");

    if(pF==NULL && pT ==NULL){
        printf(">>> Error in opening/locating file <<<\n\n");
        printf("> Redirecting to Admin Menu");
        getch();
        admin_menu();
    }else{
        //deletes the user
        while(fread(&user,sizeof(user),1,pF)==1)
        {

            if(strcmp(find, user.userId)==0)
            {
                printf("\n\n");
                printf("----------------------\n");
                printf(">>> RECORD DELETED <<<\n");
                printf("----------------------\n\n");
                printf("Redirecting to Admin Menu...");


                flag = 1;
            }
            else fwrite(&user,sizeof(user),1,pT);
        }

        fclose(pF);
        fclose(pT);

        if(!flag)
        {
            printf("\n\n");
            printf("------------------------\n");
            printf(">>> RECORD NOT FOUND <<<\n");
            printf("------------------------\n\n");
            printf("> Redirecting to Admin Menu");
            getch();
            admin_menu();
        }else{
            pF = fopen("user_Records.txt", "wb");
            pT = fopen("temporary.txt", "rb");

            while(fread(&user,sizeof(user),1,pT))
            {
                fwrite(&user,sizeof(user),1,pF);
            }

            fclose(pF);
            fclose(pT);

            pT = fopen("temporary.txt", "wb");
            fclose(pT);

            admin_menu();
        }
    }
}

//Menu interface for admin
void admin_menu(){
    int choice;
    char uid[11];
label1:
    system("cls");

    printf("\n\n");
    printf("------------------\n");
    printf(">>> ADMIN MENU <<<\n");
    printf("------------------\n\n");
    printf("-----------------------------\n");
    printf(">  1 : Add Books\n");
    printf(">  2 : View Books\n");
    printf(">  3 : List of User\n");
    printf(">  4 : Issue Books\n");
    printf(">  5 : Return Books\n");
    printf(">  6 : Delete Books\n");
    printf(">  7 : User information\n");
    printf(">  8 : Add User\n");
    printf(">  9 : Menu\n");
    printf("> 10 : Delete user\n");
    printf("> 11 : View Issued Books List\n");
    printf("> 12 : Modify User\n");
    printf("> 13 : Search User\n");
    printf("-------------------------------\n\n");
    fflush(stdin);
    printf(">>Enter choice : ");
    scanf("%d",&choice);
    
    switch(choice){
        case 1: 
        add_book();
        break;
        case 2:
        view_books();
        break;
        case 3:
        listUser();
        break;
        case 4:
        issue_book();
        break;
        case 5:
        return_book();
        break;
        case 6:
        remove_book();
        break;
        case 7:
        user_search();
        break;
        case 8:
        addUser();
        break;
        case 9:
        menu();
        break;
        case 10:
        printf("Enter the User ID: ");
        scanf(" %[^\n]", uid);
        deleteUser(uid);
        break;
        case 11:
        issueinfo();
        break;
        case 12:
        printf("Enter the User ID: ");
        scanf(" %[^\n]", uid);
        modifyUser(uid);
        case 13:
        searchUser();
        break;
        default:
        printf("> Invalid Choice <\n\n");
        printf("> Try Again");
        getch();
        goto label1;
        break;
    }
    
}

//add new books
void add_book(){ 
    system("cls");

    FILE *ptr;
    char a ='y'; 
    struct library book; 
    ptr = fopen("book.txt","ab"); 

    if(ptr == NULL){ 
        printf(">>> Error opening/locating file <<<\n\n");
        printf("> Redirecting to Admin Menu ");
        getch();
        admin_menu(); 
    } 

    while( a == 'y'){
        printf("\n\n");
        printf("---------------------\n");
        printf(">>> ADD BOOK DATA <<<\n");
        printf("---------------------\n\n");
        printf("> Book Name: "); 
        scanf(" %[^\n]", book.Book_Name); 

        printf("> Author Name: "); 
        scanf("  %[^\n]", book.Author_Name);

        printf("> Publication year: ");
        scanf("  %[^\n]", book.year_publication); 

        printf("> Book Number: "); 
        scanf("  %[^\n]", book.Book_Number);

        printf("> Edition of Book: "); 
        scanf("  %[^\n]", book.edition);

        printf("> Category of Book: "); 
        scanf("  %[^\n]", book.category);

        printf("> Price of Book: ");
        scanf("  %[^\n]", book.price);

        printf("> Rack no of Book: ");
        scanf("  %[^\n]", book.rack);

        fwrite(&book,sizeof(struct library),1,ptr); 
        printf("> Do you want to continue [Y/N]: "); 
        scanf(" %c",&a);
        system("cls");
    } 
    fclose(ptr);
    admin_menu(); 
}

//list existing books for user
void list_books(){
    system("cls");

    FILE *fptr;
    fptr=fopen("book.txt","rb");

    printf("\n\n");
    printf("----------------------\n");
    printf(">>> VIEW BOOK DATA <<<\n");
    printf("----------------------\n\n");
    while(fread(&book,sizeof(struct library),1,fptr)){
        printf("\n-------------------------------------------\n\n");
        printf("> Book Name: %s\n",book.Book_Name);
        printf("> Author Name: %s\n",book.Author_Name);
        printf("> Book Published: %s \n",book.year_publication);
        printf("> Book Number: %s \n",book.Book_Number); 
        printf("> Book Edition: %s \n",book.edition);
        printf("> Book Price: %s \n",book.price);
        printf("> Rack No: %s \n",book.rack);
        printf("\n-------------------------------------------\n\n");
    }
    fclose(fptr);
    printf("Press any key to continue....");
    getch();
    user_menu();
}

//search books main function
void search_book()
{
label1:
    system("cls");

    printf("\n\n");
    printf("------------------------\n");
    printf(">>> SEARCH BOOK DATA <<<\n");
    printf("------------------------\n\n");
    printf(">  1. Search By ID\n");
    printf(">  2. Search By Name\n\n");
    printf(">  Enter Your Choice: ");
    switch(getch())
    {
        case '1':
            searchByID();
            break;
        break;
        case '2':
            searchByName();
            break;
        default :
            printf("> Invalid Input\n\n");
            printf("> Try Again");
            getch();
            search_book();
    }
}

//issue books for user
void issue_book(){
    system("cls");

    printf("\n\n");
    printf("------------------\n");
    printf(">>> ISSUE BOOK <<<\n");
    printf("------------------\n\n");

    char bookid[10];
    char id_no[11];
    int found=0;
    char date[20];
    char date_string[20];
    struct library book;

    //takes issued book data
    printf("> Enter book number to be issued : ");
    scanf(" %[^\n]", bookid);

    printf("> Enter User ID: ");
    scanf(" %[^\n]", id_no);

    //takes current time
    time_t current_time = time(NULL);
    struct tm *currentDate = localtime(&current_time);
    
    //store date data in date_string in YYYY-MM-DD format
    strftime(date_string, 20, "%Y-%m-%d", currentDate);

    //opens book file and issue file
    FILE *fptr;
    FILE *file;
    fptr=fopen("book.txt","rb+");
    file=fopen("issue.txt","ab");

    if(fptr==NULL && file == NULL){
        printf(">>> ERROR IN OPENING/LOCATING FILE <<<\n\n");
    }

    //calculates due date
    due_date(20,currentDate);

    //stores due date in YYYY-MM-DD format in date
    strftime(date,20,"%Y-%m-%d",currentDate);

    //Removes allocated memory to currentDate
    free(currentDate);


    while(fread(&book,sizeof(book),1,fptr)){
        if(!strcmp(book.Book_Number,bookid)){
            strcpy(book.issue_to,id_no);
            strcpy(book.issue_date,date_string);
            strcpy(book.duedate,date);

            //Stores data in issue file
            fwrite(&book,sizeof(struct library),1,file);

            printf("\n\n");
            printf("--------------------------------\n");
            printf(">>> BOOK ISSUED SUCCESSFULLY <<<\n"); 
            printf("--------------------------------\n\n");
            found=1;
            break;
        }
    }

    fclose(fptr);
    fclose(file);

    if(!found){
        printf("> Book does not exist\n\n");
    }

    printf("Press any key for continue....");
    getch();
    admin_menu();

}

//returned book from user
void return_book(){
    system("cls");

    printf("\n\n");
    printf("-------------------\n");
    printf(">>> RETURN BOOK <<<\n");
    printf("-------------------\n\n");

    int found =0;
    char bookid[10];

    //takes returned book credential
    printf("> Book ID of the book to be returned: ");
    scanf(" %[^\n]", bookid);

    printf("\n");

    //opens issue and temp file
    FILE *fptr, *tptr;
    fptr=fopen("issue.txt","rb");
    tptr=fopen("temp.txt", "wb");
    if(fptr==NULL){
        //Error encountered in file handeling
        printf(">>> ERROR IN OPENING/LOCATING FILE <<<\n\n");
        printf("Press any key to continue...");
        getch();
        admin_menu();
    }else{
        while(fread(&book ,sizeof(book),1,fptr)){
            if(!strcmp(book.Book_Number,bookid) && !found){
                //if book is found
                found=1;

                printf("\n\n");
                printf("----------------------------------\n");
                printf(">>> BOOK RETURNED SUCCESSFULLY <<<\n"); 
                printf("----------------------------------\n\n");
                continue;
            }
            fwrite(&book, sizeof(book), 1, tptr);
        }
        fclose(fptr);
        fclose(tptr);

        if(!found){
            printf("> Book not issued\n\n");
            printf("> Redirecting to Admin Menu");
            getch();
            tptr = fopen("temp.txt", "wb");
            fclose(tptr);
            admin_menu();
        }else{
            fptr=fopen("issue.txt","wb");
            tptr=fopen("temp.txt", "rb");
            while(fread(&book, sizeof(book), 1, tptr)){
                fwrite(&book, sizeof(book), 1, fptr);
            }
            fclose(fptr);
            fclose(tptr);
            tptr=fopen("temp.txt", "wb");
            fclose(tptr);
            printf("Press any key to continue..."); 
            getch();
            admin_menu();
        }
    }
 
}

//removes existing book
void remove_book(){
    system("cls");

    printf("\n\n");
    printf("-------------------------\n");
    printf(">>> BOOK REMOVAL MENU <<<\n");
    printf("-------------------------\n\n");

    char bookid[10];

    //takes book id
    printf("> Enter ID of the book you want to delete: ");
    scanf(" %[^\n]", bookid);

    FILE *fptr;
    FILE *ptr;
    int found=0;
    struct library book;

    //opens book and temp file
    fptr=fopen("book.txt","rb");
    ptr=fopen("temp.txt","wb");
    if(fptr ==NULL && ptr==NULL){
        printf("Error in file opening  ");
    }else{
        while (fread(&book,sizeof(struct library),1,fptr)){
            if (!strcmp(book.Book_Number,bookid) && !found){
                //if book is found
                printf("\n\n");
                printf("---------------------------------\n");
                printf(">>> BOOK DELETED SUCCESSFULLY <<<\n");
                printf("---------------------------------\n\n");
                found=1;
                continue;
            } 
            fwrite(&book, sizeof(struct library), 1, ptr);
        }
        fclose(fptr);
        fclose(ptr);
        if (!found) {
            printf("> No record found with the requested name of book : %s\n\n",bookid);
            printf("> Redirecting to Admin Menu ");
            getch();
            admin_menu();
        }else{
            fptr = fopen("book.txt", "wb");
            ptr = fopen("temp.txt", "rb");

            while(fread(&book,sizeof(book),1,ptr)){
                fwrite(&book,sizeof(book),1,fptr);
            }

            fclose(fptr);
            fclose(ptr);

            ptr= fopen("temp.txt", "wb");
            fclose(ptr);

            printf("Press any key to continue....");
            getch();
            admin_menu();
        }
    }
}


//search user for admin using user id
void user_search(){
    int found=0;
    char another;
    FILE *fptr;

label:
    system("cls");
    
    //opens user_records file
    fptr=fopen("user_Records.txt","rb");

    if(fptr==NULL){
        printf(">>> ERROR IN OPENING/LOCATING FILE <<<\n\n");
        printf("> Redirecting to Admin Menu ");
        getch();
        admin_menu();
    }
    else{
        char id_no[11];
        struct user user;

        //takes user id
        printf("> Enter user id :");
        scanf(" %[^\n]", id_no);

        while(fread(&user, sizeof(user), 1, fptr)){
            if(!strcmp(user.userId, id_no)){
                //if user is found
                found = 1;

                printf("\n\n");
                printf("------------------\n");
                printf(">>> USER FOUND <<<\n");
                printf("------------------\n\n");

                //list credentials of the searched user
                printf("User Name :");
                printf(user.username);
                printf("\nUser ID : ");
                printf(user.userId);
                printf("\nUser Mobile No : ");
                printf(user.phone);
                printf("\n");
            }
        }
        fclose(fptr);
        
        if(!found){
            printf("> User not found\n");
        }
        goto retry;
    }
retry:
    //retry screen

    printf("> Do you want to search again [Y/N]: ");
    scanf(" %c", &another);
    if(another=='y' || another=='Y'){
        goto label;
    }else{
        admin_menu();
    }
}

//Search book by id for user
void searchByID(){
    system("cls");

    printf("\n\n");
    printf("-------------------------\n");
    printf(">>> SEARCH BOOK BY ID <<<\n");
    printf("-------------------------\n\n");

    char id[10];
    int findBook = 0;
    FILE *fp;

    //takes book id
    printf("> Enter the book id:");
    scanf(" %[^\n]", id);

    fp = fopen("book.txt","rb+");

    while(fread(&book,sizeof(book),1,fp)){
        if(!strcmp(book.Book_Number,(id))){
            //if book is found
            printf("\n\n");
            printf("------------------\n");
            printf(">>> BOOK FOUND <<<\n");
            printf("------------------\n\n");

            //lists data of the searched book
            printf("\n--------------------------------------------------\n");
            printf("> ID: %s \n",book.Book_Number);
            printf("> Name: %s \n",book.Book_Name);
            printf("> Author: %s \n ", book.Author_Name);
            printf("> Year of publication : %s \n ",book.year_publication);
            printf("> Price: %s \n",book.price);
            printf("> Rack No: %s \n",book.rack);
            printf("\n--------------------------------------------------\n");
            findBook = 1;
        }
    }
    fclose(fp);

    if(!findBook){  //checks whether conditiion enters inside loop or not
        printf(">>> BOOK NOT FOUND <<<\n");
    }

    printf("> Try another search [Y/N]: ");
    if(getch()=='y')
        search_book();
    else
        user_menu();
}

//search book by name for user
void searchByName(){
    system("cls");

    printf("\n\n");
    printf("---------------------------\n");
    printf(">>> SEARCH BOOK BY NAME <<<\n");
    printf("---------------------------\n\n");

    char s[50];
    int d=0;
    FILE *fp;

    //takes book name
    printf("> Enter Book Name:");
    scanf(" %[^\n]", s);

    fp = fopen("book.txt","rb");

    while(fread(&book,sizeof(book),1,fp)){
        if(!strcmp(book.Book_Name,(s))){
            //checks whether book.name is equal to s or not
            printf("\n\n");
            printf("------------------\n");
            printf(">>> BOOK FOUND <<<\n");
            printf("------------------\n\n");

            //list data of the searched book
            printf("\n-------------------------------------------------\n");
            printf("> ID: %s \n",book.Book_Number);
            printf("> Name: %s \n",book.Book_Name);
            printf("> Author: %s \n",book.Author_Name);
            printf("> Year of publication : %s \n",book.year_publication );
            printf("> Price: %s \n",book.price);
            printf("> Rack No: %s \n",book.rack);
            printf("\n--------------------------------------------------\n");
            d=1;
        }
    }
    fclose(fp);

    if(d==0){
        printf(">>> BOOK NOT FOUND <<<\n\n");
    }
    
    //retry screen
    printf("> Try another search [Y/N]: ");
    if(getch()=='y')
        search_book();
    else
        user_menu();
}

//shows book issued by the logged in user
void view_issueinfo(char uid[]){
    system("cls");

    printf("\n\n");
    printf("------------------------------\n");
    printf(">>> SHOW ISSUE INFORMATION <<<\n");
    printf("------------------------------\n\n");

    int found=0;
    int i=0;

    //opens issue and user_records file
    FILE *fptr;
    FILE *ptr;
    fptr =fopen("issue.txt","rb+");
    ptr =fopen("user_Records.txt","rb");
    while(fread(&user,sizeof(user),1,ptr)==1){
        if(!strcmp(user.userId,uid)){
            found=1;
        }
    }
    if(found){
        while(fread(&book,sizeof(struct library),1,fptr)){
            if(!strcmp(book.issue_to, uid)){
                //lists all the issued books
                printf("\n-----------------------------------------\n\n");
                printf("> Book name: %s \n",book.Book_Name);
                printf("> Book number: %s \n",book.Book_Number);
                printf("> Author name: %s \n",book.Author_Name);
                printf("> User Id: %s \n",book.issue_to);
                printf("> Issue date: %s \n",book.issue_date);
                printf("> Due date: %s \n",book.duedate);
                printf("\n------------------------------------------\n\n");
                i=1;
            }
        }
    }
    fclose(ptr);
    fclose(fptr);
    if(!i){
        printf(">>> NO BOOK ISSUED <<<\n\n");
    }
    fflush(stdin);
    printf("Press any key to continue......");
    getch();
    user_menu();
}

//caluculates due date
void due_date(int days, struct tm *date){
    time_t time = mktime(date); // Convert struct tm to time_t
    time += days * 24 * 60 * 60; // Add 'days' seconds to current time
    *date = *localtime(&time); // Convert time_t back to struct tm
}

//shows issued book by all users for admin
void issueinfo(){
    system("cls");

    printf("\n\n");
    printf("------------------------------\n");
    printf(">>> SHOW ISSUE INFORMATION <<<\n");
    printf("------------------------------\n\n");

    //opens issue file
    FILE *fptr;
    fptr =fopen("issue.txt","rb+");
    int found=0;

    while(fread(&book,sizeof(struct library),1,fptr)){
        //list data for all the issued books
        printf("\n-----------------------------------------\n\n");
        printf("> Book name: %s \n",book.Book_Name);
        printf("> Book number: %s \n",book.Book_Number);
        printf("> Author name: %s \n",book.Author_Name);
        printf("> User Id: %s \n",book.issue_to);
        printf("> Issue date: %s \n",book.issue_date);
        printf("> Due date: %s \n",book.duedate);
        printf("\n------------------------------------------\n\n");
        found=1;
    }

    fclose(fptr);

    if(!found){
        printf(">>> NO BOOK ISSUED <<<\n\n");
    }
    
    fflush(stdin);
    printf("Press any key to continue......");
    getch();
    admin_menu();
}