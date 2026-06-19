#include "AttendanceSystem.h"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

static string trimInput(const string& s)
{
    int start = 0;
    int end = (int)s.length() - 1;

    while (start <= end && (s[start] == ' ' || s[start] == '\t')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t')) end--;

    if (start > end) return "";
    return s.substr(start, end - start + 1);
}

static int docLuaChonMenuChinh(const string& line)
{
    string input = trimInput(line);

    if (input == "0") return 0;
    if (input.empty()) return -1;

    int len = (int)input.length();
    for (int i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return -1;
        }
    }

    int value = atoi(input.c_str());
    if (value == 0) return -1;
    return value;
}

int main()
{
	HeThongDiemDanh heThong;
    int luaChon;

	do {
        system("cls");
        cout << endl;
        for (int i=0; i<50; i++) cout<<"=";
        cout << endl;
        cout << "    HE THONG QUAN LY DIEM DANH LOP HOC" << endl;
        for (int i=0; i<50; i++) cout<<"=";
        cout << endl;
        cout << "  1. Quan ly lop hoc" << endl;
        cout << "  2. Quan ly sinh vien" << endl;
		cout << "  3. Diem danh" << endl;
        cout << "  4. Tim kiem diem danh" << endl;
        cout << "  5. Bao cao & Thong ke" << endl;
        cout << "  0. Thoat chuong trinh" << endl;
        for (int i=0; i<50; i++) cout<<"=";
        cout << endl;

        string line;
        cout << "  Lua chon cua ban: ";
        getline(cin, line);
        luaChon = docLuaChonMenuChinh(line);

        switch(luaChon) {
            case 1: 
                heThong.menuQuanLyLop(); 
                break;
            case 2: 
                heThong.menuQuanLySV(); 
                break;
            case 3: heThong.menuDiemDanh(); break;
            case 4: heThong.menuTimKiem(); break;
            case 5: heThong.menuBaoCao(); break;
            case 0:
                cout << endl << "  Cam on da su dung chuong trinh!" << endl;
                cout << "  Du lieu da duoc luu tu dong." << endl;
                cout << endl;
                break;
            default:
                cout << "  [LOI] Lua chon khong hop le! Vui long chon 0-5." << endl;
                cout << "  Nhan Enter de tiep tuc...";
                getline(cin, line);
        }
	} while(luaChon != 0);

    return 0;
}
