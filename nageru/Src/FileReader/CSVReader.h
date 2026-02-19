#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <vector>
#include <string>

class CsvReader {
public:
    // コンストラクタ
    CsvReader() = default;
    // コンストラクタ
    CsvReader(const std::string& file_name);
    // ファイルの読み込み
    void load(const std::string& file_name);
    // データの取得(文字列で取得）
    const std::string& get(int row, int column) const;
    // データの取得（int型に変換して取得）
    int geti(int row, int column) const;
    // データの取得（float型に変換して取得）
    float getf(int row, int column) const;
    // 行数を返す
    int rows() const;
    // 列数を返す
    int columns(int row = 0) const;

private:
    // １行
    using Row = std::vector<std::string>;
    // 行の配列
    using Rows = std::vector<Row>;
    // CSVデータ
    Rows rows_;
};

#endif