#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;


double derivative_f(double x) {return 2 * x * x + 3 * x;} // функция вычислябщая произвдную


int main(){
	cout << "Численое решение дифференциального уравнения f´(x)=2x^2+3x методом Эйлера" << endl;
	cout.setf(ios_base::fixed); //фиксираванное число знаков после запятой
	cout.precision(5); //равное пяти
	cout << setw(5) << " #" << " | "<< setw(8) << "x" << " | "<< setw(8) << "f(x)" << " | " << "  f´(x)" << endl; //загаловок таблицы
																												  // с помощью setw() устанавливаем размер поля для вывода
	double f_x= - 0.4, begin = 0, end = 60, step = 0.001; //константы из условия
	int i = 0; //счетчик
	for(double x = begin; x <= end; x += step){
		f_x += + step * derivative_f(x); //вычисляем счетчик 
		cout << setw(5) << i << " | "<< setw(8) << x << " | "<< setw(8) << f_x << " | " << derivative_f(x) << endl; //выводим строку таблицы
		i++; //инкрементируем счетчик
	}
}
