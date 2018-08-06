#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


const double eps = 0.001, delta = 0.001 * 0.25; //константы


double f(double x) {return pow(x, 3) / ((x + 2) * (x + 3));} //функция возращает f(x)


double find_minimun(int i, double begin, double end){ //функция ищет минимум до тех пор пока абсолютное значение begin-end не станет меньше эпсилон
	double x = (begin + end) / 2; //вычисляем x
	if(f(x - delta) < f(x + delta)) end = x + delta; //если f(x - delta) < f(x + delta) изменяем end (конец отрезка)
	else begin = x - delta; //иначе изменяем begin (начало отрезка)
	cout << setw(4) << i << " |"<< setw(10) << x << " | " << begin << " | " << end << endl; //выводим строку таблицы
	if(fabs(begin - end) < eps) return x; //если абсолютное значение begin-end меньше эпсилон возвращаем x (точка минимума)
	else return find_minimun(i + 1, begin, end); //иначе снова вызываем find_minimum, c увеличенным счетчиком и измененными границами отрезка
}

int main(){
	cout.setf(ios_base::fixed); //фиксираванное кол-во знаков после запятой 
	cout.precision(5); //равное 5
	cout << "Поиск локального минимума функции f(x)=x^3/((x+2)(x+3)) на интервале [-10, -3]" << endl; 
	int begin = -10, end = -3;
	cout << setw(4) << "#" << " |"<< setw(10) << "x" << " |" << setw(9) << "a" << " |" << setw(9) << "b" << endl;
	cout << find_minimun(1, begin, end) << " – локальный минимум функции f(x)=x^3/((x+2)(x+3)) на интервале [-10, -3]" << endl;
	return 0;
}