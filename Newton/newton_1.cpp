#include <iostream> 
#include <cmath> 
#include <iomanip>

using namespace std;


const double dx = 0.001, eps = 0.0001; //константы из условия задачи


double f(double x) {return pow(1.5, x) * (x - 0.5) * (x - 0.5) - 3;} //функция вычисляющая f(x)


double derivative_f(double x) {return (f(x + dx) - f(x - dx)) / (2 * dx);} //функция вычисляющая f'(x)


int main(){
	cout << "Решение уравнения f(x)=1.5^x(x-0.5)^2-3 методом Ньютона" << endl;
	cout.setf(ios_base::fixed); //фиксированное колличество знаков после запятой, равное...
	cout.precision(5); //5
	double x0 = 0.25; //начальное приближение будет изменяться по ходу рассчета, поэтому не константа
	int i = 1; //номер шага
	cout << setw(2) << "#"  << " |" << setw(10) << "f(x0)" << " |" << setw(10) << "x0" << endl; //выводим "загаловок" таблицы
																								// с помощью setw() устанавливаем размер поля для вывода
	while(fabs(f(x0)) > eps){ //пока абсолютное значение f(x0) больше эпсилон
		x0 -= f(x0) / derivative_f(x0); //вычисляем x0
		cout << setw(2) << i << " |" << setw(10) << f(x0)  << " |" << setw(10) << x0 << endl; //выводим номер шага, f(x0) и x0
		i++;  //увеличиваем номер шага
	}
	cout << x0 << " – корень уравнения f(x)=1.5^x*(x-0.5)^2-3" << endl; //последние значение x0 и есть корень уравнения
	return 0;
}