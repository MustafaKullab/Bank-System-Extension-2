#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

const string FileName = "Client.txt";
const string UsersFileName = "Users.txt";

void login();
void ShowMainMenue();
void ShowTransactionsScreen();
void ShowManageUsersScreen();
void GoToMainMenueScreen();
struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    bool MarkDelete = false;
    double AccountBalance;
};

struct stUser
{
    string Username;
    string Password;
    int Permissions;
    bool MarkDelete = false;
};

stUser CurrentUser;

enum enMainMenueOptions
{
    eShowClients = 1,
    eAddClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
    eManageUsers = 7,
    eLogout = 8
};

enum enTransactionOptions
{
    eDeposit = 1,
    eWithDraw = 2,
    eTotalBalance = 3,
    eMainMenue = 4
};

enum enManageUsersOptions
{
    eListUsers = 1,
    eAddUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenueUser = 6,
};

enum enPermissions
{
    pAll = -1,
    pListClients = 1,
    pAddClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64,
};

short ReadMainMenueOption()
{
    short Option;
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> Option;
    return Option;
}

vector<string> SplitString(string line, string separator)
{
    vector<string> vStr;
    string stWord = "";
    short pos = 0;

    while ((pos = line.find(separator)) != std::string::npos)
    {
        stWord = line.substr(0, pos);

        if (stWord != "")
        {
            vStr.push_back(stWord);
        }

        line.erase(0, pos + separator.length());
    }

    if (line != "")
    {
        vStr.push_back(line);
    }

    return vStr;
}

stClient ConvertLineToRecored(string line, string separator = "#//#")
{
    stClient Client;
    vector<string> vStrClient = SplitString(line, separator);

    Client.AccountNumber = vStrClient[0];
    Client.PinCode = vStrClient[1];
    Client.Name = vStrClient[2];
    Client.Phone = vStrClient[3];
    Client.AccountBalance = stod(vStrClient[4]);

    return Client;
}

vector<stClient> LoadClientsFromFile()
{
    fstream MyFile;
    vector<stClient> vClients;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {

        string line;
        stClient Client;
        while (getline(MyFile, line))
        {
            Client = ConvertLineToRecored(line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void PrintClientLine(stClient Client)
{
    cout << " | " << setw(15) << left << Client.AccountNumber;
    cout << " | " << setw(12) << left << Client.PinCode;
    cout << " | " << setw(35) << left << Client.Name;
    cout << " | " << setw(15) << left << Client.Phone;
    cout << " | " << setw(15) << left << to_string(Client.AccountBalance);
}

bool CheckAccessPermission(enPermissions Permissions)
{
    if (CurrentUser.Permissions == enPermissions::pAll)
        return true;

    if ((Permissions & CurrentUser.Permissions) == Permissions)
        return true;
    else
        return false;
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void ShowClientsScreen()
{

    if (!CheckAccessPermission(enPermissions::pListClients))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    vector<stClient> vClients = LoadClientsFromFile();
    cout << "\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "____________________________________________________________";
    cout << "_____________________________________________\n\n";
    cout << " | " << setw(15) << left << "Account Number";
    cout << " | " << setw(12) << left << "Pin Code";
    cout << " | " << setw(35) << left << "Client Name";
    cout << " | " << setw(15) << left << "Phone";
    cout << " | " << setw(15) << left << "Balance";
    cout << "\n____________________________________________________________";
    cout << "_____________________________________________\n\n";

    if (vClients.size() == 0)
        cout << "No Clients In The system!\n";
    else
        for (stClient Client : vClients)
        {
            PrintClientLine(Client);
            cout << endl;
        }
    cout << "____________________________________________________________";
    cout << "_____________________________________________\n\n";
}

void GoToMainMenueScreen()
{
    cout << "\nPress any key to go back to Main Menue...\n";
    system("pause>0");
    system("cls");
    ShowMainMenue();
}

bool ClientIsExistByAccountNumber(string AccountNumber, vector<stClient> vClients)
{

    for (stClient Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            return true;
        }
    }

    return false;
}

string ConverRecordToString(stClient Client, string separator = "#//#")
{
    string line;
    line += Client.AccountNumber + separator;
    line += Client.PinCode + separator;
    line += Client.Name + separator;
    line += Client.Phone + separator;
    line += to_string(Client.AccountBalance);

    return line;
}

void SaveClientsInFile(vector<stClient> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {

        for (stClient Client : vClients)
        {
            if (Client.MarkDelete == false)
            {

                string line = ConverRecordToString(Client);
                MyFile << line << endl;
            }
        }

        MyFile.close();
    }
}

void AddClientToFile(string line)
{

    fstream MyFile;

    MyFile.open(FileName, ios::app);
    if (MyFile.is_open())
    {
        MyFile << line << endl;

        MyFile.close();
    }
}

stClient ReadNewClient()
{
    stClient Client;
    vector<stClient> vClients = LoadClientsFromFile();

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientIsExistByAccountNumber(Client.AccountNumber, vClients))
    {
        cout << "Client with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pin Code? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddNewClient()
{
    stClient Client = ReadNewClient();

    AddClientToFile(ConverRecordToString(Client));
}

void AddNewClients()
{
    char Answer = 'n';

    do
    {
        cout << "Adding New Client: \n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more Clients? Y/N? \n";
        cin >> Answer;
    } while (Answer == 'Y' || Answer == 'y');
}

void ShowAddClientScreen()
{

    if (!CheckAccessPermission(enPermissions::pAddClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    cout << "==========================================================\n";
    cout << "\t\tAdd New Clients Screen\n";
    cout << "==========================================================\n";

    AddNewClients();
}

string ReadAccountNumber()
{
    string AccountNumber;
    cout << "Please enter AccountNumber? ";
    getline(cin >> ws, AccountNumber);
    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient &Client)
{

    for (stClient &C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

void PrintClientCard(stClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "Account Number          : " << Client.AccountNumber << endl;
    cout << "Pin Code                : " << Client.PinCode << endl;
    cout << "Name                    : " << Client.Name << endl;
    cout << "Phone                   : " << Client.Phone << endl;
    cout << "Account Balance         : " << to_string(Client.AccountBalance) << endl;
}

void MarkClientForDelete(vector<stClient> &vClients, string AccountNumber)
{

    for (stClient &Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client.MarkDelete = true;
        }
    }
}

void DeleteClient(string AccountNumber, vector<stClient> &vClients)
{
    char Answer = 'n';
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        char SureDelete = 'n';
        cout << "\nAre you sure you want delete this client? Y/N ? ";
        cin >> SureDelete;

        if (SureDelete == 'y' || SureDelete == 'Y')
        {

            MarkClientForDelete(vClients, AccountNumber);

            SaveClientsInFile(vClients);

            vClients = LoadClientsFromFile();

            cout << "\nClient Deleted Successfully.\n\n";
        }
    }
    else
    {
        cout << "Client with Account Number (" << AccountNumber << ") is Not Found!\n";
    }
}

void ShowDeleteClientScreen()
{

    if (!CheckAccessPermission(enPermissions::pDeleteClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    cout << "==========================================================\n";
    cout << "\t\tDelete Client Screen\n";
    cout << "==========================================================\n";

    vector<stClient> vClients = LoadClientsFromFile();
    string AccountNumber = ReadAccountNumber();
    DeleteClient(AccountNumber, vClients);
}

stClient ChangeClient(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone? ";
    getline(cin >> ws, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClient(string AccountNumber, vector<stClient> &vClients)
{

    char Answer = 'n';
    stClient Client;

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "Are you sure you want update this client? Y/N ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {

            for (stClient &c : vClients)
            {

                if (c.AccountNumber == AccountNumber)
                {
                    c = ChangeClient(AccountNumber);
                    break;
                }
            }

            SaveClientsInFile(vClients);

            vClients = LoadClientsFromFile();

            cout << "\nClient Updated Successfully.\n\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n\n";
    }
}

void ShowUpdateClientScreen()
{

    if (!CheckAccessPermission(enPermissions::pUpdateClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    cout << "==========================================================\n";
    cout << "\t\tUpdate Client Screen\n";
    cout << "==========================================================\n";

    vector<stClient> vClients = LoadClientsFromFile();
    string AccountNumber = ReadAccountNumber();
    UpdateClient(AccountNumber, vClients);
}

void FindClient(string AccountNumber, vector<stClient> vClients)
{

    bool FoundClient = false;

    for (stClient Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            PrintClientCard(Client);
            FoundClient = true;
            break;
        }
    }

    if (!FoundClient)
        cout << "\nClient with Account Number [" << AccountNumber << "] is Not Found!\n";
}

void ShowFindClientScreen()
{

    if (!CheckAccessPermission(enPermissions::pFindClient))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    cout << "==========================================================\n";
    cout << "\t\tFind Client Screen\n";
    cout << "==========================================================\n";

    vector<stClient> vClients = LoadClientsFromFile();
    string AccountNumber = ReadAccountNumber();
    FindClient(AccountNumber, vClients);
}

void ShowEndProgramScreen()
{
    cout << "==========================================================\n";
    cout << "\t\tProgram Ends :-)\n";
    cout << "==========================================================\n";
}

short ReadTransactionOption()
{
    short Option;
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> Option;
    return Option;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClient> &vClients, double Amount)
{
    char Answer = 'n';

    cout << "Are you sure you want perform this transaction? Y/N ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient &c : vClients)
        {
            if (c.AccountNumber == AccountNumber)
            {
                c.AccountBalance += Amount;
                SaveClientsInFile(vClients);
                cout << "\nDone Successfully. New balance is: " << c.AccountBalance << endl;
                return true;
            }
        }
    }

    return false;
}

void ShowDepositScreen()
{
    cout << "============================================\n";
    cout << "\t\tDeposit Screen \n";
    cout << "============================================\n";

    stClient Client;
    vector<stClient> vClients = LoadClientsFromFile();
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Client with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientCard(Client);

    double DepositAmount;
    cout << "Please enter deposit amount? ";
    cin >> DepositAmount;

    while (DepositAmount <= 0)
    {

        cout << "Amount is not valid, Please enter amount grather than 0? ";
        cin >> DepositAmount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, vClients, DepositAmount);
}

void ShowWithdrawScreen()
{
    cout << "============================================\n";
    cout << "\t\tWithdraw Screen \n";
    cout << "============================================\n";

    stClient Client;
    vector<stClient> vClients = LoadClientsFromFile();
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "Client with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientCard(Client);

    double WithdrawAmount;
    cout << "Please enter withdraw amount? ";
    cin >> WithdrawAmount;

    while (WithdrawAmount <= 0 || WithdrawAmount > Client.AccountBalance)
    {
        cout << "Amount is not valid, Please enter amount grather than 0 And less than balance? ";
        cin >> WithdrawAmount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, vClients, WithdrawAmount * -1);
}

void PrintClientBalanceLine(stClient Client)
{
    cout << "| " << setw(25) << left << Client.AccountNumber;
    cout << "| " << setw(60) << left << Client.Name;
    cout << "| " << setw(25) << left << Client.AccountBalance;
}

void BackToTransactionMenue()
{
    cout << "\nPress any key to go back to Transaction Menue...\n";
    system("pause>0");
    ShowTransactionsScreen();
}

double CalculateTotalBalance(vector<stClient> vClients)
{

    double TotalBalance = 0;
    for (stClient Client : vClients)
    {
        TotalBalance += Client.AccountBalance;
    }

    return TotalBalance;
}

void ShowTotalBalanceScreen()
{
    vector<stClient> vClients = LoadClientsFromFile();

    cout << "\t\t\t\t\t\t\tBalance List (" << vClients.size() << ") Client(s)\n";
    cout << "_________________________________________________________________________";
    cout << "______________________________________________________________\n\n";
    cout << "| " << setw(25) << left << "Account Number";
    cout << "| " << setw(60) << left << "Client Name";
    cout << "| " << setw(25) << left << "Balance";
    cout << "\n_________________________________________________________________________";
    cout << "______________________________________________________________\n\n";

    for (stClient Client : vClients)
    {
        PrintClientBalanceLine(Client);
        cout << endl;
    }

    cout << "\n_________________________________________________________________________";
    cout << "______________________________________________________________\n\n";

    double TotalBalance = CalculateTotalBalance(vClients);

    cout << "\t\t\t\t\t\t\t\t\t Total Balance = " << TotalBalance << endl
         << endl;
}

void PerformTransactionsOption(enTransactionOptions TransactionsOption)
{
    switch (TransactionsOption)
    {
    case enTransactionOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        BackToTransactionMenue();
        break;
    case enTransactionOptions::eWithDraw:
        system("cls");
        ShowWithdrawScreen();
        BackToTransactionMenue();
        break;
    case enTransactionOptions::eTotalBalance:
        system("cls");
        ShowTotalBalanceScreen();
        BackToTransactionMenue();
        break;
    case enTransactionOptions::eMainMenue:
        ShowMainMenue();
        break;
    default:
        cout << "Invalid option, please choose from 1 to 4.\n\n";
        ShowTransactionsScreen();
        break;
    }
}

void ShowTransactionsScreen()
{

    if (!CheckAccessPermission(enPermissions::pTransactions))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    cout << "==========================================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "==========================================================\n";
    cout << "\t\t[1] Deposit.\n";
    cout << "\t\t[2] Withdraw.\n";
    cout << "\t\t[3] Total Balances.\n";
    cout << "\t\t[4] Main Menue.\n";
    cout << "==========================================================\n";
    PerformTransactionsOption((enTransactionOptions)ReadTransactionOption());
}

short ReadManageUsersOption()
{
    short Option;

    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> Option;

    return Option;
}

void PrintUser(stUser User)
{
    cout << "| " << left << setw(20) << User.Username;
    cout << "| " << left << setw(20) << User.Password;
    cout << "| " << left << setw(20) << to_string(User.Permissions);
}

stUser ConvertUserLineToRecord(string Line, string Separator = "#//#")
{
    stUser User;
    vector<string> vUserString;

    vUserString = SplitString(Line, Separator);

    User.Username = vUserString[0];
    User.Password = vUserString[1];
    User.Permissions = stoi(vUserString[2]);

    return User;
}

vector<stUser> LoadUsersFromFile(string FileName)
{

    vector<stUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in); // Read Mode

    if (MyFile.is_open())
    {
        string line;
        stUser User;

        while (getline(MyFile, line))
        {
            User = ConvertUserLineToRecord(line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }

    return vUsers;
}

void ShowListUsers()
{
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    cout << "\t\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).\n";
    cout << "_____________________________________________________________";
    cout << "_____________________________________________________________\n\n";
    cout << "| " << left << setw(20) << "User Name";
    cout << "| " << left << setw(20) << "Password";
    cout << "| " << left << setw(20) << "Premissions";
    cout << "\n_____________________________________________________________";
    cout << "_____________________________________________________________\n\n";

    if (vUsers.size() == 0)
        cout << "No Users Found .\n";

    for (stUser &User : vUsers)
    {
        PrintUser(User);
        cout << endl;
    }
}

bool FindUserByUsername(string Username, string FileName, stUser &User)
{
    vector<stUser> vUsers = LoadUsersFromFile(FileName);

    for (stUser &U : vUsers)
    {
        if (U.Username == Username)
        {
            User = U;
            return true;
        }
    }

    return false;
}

int AddPermissions()
{

    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pListClients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pAddClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pUpdateClient;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pTransactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions = Permissions | enPermissions::pManageUsers;
    }

    return Permissions;
}

stUser ReadUser()
{
    stUser User;

    cout << "Enter Username? ";
    getline(cin >> ws, User.Username);

    while (FindUserByUsername(User.Username, UsersFileName, User))
    {
        cout << "User with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = AddPermissions();

    return User;
}

string ConvertUserRecordToLine(stUser User, string Separator = "#//#")
{
    string StringUser = "";

    StringUser += User.Username + Separator;
    StringUser += User.Password + Separator;
    StringUser += to_string(User.Permissions);

    return StringUser;
}

void AddUserToFile(string User, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::app);

    if (MyFile.is_open())
    {
        MyFile << User << endl;

        MyFile.close();
    }
}

void AddNewUsers()
{
    char AddMore = 'n';
    stUser User;

    do
    {
        User = ReadUser();
        AddUserToFile(ConvertUserRecordToLine(User), UsersFileName);

        cout << "\nUser Addedd Successfully, do you want to add more Users? y/n? ";
        cin >> AddMore;

    } while (AddMore == 'y' || AddMore == 'Y');
}

void ShowAddUserScreen()
{
    cout << "---------------------------------------------------\n";
    cout << "\t\tAdd New User Screen\n";
    cout << "---------------------------------------------------\n";

    AddNewUsers();
}

void GoBackToManageUserMenue()
{
    cout << "\nPress any key to go back to Main Menue...\n";
    system("pause>0");
    system("cls");
    ShowManageUsersScreen();
}

string ReadUserName()
{
    string Username;
    cout << "Please enter Username? ";
    getline(cin >> ws, Username);
    return Username;
}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details: \n";
    cout << "-----------------------------------\n";
    cout << "Username : " << User.Username << endl;
    cout << "Password : " << User.Password << endl;
    cout << "Permissions : " << to_string(User.Permissions) << endl;
    cout << "\n-----------------------------------\n";
}

void MarkUserToDelete(vector<stUser> &vUsers, string Username)
{

    for (stUser &User : vUsers)
    {
        if (User.Username == Username)
        {
            User.MarkDelete = true;
        }
    }
}

void SaveUsersInFile(vector<stUser> &vUsers, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out); // Replace Users

    if (MyFile.is_open())
    {
        string line;

        for (stUser User : vUsers)
        {
            if (User.MarkDelete == false)
            {
                line = ConvertUserRecordToLine(User);
                MyFile << line << endl;
            }
        }

        MyFile.close();
    }
}

bool DeleteUser(vector<stUser> &vUsers, string Username)
{
    if (Username == "Admin")
    {
        cout << "Can't Delete this User.\n";
        return false;
    }

    char Answer = 'n';
    stUser User;

    if (FindUserByUsername(Username, UsersFileName, User))
    {
        PrintUserCard(User);

        cout << "\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkUserToDelete(vUsers, Username);
            SaveUsersInFile(vUsers, UsersFileName);

            vUsers = LoadUsersFromFile(UsersFileName);
            cout << "\nUser Deleted Successfully. \n";
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        cout << "User with username [" << Username << "] is not found.\n";
        return false;
    }
}

void ShowDeleteUserScreen()
{
    cout << "-----------------------------------\n";
    cout << "\tDelete Users Screen\n";
    cout << "-----------------------------------\n";

    string Username = ReadUserName();
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    DeleteUser(vUsers, Username);
}

void UpdateSpecificUser(vector<stUser> &vUsers, string Username)
{
    for (stUser &User : vUsers)
    {
        if (User.Username == Username)
        {
            cout << "Enter Password? ";
            getline(cin >> ws, User.Password);

            User.Permissions = AddPermissions();
        }
    }
}

bool UpdateUser(vector<stUser> &vUsers, string Username)
{
    char Answer = 'n';
    stUser User;

    if (FindUserByUsername(Username, UsersFileName, User))
    {
        PrintUserCard(User);

        cout << "\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {

            UpdateSpecificUser(vUsers, Username);
            SaveUsersInFile(vUsers, UsersFileName);

            vUsers = LoadUsersFromFile(UsersFileName);
            cout << "\nUser Updated Successfully. \n";
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        cout << "User with username [" << Username << "] is not found.\n";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "-----------------------------------\n";
    cout << "\tUpdate Users Screen\n";
    cout << "-----------------------------------\n";

    string Username = ReadUserName();
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    UpdateUser(vUsers, Username);
}

void ShowFindUserScreen()
{
    cout << "-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    string Username = ReadUserName();
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);
    stUser User;

    if (FindUserByUsername(Username, UsersFileName, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "\nUser with username [" << Username << "] Is not found.\n";
    }
}

void PerformManageUsersOption(enManageUsersOptions ManageUserOption)
{
    switch (ManageUserOption)
    {
    case enManageUsersOptions::eListUsers:
        system("cls");
        ShowListUsers();
        GoBackToManageUserMenue();
        break;
    case enManageUsersOptions::eAddUser:
        system("cls");
        ShowAddUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUserMenue();
        break;
    case enManageUsersOptions::eMainMenueUser:
        system("cls");
        ShowMainMenue();
        break;
    }
}

void ShowManageUsersScreen()
{

    if (!CheckAccessPermission(enPermissions::pManageUsers))
    {
        system("cls");
        ShowAccessDeniedMessage();
        GoToMainMenueScreen();
        return;
    }

    system("cls");

    cout << "==========================================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "==========================================================\n";
    cout << "\t\t\t[1] List Users.\n";
    cout << "\t\t\t[2] Add New User.\n";
    cout << "\t\t\t[3] Delete User.\n";
    cout << "\t\t\t[4] Update User.\n";
    cout << "\t\t\t[5] Find User.\n";
    cout << "\t\t\t[6] Main Menue.\n";
    cout << "==========================================================\n";

    PerformManageUsersOption(enManageUsersOptions(ReadManageUsersOption()));
}

void PerfoemMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eShowClients:
        system("cls");
        ShowClientsScreen();
        GoToMainMenueScreen();
        break;
    case enMainMenueOptions::eAddClient:
        system("cls");
        ShowAddClientScreen();
        GoToMainMenueScreen();
        break;
    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoToMainMenueScreen();
        break;
    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoToMainMenueScreen();
        break;
    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoToMainMenueScreen();
        break;
    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransactionsScreen();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersScreen();
        break;
    case enMainMenueOptions::eLogout:
        system("cls");
        login();
        break;
    }
}

void ShowMainMenue()
{
    cout << "================================================================\n";
    cout << "\t\t\t Main Menue Screen\n";
    cout << "================================================================\n";
    cout << "\t\t [1] Show Client List.\n";
    cout << "\t\t [2] Add New Client.\n";
    cout << "\t\t [3] Delete Client.\n";
    cout << "\t\t [4] Update Client Info.\n";
    cout << "\t\t [5] Find Client.\n";
    cout << "\t\t [6] Transactions.\n";
    cout << "\t\t [7] Manage Users.\n";
    cout << "\t\t [8] Logout.\n";
    cout << "================================================================\n";
    PerfoemMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool FindUserByUsernameAndPassword(string Username, string Password, stUser &User)
{
    vector<stUser> vUsers = LoadUsersFromFile(UsersFileName);

    for (stUser &U : vUsers)
    {
        if (U.Username == Username && U.Password == Password)
        {
            User = U;
            return true;
        }
    }

    return false;
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void login()
{
    bool LoginFailed = false;
    string Username, Password;

    do
    {

        system("cls");

        cout << "---------------------------------\n";
        cout << "\tLogin Screen\n";
        cout << "---------------------------------\n";

        if (LoginFailed)
        {
            cout << "Invalid Username/Password\n";
        }

        cout << "Enter Username? ";
        getline(cin >> ws, Username);

        cout << "Enter Password? ";
        getline(cin >> ws, Password);

        LoginFailed = !LoadUserInfo(Username, Password);
    } while (LoginFailed);

    system("cls");
    ShowMainMenue();
}

int main()
{
    login();
}