// Project - ATM System
//_______________________________________________________________________________

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

void Login();
void ShowMainMenu();
void ShowQuickWithdrawScreen();
void ShoWNormalWithdrawScreen();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;

enum enMainMenuOptions
{
    eQuickWithdraw = 1, eNormalWithdraw = 2,
    eDeposit = 3, eCheckBalance = 4, eExit = 5
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client) {

    vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);

    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;

}

bool LoadClientInfo(string AccountNumber, string PinCode) {

    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

short ReadMainMenuOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadQucikWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    }
    return Choice;

}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients) {


    char Answer = 'n';
    cout << "\n\nAre you sure you want Perform this Transactions? y/n ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {

        for (sClient& C : vClients)
        {

            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully, New Balance is : " << C.AccountBalance << endl;;
                return true;
            }

        }

    }
    return false;

}

short GetQuickWithdrawAmount(short QucikWithdrawOption) {

    switch (QucikWithdrawOption)
    {
    case 1:
        return 20;
        break;
    case 2:
        return 50;
        break;
    case 3:
        return 100;
        break;
    case 4:
        return 200;
        break;
    case 5:
        return 400;
        break;
    case 6:
        return 600;
        break;
    case 7:
        return 800;
        break;
    case 8:
        return 1000;
        break;
    }

}

void PerfromQuickWithdrawOption(short QucikWithdrawOption) {

    if (QucikWithdrawOption == 9)
        return;

    short WithdrawBalance = GetQuickWithdrawAmount(QucikWithdrawOption);

    while (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\n\nAmount Exceeds the balance, you can Withdraw up to : " << CurrentClient.AccountBalance;
        cout << "\n\nPress any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawBalance;
}

void ShowQuickWithdrawScreen() {

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    PerfromQuickWithdrawOption(ReadQucikWithdrawOption());
}

int ReadWithdrawAmount() {

    int Amount;
    do
    {
        cout << "Enter an amount multiple of 5's ? ";
        cin >> Amount;

    } while (Amount % 5 != 0);

    return Amount;
}

void PerfromNormalWithdrawOption() {

    int WithdrawBalance = ReadWithdrawAmount();

    while (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\n\nAmount Exceeds the balance, you can Withdraw up to : " << CurrentClient.AccountBalance;
        cout << "\n\nPress any key to continue...";
        system("pause>0");
        ShoWNormalWithdrawScreen();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawBalance;

}

void ShoWNormalWithdrawScreen() {

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    PerfromNormalWithdrawOption();
}

void ShowCheckBalanceScreen() {


    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

}

double ReadDepositAmount() {

    double Amount = 0;

    do {
        cout << "\nEnter a positive deposit amount ? ";
        cin >> Amount;

    } while (Amount <= 0);

    return Amount;
}

void PerfromDepositOption() {

    double WithdrawBalance = ReadDepositAmount();

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance, vClients);
    CurrentClient.AccountBalance += WithdrawBalance;
};

void ShowDepositScreen() {

    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    PerfromDepositOption();
    
}

void PerfromMainMenuOption(enMainMenuOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenuOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eNormalWithdraw:
        system("cls");
        ShoWNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eExit:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

void Login() {

    bool LoginFaild = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Account Number/PinCode\n";
        }

        cout << "Enter Account Number ? ";
        cin >> AccountNumber;
        cout << "Enter PinCode ? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowMainMenu();

}

int main()
{
    Login();
    system("pause>0");
    return 0;
}
