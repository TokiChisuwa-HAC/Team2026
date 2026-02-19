#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// コンストラクタ
CsvReader::CsvReader(const std::string& file_name) {
    load(file_name);
}


// ファイルの読み込み
void CsvReader::load(const std::string& file_name) {
    // ファイルオープン
    std::ifstream file{ file_name };
    if (!file) throw std::runtime_error("CSVファイルがオープンできませんでした");
    rows_.clear();                     // 古いデータを消去
    std::string line;                  // 改行文字までの1行分の文字列
    while (std::getline(file, line)) { // 改行区切りで１行分読み込み
        std::stringstream ss{ line };  // １行分データをstringstreamに格納
        std::string value;             // 各セルのデータ
        Row row;                       // １行分のデータ      
        while (std::getline(ss, value, ',')) { // カンマ区切りで読み込み
            row.push_back(value);      // 各セルのデータを行に追加
        }
        rows_.push_back(row);          // 1行分のデータを追加
    }
}


// データの取得(文字列で取得）
const std::string& CsvReader::get(int row, int column) const {
    return rows_[row][column];
}


// データの取得（int型に変換して取得）
int CsvReader::geti(int row, int column) const {
    return std::stoi(get(row, column));
}

// データの取得（float型に変換して取得）
float CsvReader::getf(int row, int column) const {
    return std::stof(get(row, column));
}


// 行数を返す
int CsvReader::rows() const {
    return (int)rows_.size();
}


// 列数を返す
int CsvReader::columns(int row) const {
    return (int)rows_[row].size();
}