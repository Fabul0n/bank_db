#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <cassert>
#include <sstream>
#include <numeric>
#include <functional>
#include <fstream>
#include <random>


using namespace std;

class Loan
{
    double ammount;
public:
    Loan()
    {
        ammount = 0;
    }
    Loan(double ammount)
    {
        this->ammount = ammount;
    }
    Loan(string amount)
    {
        stringstream ss(amount);
        ss >> this->ammount;
    }
    Loan(const Loan& other)
    {
        this->ammount = other.ammount;
    }
    void setAmmount(double ammount)
    {
        this->ammount = ammount;
    }
    void addAmmount(double ammount)
    {
        this->ammount += ammount;
    }
    void subAmmount(double ammount)
    {
        this->ammount -= ammount;
    }
    void mulAmmount(double ammount)
    {
        this->ammount *= ammount;
    }
    void divAmmount(double ammount)
    {
        assert(((void)"Division by zero", ammount != 0));
        this->ammount /= ammount;
    }
    
    double getAmmount()
    {
        return this->ammount;
    }

    int getLength() const
    {
        ostringstream oss;
        oss << fixed << setprecision(2) << round(this->ammount * 100.0) / 100.0;
        return oss.str().length();
    }

    friend ostream& operator<<(ostream& stream, const Loan& loan)
    {
        stream << fixed << setprecision(2) << round(loan.ammount * 100.0) / 100.0;
        return stream;
    }
    friend istream& operator>>(istream& stream, Loan& loan)
    {
        double value; stream >> value;
        loan.setAmmount(value);
        return stream;
    }
    Loan& operator=(const Loan& other)
    {
        this->ammount = other.ammount;
        return *this;
    }
};

class Percentage
{
    double value;
public:
    Percentage() = default;
    Percentage(double value)
    {
        this->value = value;
    }
    Percentage(const Percentage& other)
    {
        this->value = other.value;
    }
    Percentage(string value)
    {
        stringstream ss(value);
        ss >> this->value;
    }
    Percentage& operator=(const Percentage& other)
    {
        this->value = other.value;
        return *this;
    }

    void setValue(double value)
    {
        this->value = value;
    }
    double getValue()
    {
        return this->value;
    }
    void addValue(double value)
    {
        this->value += value;
    }

    friend ostream& operator<<(ostream& stream, Percentage& percentage)
    {
        stream << fixed << setprecision(2) << round(percentage.value * 100.0) / 100.0;
        return stream;
    }
};

class Client
{
    string name;
    vector< Loan > loans;
    vector< Percentage > percentages;
public:
    Client() = default;
    Client(string name)
    {
        this->name = name;
    }
    Client(string name, vector< Loan > loans, vector< Percentage > percentages)
    {
        assert(((void)"percentages len is not equal to loans len", loans.size() == percentages.size()));
        this->name = name;
        this->loans = loans;
        this->percentages = percentages;
    }

    vector< Loan > getLoans() const
    {
        return this->loans;
    }
    vector< Percentage > getPercentages() const
    {
        return this->percentages;
    }
    string getName() const
    {
        return this->name;
    }

    double withdraw()
    {
        vector< Loan* > deposits;
        vector< Percentage* > deposit_percentages;
        for (int i = 0; i < this->loans.size(); i++)
        {
            if (loans[i].getAmmount() > 0)
            {
                deposits.push_back(&loans[i]);
                deposit_percentages.push_back(&percentages[i]);
            }
        }
        cout << "Доступные счета: \n";
        for (int i = 0; i < deposits.size(); i++)
        {
            cout << i+1 << ") " << *deposits[i] << " with " << *deposit_percentages[i] << " percentage\n";
        }
        cout << "С какого желаете снять?\n";
        int depositIndex; cin >> depositIndex;
        --depositIndex;
        if (depositIndex < deposits.size() && depositIndex >= 0)
        {
            Loan& deposit = *deposits[depositIndex];
            cout << "Сколько вы хотите снять?\n";
            double ammount; cin >> ammount;
            if (deposit.getAmmount() < ammount)
            {
                cout << "Недостаточно денег\n";
                return 0;
            }
            deposit.subAmmount(ammount);
            return ammount;
        }
        else
        {
            cout << "Депозита с таким номером не существует\n";
            return 0;
        }
    }
    void makeDeposit()
    {
        cout << "Сколько вы бы хотели внести?\n";
        double value; cin >> value;
        double percentage = rand()%100 * 1.0 / 100.0;
        cout << "Вклад будет сделан под " << percentage << " процентов.\n";
        cout << "Чтобы согласиться введите 1\n";
        cout << "Чтобы отказаться введите 0\n";
        int agreement; cin >> agreement;
        if (agreement)
        {
            this->loans.push_back(Loan(value));
            this->percentages.push_back(Percentage(percentage));
            cout << "Вклад успешно сделан\n";
        }
    }
    void payOffCredit()
    {
        vector< Loan* > credits;
        vector< Percentage* > credit_percentages;
        vector< int > indexes;
        for (int i = 0; i < this->loans.size(); i++)
        {
            if (loans[i].getAmmount() < 0)
            {
                credits.push_back(&loans[i]);
                credit_percentages.push_back(&percentages[i]);
                indexes.push_back(i);
            }
        }
        cout << "Доступные счета: \n";
        for (int i = 0; i < credits.size(); i++)
        {
            cout << i+1 << ") " << *credits[i] << " with " << *credit_percentages[i] << " percentage\n";
        }
        cout << "Какой желаете погасить?\n";
        int creditIndex; cin >> creditIndex;
        --creditIndex;
        if (creditIndex < credits.size() && creditIndex >= 0)
        {
            Loan& credit = *credits[creditIndex];
            cout << "Сколько вы хотите погасить?\n";
            double ammount; cin >> ammount;
            credit.addAmmount(ammount);
            if (credit.getAmmount() == 0.0)
            {
                vector< Loan >::iterator it = loans.begin();
                advance(it, indexes[creditIndex]);
                loans.erase(it);
            }
            return;
        }
        else
        {
            cout << "Депозита с таким номером не существует\n";
            return;
        }
    }
    double takeLoan()
    {
        cout << "Сколько вы бы хотели взять?\n";
        double value; cin >> value;
        double percentage = rand()%100 * 1.0 / 10.0 + 20.0;
        cout << "Кредит будет взят под " << percentage << " процентов.\n";
        cout << "Чтобы согласиться введите 1\n";
        cout << "Чтобы отказаться введите 0\n";
        int agreement; cin >> agreement;
        if (agreement)
        {
            this->loans.push_back(Loan(-value));
            this->percentages.push_back(Percentage(percentage));
            return value;
        }
        return 0.0;
    }

    void changePercentage()
    {
        cout << "Выберите счёт:\n";
        for (int i = 0; i < loans.size(); i++)
        {
            cout << i+1 << ") " << loans[i] << " with " << percentages[i] << " percentage" << "\n";
        }
        int loanIndex; cin >> loanIndex;
        --loanIndex;
        if (loanIndex >= loans.size() || loanIndex < 0)
        {
            cout << "Такого счёта нет\n";
            return;
        }
        cout << "На сколько увеличить процент?\n";
        double diff; cin >> diff;
        percentages[loanIndex].addValue(diff);
        cout << "Процент успешно изменён\n";
    }
    void changeDepositPercentage(double value)
    {
        for (int i = 0; i < loans.size(); i++)
        {
            if (loans[i].getAmmount() >= 0)
            {
                percentages[i].addValue(value);
            }
        }
    }
    void changeCreditPercentage(double value)
    {
        for (int i = 0; i < loans.size(); i++)
        {
            if (loans[i].getAmmount() < 0)
            {
                percentages[i].addValue(value);
            }
        }
    }

    void payDay()
    {
        for (int i = 0; i < loans.size(); i++)
        {
            loans[i].divAmmount(100.0);
            loans[i].mulAmmount(percentages[i].getValue()+100.0);
        }
    }

    friend ostream& operator<<(ostream& stream, const Client& client)
    {
        stream << client.name << "\n";
        vector< Loan > loans = client.getLoans();
        vector< Percentage > percentages = client.getPercentages();
        for (size_t i = 0; i < loans.size(); i++)
        {
            stream << loans[i] << " with " << percentages[i] << " percentage" << "\n";
        }
        return stream;
    }
};

class Table
{
    vector< Client > clients;
    string name;
public:
    Table() = default;
    Table(vector< Client > clients, string name)
    {
        this->clients = clients;
        this->name = name;
    }
    Table(const Table& other)
    {
        this->clients = other.clients;
        this->name = other.name;
    }
    ~Table() = default;

    vector< Client > getClients()
    {
        return this->clients;
    }

    double withdraw()
    {
        vector< string > names;
        for (const auto client : clients)
        {
            names.push_back(client.getName());
        }
        cout << "Введите ваше имя: ";
        string name; cin >> name;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].getName() == name)
            {
                return clients[i].withdraw();
            }
        }
        cout << "Такого клиента нет\n";
        return 0;
    }
    void makeDeposit()
    {
        vector< string > names;
        for (const auto client : clients)
        {
            names.push_back(client.getName());
        }
        cout << "Введите ваше имя: ";
        string name; cin >> name;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].getName() == name)
            {
                clients[i].makeDeposit();
                return;
            }
        }
        clients.push_back(Client(name));
        clients[clients.size()-1].makeDeposit();
    }
    void payOffCredit()
    {
        vector< string > names;
        for (const auto client : clients)
        {
            names.push_back(client.getName());
        }
        cout << "Введите ваше имя: ";
        string name; cin >> name;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].getName() == name)
            {
                clients[i].payOffCredit();
                return;
            }
        }
        cout << "Такого клиента нет\n";
    }
    double takeLoan()
    {
        vector< string > names;
        for (const auto client : clients)
        {
            names.push_back(client.getName());
        }
        cout << "Введите ваше имя: ";
        string name; cin >> name;
        for (int i = 0; i < clients.size(); i++)
        {
            if (clients[i].getName() == name)
            {
                return clients[i].takeLoan();
            }
        }
        clients.push_back(Client(name));
        return clients[clients.size()-1].takeLoan();
    }

    void changePercentage()
    {
        int groupSwitch;
        cout << "1) изменить ставку одному клиенту\n";
        cout << "2) изменить ставки по вкладам\n";
        cout << "3) изменить ставки по кредитам\n";
        cout << "4) изменить все ставки \n";
        cout << "Выберите категорию: ";
        cin >> groupSwitch;
        string name;
        vector< string > names;
        double diff;
        switch (groupSwitch)
        {
        case 1:
        {
            cout << "Введите имя клиента: ";
            cin >> name;
            for (const auto client : clients)
            {
                names.push_back(client.getName());
            }
            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].getName() == name)
                {
                    clients[i].changePercentage();
                }
            }
            cout << "Проценты изменены\n";
            break;
        }  
        case 2:
        {
            cout << "На сколько увеличить проценты по вкладам?\n";
            cin >> diff;
            for (int i = 0; i < clients.size(); i++)
            {
                clients[i].changeDepositPercentage(diff);
            }
            cout << "Проценты изменены\n";
            break;
        }
        case 3:
        {
            cout << "На сколько увеличить проценты по кредитам?\n";
            cin >> diff;
            for (int i = 0; i < clients.size(); i++)
            {
                clients[i].changeCreditPercentage(diff);
            }
            cout << "Проценты изменены\n";
            break;
        }
        case 4:
        {
            cout << "На сколько увеличить проценты?\n";
            cin >> diff;
            for (int i = 0; i < clients.size(); i++)
            {
                clients[i].changeCreditPercentage(diff);
                clients[i].changeDepositPercentage(diff);
            }
            cout << "Проценты изменены\n";
            break;
        }
        default:
        {
            cout << "Такой категории нет\n";
            break;
        }
        }
    }

    void payDay()
    {
        for (int i = 0; i < clients.size(); i++)
        {
            clients[i].payDay();
        }
    }

    void meanCreditDeposit()
    {
        double depositSum = 0; double depositCounter = 0;
        double creditSum = 0; double creditCounter = 0;
        for (const auto client : clients)
        {
            vector< Loan > loans = client.getLoans();
            for (auto loan : loans)
            {
                if (loan.getAmmount() < 0.0)
                {
                    creditSum -= loan.getAmmount();
                    creditCounter += 1;
                }
                else
                {
                    depositSum += loan.getAmmount();
                    depositCounter += 1;
                }
            }
        }
        cout << "Средняя сумма кредитов: " << creditSum / creditCounter << "\n";
        cout << "Средняя сумма вкладов: " << depositSum / depositCounter << "\n";
    }

    void write_csv(const char* filename)
    {
        ofstream fout(filename);
        if (!fout.is_open())
        {
            cout << "Не удалось открыть файл\n";
            return;
        }
        for (const auto client : clients)
        {
            fout << client.getName();
            vector< Loan > loans = client.getLoans();
            vector< Percentage > percentages = client.getPercentages();
            for (size_t i = 0; i < loans.size(); i++)
            {
                fout << ',' << loans[i] << ',' << percentages[i];;
            }
            fout << "\n";
        }
        cout << "Запись прошла успешно\n";
    }
    void read_csv(const char* filename, string table_name)
    {
        ifstream fin(filename);
        vector< Client > clients;
        string row;
        //int len = 0;
        while (getline(fin, row))
        {
            vector< Loan > loans;
            vector< Percentage > percentages;
            string name;
            stringstream ss(row);
            string value;
            int k = 1;
            while(getline(ss, value, ','))
            {
                if (name.empty())
                {
                    name = value;
                }
                else
                {
                    if (k % 2)
                    {
                        percentages.push_back(value);
                    }
                    else
                    {
                        loans.push_back(value);
                    }
                }
                ++k;
            }
            assert(((void)"Corrupted file", loans.size() == percentages.size()));
            //assert(((void)"Corrupted file", len == 0 || len == res.size()));
            //len = res.size();
            clients.push_back(Client(name, loans, percentages));
        }
        this->clients = clients;
        this->name = table_name;
    }

    Table& operator=(const Table& other)
    {
        this->clients = other.clients;
        this->name = other.name;
        return *this;
    }
    friend ostream& operator<<(ostream& stream, const Table& table)
    {
        if (table.clients.empty())
        {
            return stream;
        }

        int max_len = 0;
        for (const auto client : table.clients)
        {
            max_len = max((int) client.getLoans().size(), max_len);
        }

        vector< int > colWidths(max_len+1, 0);

        for (const auto client : table.clients)
        {
            colWidths[0] = max(colWidths[0], (int) client.getName().size()+1);
            vector< Loan > loans = client.getLoans();
            for (size_t i = 0; i < loans.size(); i++)
            {
                int len = loans[i].getLength();
                colWidths[i+1] = max(colWidths[i+1], len+1);
            }
        }

        string row_sep(accumulate(colWidths.begin(), colWidths.end(), 0)+max_len+1+max_len*8+1, '-');

        if (table.name.size() >= row_sep.size())
        {
            stream << table << "\n\n";
        }
        else
        {
            int part = (row_sep.size() - table.name.size()) / 2;
            stream << string(part, '=') << table.name << string(part+((row_sep.size() - table.name.size()) % 2), '=') << "\n";
        }

        stream << row_sep << "\n";
        for (const auto client : table.clients)
        {
            stream << setw(1) << "|" << setw(colWidths[0]) << client.getName() << setw(1) << "|";
            vector< Loan > loans = client.getLoans();
            vector< Percentage > percentages = client.getPercentages();
            for (size_t i = 0; i < loans.size(); i++)
            {
                stream << setw(colWidths[i+1]) << loans[i] << setw(1) << "|" << setw(6) << percentages[i] << setw(1) << '%' << setw(1) << "|" ;
            }
            stream << "\n" << row_sep << "\n";
        }
        return stream;
    }
};

class DataBase
{
    vector< Table > tables;
public:
    DataBase() = default;
    DataBase(vector< Table > other)
    {
        this->tables = other;
    }
    DataBase(const DataBase& other)
    {
        this->tables = other.tables;
    }
    ~DataBase() = default;

    void write_csv(const char* filename, ios_base::openmode mode = ios_base::out)
    {
        ofstream fout(filename, mode);
        fout << *this;
    }
    void read_csv(const char* filename, ios_base::openmode mode = ios_base::in)
    {

    }

    friend ostream& operator<<(ostream& stream, DataBase db)
    {
        for (const auto table : db.tables)
        {
            stream << table << "\n";
        }
        return stream;
    }
};


int main()
{
    srand(time(0));
    int globalMenuSwitch;
    int breakFlag = 0;
    Table db;
    db.read_csv("out", "Table");
    while (1)
    {
        if (breakFlag)
        {
            break;
        }
        //system("clear");
        cout << "0) вывести таблицу\n";
        cout << "1) снять вклад\n";
        cout << "2) завести новый вклад\n";
        cout << "3) частичное или полное погашение кредита\n";
        cout << "4) взять кредит\n";
        cout << "5) вывести состояние базы данных в файл\n";
        cout << "6) изменить процентные ставки\n";
        cout << "7) начислить проценты по вкладам и кредитам\n";
        cout << "8) подсчитать и вывести на экран среднюю сумму вклада и кредита\n";
        cout << "9) для выхода из программы\n";
        cout << "Введите номер желаемого пункта: ";
        cin >> globalMenuSwitch;
        getchar();
        double res;
        string filename;
        switch (globalMenuSwitch)
        {
        case 0:{
            cout << db;
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 1:{
            res = db.withdraw();
            if (res != 0.0)
            {
                cout << "Успешно снято " << res << "\n";
            }
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 2:{
            db.makeDeposit();
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 3:{
            db.payOffCredit();
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 4:{
            res = db.takeLoan();
            if (res != 0.0)
            {
                cout << "Взят кредит на " << res << "\n";
            }
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 5:{
            cout << "Введите название файла, в который будет произведена запись: ";
            cin >> filename;
            db.write_csv(filename.c_str());
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 6:{
            db.changePercentage();
            getchar();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 7:{
            db.payDay();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        case 8:{
            db.meanCreditDeposit();
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        } 
        case 9:{
            breakFlag = 1;
            break;
        }
        default:{
            cout << "Такого пункта нет\n";
            cout << "Нажмите любую кнопку для продолжения";
            getchar();
            break;
        }
        }
    }

    
    return 0;
}