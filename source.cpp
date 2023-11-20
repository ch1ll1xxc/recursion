#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <chrono>

using namespace std;

// Структура записи - Нагрузка по дисциплине
struct Company
{
    string license_number; // номер лицензии
    string name; // наименование предприятия
    string founder; // учредитель
    bool active; // состояние лицензии (0 - активна, 1 - отозвана)
};

// Структура записи в хеш-таблице
struct HashTableEntry
{
    int key;
    int index;
};

// Номер полиса, состоящий из 8 чисел
int Randomizer()
{
    int min = 1;
    int max = 99999;
    return min + rand() % (max - min + 1);
}

// Хеш-функция методом деления
int HashFun(int key, int tableSize)
{
    return key % tableSize;
}

// Вставка ключа в таблицу
void Insert(unordered_map<int, list<HashTableEntry>>& hashTable, int key, int tableSize, int index)
{
    HashTableEntry entry;
    entry.key = key;
    entry.index = index;
    int hash = HashFun(key, tableSize);
    hashTable[hash].push_back(entry);
}

// Удаление ключа из таблицы и файла
void Remove(unordered_map<int, list<HashTableEntry>>& hashTable, Company& policy, const string binary, int key, int tableSize)
{
    int hash = HashFun(key, tableSize);
    bool corr = false;

    auto& entries = hashTable[hash];
    list<HashTableEntry> tempEntries;
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->key != key)
        {
            tempEntries.push_back(*it);
        }
        else if (it->key == key)
        {
            corr = true;

            cout << endl << "Начался процесс удаления записи из файла" << endl;
            ofstream file(binary, ios::binary | ios::in | ios::out);
            if (file)
            {
                file.seekp(it->index * sizeof(Company), ios::beg);

                // Пустая запись
                Company empty;
                empty.license_number = "";
                empty.name = "";
                empty.founder = "";
                empty.active = 0;
                file.write(reinterpret_cast<char*>(&empty), sizeof(Company));
                cout << "   Полученная запись удалена из файла" << endl;
            }
            file.close();
        }
    }
    entries = tempEntries;
    if (corr == false)
        cout << endl << "Предприятие не удалось удалить из файла" << endl;
}

// Поиск ключа в таблице и файле
void Find(const unordered_map<int, list<HashTableEntry>>& hashTable, Company& policy, const string& binary, int key, int tableSize)
{
    int hash = HashFun(key, tableSize);
    bool corr = false;

    auto it = hashTable.find(hash);
    if (it != hashTable.end())
    {
        auto& entries = it->second;

        for (const HashTableEntry& entry : entries)
        {
            if (entry.key == key)
            {
                cout << endl << "Предприятие найдено в таблице" << endl;
                ifstream file(binary, ios::binary);
                if (file)
                {
                    cout << "   Получаем запись из файла" << endl;
                    corr = true;

                    file.seekg(entry.index * sizeof(Company), ios::beg);
                    if (file.read(reinterpret_cast<char*>(&policy), sizeof(Company)))
                    {
                        cout << "   Номер лицензии ---> " << policy.license_number << endl;
                        cout << "   Наименование предприятия ---> " << policy.name << endl;
                        cout << "   Учредитель ---> " << policy.founder << endl;
                        cout << "   Состояние лицензии ---> " << policy.active << endl;
                    }
                }
                file.close();
                return;
            }
            else
                corr = false;
        }
    }
    if (corr == false)
        cout << endl << "Предприятие не найдено в файле" << endl;
}

//----> Рехеширование таблицы
void Rehash(unordered_map<int, list<HashTableEntry>>& hashTable, int newTableSize)
{
    unordered_map<int, list<HashTableEntry>> newHashTable(newTableSize);
    for (const auto& entry : hashTable)//----> Проход по всем элементам хеш-таблицы
    {
        for (const HashTableEntry& hashEntry : entry.second)//----> Проход по всем записям хеша
        {
            int newHash = HashFun(hashEntry.key, newTableSize);
            newHashTable[newHash].push_back(hashEntry);
        }
    }
    hashTable = newHashTable;
}

// Определение простого числа
bool IsPrime(int n)
{
    if (n <= 1)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}

int CalculateSize(int currentSize)
{
    int newSize = currentSize * 2;
    while (!IsPrime(newSize))
    {
        newSize++;
    }
    return newSize;
}

void Output(unordered_map<int, list<HashTableEntry>>& hashTable)
{
    cout << endl << "После изменения" << endl;
    for (const auto& entry : hashTable)
    {
        int hash = entry.first;
        const list<HashTableEntry>& entries = entry.second;
        for (const HashTableEntry& hashEntry : entries)
        {
            int key = hashEntry.key;
            printf("   Индекс записи:  %3d   Ключ:  %2d   Хеш:  %2d\n", hashEntry.index, key, hash);
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(static_cast<unsigned int>(time(nullptr)));
    const string text = "textFile.txt";
    const string binary = "binaryFile.bin";

    ofstream textFile(text);
    if (!textFile)
    {
        cout << "Текстовый файл не удалось открыть для записи" << endl;
        return 1;
    }
    cout << "Текстовый файл успешно открыт для записи" << endl << endl;

    ofstream binaryFileOut(binary, ios::binary);
    if (!binaryFileOut)
    {
        cerr << "Бинарный файл не удалось открыть для записи" << endl;
        return 1;
    }
    cout << "Бинарный файл успешно открыт для записи" << endl << endl;

    int quant = 5;
    set<int> unique;

    Company policy;
    for (int i = 0; i < quant; ++i)
    {
        policy.license_number = to_string(Randomizer());
        std::cout << "\nEnter name: ";
        std::cin >> policy.name;
        std::cout << "\nEnter founder: ";
        std::cin >> policy.founder;
        std::cout << "\nEnter license`s status: (0 - active, 1 - inactive) \n";
        std::cin >> policy.active;

        textFile << policy.license_number << ' ' << policy.name << ' ' << policy.founder << ' ' << policy.active << '\n';

        binaryFileOut.write(reinterpret_cast<const char*>(&policy), sizeof(policy));
    }
    cout << "Генерация текстового и бинарного файла завершена" << endl;

    size_t bytes = sizeof(Company);
    cout << "Размер записи в байтах: " << bytes << endl;
    cout << "Количество записей: " << quant << endl << endl;

    textFile.close();
    binaryFileOut.close();

    int tableSize = 3;
    unordered_map<int, list<HashTableEntry>> hashTable;

    ifstream binaryFileIn(binary, ios::binary);
    if (!binaryFileIn)
    {
        cout << "Бинарный файл не удалось открыть для чтения" << endl;
        return 1;
    }
    cout << "Бинарный файл успешно открыт для чтения" << endl << endl;

    cout << "   Проверка хеш-индексов" << endl << endl;
    int index = 0;
    while (binaryFileIn.read(reinterpret_cast<char*>(&policy), sizeof(policy)))
    {
        HashTableEntry entry;
        string tmp = policy.license_number;
        entry.key = stoi(tmp);
        entry.index = index;
        index++;
        int hash = HashFun(entry.key, tableSize);
        hashTable[hash].push_back(entry);

        printf("   Индекс записи:  %3d   Ключ:  %2d   Хеш:  %2d\n", entry.index, entry.key, hash);
    }
    binaryFileIn.close();

    double loadFactor = static_cast<double>(hashTable.size()) / tableSize;
    if (loadFactor > 0.8)
    {
        int newTableSize = CalculateSize(tableSize);
        tableSize = newTableSize;
        Rehash(hashTable, tableSize);
    }

    bool flag = true;
    auto start_time = chrono::high_resolution_clock::now();
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    while (flag)
    {
        int choice;
        cout << endl << "Выберите действие со структурой: " << endl
            << "   1 - Вставка ключа в таблицу" << endl
            << "   2 - Поиск ключа в файле" << endl
            << "   3 - Удаление ключа из файла и таблицы" << endl
            << "   4 - Выход из тестирования" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            int insert;
            cout << "Введите номер лицензии предприятия, которого хотите добавить ---> ";
            cin >> insert;
            Insert(hashTable, insert, tableSize, index);
            loadFactor = static_cast<double>(hashTable.size()) / tableSize;
            if (loadFactor > 0.8)
            {
                int newTableSize = CalculateSize(tableSize);
                tableSize = newTableSize;
                Rehash(hashTable, newTableSize);
            }
            Output(hashTable);
            break;
        case 2:
            int found;
            cout << "Введите номер лицензии предприятия, которого вы желаете найти ---> ";
            cin >> found;
            start_time = chrono::high_resolution_clock::now();
            Find(hashTable, policy, binary, found, tableSize);
            end_time = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
            cout << endl << " Время выполнения: " << duration.count() << " [мкс]" << endl;
            break;
        case 3:
            int remove;
            cout << "Введите номер лицензии предприятия, которого вы желаете удалить ---> ";
            cin >> remove;
            Remove(hashTable, policy, binary, remove, tableSize);
            Output(hashTable);
            break;
        case 4:
            flag = false;
            break;
        default:
            break;
        }
    }
    return 0;
}
