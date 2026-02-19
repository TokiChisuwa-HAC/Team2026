#include "FileHandler.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

FileHandler::FileHandler(const std::string& filepath) : m_filepath(filepath) {
    // ファイルを開く
    m_file.open(m_filepath, std::ios::in | std::ios::out);

    // ファイルが存在しない場合
    if (!m_file.is_open()) {
        // 書き込みモードで開いてファイルを作成
        m_file.open(m_filepath, std::ios::out);
        if (m_file.is_open()) {
            m_file.close();
            // ファイルを開きなおす
            m_file.open(m_filepath, std::ios::in | std::ios::out);
        }
    }

    if (!m_file.is_open()) {
        std::cerr << "ファイルを開けませんでした: " << m_filepath << std::endl;
    }
}

FileHandler::~FileHandler() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

// ファイルが開かれているか
bool FileHandler::is_open() const {
    return m_file.is_open();
}

// 特定の文字列を含む行を検索（文字列を返す）
std::string FileHandler::findLine(const std::string& searchTerm) {
    if (!is_open()) return "";

    std::string line;

    // ストリームの状態をクリア
    m_file.clear();
    // ファイルポインタを先頭に戻す
    m_file.seekg(0, std::ios::beg);

    while (std::getline(m_file, line)) {
        if (line.find(searchTerm) != std::string::npos) {
            return line;
        }
    }
    // 見つからなかったら空行を返す
    return "";
}

// 特定の文字列を含む行を検索（行番号を返す）
int FileHandler::findLineNumber(const std::string& searchTerm) {
    if (!is_open()) return -1;

    std::string line;
    int currentLineNumber = 1; // 行番号は1から開始

    // ストリームの状態をクリア
    m_file.clear();
    // ファイルポインタを先頭に戻す
    m_file.seekg(0, std::ios::beg);

    while (std::getline(m_file, line)) {
        // 文字列を検索
        if (line.find(searchTerm) != std::string::npos) {
            return currentLineNumber;
        }
        currentLineNumber++;
    }
    // 見つからなかったら行番号-1を返す
    return -1;
}

int FileHandler::getParami(const std::string& key) {
    // ファイルが見つからなかった場合
    if (!is_open()) return -1;

    std::string line;
    m_file.clear();
    m_file.seekg(0, std::ios::beg);

    while (std::getline(m_file, line)) {
        // キーが含まれているか確認
        size_t keyPos = line.find(key);
        if (keyPos == std::string::npos) continue;
        // '=' が含まれているか確認
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;
        // キーが '=' より前にあるか確認
        if (keyPos > eqPos) continue;

        // '=' の後の数値を抽出
        std::string valueStr = line.substr(eqPos + 1);

        // valueStrの前後の空白を削除
        size_t first = valueStr.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) {
            continue;
        }
        size_t last = valueStr.find_last_not_of(" \t\n\r\f\v");
        std::string trimmedValueStr = valueStr.substr(first, (last - first + 1));


        // 文字列をintに変換
        try {
            int value = std::stoi(trimmedValueStr);
            return value;
        }
        catch (const std::invalid_argument& e) {
            continue;
        }
        catch (const std::out_of_range& e) {
            continue;
        }
    }
    // 見つからなかった場合-1を返す
    return -1;
}

float FileHandler::getParamf(const std::string& key) {
    // ファイルが見つからなかった場合
    if (!is_open()) return -1.0f;

    std::string line;
    m_file.clear();
    m_file.seekg(0, std::ios::beg);

    while (std::getline(m_file, line)) {
        // キーが含まれているか確認
        size_t keyPos = line.find(key);
        if (keyPos == std::string::npos) continue;
        // '=' が含まれているか確認
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;
        // キーが '=' より前にあるか確認
        if (keyPos > eqPos) continue;

        // '=' の後の数値を抽出
        std::string valueStr = line.substr(eqPos + 1);

        // valueStrの前後の空白を削除
        size_t first = valueStr.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) continue;
        size_t last = valueStr.find_last_not_of(" \t\n\r\f\v");
        std::string trimmedValueStr = valueStr.substr(first, (last - first + 1));

        // 文字列をfloatに変換
        try {
            float value = std::stof(trimmedValueStr);
            return value;
        }
        catch (const std::invalid_argument& e) {
            continue;
        }
        catch (const std::out_of_range& e) {
            continue;
        }
    }

    // 見つからなかった場合-1を返す
    return -1.0f;
}

// 指定した行の内容を書き換える
bool FileHandler::rewriteLine(int lineNum, const std::string& newContent) {
    if (!is_open() || lineNum < 1) return false;

    std::vector<std::string> lines;
    std::string line;

    // 全行を読み込んでおく
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
    while (std::getline(m_file, line)) {
        lines.push_back(line);
    }

    // 指定された行番号が有効かチェック
    if (lineNum > static_cast<int>(lines.size())) {
        std::cerr << "Error: 行番号が範囲外です (Total: " << lines.size() << ", Requested: " << lineNum << ")" << std::endl;
        return false;
    }

    // 内容を書き換える
    lines[lineNum - 1] = newContent;

    // ファイルを上書きモードで開き直す
    m_file.close();
    m_file.open(m_filepath, std::ios::out | std::ios::trunc);

    if (!is_open()) {
        std::cerr << "Error: 書き込み用にファイルを開き直せませんでした。" << std::endl;
        m_file.open(m_filepath, std::ios::in | std::ios::out);
        return false;
    }

    // 書き換えた内容をファイルに出力
    for (size_t i = 0; i < lines.size(); ++i) {
        m_file << lines[i];
        if (i < lines.size() - 1) {
            m_file << '\n';
        }
    }

    // ファイルを開きなおす
    m_file.close();
    m_file.open(m_filepath, std::ios::in | std::ios::out);

    return is_open();
}

bool FileHandler::addParami(const std::string& key, int num) {
    // キーが存在するか確認する
    if (findLineNumber(key) == -1) {
        std::cerr << "Error: キー '" << key << "' が見つかりません。" << std::endl;
        return false;
    }

    try {
        // 現在の値を取得する
        int currentValue = getParami(key);

        // 加算する
        int newValue = currentValue + num;

        // 新しい値を書き込む
        return updateParam(key, newValue);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: 数値の変換または書き込みに失敗しました: " << e.what() << std::endl;
        return false;
    }
}

bool FileHandler::addParamf(const std::string& key, float num) {
    // キーが存在するか確認する
    if (findLineNumber(key) == -1) {
        std::cerr << "Error: キー '" << key << "' が見つかりません。" << std::endl;
        return false;
    }

    try {
        // 現在の値を取得する
        float currentValue = getParamf(key);

        // 加算する
        float newValue = currentValue + num;

        // 新しい値を書き込む
        return updateParam(key, newValue);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: 数値の変換または書き込みに失敗しました: " << e.what() << std::endl;
        return false;
    }
}

// コンソールにすべての行を出力
void FileHandler::printAllLines() {
    if (!is_open()) return;

    std::string line;
    m_file.clear();
    m_file.seekg(0, std::ios::beg);
    std::cout << "--- File Content (" << m_filepath << ") ---" << std::endl;
    int count = 1;
    while (std::getline(m_file, line)) {
        std::cout << count++ << ": " << line << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;
}

// 初期データの書き込み
void FileHandler::writeInitialContent(const std::vector<std::string>& content) {
    if (!is_open()) return;

    // ファイルポインタを先頭に戻す
    m_file.seekp(0, std::ios::beg);
    for (size_t i = 0; i < content.size(); ++i) {
        m_file << content[i];
        if (i < content.size() - 1) {
            m_file << '\n';
        }
    }
    m_file.flush();
}