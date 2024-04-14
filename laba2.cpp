#include <iostream>
#include <vector>
#include <optional>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <chrono>

using namespace std;

int globUnsuccess = 0;
int globSuccess = 0;

int flagFound = 0;
int flagNotFound = 0;

class HashTable {
public:
    HashTable(size_t size, bool useDoubleHashing) : size(size), count(0), table(size, nullopt), useDoubleHashing(useDoubleHashing) {}

    void add(int key) {
        //if (2 * count >= size) {
        //    // Rehash when load factor becomes greater than 0.5
        //    rehash();  
        //}
        if (count==size) {
            //cout << "таблица заполнена" << endl;
        }
        else {
            size_t hash1 = hashFunc1(key);
            size_t hash2 = useDoubleHashing ? hashFunc2(key) : 1;
            while (table[hash1].has_value()) {
                hash1 = (hash1 + hash2) % size;
            }
            table[hash1] = key;
            count++;
        }
    }

    void search(int key) {
        int count = 0;
        size_t hash1 = hashFunc1(key);
        size_t hash2 = useDoubleHashing ? hashFunc2(key) : 1;
  
        if (!table[hash1].has_value()) {
            globUnsuccess = 1;
            flagNotFound++;
            return;
        }
        else {
            while (table[hash1].has_value()) {
                count++;
                if (table[hash1].value() == key) {
                    //count = 1;
                    //return count;
                    globSuccess = count;
                    globUnsuccess = 0;
                    flagFound++;
                    return;
                }
                else {
                    globUnsuccess = count;
                    globSuccess = 0;
                }
                hash1 = (hash1 + hash2) % size;
            }
            if (globUnsuccess!=0) {
                flagNotFound++;
            }
        }

        
        //return count;
        //globUnsuccess = count;
        //flagNotFound=1;
        return;
    }

    void change(int hash1) {
        int flag = 0;
        int j=hash1+1;

        while (table[j]!=nullopt) {
            if (hash1 == hashFunc1(table[j].value())) {
                table[hash1] = table[j].value();
                table[j] = nullopt;
                flag = 1;
                change(j);
                break;
            }
            /*else if ((hash1 > hashFunc1(table[j].value()))&&(flag==0)) {
                table[hash1] = table[j].value();
                table[j] = nullopt;
                flag = 1;
                change(j);
                break;
            }*/
            else {
                j++;
            }   
        }

        
        j = hash1 + 1;

            while (table[j] != nullopt) {
                if (hash1 > hashFunc1(table[j].value())) {
                    table[hash1] = table[j].value();
                    table[j] = nullopt;
                    flag = 1;
                    change(j);
                    break;
                }
                else {
                    j++;
                }
            }
        



        //for (int i = hash1+1; i < size; i++) {
        //    if (table[i]!=nullopt) {
        //        if (hash1 == hashFunc1(table[i].value())) {
        //            table[hash1] = table[i].value();
        //            table[i] = nullopt;
        //            change(i);
        //        }
        //        else {
        //            flag = 1;
        //            break;
        //        }
        //    }
        //    else {
        //        flag = 1;
        //        break;
        //    }
        //}

        //if (flag==1) {
        //    for (int i = 1; i < size-hash1; i++) {
        //        if (table[hash1 + i] != nullopt) {
        //            if (hash1 + i == hashFunc1(table[hash1 + i].value())) {
        //                //continue;
        //            }
        //            

        //            else if(hash1 + i != hashFunc1(table[hash1 + i].value())){
        //                table[hash1] = table[hash1+i].value();
        //                table[hash1 + i] = nullopt;
        //                change(hash1 + i);
        //            }

        //            
        //        }
        //        else {
        //            break;
        //        }
        //    }

        //}


    }

    void remove(int key) {
        size_t hash1 = hashFunc1(key);
        size_t hash2 = useDoubleHashing ? hashFunc2(key) : 1;
        while (table[hash1].has_value()) {
            if (table[hash1].value() == key) {
                table[hash1] = nullopt;
                //
                change(hash1);

                count--;
                return;
            }
            hash1 = (hash1 + hash2) % size;
        }
    }

    void print() {
        for (size_t i = 0; i < size; i++) {
            if (table[i].has_value()) {
                std::cout << i << ": " << table[i].value() << std::endl;
            }
            else {
                std::cout << i << ": " << "empty" << std::endl;
            }
        }
    }

private:
    size_t hashFunc1(int key) {
        return key % size;
    }

    size_t hashFunc2(int key) {
        // Second hash function. This must be non-zero and less than size
        return (key / size) % size + 1;
    }

    //void rehash() {
    //    size_t newSize = 2 * size;
    //    std::vector<optional<int>> newTable(newSize, nullopt);
    //    for (size_t i = 0; i < size; i++) {
    //        if (table[i].has_value()) {
    //            size_t hash1 = table[i].value() % newSize;
    //            size_t hash2 = useDoubleHashing ? ((table[i].value() / newSize) % newSize) + 1 : 1;
    //            while (newTable[hash1].has_value()) {
    //                hash1 = (hash1 + hash2) % newSize;
    //            }
    //            newTable[hash1] = table[i];
    //        }
    //    }
    //    table = newTable;
    //    size = newSize;
    //}

    size_t size;
    size_t count;
    bool useDoubleHashing;
    std::vector<optional<int>> table;
};

//void fillVectorWithRandomProbabilities(std::vector<double>& probabilities, int n) {
//    std::random_device rd;
//    std::mt19937 generator(rd());
//    std::uniform_real_distribution<double> distribution(0.0, 1.0);
//
//    double sum = 0.0;
//    for (int i = 0; i < n; ++i) {
//        probabilities[i] = distribution(generator);
//        sum += probabilities[i];
//    }
//
//    // Normalize the probabilities so they sum to 1
//    for (int i = 0; i < n; ++i) {
//        probabilities[i] /= sum;
//    }
//}
//
//void fillVectorWithNonUniformRandom(std::vector<int>& vec, std::vector<double>& probabilities) {
//    std::random_device rd;
//    std::mt19937 generator(rd());
//    std::discrete_distribution<int> distribution(0, 1000, probabilities.begin(), probabilities.end());
//
//    for (int& value : vec) {
//        value = distribution(generator);
//    }
//}


int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");   

    int size = 30;

    HashTable ht1(size, false); 
    HashTable ht2(size, true);  

    HashTable ht3(size, false); 
    HashTable ht4(size, true);  

    int a;

    int count = 5;
    double av_success = 0;
    double av_unsuccess = 0;

    int quantity = 29;

    //равномерный закон

    for (int i = 0; i < quantity;i++) {
        a = rand()%1000;       
        ht1.add(a);      
    }
    ht1.print();
    std::cout << std::endl;

    //среднее количество действий
    int find = 0;
    int notFound = 0;

    for (int k = 0; k < count; k++) {
        cin >> a;
        ht1.search(a);

        av_success += globSuccess;
        av_unsuccess += globUnsuccess;
    }

    if (flagFound != 0 && flagNotFound != 0) {
        cout << "Среднее кол-во действий успешного поиска: " << av_success / count << endl;
        cout << "Среднее кол-во действий неуспешного поиска: " << av_unsuccess / count << endl;
    }
    else {
        if (flagFound == 0) {
            cout << "Среднее кол-во действий неуспешного поиска: " << av_unsuccess / count << endl;
        }
        else if (flagNotFound == 0) {
            cout << "Среднее кол-во действий успешного поиска: " << av_success / count << endl;
        }
    }

    //добавление

    /*ht1.add(4);
    ht1.add(5);
    ht1.add(6);
    ht1.add(16);
    ht1.add(14);*/

    /*ht1.add(5);
    ht1.add(15);
    ht1.add(25);
    ht1.add(35);*/
    
    /*ht1.print();   
    std::cout << std::endl;*/

    //удаление и поиск
    int b;
    cin >> b;

    ht1.remove(b);
    std::cout << std::endl;

    ht1.print();
    std::cout << std::endl;

    cin >> b;

    //cout << "Кол - во найденных элементов : " << ht1.search(b) << endl;
    std::cout << std::endl;


    //неравномерный закон

    quantity = 20;

    default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distributionRange(0, 9);
    exponential_distribution<double> distribution1(3.5);
    exponential_distribution<double> distribution2(7.0);

    // Выбираем диапазон чисел, который будет появляться чаще, перед началом цикла
    int frequentRange = distributionRange(generator);

    unordered_set<int> generatedNumbers;

    //while (generatedNumbers.size() < quantity) {
    //    //double number1 = distribution1(generator);
    //    //double number2 = distribution2(generator);
    //    //int numberToAdd = 0;
    //    //if (/*number1 < 1.0 && */frequentRange != distributionRange(generator)) {
    //    //    numberToAdd = frequentRange * 100 + static_cast<int>(number1 * 100);
    //    //}
    //    //else if (/*number2 < 1.0 &&*/ frequentRange == distributionRange(generator)) {
    //    //    numberToAdd = frequentRange * 100 + static_cast<int>(number2 * 100);
    //    //}

    //    int range = distributionRange(generator);
    //    double number = (range == frequentRange) ? distribution2(generator) : distribution1(generator);
    //    int numberToAdd = range * 100 + static_cast<int>(number * 100);

    //    // Проверяем, что число еще не было сгенерировано
    //    if (generatedNumbers.find(numberToAdd) == generatedNumbers.end()) {
    //        ht3.add(numberToAdd);
    //        ht4.add(numberToAdd);
    //        generatedNumbers.insert(numberToAdd);
    //    }
    //}

    // ht3.print();

    return 0;
}
