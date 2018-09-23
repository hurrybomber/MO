#include "SimplexTable.hpp"
#include <stdexcept>



int SimplexTable::check(int col) {
	for ( int index = 0; index < get_dimention_row() - 1; index++)
		if (table[index][col] < 0)
			return index;
	return -1;
}


auto SimplexTable::find_second_negative(int row) -> int {
	for (int index = 1; index <  get_dimention_col(); index++)
		if (table[row][index] < 0) return index;
	return 0;
}


auto SimplexTable::find_min_positive_ratio_si0_to_solv_col_element(int col) -> int {
	double min_ratio = std::numeric_limits<double>::max();
	int min_index = -1;
	for ( int index = 0; index < get_dimention_row() - 1; index++) {
		double ratio = table[0][index] / table[col][index];
		if ((ratio < min_ratio) & (ratio > 0)) {
			min_ratio = ratio;
			min_index = index;
		}
	}
	return min_index;
}


SimplexTable::SimplexTable() :
	dimention_col{ 4 },
	dimention_row{ 4 },
	table{ nullptr }
	//meta{{"si0", "x1", "x2", "x3"}, {"x4", "x5", "x6", "F"}}
{
	meta.meta_col = new std::string[4];
	meta.meta_row = new std::string[4];

	meta.meta_col[1] = "si0";
	meta.meta_col[2] = "x1";
	meta.meta_col[3] = "x2";
	meta.meta_col[4] = "x3";

	meta.meta_row[1] = "x4";
	meta.meta_row[2] = "x5";
	meta.meta_row[3] = "x6";
	meta.meta_row[4] = "F";

	table = new double*[dimention_row];
	for (int index = 0; index < dimention_row; index++)
		table[index] = new double[dimention_col];

}


void SimplexTable::Fill() {
	std::cout << "Введите данные из задания:" << std::endl
		<< "Введите вектор с" << std::endl;

	for (auto index = 1; index < dimention_col; index++)
		std::cin >> table[index][dimention_row - 1];

	table[0][dimention_row - 1] = 0;

	std::cout << "Введите матрицу А (столбец за столбцом)" << std::endl;

	for (auto index = 1; index < dimention_col; index++)
		for (auto index_row = 0; index_row < dimention_row - 1; index_row++)
			std::cin >> table[index][index_row];

	std::cout << "Введите вектор В(транспонированный)" << std::endl;

	for (auto index = 0; index < dimention_row - 1; index++)
		std::cin >> table[0][index];
}

SimplexTable::~SimplexTable() {
	for (auto index = 0; index < dimention_row; index++)
		delete[] table[index];
	delete[] table;
}



auto SimplexTable::change_sij(int solv_row, int solv_col) ->void {
	for ( int index = 0; index < get_dimention_col(); index++)
		for ( int index_j = 0; index_j < get_dimention_row(); index_j++)
			if ((index != solv_col)  &  (index_j != solv_row))
				table[index][index_j] -= (table[index][solv_col]
					* table[solv_row][index_j]) / table[solv_row][solv_col];
}


auto SimplexTable::change_sjk(int solv_row, int solv_col) ->void {
	for ( int index = 0; index < dimention_row; index++)
		if (index != solv_row)
			table[index][solv_col] /= table[solv_row][solv_col] * -1;
}


auto SimplexTable::change_srj(int solv_row, int solv_col) ->void {
	for ( int index = 0; index < dimention_col; index++)
		if (index != solv_col)
			table[solv_row][index] /= table[solv_row][solv_col];
}


auto SimplexTable::change_srk(int solv_row, int solv_col) ->void {
	table[solv_row][solv_col] = 1 / table[solv_row][solv_col];
}

auto optimal() -> bool {


	return false;
}
auto SimplexTable::iterate() -> void {
	int row_with_negative = check(0);
	if ((row_with_negative == -1) || (row_with_negative == 4))
		std::cout << "Найдено опорное решение" << std::endl;
	int solving_col = find_second_negative(row_with_negative);
	if (solving_col == -1)
		throw std::logic_error("решения нет: найти разшающий столбец не найден");
	int solving_row = find_min_positive_ratio_si0_to_solv_col_element(solving_col);
	if (solving_row == -1)
		throw std::logic_error("решения нет: найти разрешающую строку не удалось");
	change(solving_row, solving_col);
}
