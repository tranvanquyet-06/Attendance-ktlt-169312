#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

DynamicArray<string> FileManager::split(const string& s, char delimiter) {
    DynamicArray<string> tokens;
    string token = "";
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == delimiter) {
            tokens.push_back(token);
            token = "";
        } else {
            token += s[i];
        }
    }
    if (!token.empty() || !s.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

string FileManager::trim(const string& s) {
    if (s.empty()) return s;
    int start = 0, end = s.length() - 1;

    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n'))
        start++;

    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n'))
        end--;

    if (start > end) return "";
    return s.substr(start, end - start + 1);
}

void FileManager::docLopHoc(const string& filename, DynamicArray<LopHoc>& ds) {
    ds.clear();
    ifstream fin(filename.c_str());
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;

        DynamicArray<string> parts = split(line, ',');
        if (parts.size() >= 7) {
            LopHoc lh;
            lh.maLop      = trim(parts[0]);
            lh.tenLop     = trim(parts[1]);
            lh.thu        = trim(parts[2]);
            lh.tietBatDau = atoi(trim(parts[3]).c_str());
            lh.soTiet     = atoi(trim(parts[4]).c_str());
            lh.phong      = trim(parts[5]);
            lh.tongSoBuoi = atoi(trim(parts[6]).c_str());
            ds.push_back(lh);
        }
    }
    fin.close();
}

void FileManager::ghiLopHoc(const string& filename, const DynamicArray<LopHoc>& ds) {
    ofstream fout(filename.c_str());
    if (!fout.is_open()) {
        cout << "  [LOI] Khong the ghi file: " << filename << endl;
        return;
    }
    for (int i = 0; i < ds.size(); i++) {
        fout << ds[i].maLop << ","
             << ds[i].tenLop << ","
             << ds[i].thu << ","
             << ds[i].tietBatDau << ","
             << ds[i].soTiet << ","
             << ds[i].phong << ","
             << ds[i].tongSoBuoi << "\n";
    }
    fout.close();
}

void FileManager::docSinhVien(const string& filename, DynamicArray<SinhVien>& ds) {
    ds.clear();
    ifstream fin(filename.c_str());
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;

        DynamicArray<string> parts = split(line, ',');
        if (parts.size() >= 3) {
            SinhVien sv;
            sv.maSV  = trim(parts[0]);
            sv.hoTen = trim(parts[1]);
            sv.maLop = trim(parts[2]);
            ds.push_back(sv);
        }
    }
    fin.close();
}

void FileManager::ghiSinhVien(const string& filename, const DynamicArray<SinhVien>& ds) {
    ofstream fout(filename.c_str());
    if (!fout.is_open()) {
        cout << "  [LOI] Khong the ghi file: " << filename << endl;
        return;
    }
    for (int i = 0; i < ds.size(); i++) {
        fout << ds[i].maSV << ","
             << ds[i].hoTen << ","
             << ds[i].maLop << "\n";
    }
    fout.close();
}

void FileManager::docDiemDanh(const string& filename, DynamicArray<DiemDanh>& ds) {
    ds.clear();
    ifstream fin(filename.c_str());
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;

        DynamicArray<string> parts = split(line, ',');
        if (parts.size() >= 4) {
            DiemDanh dd;
            dd.maLop     = trim(parts[0]);
            dd.maSV      = trim(parts[1]);
            dd.ngay      = trim(parts[2]);
            dd.trangThai = atoi(trim(parts[3]).c_str());
            ds.push_back(dd);
        }
    }
    fin.close();
}

void FileManager::ghiDiemDanh(const string& filename, const DynamicArray<DiemDanh>& ds) {
    ofstream fout(filename.c_str());
    if (!fout.is_open()) {
        cout << "  [LOI] Khong the ghi file: " << filename << endl;
        return;
    }
    for (int i = 0; i < ds.size(); i++) {
        fout << ds[i].maLop << ","
             << ds[i].maSV << ","
             << ds[i].ngay << ","
             << ds[i].trangThai << "\n";
    }
    fout.close();
}
