#define GRAVITATIONAL_CONSTANT 398600.4415 //гравитационная постоянная
#define GRAVITATIONAL_ACCELERATION 9.81 //ускорение свободного падения
#define KMPS_TO_MPS 0.001 //константа для перевода км/с в м/с
#define EARTH_RADIUS 6378 //радиус Земли

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

//переменные определены глобально, чтобы к ним был доступ и из структуры и из new_sc_array()
double start_orbit, end_orbit; //начальная орбита и орбита перехода
int ID = 1; //порядковый номер КА

//функция оценивает сможет ли КА совершить перелет
bool can_travel(double start_orbit, double end_orbit, double mass, double propulsion_thrust, double specific_impulse, double fuel_mass){
    double start_orbit_velocity = sqrt(GRAVITATIONAL_CONSTANT / start_orbit); //скорость полета КА по начальной орбите
    double orbit_ratio = end_orbit / start_orbit; //отношение орбит
    //расчет потребной характеристической скорости
    double delta_velocity_1 = start_orbit_velocity * (sqrt(2 * orbit_ratio / (orbit_ratio + 1)) - 1);
    double delta_velocity_2 = start_orbit_velocity / sqrt(orbit_ratio) * (1 - sqrt(2 / (orbit_ratio + 1)));
    double delta_velocity = abs(delta_velocity_1 + delta_velocity_2); 
    double available_velocity = specific_impulse * GRAVITATIONAL_ACCELERATION * log(mass / (mass - fuel_mass)) * KMPS_TO_MPS; //расчет располагаемой скорости и перевод в м/с
    return available_velocity > delta_velocity; //если располагаемая скорость больше потребной то перелет возможен
}


class spacecraft{ //космический аппарат
private: //приватные переменные
    int id; //порядковый номер КА
    double mass, propulsion_thrust, specific_impulse, fuel_mass; //масса, тяга, удельный импульс и масса топлива
public: //публичные методы (переопределение операторов)
    friend ostream& operator << (ostream &s, const spacecraft &sc){ //переопределение оператора вывода (принимает ссылку на поток и ссылку на КА)
        s << "Космический аппарат #" << sc.id << endl;
        s << "Масса космического аппарата: " << sc.mass << endl;
        s << "Тяга двигательной установки: " << sc.propulsion_thrust << endl;
        s << "Удельный импульс двигательной установки: " << sc.specific_impulse << endl;
        s << "Масса топлива: " << sc.fuel_mass << endl;
        return s; //оператор ввода должен вернуть поток
    }

    
    friend istream& operator >> (istream &s, spacecraft &sc){ //переопределение оператора ввода (принимает ссылку на поток и ссылку на КА)
        sc.id = ID; ID++; //записывем порядковый номер КА и увелчичиваем глобальный порядковый номер, который будет присвоен следующему КА
        cout << "Введите массу космического аппарата: ";
        cin >> sc.mass;
        sc.mass = abs(sc.mass); //все переменные берутся по абсолютному значению
        cout << "Введите тягу двигательной установки: ";
        cin >> sc.propulsion_thrust;
        sc.propulsion_thrust = abs(sc.propulsion_thrust);
        cout << "Введите удельный импульс двигательной установки: ";
        cin >> sc.specific_impulse;
        sc.specific_impulse = abs(sc.specific_impulse);
        while (true){ //бесконечный цикл
            cout << "Введите массу топлива: ";
            cin >> sc.fuel_mass;
            sc.fuel_mass = abs(sc.fuel_mass);
            if (sc.fuel_mass > sc.mass) cout << "Масса топлива должна быть меньше массы КА!" << endl; //если масса топлива больше массы КА,
                                                                                                      //выводим сообщение об ошибке и снова запрашиваем массу топлива
            else break; //иначе прерываем цикл
        }
        return s; //оператор вывода должен вернуть поток
    }

    //переопределение операторов сравнения для упрощения сортировки
    const bool operator < (const spacecraft &sc2){ //оператор сравнения принимает правый операнд, а левый передается в него автоматически ввиде указателя this
        bool can_travel_1 =  can_travel(start_orbit, end_orbit, this -> mass, this -> propulsion_thrust, this -> specific_impulse, this -> fuel_mass); //обращение через ->, 
                                                                                                                                                       //тк this – указатель
        bool can_travel_2 =  can_travel(start_orbit, end_orbit, sc2.mass, sc2.propulsion_thrust, sc2.specific_impulse, sc2.fuel_mass);
        if (can_travel_1){ 
            if (can_travel_2){ 
                return this -> mass < sc2.mass; //если оба КА могут совершить прелет, то сравниваем их по массе
            } else return true; //если только левый КА сможет долететь, то он автоматически становится меньше (выше при сортировке)
        } else if (can_travel_2){ //если только правый КА может долететь
            return false; //то он становится меньше 
        } return this -> propulsion_thrust > sc2.propulsion_thrust; //если оба КА не долетят, то они сравниваются по тяге с обратным знаком (тк такие КА упорядочиваются в порядке убывания)
    }

    //аналогично для оператора больше
    const bool operator > (const spacecraft &sc2){
        bool can_travel_1 =  can_travel(start_orbit, end_orbit, this -> mass, this -> propulsion_thrust, this -> specific_impulse, this -> fuel_mass);
        bool can_travel_2 =  can_travel(start_orbit, end_orbit, sc2.mass, sc2.propulsion_thrust, sc2.specific_impulse, sc2.fuel_mass);
        if (can_travel_1){
            if (can_travel_2){
                return this -> mass > sc2.mass;
            } else return false; 
        } else if (can_travel_2){
            return true;
        } return this -> propulsion_thrust < sc2.propulsion_thrust;  
    }
};


void menu(); //загаловок функции menu, чтобы она была доступна раньше определения


//алгоритм быстрой сортировки
void quickSort(spacecraft arr[], int left, int right){
    int i = left, j = right; //правая граница массива
    spacecraft tmp; //временный объект для обмена значений
    spacecraft pivot = arr[(left + right) / 2]; //разрешающий элемент (середина массива)
    while (i <= j){ //пока границы не сомкнутся 
        while (arr[i] < pivot) i++; //если левый элемент меньше разрешающего, увеличиваем левую границу
        while (arr[j] > pivot) j--; //если правый элемент меньше разрешающего, уменьшаем правую границу
        if (i <= j){ //если левая граница меньше или равна правой
            //меняем arr[i] и arr[j] местами
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++; //увеличиваем левую границу
            j--; //уменьшаем правую
        }
    }
    //если осталось что сортировать рекурсивно вызываем quickSort
    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}


void new_sc_array(){ //создание массива КА и запись в файл
    cout << "Введите количество космических аппаратов: ";
    int sc_number; //размер массива
    cin >> sc_number; 
    sc_number = abs(sc_number);
    spacecraft *sc_array = new spacecraft[sc_number]; //создаем динамический массив нужного размера
    spacecraft sc;
    for(int i = 0; i < sc_number; ++i){
        cout << "Космический аппарат #" << i + 1 << endl;
        cin >> sc_array[i]; //вводим элемент массива с помощью переопределенного оператора ввода
    }
    cout << endl << endl;
    while (true){ //бесконечный цикл
        cout << "Введите радиусы начальной и целевой орбиты: ";
        cin >> start_orbit >> end_orbit;
        start_orbit = abs(start_orbit);
        end_orbit = abs(end_orbit);
        if(start_orbit < 120 || end_orbit < 120) cout << "Орбита должна быть выше 120 км!" << endl; //если начальная или конечная орбита меньше 120,
                                                                                                    //выводим сообщение об ошибке
        else break; //иначе прерываем цикл
    }
    start_orbit += EARTH_RADIUS; //прибавляем радиус земли к радиусу орбит
    end_orbit += EARTH_RADIUS;
    quickSort(sc_array, 0, sc_number - 1); //сортируем массив
    cout << endl << endl;
    for(int i = 0; i < sc_number; ++i) cout << sc_array[i]; //выводим элемент массива с помощью переопределенного оператора ввода
    string filename; //название файла
    cout << "Введите название файла для записи: ";
    cin >> filename;
    ofstream out(filename, ios::binary|ios::out); //создаем поток, открывая файл на запись в бинарном режиме
    out.write((char*)&sc_number, sizeof(int)); //записываем колличество элементов массива
    out.write((char*)sc_array, sc_number * sizeof(spacecraft)); //записываем массив, колличество элементов умноженное на размер одного из них – размер массива
    out.close(); //закрываем файл
    delete [] sc_array; //освобождаем память
    ID = 1; //снова приравниваем ID единице, чтобы мжно было создать новый массив КА
    cout << endl << endl;
    menu(); //вызывем меню
}


void read_sc_array(){ //чтение массива из бинарного файла
    string filename;
    while(true){ //бесконечный цикл
        cout << "Введите название файла для чтения: ";
        cin >> filename;
        fstream in(filename, ios::binary|ios::in);
        if (in == NULL) cout << "Этого файла не существует!" << endl; //Проверка на существование файла, иначе выведется мусор
        else break; //прерываем если файл существует
    }
    fstream in(filename, ios::binary|ios::in); //создаем поток, открывая файл на чтение в бинарном режиме
    int sc_number; 
    in.read((char*)&sc_number, sizeof(int)); //читаем кол-во элементов массива в sc_number
    spacecraft *sc_array = new spacecraft[sc_number]; //создаем массив нужного массива
    in.read((char*)sc_array, sc_number * sizeof(spacecraft)); //читаем его из бинарного файла
    in.close(); //закрываем файл
    for(int i = 0; i < sc_number; ++i) cout << sc_array[i]; //выводим массив
    cout << endl << endl;
    menu(); //вызываем меню
}


void menu(){
    int menu_i; //пункт меню
    cout << "1. Создать новый массив КА" << endl;
    cout << "2. Прочитать массив КА из бинарного файла" << endl;
    cout << "3. Завершить программу" << endl;
    cout << "Выберите пункт меню: ";
    cin >> menu_i;
    menu_i = abs(menu_i);
    switch (menu_i){ //в зависимости от выбраного пункта
        case(1): new_sc_array(); break; //создаем новый массив
        case(2): read_sc_array(); break; //читаем массив из бинарного файла
        case(3): exit(EXIT_SUCCESS); break; //успешно завершаем команду
        default: cout << "Такого пункта в меню нет" << endl; menu(); //если пункта в меню нет выводим сообщение об ошибке и снова вызываем menu
    }
}


int main(){
    setlocale(LC_ALL, "Russian"); //добавляем поддержку кирилицы в консоль
    cout << "Все введенные числа берутся по модулю!" << endl; //для того чтобы не надо было проверять каждую переменную на отрицательность
    menu(); //вызываем menu
    return 0; 
}
