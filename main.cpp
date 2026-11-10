#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

const string FileName = "Client.txt";
short PermissionUser = 0;

void ShowMainMenue(short Permissions);
void ShowTransactionsScreen();
void login();
void ShowManageUsersScreen();
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
    short permissions = 0;
    bool MarkDelete = false;
};

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
    eMainMenueScreen = 6,
};

enum enPremissions
{
    eFullPremissionsPer = -1,
    eClientListPer = 1,
    eAddClientPer = 2,
    eDeleteClientPer = 4,
    eUpdateClientPer = 8,
    eFindClientPer = 16,
    eTransactionPer = 32,
    eManageUsersPer = 64,

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
    vector<string> vStrClient;
    string stWord = "";
    short pos = 0;

    while ((pos = line.find(separator)) != std::string::npos)
    {
        stWord = line.substr(0, pos);

        if (stWord != "")
        {
            vStrClient.push_back(stWord);
        }

        line.erase(0, pos + separator.length());
    }

    if (line != "")
    {
        vStrClient.push_back(line);
    }

    return vStrClient;
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

void ShowClientsScreen()
{
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
    ShowMainMenue(PermissionUser);
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
        ShowMainMenue(PermissionUser);
        break;
    default:
        cout << "Invalid option, please choose from 1 to 4.\n\n";
        ShowTransactionsScreen();
        break;
    }
}

void ShowTransactionsScreen()
{
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

stUser ConvertLineToRecoredUser(string line, string separator = "#//#")
{
    stUser User;
    vector<string> vStrUser = SplitString(line, separator);

    User.Username = vStrUser[0];
    User.Password = vStrUser[1];
    User.permissions = stoi(vStrUser[2]);

    return User;
}

vector<stUser> LoadUsersFromFile()
{
    vector<stUser> vUsers;
    fstream MyFile;

    MyFile.open("Users.txt", ios::in);

    if (MyFile.is_open())
    {

        string line = "";
        stUser User;
        while (getline(MyFile, line))
        {
            User = ConvertLineToRecoredUser(line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }
    return vUsers;
}

void PrintUserCard(stUser User)
{
    cout << "| " << setw(20) << left << User.Username;
    cout << "| " << setw(20) << left << User.Password;
    cout << "| " << setw(20) << left << User.permissions;
}

void ShowListUsersScreen()
{
    vector<stUser> vUsers = LoadUsersFromFile();

    cout << "\t\t\t\t\t\t Users List (" << vUsers.size() << ") User(s).\n";

    cout << "_________________________________________________";
    cout << "_________________________________________________________________________\n\n";
    cout << "| " << setw(20) << left << "User Name";
    cout << "| " << setw(20) << left << "Password";
    cout << "| " << setw(20) << left << "Permissions";
    cout << "\n_________________________________________________";
    cout << "_________________________________________________________________________\n\n";

    for (stUser User : vUsers)
    {
        PrintUserCard(User);
        cout << endl;
    }

    cout << "_________________________________________________";
    cout << "_________________________________________________________________________\n";
}

void BackToManageUsersScreen()
{
    cout << "\n\n\nPress any key to go back to Manage Users Screen...";
    system("pause>0");
    system("cls");
    ShowManageUsersScreen();
}

string ConvertUserRecordToString(stUser User, string Separator = "#//#")
{
    string UserString = "";
    UserString += User.Username + Separator;
    UserString += User.Password + Separator;
    UserString += to_string(User.permissions);

    return UserString;
}

void AddUsersToFile(vector<stUser> vUsers)
{
    fstream MyFile;
    MyFile.open("Users.txt", ios::out);

    if (MyFile.is_open())
    {
        string line = "";
        for (stUser &User : vUsers)
        {
            if (User.MarkDelete == false)
            {

                line = ConvertUserRecordToString(User);
                MyFile << line << endl;
            }
        }
        MyFile.close();
    }
}

bool IsUsernameExist(vector<stUser> vUsers, string Username, stUser &UserObj)
{
    for (stUser &User : vUsers)
    {
        if (User.Username == Username)
        {
            UserObj = User;
            return true;
        }
    }

    return false;
}

void AddPermissions(stUser &User)
{

    char Answer = 'n';
    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        User.permissions = enPremissions::eFullPremissionsPer;
        cout << "User Permission : " << User.permissions << endl;
        cout << "Enum Permission : " << enPremissions::eFullPremissionsPer << endl;
    }
    else
    {
        cout << "\nDo you want to give access to : \n"
             << endl;

        cout << "Show Client List? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eClientListPer;
        }

        cout << "\nAdd New Client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eAddClientPer;
        }

        cout << "\nDelete Client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eDeleteClientPer;
        }

        cout << "\nUpdate Client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eUpdateClientPer;
        }

        cout << "\nFind Client? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eFindClientPer;
        }

        cout << "\nTransactions? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eTransactionPer;
        }

        cout << "\nManage Users? y/n? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            User.permissions = User.permissions | enPremissions::eManageUsersPer;
        }
    }
}

void AddNewUser(vector<stUser> &vUsers)
{
    stUser User;
    string Username = "";
    bool UserExist = false;

    do
    {
        cout << "Enter Username? ";
        getline(cin >> ws, Username);

        UserExist = IsUsernameExist(vUsers, Username, User);

        if (UserExist)
        {
            cout << "User with [" << User.Username << "] already exists, Enter another Username? ";
        }
    } while (UserExist);

    User.Username = Username;
    User.permissions = 0;

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    AddPermissions(User);

    vUsers.push_back(User);

    AddUsersToFile(vUsers);
}

void ShowAddNewUserScreen()
{
    cout << "---------------------------------------------------\n";
    cout << "\t\tAdd New User Screen\n";
    cout << "---------------------------------------------------\n";
    cout << "Adding New User:\n";

    vector<stUser> vUsers = LoadUsersFromFile();
    char AddMore = 'n';

    do
    {

        AddNewUser(vUsers);
        cout << "User Addedd Successfully, do you want to add more Users? y/n? ";
        cin >> AddMore;
    } while (AddMore == 'y' || AddMore == 'Y');
}

void PrintUser(stUser User)
{
    cout << "---------------------------------------------------\n";
    cout << "Username     : " << User.Username << endl;
    cout << "Password     : " << User.Password << endl;
    cout << "Permissions  : " << to_string(User.permissions) << endl;
    cout << "---------------------------------------------------\n";
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

bool DeleteUser(vector<stUser> &vUsers, string Username)
{

    stUser User;
    char SureDelete = 'n';

    bool IsUserExist = IsUsernameExist(vUsers, Username, User);

    if (!IsUserExist)
    {
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
        return false;
    }
    else
    {
        cout << "\nThe following are the user to delete: \n";
        PrintUser(User);

        cout << "\n\nAre you sure you want delete this User? y/n? ";
        cin >> SureDelete;

        if (SureDelete == 'y' || SureDelete == 'Y')
        {
            MarkUserToDelete(vUsers, User.Username);
            AddUsersToFile(vUsers);
            return true;
        }
        else
        {
            return false;
        }
    }
}

void ShowDeleteUserScreen()
{
    cout << "---------------------------------------------------\n";
    cout << "\t\tDelete User Screen\n";
    cout << "---------------------------------------------------\n";

    vector<stUser> vUsers = LoadUsersFromFile();
    string Username = "";

    cout << "\nPlease Enter Username? ";
    getline(cin >> ws, Username);

    if (Username == "Admin")
    {
        cout << "\nYou can't Delete This User .\n";
        return;
    }

    bool deleted = DeleteUser(vUsers, Username);

    if (deleted)
    {

        cout << "\nUser Deleted Successfully.\n";
    }
}

bool UpdateUserFromList(vector<stUser> &vUsers, string Username)
{

    char SureUpdate = 'n';

    cout << "\n\nAre you sure you want update this User? y/n? ";
    cin >> SureUpdate;

    if (SureUpdate == 'y' || SureUpdate == 'Y')
    {
        for (stUser &User : vUsers)
        {

            if (User.Username == Username)
            {

                cout << "Enter Password? ";
                getline(cin >> ws, User.Password);

                User.permissions = 0;

                AddPermissions(User);
            }
        }
        return true;
    }
    return false;
}

bool UpdateUser(vector<stUser> &vUsers)
{

    stUser User;
    string Username = "";

    cout << "\nPlease Enter Username? ";
    getline(cin >> ws, Username);

    if (IsUsernameExist(vUsers, Username, User))
    {
        cout << "\nThe following are the user to Update: \n";
        PrintUser(User);

        bool IsUpdated = UpdateUserFromList(vUsers, Username);
        return IsUpdated;
    }
    else
    {
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "---------------------------------------------------\n";
    cout << "\t\tUpdate Users Screen\n";
    cout << "---------------------------------------------------\n";

    vector<stUser> vUsers = LoadUsersFromFile();

    bool Updated = UpdateUser(vUsers);

    if (Updated == true)
    {
        AddUsersToFile(vUsers);
        cout << "\nUser Updated Successfully.\n";
    }
}

void FindUser(vector<stUser> vUsers)
{
    string Username = "";
    stUser User;

    cout << "\nPlease Enter Username? ";
    getline(cin >> ws, Username);

    if (IsUsernameExist(vUsers, Username, User))
    {
        cout << "\nThe following are the user : \n";
        PrintUser(User);
    }
    else
    {
        cout << "\nUser with Username [" << Username << "] is Not Found!\n";
    }
}

void ShowFindUserScreen()
{
    cout << "---------------------------------------------------\n";
    cout << "\t\tFind User Screen\n";
    cout << "---------------------------------------------------\n";

    vector<stUser> vUsers = LoadUsersFromFile();

    FindUser(vUsers);
}

void PerformManageUsersOption(enManageUsersOptions ManageUsersOption)
{
    switch (ManageUsersOption)
    {
    case enManageUsersOptions::eListUsers:
        system("cls");
        ShowListUsersScreen();
        BackToManageUsersScreen();
        break;
    case enManageUsersOptions::eAddUser:
        system("cls");
        ShowAddNewUserScreen();
        BackToManageUsersScreen();
        break;
    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        BackToManageUsersScreen();
        break;
    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        BackToManageUsersScreen();
        break;
    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        BackToManageUsersScreen();
        break;
    case enManageUsersOptions::eMainMenueScreen:
        system("cls");
        ShowMainMenue(PermissionUser);
        break;
    }
}

void ShowManageUsersScreen()
{
    cout << "==========================================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "==========================================================\n";
    cout << "\t\t[1] List Users.\n";
    cout << "\t\t[2] Add New User.\n";
    cout << "\t\t[3] Delete User.\n";
    cout << "\t\t[4] Update User.\n";
    cout << "\t\t[5] Find User.\n";
    cout << "\t\t[6] Main Menue.\n";
    cout << "==========================================================\n";
    PerformManageUsersOption((enManageUsersOptions)ReadManageUsersOption());
}

void ShowAccessDeniedScreen()
{
    cout << "----------------------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You dont Have Permission To Do this,\n";
    cout << "Please Concat Your Admin.\n";
    cout << "----------------------------------------------------\n";
}

void PerfoemMainMenueOption(enMainMenueOptions MainMenueOption, short PermissionUser)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eShowClients:
        if (((PermissionUser & enPremissions::eClientListPer) == enPremissions::eClientListPer))
        {
            system("cls");
            ShowClientsScreen();
            GoToMainMenueScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eAddClient:
        if (((PermissionUser & enPremissions::eAddClientPer) == enPremissions::eAddClientPer))
        {

            system("cls");
            ShowAddClientScreen();
            GoToMainMenueScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eDeleteClient:
        if (((PermissionUser & enPremissions::eDeleteClientPer) == enPremissions::eDeleteClientPer))
        {

            system("cls");
            ShowDeleteClientScreen();
            GoToMainMenueScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eUpdateClient:
        if (((PermissionUser & enPremissions::eUpdateClientPer) == enPremissions::eUpdateClientPer))
        {

            system("cls");
            ShowUpdateClientScreen();
            GoToMainMenueScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eFindClient:
        if (((PermissionUser & enPremissions::eFindClientPer) == enPremissions::eFindClientPer))
        {

            system("cls");
            ShowFindClientScreen();
            GoToMainMenueScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eTransactions:
        if (((PermissionUser & enPremissions::eTransactionPer) == enPremissions::eTransactionPer))
        {

            system("cls");
            ShowTransactionsScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }
    case enMainMenueOptions::eManageUsers:
        if (((PermissionUser & enPremissions::eManageUsersPer) == enPremissions::eManageUsersPer))
        {

            system("cls");
            ShowManageUsersScreen();
            break;
        }
        else
        {
            system("cls");
            ShowAccessDeniedScreen();
            GoToMainMenueScreen();
            break;
        }

    case enMainMenueOptions::eLogout:
        system("cls");
        login();
        break;
    }
}

void ShowMainMenue(short PermissionsUser)
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
    PerfoemMainMenueOption((enMainMenueOptions)ReadMainMenueOption(), PermissionUser);
}

bool CheckUser(string Username, string Password, vector<stUser> vUsers, short &PermissionUser)
{

    bool IsLoginCorrect = false;

    for (stUser User : vUsers)
    {
        if (User.Username == Username)
        {
            if (User.Password == Password)
            {
                IsLoginCorrect = true;
                PermissionUser = User.permissions;
            }
        }
    }

    return IsLoginCorrect;
}

void loginUser()
{
    string Username = "";
    string Password = "";
    bool IsLoginCorrect = false;
    vector<stUser> vUsers = LoadUsersFromFile();

    do
    {
        cout << "Enter Username? ";
        getline(cin >> ws, Username);

        cout << "Enter Password? ";
        getline(cin >> ws, Password);
        IsLoginCorrect = CheckUser(Username, Password, vUsers, PermissionUser);
        if (!IsLoginCorrect)
        {
            cout << "Invalid Username/Password\n";
        }

    } while (!IsLoginCorrect);

    system("cls");
    ShowMainMenue(PermissionUser);
}

void login()
{
    cout << "------------------------------------------------------------\n";
    cout << "\t\t\tLogin Screen";
    cout << "\n------------------------------------------------------------\n";

    loginUser();
}

int main()
{

    login();
}