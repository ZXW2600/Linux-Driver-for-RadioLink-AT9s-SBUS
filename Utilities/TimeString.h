/**
 * @file TimeString.h
 * @author ZXW2001 (wuaikali@qq.com)
 * @brief a lib used for transform time_t to string in a certain form
 * @version 0.1
 * @date 2020-10-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#pragma once
#include <sstream>

#include <time.h>
#include <memory.h>

using namespace std;
namespace TimeString
{

    // time转格式化字符__H_TIME__STRING__teTime(const time_t &t, const string &format);
    std::string ShowDateTime(const tm &t, const char &dateDiv = '-', const char &timeDiv = ':');
    std::string ShowDateTime(const time_t &t, const char &dateDiv = '-', const char &timeDiv = ':');

    std::string ShowYMD(const time_t &t, const char &dateDiv = '-');
    std::string ShowHMS(const time_t &t, const char &timeDiv = ':');
    std::string ShowHM(const time_t &t, const char &timeDiv = ':');

    // 格式化字符串转time=====================================

    time_t mkgmtime(tm *pTm);
    time_t str2time(const string &dateStr, const string &format);

    time_t str2time(const string &dateStr, const char &dateDiv = '-', const char &timeDiv = ':');

    time_t str2date(const string &dateStr, const char &dateDiv = '-');
    // 使用====================================

    // int main() {
    //     time_t now = time(0);
    //     cout <<cTimeString::ShowDateTime(t)<<endl;

    //     system("pause");// 暂停以显示终端窗口
    //     return 0;
    // }

}; // namespace TimeString
