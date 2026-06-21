#include "AttendanceSystem.h"
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

static void nhanEnter() {
    cout << "  Nhan Enter de tiep tuc...";
    string tmp;
    getline(cin, tmp);
}

static int docSoNguyen(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);

    while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
        line.erase(0, 1);

    if (line.empty()) return -9999;

    int start = 0;
    if (line[0] == '-') start = 1;

    for (int i = start; i < (int)line.length(); i++) {
        if (line[i] < '0' || line[i] > '9')
            return -9999;
    }
    return atoi(line.c_str());
}

static char docXacNhan(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    if (line.empty()) return 'N';
    return line[0];
}

static int docSoNguyenTrongKhoang(const string& prompt, int min, int max) {
    int val;
    while (true) {
        val = docSoNguyen(prompt);
        if (val >= min && val <= max)
            return val;
        cout << "  [LOI] Vui long nhap so tu " << min << " den " << max << "!" << endl;
    }
}

static string docChuoiKhongRong(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        while (!line.empty() && (line[0] == ' ' || line[0] == '\t'))
            line.erase(0, 1);
        while (!line.empty() && (line[line.length() - 1] == ' ' || line[line.length() - 1] == '\t'))
            line.erase(line.length() - 1, 1);
        if (!line.empty())
            return line;
        cout << "  [LOI] Khong duoc de trong! Vui long nhap lai." << endl;
    }
}

static bool kiemTraNgay(const string& ngay) {
    if (ngay.length() != 10) return false;
    if (ngay[2] != '/' || ngay[5] != '/') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (ngay[i] < '0' || ngay[i] > '9') return false;
    }

    int ngayNum = atoi(ngay.substr(0, 2).c_str());
    int thang = atoi(ngay.substr(3, 2).c_str());
    int nam = atoi(ngay.substr(6, 4).c_str());

    if (thang < 1 || thang > 12) return false;
    if (ngayNum < 1 || ngayNum > 31) return false;
    if (nam < 2000 || nam > 2100) return false;

    int soNgayThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((nam % 4 == 0 && nam % 100 != 0) || nam % 400 == 0)
        soNgayThang[2] = 29;
    if (ngayNum > soNgayThang[thang]) return false;

    return true;
}

static string docNgayHopLe(const string& prompt) {
    string ngay;
    while (true) {
        cout << prompt;
        getline(cin, ngay);
        if (kiemTraNgay(ngay))
            return ngay;
        cout << "  [LOI] Ngay khong hop le! Nhap theo dinh dang dd/mm/yyyy" << endl;
    }
}

static string ngayToSortKey(const string& ngay) {
    if (ngay.length() >= 10)
        return ngay.substr(6, 4) + ngay.substr(3, 2) + ngay.substr(0, 2);
    return ngay;
}

static bool soSanhNgay(const string& a, const string& b) {
    return ngayToSortKey(a) < ngayToSortKey(b);
}

struct ThongKeVang {
    string maSV;
    string hoTen;
    string maLop;
    int tongVang;
    int vangKP;
    int tongBuoi;
};

static bool soSanhVangGiamDan(const ThongKeVang& a, const ThongKeVang& b) {
    return a.tongVang > b.tongVang;
}

HeThongDiemDanh::HeThongDiemDanh() {
    system("mkdir data 2>nul");
    fileLopHoc = "data/lophoc.txt";
    fileSinhVien = "data/sinhvien.txt";
    fileDiemDanh = "data/diemdanh.txt";
    docTatCa();
}

void HeThongDiemDanh::docTatCa() {
    FileManager::docLopHoc(fileLopHoc, dsLopHoc);
    FileManager::docSinhVien(fileSinhVien, dsSinhVien);
    FileManager::docDiemDanh(fileDiemDanh, dsDiemDanh);
}

void HeThongDiemDanh::luuTatCa() {
    FileManager::ghiLopHoc(fileLopHoc, dsLopHoc);
    FileManager::ghiSinhVien(fileSinhVien, dsSinhVien);
    FileManager::ghiDiemDanh(fileDiemDanh, dsDiemDanh);
}

int HeThongDiemDanh::timLopHoc(const string& maLop) {
    for (int i = 0; i < dsLopHoc.size(); i++) {
        if (dsLopHoc[i].maLop == maLop)
            return i;
    }
    return -1;
}

int HeThongDiemDanh::timSinhVien(const string& maSV) {
    for (int i = 0; i < dsSinhVien.size(); i++) {
        if (dsSinhVien[i].maSV == maSV)
            return i;
    }
    return -1;
}

DynamicArray<int> HeThongDiemDanh::laySVTheoLop(const string& maLop) {
    DynamicArray<int> result;
    for (int i = 0; i < dsSinhVien.size(); i++) {
        if (dsSinhVien[i].maLop == maLop) {
            result.push_back(i);
        }
    }
    return result;
}

DynamicArray<string> HeThongDiemDanh::layNgayDiemDanh(const string& maLop) {
    DynamicArray<string> ngays;
    for (int i = 0; i < dsDiemDanh.size(); i++) {
        if (dsDiemDanh[i].maLop == maLop) {
            if (!daTonTai(ngays, dsDiemDanh[i].ngay)) {
                ngays.push_back(dsDiemDanh[i].ngay);
            }
        }
    }
    return ngays;
}

bool HeThongDiemDanh::daTonTai(const DynamicArray<string>& ds, const string& val) {
    for (int i = 0; i < ds.size(); i++) {
        if (ds[i] == val)
            return true;
    }
    return false;
}

void HeThongDiemDanh::veDuongKe(int doDai) {
    for (int i = 0; i < doDai; i++)
        cout << "=";
    cout << endl;
}

void HeThongDiemDanh::hienThiTieuDe(const string& tieuDe) {
    cout << endl;
    veDuongKe(60);
    cout << "  " << tieuDe << endl;
    veDuongKe(60);
}

void HeThongDiemDanh::menuQuanLyLop() {
    int luaChon;
    do {
        system("cls");
        hienThiTieuDe("QUAN LY LOP HOC");
        cout << "  1. Them lop hoc" << endl;
        cout << "  2. Sua thong tin lop" << endl;
        cout << "  3. Xoa lop hoc" << endl;
        cout << "  4. Hien thi danh sach lop" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        veDuongKe(60);
        luaChon = docSoNguyen("  Lua chon: ");

        switch (luaChon) {
            case 1:  themLopHoc();          break;
            case 2:  suaLopHoc();           break;
            case 3:  xoaLopHoc();           break;
            case 4:  hienThiDanhSachLop();  break;
            case 0:  break;
            default:
                cout << "  [LOI] Lua chon khong hop le!" << endl;
                nhanEnter();
        }
    } while (luaChon != 0);
}

void HeThongDiemDanh::menuQuanLySV() {
    int luaChon;
    do {
        system("cls");
        hienThiTieuDe("QUAN LY SINH VIEN");
        cout << "  1. Them sinh vien" << endl;
        cout << "  2. Sua thong tin sinh vien" << endl;
        cout << "  3. Xoa sinh vien" << endl;
        cout << "  4. Hien thi danh sach sinh vien" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        veDuongKe(60);
        luaChon = docSoNguyen("  Lua chon: ");

        switch (luaChon) {
            case 1:  themSinhVien();          break;
            case 2:  suaSinhVien();           break;
            case 3:  xoaSinhVien();           break;
            case 4:  hienThiDanhSachSV();     break;
            case 0:  break;
            default:
                cout << "  [LOI] Lua chon khong hop le!" << endl;
                nhanEnter();
        }
    } while (luaChon != 0);
}

void HeThongDiemDanh::menuDiemDanh() {
    int luaChon;
    do {
        system("cls");
        hienThiTieuDe("DIEM DANH");
        cout << "  1. Diem danh theo lop" << endl;
        cout << "  2. Sua diem danh" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        veDuongKe(60);
        luaChon = docSoNguyen("  Lua chon: ");

        switch (luaChon) {
            case 1:  diemDanhTheoLop();  break;
            case 2:  suaDiemDanh();      break;
            case 0:  break;
            default:
                cout << "  [LOI] Lua chon khong hop le!" << endl;
                nhanEnter();
        }
    } while (luaChon != 0);
}

void HeThongDiemDanh::menuTimKiem() {
    int luaChon;
    do {
        system("cls");
        hienThiTieuDe("TIM KIEM DIEM DANH");
        cout << "  1. Tim theo ngay cua lop" << endl;
        cout << "  2. Tim theo ma sinh vien" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        veDuongKe(60);
        luaChon = docSoNguyen("  Lua chon: ");

        switch (luaChon) {
            case 1:  timKiemTheoNgay();   break;
            case 2:  timKiemTheoMaSV();   break;
            case 0:  break;
            default:
                cout << "  [LOI] Lua chon khong hop le!" << endl;
                nhanEnter();
        }
    } while (luaChon != 0);
}

void HeThongDiemDanh::menuBaoCao() {
    int luaChon;
    do {
        system("cls");
        hienThiTieuDe("BAO CAO & THONG KE");
        cout << "  1. Ty le vang mat tung sinh vien" << endl;
        cout << "  2. Canh bao cam thi (vang > 20%)" << endl;
        cout << "  3. Thong ke si so theo buoi" << endl;
        cout << "  4. Danh sach sinh vien vang nhieu nhat" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        veDuongKe(60);
        luaChon = docSoNguyen("  Lua chon: ");

        switch (luaChon) {
            case 1:  tinhTyLeVang();           break;
            case 2:  canhBaoCamThi();          break;
            case 3:  thongKeSiSo();            break;
            case 4:  danhSachVangNhieuNhat();  break;
            case 0:  break;
            default:
                cout << "  [LOI] Lua chon khong hop le!" << endl;
                nhanEnter();
        }
    } while (luaChon != 0);
}

void HeThongDiemDanh::themLopHoc() {
    system("cls");
    hienThiTieuDe("THEM LOP HOC MOI");

    LopHoc lh;
    lh.maLop = docChuoiKhongRong("  Nhap ma lop: ");

    if (timLopHoc(lh.maLop) != -1) {
        cout << endl << "  [LOI] Ma lop \"" << lh.maLop << "\" da ton tai!" << endl;
        nhanEnter();
        return;
    }

    lh.tenLop = docChuoiKhongRong("  Nhap ten lop/mon hoc: ");
    lh.thu = docChuoiKhongRong("  Nhap thu hoc (VD: Thu 2): ");
    lh.tietBatDau = docSoNguyenTrongKhoang("  Nhap tiet bat dau (1-12): ", 1, 12);
    lh.soTiet = docSoNguyenTrongKhoang("  Nhap so tiet moi buoi (1-6): ", 1, 6);
    lh.phong = docChuoiKhongRong("  Nhap phong hoc: ");
    lh.tongSoBuoi = docSoNguyenTrongKhoang("  Nhap tong so buoi trong ky (1-50): ", 1, 50);

    cout << endl << "  Thong tin lop hoc:" << endl;
    cout << "    Ma lop:     " << lh.maLop << endl;
    cout << "    Ten lop:    " << lh.tenLop << endl;
    cout << "    Thu:        " << lh.thu << endl;
    cout << "    Tiet:       " << lh.tietBatDau << " (" << lh.soTiet << " tiet)" << endl;
    cout << "    Phong:      " << lh.phong << endl;
    cout << "    Tong buoi:  " << lh.tongSoBuoi << endl;

    char xn = docXacNhan("  Xac nhan them lop nay? (Y/N): ");
    if (xn != 'Y' && xn != 'y') {
        cout << "  Da huy thao tac them lop." << endl;
        nhanEnter();
        return;
    }

    dsLopHoc.push_back(lh);
    FileManager::ghiLopHoc(fileLopHoc, dsLopHoc);

    cout << endl << "  [OK] Them lop hoc \"" << lh.maLop << "\" thanh cong!" << endl;
    nhanEnter();
}

void HeThongDiemDanh::suaLopHoc() {
    system("cls");
    hienThiTieuDe("SUA THONG TIN LOP HOC");

    if (dsLopHoc.empty()) {
        cout << "  Chua co lop hoc nao trong he thong." << endl;
        nhanEnter();
        return;
    }

    cout << "  Danh sach lop hien co:" << endl;
    for (int i = 0; i < dsLopHoc.size(); i++) {
        cout << "    " << i + 1 << ". " << dsLopHoc[i].maLop
             << " - " << dsLopHoc[i].tenLop << endl;
    }
    cout << endl;

    string maLop;
    cout << "  Nhap ma lop can sua: ";
    getline(cin, maLop);

    int idx = timLopHoc(maLop);
    if (idx == -1) {
        cout << "  [LOI] Khong tim thay lop \"" << maLop << "\"!" << endl;
        nhanEnter();
        return;
    }

    cout << endl << "  Thong tin hien tai:" << endl;
    cout << "    Ma lop: " << dsLopHoc[idx].maLop << endl;
    cout << "    Ten lop: " << dsLopHoc[idx].tenLop << endl;
    cout << "    Thu: " << dsLopHoc[idx].thu << endl;
    cout << "    Tiet bat dau: " << dsLopHoc[idx].tietBatDau << endl;
    cout << "    So tiet: " << dsLopHoc[idx].soTiet << endl;
    cout << "    Phong: " << dsLopHoc[idx].phong << endl;
    cout << "    Tong so buoi: " << dsLopHoc[idx].tongSoBuoi << endl;
    cout << endl << "  (Nhan Enter de giu nguyen gia tri cu)" << endl;

    string input;

    cout << "  Ten lop moi: ";
    getline(cin, input);
    if (!input.empty()) dsLopHoc[idx].tenLop = input;

    cout << "  Thu hoc moi: ";
    getline(cin, input);
    if (!input.empty()) dsLopHoc[idx].thu = input;

    cout << "  Tiet bat dau moi (1-12): ";
    getline(cin, input);
    if (!input.empty()) {
        int tiet = atoi(input.c_str());
        if (tiet >= 1 && tiet <= 12)
            dsLopHoc[idx].tietBatDau = tiet;
        else
            cout << "  [LOI] Tiet khong hop le (1-12), giu nguyen gia tri cu." << endl;
    }

    cout << "  So tiet moi (1-6): ";
    getline(cin, input);
    if (!input.empty()) {
        int st = atoi(input.c_str());
        if (st >= 1 && st <= 6)
            dsLopHoc[idx].soTiet = st;
        else
            cout << "  [LOI] So tiet khong hop le (1-6), giu nguyen gia tri cu." << endl;
    }

    cout << "  Phong hoc moi: ";
    getline(cin, input);
    if (!input.empty()) dsLopHoc[idx].phong = input;

    cout << "  Tong so buoi moi (1-50): ";
    getline(cin, input);
    if (!input.empty()) {
        int tb = atoi(input.c_str());
        if (tb >= 1 && tb <= 50)
            dsLopHoc[idx].tongSoBuoi = tb;
        else
            cout << "  [LOI] So buoi khong hop le (1-50), giu nguyen gia tri cu." << endl;
    }

    FileManager::ghiLopHoc(fileLopHoc, dsLopHoc);
    cout << endl << "  [OK] Cap nhat thong tin lop thanh cong!" << endl;
    nhanEnter();
}

void HeThongDiemDanh::xoaLopHoc() {
    system("cls");
    hienThiTieuDe("XOA LOP HOC");

    if (dsLopHoc.empty()) {
        cout << "  Chua co lop hoc nao trong he thong." << endl;
        nhanEnter();
        return;
    }

    string maLop;
    cout << "  Nhap ma lop can xoa: ";
    getline(cin, maLop);

    int idx = timLopHoc(maLop);
    if (idx == -1) {
        cout << "  [LOI] Khong tim thay lop \"" << maLop << "\"!" << endl;
        nhanEnter();
        return;
    }

    DynamicArray<int> svTrongLop = laySVTheoLop(maLop);
    if (!svTrongLop.empty()) {
        cout << "  [CANH BAO] Lop nay con " << svTrongLop.size()
             << " sinh vien!" << endl;
    }

    char xacNhan = docXacNhan("  Ban co chac chan muon xoa lop \"" + maLop + "\"? (Y/N): ");

    if (xacNhan == 'Y' || xacNhan == 'y') {
        for (int i = dsSinhVien.size() - 1; i >= 0; i--) {
            if (dsSinhVien[i].maLop == maLop)
                dsSinhVien.removeAt(i);
        }
        for (int i = dsDiemDanh.size() - 1; i >= 0; i--) {
            if (dsDiemDanh[i].maLop == maLop)
                dsDiemDanh.removeAt(i);
        }
        dsLopHoc.removeAt(idx);
        luuTatCa();
        cout << endl << "  [OK] Da xoa lop \"" << maLop << "\" thanh cong!" << endl;
    } else {
        cout << "  Da huy thao tac xoa." << endl;
    }
    nhanEnter();
}

void HeThongDiemDanh::hienThiDanhSachLop() {
    system("cls");
    hienThiTieuDe("DANH SACH LOP HOC");

    if (dsLopHoc.empty()) {
        cout << "  Chua co lop hoc nao trong he thong." << endl;
        nhanEnter();
        return;
    }

    cout << "  " << left << setw(12) << "Ma lop" << setw(25) << "Ten lop"
         << setw(10) << "Thu" << setw(8) << "Tiet" << setw(8) << "So tiet"
         << setw(10) << "Phong" << setw(8) << "So buoi" << endl;
    cout << "  ";
    for (int i = 0; i < 79; i++) cout << "-";
    cout << endl;

    for (int i = 0; i < dsLopHoc.size(); i++) {
        cout << "  " << left << setw(12) << dsLopHoc[i].maLop << setw(25)
             << dsLopHoc[i].tenLop << setw(10) << dsLopHoc[i].thu << setw(8)
             << dsLopHoc[i].tietBatDau << setw(8) << dsLopHoc[i].soTiet << setw(10)
             << dsLopHoc[i].phong << setw(8) << dsLopHoc[i].tongSoBuoi << endl;
    }

    cout << endl << "  Tong cong: " << dsLopHoc.size() << " lop hoc." << endl;
    nhanEnter();
}

void HeThongDiemDanh::themSinhVien() {
    system("cls");
    hienThiTieuDe("THEM SINH VIEN");

    if (dsLopHoc.empty()) {
        cout << "  [LOI] Chua co lop hoc nao! Hay them lop hoc truoc." << endl;
        nhanEnter();
        return;
    }

    cout << "  Danh sach lop hien co:" << endl;
    for (int i = 0; i < dsLopHoc.size(); i++) {
        cout << "    " << dsLopHoc[i].maLop << " - " << dsLopHoc[i].tenLop << endl;
    }
    cout << endl;
    cout << "  (Nhap * vao ma sinh vien de ket thuc)" << endl;
    cout << "  ";
    for (int i = 0; i < 50; i++) cout << "-";
    cout << endl;

    int demThem = 0;

    while (true) {
        cout << endl;
        SinhVien sv;

        cout << "  Nhap ma sinh vien: ";
        getline(cin, sv.maSV);

        if (sv.maSV == "*") break;

        if (sv.maSV.empty()) {
            cout << "  [LOI] Ma sinh vien khong duoc de trong!" << endl;
            continue;
        }

        if (timSinhVien(sv.maSV) != -1) {
            cout << "  [LOI] Ma sinh vien \"" << sv.maSV << "\" da ton tai!" << endl;
            continue;
        }

        cout << "  Nhap ho va ten: ";
        getline(cin, sv.hoTen);
        if (sv.hoTen.empty()) {
            cout << "  [LOI] Ho ten khong duoc de trong!" << endl;
            continue;
        }

        cout << "  Nhap ma lop: ";
        getline(cin, sv.maLop);
        if (timLopHoc(sv.maLop) == -1) {
            cout << "  [LOI] Lop \"" << sv.maLop << "\" khong ton tai!" << endl;
            continue;
        }

        dsSinhVien.push_back(sv);
        demThem++;
        cout << "  [OK] Da them: " << sv.maSV << " - " << sv.hoTen
             << " (Lop: " << sv.maLop << ")" << endl;
    }

    if (demThem > 0) {
        FileManager::ghiSinhVien(fileSinhVien, dsSinhVien);
        cout << endl << "  Da them thanh cong " << demThem << " sinh vien!" << endl;
    } else {
        cout << endl << "  Khong co sinh vien nao duoc them." << endl;
    }
    nhanEnter();
}

void HeThongDiemDanh::suaSinhVien() {
    system("cls");
    hienThiTieuDe("SUA THONG TIN SINH VIEN");

    string maSV;
    cout << "  Nhap ma sinh vien can sua: ";
    getline(cin, maSV);

    int idx = timSinhVien(maSV);
    if (idx == -1) {
        cout << "  [LOI] Khong tim thay sinh vien \"" << maSV << "\"!" << endl;
        nhanEnter();
        return;
    }

    cout << endl << "  Thong tin hien tai:" << endl;
    cout << "    Ma SV: " << dsSinhVien[idx].maSV << endl;
    cout << "    Ho ten: " << dsSinhVien[idx].hoTen << endl;
    cout << "    Ma lop: " << dsSinhVien[idx].maLop << endl;
    cout << endl << "  (Nhan Enter de giu nguyen gia tri cu)" << endl;

    string input;

    cout << "  Ho ten moi: ";
    getline(cin, input);
    if (!input.empty()) dsSinhVien[idx].hoTen = input;

    cout << "  Ma lop moi: ";
    getline(cin, input);
    if (!input.empty()) {
        if (timLopHoc(input) == -1) {
            cout << "  [LOI] Lop \"" << input
                 << "\" khong ton tai! Giu nguyen lop cu." << endl;
        } else {
            dsSinhVien[idx].maLop = input;
        }
    }

    FileManager::ghiSinhVien(fileSinhVien, dsSinhVien);
    cout << endl << "  [OK] Cap nhat thong tin sinh vien thanh cong!" << endl;
    nhanEnter();
}

void HeThongDiemDanh::xoaSinhVien() {
    system("cls");
    hienThiTieuDe("XOA SINH VIEN");

    string maSV;
    cout << "  Nhap ma sinh vien can xoa: ";
    getline(cin, maSV);

    int idx = timSinhVien(maSV);
    if (idx == -1) {
        cout << "  [LOI] Khong tim thay sinh vien \"" << maSV << "\"!" << endl;
        nhanEnter();
        return;
    }

    cout << "  Sinh vien: " << dsSinhVien[idx].hoTen
         << " (Lop: " << dsSinhVien[idx].maLop << ")" << endl;

    char xacNhan = docXacNhan("  Ban co chac chan muon xoa? (Y/N): ");

    if (xacNhan == 'Y' || xacNhan == 'y') {
        for (int i = dsDiemDanh.size() - 1; i >= 0; i--) {
            if (dsDiemDanh[i].maSV == maSV)
                dsDiemDanh.removeAt(i);
        }
        dsSinhVien.removeAt(idx);
        luuTatCa();
        cout << "  [OK] Da xoa sinh vien thanh cong!" << endl;
    } else {
        cout << "  Da huy thao tac xoa." << endl;
    }
    nhanEnter();
}

void HeThongDiemDanh::hienThiDanhSachSV() {
    system("cls");
    hienThiTieuDe("DANH SACH SINH VIEN");

    if (dsSinhVien.empty()) {
        cout << "  Chua co sinh vien nao trong he thong." << endl;
        nhanEnter();
        return;
    }

    cout << "  1. Xem tat ca sinh vien" << endl;
    cout << "  2. Xem theo lop" << endl;
    int lc = docSoNguyen("  Lua chon: ");

    string locLop = "";
    if (lc == 2) {
        cout << "  Nhap ma lop: ";
        getline(cin, locLop);
        if (timLopHoc(locLop) == -1) {
            cout << "  [LOI] Lop \"" << locLop << "\" khong ton tai!" << endl;
            nhanEnter();
            return;
        }
    }

    cout << endl;
    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ma SV"
         << setw(30) << "Ho ten" << setw(15) << "Ma lop" << endl;
    cout << "  ";
    for (int i = 0; i < 63; i++) cout << "-";
    cout << endl;

    int stt = 0;
    for (int i = 0; i < dsSinhVien.size(); i++) {
        if (!locLop.empty() && dsSinhVien[i].maLop != locLop)
            continue;
        stt++;
        cout << "  " << left << setw(5) << stt << setw(15) << dsSinhVien[i].maSV
             << setw(30) << dsSinhVien[i].hoTen << setw(15) << dsSinhVien[i].maLop
             << endl;
    }

    cout << endl << "  Tong cong: " << stt << " sinh vien." << endl;
    nhanEnter();
}

void HeThongDiemDanh::diemDanhTheoLop() {
    system("cls");
    hienThiTieuDe("DIEM DANH THEO LOP");

    if (dsLopHoc.empty()) {
        cout << "  Chua co lop hoc nao!" << endl;
        nhanEnter();
        return;
    }

    cout << "  Danh sach lop:" << endl;
    for (int i = 0; i < dsLopHoc.size(); i++) {
        cout << "    " << dsLopHoc[i].maLop << " - " << dsLopHoc[i].tenLop << endl;
    }
    cout << endl;

    string maLop;
    cout << "  Nhap ma lop can diem danh: ";
    getline(cin, maLop);

    int idxLop = timLopHoc(maLop);
    if (idxLop == -1) {
        cout << "  [LOI] Lop \"" << maLop << "\" khong ton tai!" << endl;
        nhanEnter();
        return;
    }

    DynamicArray<int> svTrongLop = laySVTheoLop(maLop);
    if (svTrongLop.empty()) {
        cout << "  [LOI] Lop nay chua co sinh vien!" << endl;
        nhanEnter();
        return;
    }

    string ngay = docNgayHopLe("  Nhap ngay diem danh (dd/mm/yyyy): ");

    bool daDiemDanh = false;
    for (int i = 0; i < dsDiemDanh.size(); i++) {
        if (dsDiemDanh[i].maLop == maLop && dsDiemDanh[i].ngay == ngay) {
            daDiemDanh = true;
            break;
        }
    }

    if (daDiemDanh) {
        cout << "  [CANH BAO] Ngay " << ngay << " da duoc diem danh!" << endl;
        char xn = docXacNhan("  Ban co muon diem danh lai? (Y/N): ");
        if (xn != 'Y' && xn != 'y') return;

        for (int i = dsDiemDanh.size() - 1; i >= 0; i--) {
            if (dsDiemDanh[i].maLop == maLop && dsDiemDanh[i].ngay == ngay)
                dsDiemDanh.removeAt(i);
        }
    }

    cout << endl;
    cout << "  Huong dan: 1 = Co mat | 2 = Vang co phep | 3 = Vang khong phep" << endl;
    cout << "  Lop: " << maLop << " | Ngay: " << ngay << endl;
    cout << "  ";
    for (int i = 0; i < 60; i++) cout << "-";
    cout << endl;

    for (int i = 0; i < svTrongLop.size(); i++) {
        int svIdx = svTrongLop[i];
        int trangThai;

        cout << "  " << i + 1 << ". " << dsSinhVien[svIdx].maSV
             << " - " << dsSinhVien[svIdx].hoTen << endl;

        do {
            trangThai = docSoNguyen("     Trang thai (1/2/3): ");
            if (trangThai < 1 || trangThai > 3)
                cout << "     [LOI] Vui long nhap 1, 2 hoac 3!" << endl;
        } while (trangThai < 1 || trangThai > 3);

        DiemDanh dd;
        dd.maLop = maLop;
        dd.maSV = dsSinhVien[svIdx].maSV;
        dd.ngay = ngay;
        dd.trangThai = trangThai - 1;
        dsDiemDanh.push_back(dd);
    }

    FileManager::ghiDiemDanh(fileDiemDanh, dsDiemDanh);

    cout << endl << "  [OK] Diem danh thanh cong cho " << svTrongLop.size()
         << " sinh vien!" << endl;
    nhanEnter();
}

void HeThongDiemDanh::suaDiemDanh() {
    system("cls");
    hienThiTieuDe("SUA DIEM DANH");

    string maLop = docChuoiKhongRong("  Nhap ma lop: ");
    string maSV = docChuoiKhongRong("  Nhap ma sinh vien: ");
    string ngay = docNgayHopLe("  Nhap ngay (dd/mm/yyyy): ");

    int found = -1;
    for (int i = 0; i < dsDiemDanh.size(); i++) {
        if (dsDiemDanh[i].maLop == maLop && dsDiemDanh[i].maSV == maSV &&
            dsDiemDanh[i].ngay == ngay) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        cout << "  [LOI] Khong tim thay ban ghi diem danh!" << endl;
        nhanEnter();
        return;
    }

    int svIdx = timSinhVien(maSV);
    if (svIdx != -1) {
        cout << endl << "  Sinh vien: " << dsSinhVien[svIdx].hoTen << endl;
    }
    cout << "  Trang thai hien tai: "
         << trangThaiToString(dsDiemDanh[found].trangThai) << endl;
    cout << endl;
    cout << "  Chon trang thai moi:" << endl;
    cout << "    1. Co mat" << endl;
    cout << "    2. Vang co phep" << endl;
    cout << "    3. Vang khong phep" << endl;

    int newTT;
    do {
        newTT = docSoNguyen("  Lua chon (1/2/3): ");
        if (newTT < 1 || newTT > 3)
            cout << "  [LOI] Vui long nhap 1, 2 hoac 3!" << endl;
    } while (newTT < 1 || newTT > 3);

    dsDiemDanh[found].trangThai = newTT - 1;
    FileManager::ghiDiemDanh(fileDiemDanh, dsDiemDanh);

    cout << endl << "  [OK] Da cap nhat diem danh thanh cong!" << endl;
    nhanEnter();
}

void HeThongDiemDanh::timKiemTheoNgay() {
    system("cls");
    hienThiTieuDe("TIM KIEM DIEM DANH THEO NGAY");

    string maLop;
    cout << "  Nhap ma lop: ";
    getline(cin, maLop);

    if (timLopHoc(maLop) == -1) {
        cout << "  [LOI] Lop \"" << maLop << "\" khong ton tai!" << endl;
        nhanEnter();
        return;
    }

    string ngay = docNgayHopLe("  Nhap ngay (dd/mm/yyyy): ");

    cout << endl;
    cout << "  Ket qua diem danh lop " << maLop << " ngay " << ngay << ":" << endl;
    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ma SV"
         << setw(30) << "Ho ten" << setw(20) << "Trang thai" << endl;
    cout << "  ";
    for (int i = 0; i < 68; i++) cout << "-";
    cout << endl;

    int dem = 0;
    for (int i = 0; i < dsDiemDanh.size(); i++) {
        if (dsDiemDanh[i].maLop == maLop && dsDiemDanh[i].ngay == ngay) {
            dem++;
            string hoTen = "???";
            int svIdx = timSinhVien(dsDiemDanh[i].maSV);
            if (svIdx != -1)
                hoTen = dsSinhVien[svIdx].hoTen;

            cout << "  " << left << setw(5) << dem << setw(15) << dsDiemDanh[i].maSV
                 << setw(30) << hoTen << setw(20)
                 << trangThaiToString(dsDiemDanh[i].trangThai) << endl;
        }
    }

    if (dem == 0) {
        cout << "  Khong tim thay ban ghi diem danh nao." << endl;
    } else {
        cout << endl << "  Tim thay " << dem << " ban ghi." << endl;
    }
    nhanEnter();
}

void HeThongDiemDanh::timKiemTheoMaSV() {
    system("cls");
    hienThiTieuDe("TIM KIEM DIEM DANH THEO MA SINH VIEN");

    string maSV;
    cout << "  Nhap ma sinh vien: ";
    getline(cin, maSV);

    int svIdx = timSinhVien(maSV);
    if (svIdx == -1) {
        cout << "  [LOI] Khong tim thay sinh vien \"" << maSV << "\"!" << endl;
        nhanEnter();
        return;
    }

    cout << endl << "  Sinh vien: " << dsSinhVien[svIdx].hoTen
         << " | Lop: " << dsSinhVien[svIdx].maLop << endl;
    cout << endl;

    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ma lop"
         << setw(15) << "Ngay" << setw(20) << "Trang thai" << endl;
    cout << "  ";
    for (int i = 0; i < 53; i++) cout << "-";
    cout << endl;

    int dem = 0;
    for (int i = 0; i < dsDiemDanh.size(); i++) {
        if (dsDiemDanh[i].maSV == maSV) {
            dem++;
            cout << "  " << left << setw(5) << dem << setw(15) << dsDiemDanh[i].maLop
                 << setw(15) << dsDiemDanh[i].ngay << setw(20)
                 << trangThaiToString(dsDiemDanh[i].trangThai) << endl;
        }
    }

    if (dem == 0) {
        cout << "  Chua co ban ghi diem danh nao cho sinh vien nay." << endl;
    } else {
        int coMat = 0, vangCP = 0, vangKP = 0;
        for (int i = 0; i < dsDiemDanh.size(); i++) {
            if (dsDiemDanh[i].maSV == maSV) {
                if (dsDiemDanh[i].trangThai == CO_MAT) coMat++;
                else if (dsDiemDanh[i].trangThai == VANG_CO_PHEP) vangCP++;
                else if (dsDiemDanh[i].trangThai == VANG_KHONG_PHEP) vangKP++;
            }
        }
        cout << endl << "  Tong ket: Co mat: " << coMat << " | Vang CP: " << vangCP
             << " | Vang KP: " << vangKP << " | Tong: " << dem << " buoi" << endl;
    }
    nhanEnter();
}

void HeThongDiemDanh::tinhTyLeVang() {
    system("cls");
    hienThiTieuDe("TY LE VANG MAT TUNG SINH VIEN");

    string maLop;
    cout << "  Nhap ma lop: ";
    getline(cin, maLop);

    int idxLop = timLopHoc(maLop);
    if (idxLop == -1) {
        cout << "  [LOI] Lop \"" << maLop << "\" khong ton tai!" << endl;
        nhanEnter();
        return;
    }

    DynamicArray<int> svTrongLop = laySVTheoLop(maLop);
    if (svTrongLop.empty()) {
        cout << "  Lop nay chua co sinh vien!" << endl;
        nhanEnter();
        return;
    }

    int tongBuoi = dsLopHoc[idxLop].tongSoBuoi;

    cout << endl << "  Lop: " << maLop << " - " << dsLopHoc[idxLop].tenLop << endl;
    cout << "  Tong so buoi trong ky: " << tongBuoi << endl << endl;

    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ma SV" << setw(25)
         << "Ho ten" << setw(8) << "CM" << setw(8) << "VCP" << setw(8) << "VKP"
         << setw(12) << "Ty le vang" << endl;
    cout << "  ";
    for (int i = 0; i < 79; i++) cout << "-";
    cout << endl;

    for (int i = 0; i < svTrongLop.size(); i++) {
        int svIdx = svTrongLop[i];
        string maSV = dsSinhVien[svIdx].maSV;

        int coMat = 0, vangCP = 0, vangKP = 0;
        for (int j = 0; j < dsDiemDanh.size(); j++) {
            if (dsDiemDanh[j].maLop == maLop && dsDiemDanh[j].maSV == maSV) {
                if (dsDiemDanh[j].trangThai == CO_MAT) coMat++;
                else if (dsDiemDanh[j].trangThai == VANG_CO_PHEP) vangCP++;
                else if (dsDiemDanh[j].trangThai == VANG_KHONG_PHEP) vangKP++;
            }
        }

        double tyLeVang = 0;
        if (tongBuoi > 0)
            tyLeVang = (double)vangKP / tongBuoi * 100.0;

        cout << "  " << left << setw(5) << (i + 1) << setw(15) << maSV << setw(25)
             << dsSinhVien[svIdx].hoTen << setw(8) << coMat << setw(8) << vangCP
             << setw(8) << vangKP;

        cout << fixed << setprecision(1) << tyLeVang << "%";
        if (tyLeVang > 20.0)
            cout << " !!!";
        cout << endl;
    }

    cout << endl << "  Ghi chu: CM=Co mat, VCP=Vang co phep, VKP=Vang khong phep" << endl;
    cout << "  !!! = Nguy co cam thi (vang KP > 20%)" << endl;
    nhanEnter();
}

void HeThongDiemDanh::canhBaoCamThi() {
    system("cls");
    hienThiTieuDe("CANH BAO CAM THI (VANG > 20%)");

    bool coSVBiCanhBao = false;

    for (int l = 0; l < dsLopHoc.size(); l++) {
        string maLop = dsLopHoc[l].maLop;
        int tongBuoi = dsLopHoc[l].tongSoBuoi;
        if (tongBuoi <= 0) continue;

        DynamicArray<int> svTrongLop = laySVTheoLop(maLop);

        bool inTenLop = false;
        for (int i = 0; i < svTrongLop.size(); i++) {
            int svIdx = svTrongLop[i];
            string maSV = dsSinhVien[svIdx].maSV;

            int vangKP = 0;
            for (int j = 0; j < dsDiemDanh.size(); j++) {
                if (dsDiemDanh[j].maLop == maLop && dsDiemDanh[j].maSV == maSV &&
                    dsDiemDanh[j].trangThai == VANG_KHONG_PHEP) {
                    vangKP++;
                }
            }

            double tyLe = (double)vangKP / tongBuoi * 100.0;
            if (tyLe > 20.0) {
                if (!inTenLop) {
                    cout << endl << "  Lop: " << maLop << " - " << dsLopHoc[l].tenLop
                         << " (Tong: " << tongBuoi << " buoi)" << endl;
                    cout << "  ";
                    for (int k = 0; k < 60; k++) cout << "-";
                    cout << endl;
                    inTenLop = true;
                }

                cout << "  [!!!] " << dsSinhVien[svIdx].maSV << " - "
                     << dsSinhVien[svIdx].hoTen << " | Vang KP: " << vangKP << "/"
                     << tongBuoi << " (" << fixed << setprecision(1) << tyLe << "%)"
                     << endl;
                coSVBiCanhBao = true;
            }
        }
    }

    if (!coSVBiCanhBao) {
        cout << endl << "  Khong co sinh vien nao bi nguy co cam thi." << endl;
        cout << "  (Chua co sinh vien nao vang khong phep qua 20%)" << endl;
    }

    nhanEnter();
}

void HeThongDiemDanh::thongKeSiSo() {
    system("cls");
    hienThiTieuDe("THONG KE SI SO THEO BUOI HOC");

    string maLop;
    cout << "  Nhap ma lop: ";
    getline(cin, maLop);

    int idxLop = timLopHoc(maLop);
    if (idxLop == -1) {
        cout << "  [LOI] Lop \"" << maLop << "\" khong ton tai!" << endl;
        nhanEnter();
        return;
    }

    DynamicArray<string> ngays = layNgayDiemDanh(maLop);
    if (ngays.empty()) {
        cout << "  Chua co du lieu diem danh cho lop nay." << endl;
        nhanEnter();
        return;
    }

    bubbleSort(ngays, soSanhNgay);

    DynamicArray<int> svTrongLop = laySVTheoLop(maLop);
    int tongSV = svTrongLop.size();

    cout << endl << "  Lop: " << maLop << " - " << dsLopHoc[idxLop].tenLop << endl;
    cout << "  Tong so sinh vien: " << tongSV << endl << endl;

    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ngay" << setw(10)
         << "Co mat" << setw(10) << "Vang CP" << setw(10) << "Vang KP" << setw(12)
         << "Ti le CM" << endl;
    cout << "  ";
    for (int i = 0; i < 60; i++) cout << "-";
    cout << endl;

    for (int n = 0; n < ngays.size(); n++) {
        int coMat = 0, vangCP = 0, vangKP = 0;

        for (int i = 0; i < dsDiemDanh.size(); i++) {
            if (dsDiemDanh[i].maLop == maLop && dsDiemDanh[i].ngay == ngays[n]) {
                if (dsDiemDanh[i].trangThai == CO_MAT) coMat++;
                else if (dsDiemDanh[i].trangThai == VANG_CO_PHEP) vangCP++;
                else if (dsDiemDanh[i].trangThai == VANG_KHONG_PHEP) vangKP++;
            }
        }

        int tongDiemDanh = coMat + vangCP + vangKP;
        double tiLeCM = (tongDiemDanh > 0) ? (double)coMat / tongDiemDanh * 100.0 : 0;

        cout << "  " << left << setw(5) << (n + 1) << setw(15) << ngays[n]
             << setw(10) << coMat << setw(10) << vangCP << setw(10) << vangKP
             << fixed << setprecision(1) << tiLeCM << "%" << endl;
    }

    cout << endl << "  Tong cong: " << ngays.size() << " buoi da diem danh." << endl;
    nhanEnter();
}

void HeThongDiemDanh::danhSachVangNhieuNhat() {
    system("cls");
    hienThiTieuDe("DANH SACH SINH VIEN VANG NHIEU NHAT");

    if (dsSinhVien.empty() || dsDiemDanh.empty()) {
        cout << "  Chua co du lieu de thong ke." << endl;
        nhanEnter();
        return;
    }

    DynamicArray<ThongKeVang> dsThongKe;

    for (int i = 0; i < dsSinhVien.size(); i++) {
        ThongKeVang tk;
        tk.maSV = dsSinhVien[i].maSV;
        tk.hoTen = dsSinhVien[i].hoTen;
        tk.maLop = dsSinhVien[i].maLop;
        tk.tongVang = 0;
        tk.vangKP = 0;
        tk.tongBuoi = 0;

        for (int j = 0; j < dsDiemDanh.size(); j++) {
            if (dsDiemDanh[j].maSV == tk.maSV) {
                tk.tongBuoi++;
                if (dsDiemDanh[j].trangThai == VANG_CO_PHEP) {
                    tk.tongVang++;
                } else if (dsDiemDanh[j].trangThai == VANG_KHONG_PHEP) {
                    tk.tongVang++;
                    tk.vangKP++;
                }
            }
        }

        if (tk.tongVang > 0) {
            dsThongKe.push_back(tk);
        }
    }

    if (dsThongKe.empty()) {
        cout << "  Khong co sinh vien nao vang buoi nao." << endl;
        nhanEnter();
        return;
    }

    bubbleSort(dsThongKe, soSanhVangGiamDan);

    cout << endl;
    cout << "  " << left << setw(5) << "STT" << setw(15) << "Ma SV" << setw(25)
         << "Ho ten" << setw(12) << "Ma lop" << setw(12) << "Tong vang"
         << setw(10) << "Vang KP" << endl;
    cout << "  ";
    for (int i = 0; i < 77; i++) cout << "-";
    cout << endl;

    int soHienThi = (dsThongKe.size() > 20) ? 20 : dsThongKe.size();
    for (int i = 0; i < soHienThi; i++) {
        cout << "  " << left << setw(5) << (i + 1) << setw(15) << dsThongKe[i].maSV
             << setw(25) << dsThongKe[i].hoTen << setw(12) << dsThongKe[i].maLop
             << setw(12) << dsThongKe[i].tongVang << setw(10) << dsThongKe[i].vangKP
             << endl;
    }

    cout << endl << "  Hien thi " << soHienThi << "/" << dsThongKe.size()
         << " sinh vien co vang." << endl;
    nhanEnter();
}
