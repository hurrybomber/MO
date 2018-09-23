#pragma once
#ifndef SimplexTable_h
#define SimplexTable_h


#endif /* SimplexTable_h */

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>



struct Meta {
	std::string* meta_col;
	std::string* meta_row;
};

class SimplexTable {

	// table хранит симплекс таблицу
	double** table;
	Meta meta;
	int dimention_row;
	int dimention_col;
public:


	SimplexTable();

	
	auto Fill() -> void;

	
	auto get_dimention_row() -> int { return dimention_row; };
	auto get_dimention_col() -> int { return dimention_col; };
	
	~SimplexTable();

	
	auto check(int col) -> int;

	
	auto find_second_negative(int row) -> int;


	auto find_min_positive_ratio_si0_to_solv_col_element(int col) -> int;


	
	auto change_srk(int solv_row, int solv_col) ->void;
	auto change_srj(int solv_row, int solv_col) ->void;
	auto change_sjk(int solv_row, int solv_col) ->void;
	auto change_sij(int solv_row, int solv_col) ->void;

	
	auto change_markers(int solv_row, int solv_col) -> void;

	
	auto change(int solv_row, int solv_col) -> void;

	auto optimal() -> bool;
	auto iterate() -> void;
};
