#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();
void Login();

struct sUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenuPermissions
{
    eAll = -1,
    pListClients = 1,
    pAddClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTranactions = 32,
    pManageUsers = 64
};

sUser CurrentUser;

enum enMainMenuOptions
{
    eListClients = 1,
    eAddClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindCLient = 5,
    eShowTranactionMenue = 6,
    eManageUser = 7,
    eExit = 8
};

enum enTranactionsMenuOptions
{
    eDeposit = 1,
    eWithdrow = 2,
    eShowTotel = 3,
    eShowMainMenu = 4
};

enum enManageUsersMenuOptions
{
    eListUsers = 1,
    eAddUser = 2,
    eDeleteUsers = 3,
    eUpdateUsers = 4,
    eFindUser = 5,
    eManageUsersMainMenu = 6
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = (short)S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

bool CheckAccessPermission(enMainMenuPermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuPermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You dont Have Permission To Do this,\n";
    cout << "Please Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenue();
}

sClient ConvertClientLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertClientRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertClientLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertClientRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(FileName);
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
            return true;
    }
    return false;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

// ================= Client Operations =================

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddNewClient()
{
    sClient Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pAddClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients();
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter Account Number? ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

// ================= Transactions Operations =================

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
    }
    return false;
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalances()
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient Client : vClients)
        {
            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances << endl;
}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromTranactionsMenueOption(enTranactionsMenuOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTranactionsMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;

    case enTranactionsMenuOptions::eWithdrow:
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;

    case enTranactionsMenuOptions::eShowTotel:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;

    case enTranactionsMenuOptions::eShowMainMenu:
        ShowMainMenue();
        break;
    }
}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    PerfromTranactionsMenueOption((enTranactionsMenuOptions)ReadTransactionsMenueOption());
}

// ================= Users Operations =================

sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    sUser User;
    vector<string> vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

string ConvertUserRecordToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);
    return stUserRecord;
}

vector<sUser> LoadUsersDataFromFile(string FileName)
{
    vector<sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;
        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

vector<sUser> SaveUsersDataToFile(string FileName, vector<sUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open())
    {
        for (sUser U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}

bool FindUserByUsername(string UserName, vector<sUser> vUsers, sUser& User)
{
    for (sUser U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool FindUserByUsernameAndPassword(string UserName, string Password, sUser& User)
{
    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool UserExistsByUsername(string UserName, string FileName)
{
    vector<sUser> vUsers = LoadUsersDataFromFile(FileName);
    for (sUser U : vUsers)
    {
        if (U.UserName == UserName)
            return true;
    }
    return false;
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(15) << left << User.Password;
    cout << "| " << setw(12) << left << User.Permissions;
}

void ShowListUsersScreen()
{
    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(15) << "Password";
    cout << "| " << left << setw(12) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;

    if (vUsers.size() == 0)
        cout << "\t\t\t\tNo Users Available In the System!";
    else
        for (sUser User : vUsers)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "________________________________________\n" << endl;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pListClients;

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pAddClient;

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pDeleteClient;

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pUpdateClient;

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pFindClient;

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pTranactions;

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuPermissions::pManageUsers;

    return Permissions;
}

sUser ReadNewUser()
{
    sUser User;
    cout << "Enter Username? ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUsername(User.UserName, UsersFileName))
    {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

void AddNewUser()
{
    sUser User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";
    AddNewUsers();
}

string ReadUserName()
{
    string UserName = "";
    cout << "\nPlease enter Username? ";
    cin >> UserName;
    return UserName;
}

void PrintUserCard(sUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUser Name   : " << User.UserName;
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

bool MarkUserForDeleteByUsername(string UserName, vector<sUser>& vUsers)
{
    for (sUser& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteUserByUsername(string UserName, vector<sUser>& vUsers)
{
    if (UserName == "Admin")
    {
        cout << "\n\nYou cannot Delete This User.";
        return false;
    }

    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkUserForDeleteByUsername(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            vUsers = LoadUsersDataFromFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
        return false;
    }
    return false;
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUsername(UserName, vUsers);
}

sUser ChangeUserRecord(string UserName)
{
    sUser User;
    User.UserName = UserName;

    cout << "\n\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool UpdateUserByUsername(string UserName, vector<sUser>& vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(UserName, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (sUser& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
        return false;
    }
    return false;
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUsername(UserName, vUsers);
}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector<sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    sUser User;
    string UserName = ReadUserName();

    if (FindUserByUsername(UserName, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with Username [" << UserName << "] is not found!";
}

short ReadManageUsersMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromManageUsersMenueOption(enManageUsersMenuOptions ManageUsersMenueOption)
{
    switch (ManageUsersMenueOption)
    {
    case enManageUsersMenuOptions::eListUsers:
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersMenuOptions::eAddUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersMenuOptions::eDeleteUsers:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersMenuOptions::eUpdateUsers:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersMenuOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersMenuOptions::eManageUsersMainMenu:
        ShowMainMenue();
        break;
    }
}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermission(enMainMenuPermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";

    PerfromManageUsersMenueOption((enManageUsersMenuOptions)ReadManageUsersMenueOption());
}

// ================= Main Menu & Login =================

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromMainMenueOption(enMainMenuOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenuOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eAddClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eFindCLient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eShowTranactionMenue:
        system("cls");
        ShowTransactionsMenue();
        break;

    case enMainMenuOptions::eManageUser:
        system("cls");
        ShowManageUsersMenue();
        break;

    case enMainMenuOptions::eExit:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";

    PerfromMainMenueOption((enMainMenuOptions)ReadMainMenueOption());
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;
    string Username, Password;

    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}