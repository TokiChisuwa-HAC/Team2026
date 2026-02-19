#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

class FileHandler {
private:
    std::string m_filepath; // ファイルパス
    std::fstream m_file;    // ファイルストリーム

public:
    FileHandler(const std::string& filepath);
    ~FileHandler();

    // コピー禁止
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    // ファイルが開かれているか
    bool is_open() const;
    // 特定の文字列を含む行を検索（文字列を返す）
    std::string findLine(const std::string& searchTerm);
    // 特定の文字列を含む行を検索（行番号を返す）
    int findLineNumber(const std::string& searchTerm);
    // 特定の文字列を含む行を検索し、末尾の数値を返す
    int getParami(const std::string& key);
    float getParamf(const std::string& key);

    // 指定した行の内容を書き換える
    bool rewriteLine(int lineNum, const std::string& newContent);
    // 特定の文字列を含む行を検索し、末尾の数値に引数の値を足す
    bool addParami(const std::string& key, int num);
    bool addParamf(const std::string& key, float num);
    // 特定の文字列を含む行を検索し、末尾の数値を変更する
    template<typename T>
    bool updateParam(const std::string& key, const T& newValue);

    // コンソールに全ての行を出力（デバッグ用）
    void printAllLines();
    // 初期データの書き込み
    void writeInitialContent(const std::vector<std::string>& content);
};

template<typename T>
bool FileHandler::updateParam(const std::string& key, const T& newValue) {
    if (!is_open()) return false;

    std::vector<std::string> lines;
    std::string line;

    // 全行を読み込む
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
    while (std::getline(m_file, line)) {
        lines.push_back(line);
    }

    // 新しい値を文字列に変換
    std::ostringstream oss;
    oss << newValue;
    std::string newValueStr = oss.str();

    // 対象の行を探して書き換える
    bool foundAndUpdated = false;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string& currentLine = lines[i];

        // キーが含まれているか
        size_t keyPos = currentLine.find(key);
        if (keyPos == std::string::npos) {
            continue;
        }

        // '=' が含まれているか
        size_t eqPos = currentLine.find('=');
        if (eqPos == std::string::npos) {
            continue;
        }

        // キーが '=' の前にあるか
        if (keyPos > eqPos) {
            continue;
        }

        // キーを取得
        std::string prefix = currentLine.substr(0, eqPos + 1);

        // 元の値の部分から、数値の前の空白を取得
        std::string originalValuePart = currentLine.substr(eqPos + 1);
        size_t firstCharPos = originalValuePart.find_first_not_of(" \t\n\r\f\v");

        std::string leadingWhitespace = " ";
        if (firstCharPos != std::string::npos) {
            // 元の数値の前に空白があった場合、それを維持する
            leadingWhitespace = originalValuePart.substr(0, firstCharPos);
        }

        // 新しい行を作成
        currentLine = prefix + leadingWhitespace + newValueStr;

        foundAndUpdated = true;
        break;
    }

    if (!foundAndUpdated) {
        std::cerr << "Warning: キー '" << key << "' がファイル内に見つかりませんでした。" << std::endl;
        return false;
    }

    // ファイルを上書きモードで開き直す
    m_file.close();
    m_file.open(m_filepath, std::ios::out | std::ios::trunc);

    if (!is_open()) {
        std::cerr << "Error: 書き込み用にファイルを開き直せませんでした。" << std::endl;
        m_file.open(m_filepath, std::ios::in | std::ios::out); // リカバリ試行
        return false;
    }

    // 保存しておいた内容をファイルに書き戻す
    for (size_t i = 0; i < lines.size(); ++i) {
        m_file << lines[i];
        if (i < lines.size() - 1) {
            m_file << '\n';
        }
    }

    // 再度開きなおす
    m_file.close();
    m_file.open(m_filepath, std::ios::in | std::ios::out);

    return is_open();
}

#endif