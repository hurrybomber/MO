#include <iostream>
#include <limits>
#include <iomanip>

struct Table {
	unsigned int row = 0;
	unsigned int col = 0;
	double **T;
};
//     //1.  srk* = 1 / srk;
//     //2.  srj* = srj / srk, j = 0,n, j != k;
//     //3.  sjk* = - sik / srk, i = 1,m+1, i != r;
//     //4.  sij* = sij - (sik * srj) / srk, i = 1,m+1, i != r, j = 0,n, j != k;

////4.  sij* = sij - (sik * srj) / srk, i = 1,m+1, i != r, j = 0,n, j != k;
//auto change_sij(Table table, int solv_row, int solv_col) ->void{
//    for (unsigned int index = 0; index < table.col; index++)
//        for(unsigned int index_j = 0; index_j <table.row; index_j++)
//            if((index != solv_col) and (index_j != solv_row))
//                table.T[index][index_j] -= (table.T[index][solv_col]* table.T[solv_row][index_j]) / table.T[solv_row][solv_col];
//                }
//
////3.  sjk* = - sik / srk, i = 1,m+1, i != r;
//auto change_sjk(Table table, int solv_row, int solv_col) ->void{
//    for (unsigned int index = 0; index < table.row; index++)
//        if (index != solv_row)
//            table.T[index][solv_col] /= table.T[solv_row][solv_col]*-1;
//            }
//
////2.  srj* = srj / srk, j = 0,n, j != k;
//auto change_srj(Table table, int solv_row, int solv_col) ->void{
//    for (unsigned int index = 0; index < table.col; index++)
//        if (index != solv_col)
//            table.T[solv_row][index] /= table.T[solv_row][solv_col];
//            }
//
////1.  srk* = 1 / srk;
//auto change_srk(Table table, int solv_row, int solv_col) ->void{
//    table.T[solv_row][solv_col] = 1/ table.T[solv_row][solv_col];
//}

//Функции change совершают жордановы преобразования:
//     //1.  srk* = 1 / srk;
//     //2.  srj* = srj / srk, j = 0,n, j != k;
//     //3.  sjk* = - sik / srk, i = 1,m+1, i != r;
//     //4.  sij* = sij - (sik * srj) / srk, i = 1,m+1, i != r, j = 0,n, j != k;

double** change(Table table, int solving_col, int solving_row) {
	double resolving_element = table.T[solving_row][solving_col];
	table.T[solving_row][solving_col] = 1 / resolving_element;
	for (int i = 0; i < table.row; i++) {
		for (int j = 0; j < table.col; j++) {
			if (i == solving_row || j == solving_col)
				continue;
			table.T[i][j] = table.T[i][j] - table.T[i][solving_col] * table.T[solving_row][j] / resolving_element;
		}
	}

	for (int j = 0; j < table.col; j++) {
		if (j == solving_col)
			continue;
		table.T[solving_row][j] = table.T[solving_row][j] / resolving_element;
	}

	for (int i = 0; i < table.row; i++) {
		if (i == solving_row)
			continue;
		table.T[i][solving_col] = -table.T[i][solving_col] / resolving_element;
	}
	return table.T;
}
// Функция print печатает таблицу
void print(Table table) {
	for (int i = 0; i< table.row; i++) {
		for (int j = 0; j< table.col; j++) {
			std::cout << std::setw(20) << table.T[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
// search_for_negative_in_si0 - проверяет есть ли в столбце si0 отрицательные значения,
// если найдет, то вернет его индекс,
// если не найдет, но вернет -1.
int search_for_negative_in_si0(Table table) {
	for (unsigned int i = 0; i < table.row; i++)
		if (table.T[i][0] < 0)
			return i;
	return -1;
}

//search_solving_row - находит минимальное отношение элемента
// из столбца свободных членов к соответствующему эллементу из разрешающего столбца.
// если такого найдено не будет, то вернется -1.
auto search_solving_row(Table table, int &solving_row, int solving_col) -> int {
	double min_ratio = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < table.row; i++) {
		double ratio_temp = table.T[i][0] / table.T[i][solving_col];
		if (ratio_temp > 0 && ratio_temp < min_ratio) {
			min_ratio = ratio_temp;
			solving_row = i;
		}
	}
	return solving_row == std::numeric_limits<double>::max() ? -1 : solving_row;
}

int search_solving_col_for_optimal(Table table, int & solving_col) {
	for (int j = 1; j<table.col; j++)
		if (table.T[table.row - 1][j] > 0) {
			solving_col = j;
			return solving_col;
		}
	solving_col = 0;
	return 0;
}

// Функция search_solving_col осуществляет поиск разрешающего столбца
int search_solving_col(Table table, int row_with_negative_in_si0) {
	if (row_with_negative_in_si0 == -1) return -1;
	for (int j = 1; j < table.col; j++) {
		if (table.T[row_with_negative_in_si0][j] < 0)
			return j;
	}
	return -1;
}

// Функция is_there_negative_element_in_si0_ осуществляет проверку на наличие отрицательных элементов в si0
// Если не найдет вернет true
// Если найдет вернет false
bool is_there_negative_element_in_si0_(Table table) {
	for (int i = 0; i < table.row - 1; i++) {
		if (table.T[i][0] < 0)
			return false;
	}
	return true;
}

// inline-функции использованы для повторяющихся выводов
inline void bad_input() {
	std::cout << "Введено неверное значение!" << std::endl
		<< "Продолжите ввод с последенего верно введенного значения: ";
}
inline void nice_output(int counter, int solving_col, int solving_row) {
	std::cout << "Шаг #" << counter << std::endl
		<< "Номер разрешающей строки: " << solving_row << std::endl
		<< "Номер разрешающего столбца: " << solving_col << std::endl;
}
inline void no_solutions() {
	std::cout << "Решений нет." << std::endl;
}
int main() {
	setlocale(LC_ALL, "rus");
	Table table;
	double ** & T = table.T;
	unsigned int & row = table.row;
	unsigned int & col = table.col;

	unsigned int Number_of_variables = 3;
	unsigned int Number_of_restrictions = 3;

	row = Number_of_restrictions + 1;
	col = Number_of_variables + 1;

	T = new double *[row];
	for (int i = 0; i < row; i++)
		T[i] = new double[col] {0};
	// С - вектор коэффициентов ЦФ F
	std::cout << "Введите коэффициенты (вектор с): ";
	T[row - 1][0] = 0;
	
	for (unsigned int j = 1; j < col; j++) {
		std::cin >> T[row - 1][j];
		while (!std::cin) {
			bad_input();
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cin >> T[row - 1][j];
		}
	}
	//А - мастрица системы ограничений
	std::cout << "Введите матрицу A: ";
	for (unsigned int i = 0; i < row - 1; i++) {
		for (unsigned int j = 1; j < col; j++) {
			std::cin >> T[i][j];
			while (!std::cin) {
				bad_input();
				std::cin.clear();
				while (std::cin.get() != '\n');
				std::cin >> T[i][j];
			}
		}
	}
	//В - вектор правой части системы ограничений
	std::cout << "Введите вектор В(трансп.): ";
	for (unsigned int i = 0; i < row - 1; i++) {
		std::cin >> T[i][0];
		while (!std::cin) {
			bad_input();
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cin >> T[i][0];
		}
	}
	std::cout << "Cимплекс-таблица:" << std::endl;
	print(table);

	unsigned int counter = 0;
	bool f = true;
	bool Flag_optimal_solution_found = false;
	while (!Flag_optimal_solution_found) {

		counter++;
		bool flag = true;

		int solving_col;
		int solving_row;

		flag = is_there_negative_element_in_si0_(table);


		if (flag) {
			if (f) {
				std::cout << "Опорное решение найдено: " << std::endl;
				f = false;
			}
			solving_col = search_solving_col_for_optimal(table, solving_col);
			if (solving_col == 0) {
				std::cout << "Найденное решение является оптимальным." << std::endl
					<< "F= " << -T[row - 1][0] << std::endl;
				Flag_optimal_solution_found = true;
				break;
			}
		}
		else {
			int row_with_negative_in_si0 = search_for_negative_in_si0(table);
			solving_col = search_solving_col(table, row_with_negative_in_si0);
			if (solving_col == -1) {
				no_solutions();
				break;
				
			}
		}

		search_solving_row(table, solving_row, solving_col);
		if (solving_row == -1) {
			no_solutions();
			break;
		}

		//        change_sij(table, solving_row, solving_col);
		//        change_sjk(table, solving_row, solving_col);
		//        change_srj(table, solving_row, solving_col);
		//        change_srk(table, solving_row, solving_col);
		T = change(table, solving_col, solving_row);
		nice_output(counter, solving_col, solving_row);
		print(table);
		
	}

	for (int i = 0; i < row; i++)
		delete[] T[i];
	delete[] T;
	system("pause");
}
