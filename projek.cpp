#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <iomanip>

using namespace std;

string rp (double v) { //ABSTRACT
    string s = to_string(static_cast<long long>(v + 0.5));
    for (int i = static_cast<int>(s.size()) - 3; i>0; i-=3) s. insert(i, ".");
    return "Rp" + s;
}

// barang perpustakaan 

class LibraryItem {
    /*ENKAPSULASI
    cegah variabel dapat diubah dari main
    */
    private:
        string id;
        string title;
        int year;
        bool isBorrowed;
    
    public:
        LibraryItem(string id, string title, int year) : id(id), title(title), year(year), isBorrowed(false) {
            if (id.empty() || title.empty())
                throw invalid_argument("ID dan judul tidak boleh kosong.");
            if (year < 1000 || year > 2026)
                throw invalid_argument("Tahun harus antara 1000 dan 2026");
        }        
        virtual ~LibraryItem() {}

        // enkapsulasi getter setter
        string getId() const {
            return id;
        }
        string getTitle() const {
            return title;
        }
        int getYear() const{
            return year;
        }
         bool getIsBorrowed() const {
            return isBorrowed;
        }
        bool borrowItem() {
            if (isBorrowed) return false;   // aturan: tidak boleh dipinjam dua kali
            isBorrowed = true;
            return true;
        }
        void returnItem() {
            isBorrowed = false;
        }

        //ABSTRACT
        virtual int getLoanDays() const = 0;
        virtual double calculateFine(int daysLate) const = 0;
        virtual string getType() const = 0;

        //polimorfisme (agar bisa digunakan all its child class)
        virtual void display() const {
            cout << left
                 << setw(10) << ("[" + getType() + "]")
                 << setw(6)  << id
                 << setw(28) << title
                 << setw(7)  << year
                 << setw(11) << (isBorrowed ? "Dipinjam" : "Tersedia");
        }
};

//inheritance (mewarisi semua sifat library)
class Book : public LibraryItem {
    private:
        string author;
    public:
        Book(string id, string title, int year, string author) : LibraryItem(id, title, year), author(author) {}
        
        int getLoanDays() const override { return 14; }
        double calculateFine(int daysLate) const override { return daysLate * 1000.0; }
        string getType() const override { return "Buku"; }
        
        //polimorfisme (override induk + output nama)
        void display () const override{
            LibraryItem::display();
            cout << "Penulis: " << author << "\n";
        }
};

// inheritance turunan dari library
class Magazine : public LibraryItem{
    private:
        int issueNumber;
    public:
    Magazine(string id, string title, int year, int issue) : LibraryItem(id, title, year), issueNumber(issue) {}
    
    int getLoanDays() const override { return 7; }
    double calculateFine(int daysLate) const override { return daysLate * 500.0; }
    string getType() const override { return "Majalah"; }    
    
    void display() const override{
        LibraryItem :: display();
        cout << "Edisi: " << issueNumber << "\n";
    }
};

// inheritance turunan dari library
class DVD : public LibraryItem{
    private :
        string director;
    public:
        DVD(string id, string title, int year, string director) : LibraryItem(id, title, year), director(director) {}
        
        int getLoanDays() const override { return 3; }
        double calculateFine(int daysLate) const override { return daysLate * 5000.0; }
        string getType() const override { return "DVD"; }
        
        void display() const override{
            LibraryItem :: display();
            cout << "Sutradara: " << director << "\n";
        }
        
};

//inheritance turunan dari library
class Journal : public LibraryItem{
    private:
        int volume;
    public:
        Journal(string id, string title, int year, int volume) : LibraryItem(id, title, year), volume(volume){}
        
        int getLoanDays() const override { return 5; }
        double calculateFine(int daysLate) const override { return daysLate * 2500.0; }
        string getType() const override { return "Jurnal"; }
        
        void display() const override{
            LibraryItem::display();
            cout << "Volume: " << volume << "\n";
        }
};

//ANGGOTA PERPUSTAKAAN
class Member { //abstract dengan batas pinjam
    private:
        string memberId;
        string name;
        vector<string> borrowedIds;
    public:
        Member(string id, string memberName) : memberId(id), name(memberName) {
            if (memberId.empty() || name.empty())
                throw invalid_argument("ID dan nama anggota tidak boleh kosong.");
        }
        virtual ~Member() {}

        string getId() const { return memberId; }
        string getName() const { return name; }
        int loanCount() const { return static_cast<int>(borrowedIds.size()); }

        bool canBorrow() const { return loanCount() < getMaxLoans(); }
        bool addLoan(const string& itemId) {
            if (!canBorrow()) return false;
            borrowedIds.push_back(itemId);
            return true;
        }
        void removeLoan(const string& itemId) {
            borrowedIds.erase(remove(borrowedIds.begin(), borrowedIds.end(), itemId), borrowedIds.end());
        }

        // Kontrak abstrak: aturan berbeda per jenis anggota (POLIMORFISME)
        virtual int getMaxLoans() const = 0;
        virtual double getFineDiscount() const = 0;  // 0.0 sampai 1.0
        virtual string getRole() const = 0;

        virtual void display() const {
            cout << left
                 << setw(13) << ("[" + getRole() + "]")
                 << setw(6)  << memberId
                 << setw(14) << name
                 << setw(10) << (to_string(loanCount()) + "/" + to_string(getMaxLoans()))
                 << setw(8)  << (to_string(static_cast<int>(getFineDiscount() * 100)) + "%");
        }
};

class Student : public Member{
    private:
        string major;
    public:
        Student (string id, string name, string major) : Member(id, name), major (major){}

        int getMaxLoans() const override { return 3; }
        double getFineDiscount() const override { return 0.0; }
        string getRole() const override { return "Mahasiswa"; }

        void display() const override{
            Member::display();
            cout << "Jurusan: " << major << "\n";
        }
};

class Lecturer : public Member {
    private:
        string department;
    public:
        Lecturer(string id, string title, string department) : Member(id, title), department(department){}
        
        int getMaxLoans() const override { return 10; }
        double getFineDiscount() const override { return 0.5; }
        string getRole() const override { return "Dosen"; }       
        
        void display() const override{
            Member::display();
            cout << "Departemen: " << department << "\n";
        }
};

// ==========================================
// 3. KELAS LOAN (PEMINJAMAN)
// ==========================================
// Kelas pembantu untuk mencatat histori transaksi.
class Loan {
private:
    string itemId;
    string memberId;
    int borrowDay;
    int durationDays;

public:
    Loan(string item, string member, int day, int duration) : itemId(item), memberId(member), borrowDay(day), durationDays(duration) {}
    
    string getItemId() const { return itemId; }
    string getMemberId() const { return memberId; }
    int getBorrowDay() const { return borrowDay; }
    int getDueDay() const { return borrowDay + durationDays; }

    // ENKAPSULASI: durasi hanya bisa bertambah, tidak boleh nol atau negatif
    bool extendDuration(int extraDays) {
        if (extraDays <= 0) return false;
        durationDays += extraDays;
        return true;
    }
};

// ==========================================
// 4. KELAS LIBRARY (MANAJEMEN UTAMA)
// ==========================================

void printItemHeader() {
    cout << left << setw(10) << "TIPE" << setw(6) << "ID" << setw(28) << "JUDUL"
         << setw(7) << "TAHUN" << setw(11) << "STATUS" << "DETAIL\n";
    cout << string(90, '-') << "\n";
}

void printMemberHeader() {
    cout << left << setw(13) << "PERAN" << setw(6) << "ID" << setw(14) << "NAMA"
         << setw(10) << "PINJAMAN" << setw(8) << "DISKON" << "DETAIL\n";
    cout << string(70, '-') << "\n";
}

class Library {
private:
    // [OOP: POLIMORFISME] 
    // Menyimpan BERBAGAI JENIS objek (Book, DVD, Journal) ke dalam SATU TIPE wadah 
    // menggunakan pointer ke kelas Induk (LibraryItem*). 
    vector<LibraryItem*> items;
    vector<Member*> members;
    vector<Loan> loans;

    // fungsi bantu: cari item / anggota berdasarkan ID (private = disembunyikan dari pengguna)
    LibraryItem* findItem(string id) {
        for (auto item : items) {
            if (item->getId() == id) return item;
        }
        return nullptr;
    }
    Member* findMember(string id) {
        for (auto member : members) {
            if (member->getId() == id) return member;
        }
        return nullptr;
    }

public:
    ~Library() {
        for (auto item : items) delete item;
        for (auto member : members) delete member;
    }

    void addItem(LibraryItem* item) { items.push_back(item); }
    void addMember(Member* member) { members.push_back(member); }

    // [OOP: ABSTRAKSI]
    // Fungsi ini adalah wujud nyata abstraksi. 
    // Pengguna sistem (dari main.cpp) HANYA perlu memanggil fungsi ini untuk meminjam buku.
    // Pengguna TIDAK PERLU TAHU cara kerja rumit di baliknya: seperti perulangan pencarian ID, 
    // validasi ketersediaan, pengubahan status buku, dan pencatatan objek 'Loan' baru.
    // Semua detail kompleks (Implementation Details) "disembunyikan" di dalam metode ini.
    void borrowItem(string itemId, string memberId, int today) {
        LibraryItem* item = findItem(itemId);
        Member* member = findMember(memberId);

        if (item == nullptr) { cout << "-> DITOLAK: Item tidak ditemukan.\n"; return; }
        if (member == nullptr) { cout << "-> DITOLAK: Anggota belum terdaftar.\n"; return; }
        if (!member->canBorrow()) {
            cout << "-> DITOLAK: " << member->getName() << " sudah mencapai batas pinjam.\n";
            return;
        }
        if (!item->borrowItem()) { cout << "-> DITOLAK: Item ini sedang dipinjam orang lain.\n"; return; }

        member->addLoan(itemId);
        loans.push_back(Loan(itemId, memberId, today, item->getLoanDays()));
        cout << "-> BERHASIL: " << member->getName() << " meminjam '" << item->getTitle()
             << "', jatuh tempo hari ke-" << today + item->getLoanDays() << ".\n";
    }
    void returnItem(string itemId, int today) {
        // cari nomor urut peminjaman untuk item ini
        int index = -1;
        for (int i = 0; i < static_cast<int>(loans.size()); i++) {
            if (loans[i].getItemId() == itemId) index = i;
        }
        if (index == -1) { cout << "-> DITOLAK: Tidak ada peminjaman aktif untuk item ini.\n"; return; }

        LibraryItem* item = findItem(itemId);
        Member* member = findMember(loans[index].getMemberId());

        int daysLate = today - loans[index].getDueDay();
        if (daysLate < 0) daysLate = 0;

        double baseFine = item->calculateFine(daysLate);   // tergantung TIPE ITEM
        double discount = member->getFineDiscount();       // tergantung TIPE ANGGOTA
        double finalFine = baseFine - baseFine * discount;

        item->returnItem();
        member->removeLoan(itemId);
        loans.erase(loans.begin() + index);

        cout << "-> BERHASIL: '" << item->getTitle() << "' dikembalikan oleh " << member->getName() << ".\n";
        cout << "   Telat          : " << daysLate << " hari\n";
        cout << "   Denda dasar    : " << rp(baseFine) << "\n";
        cout << "   Diskon anggota : " << static_cast<int>(discount * 100) << "%\n";
        cout << "   Denda akhir    : " << rp(finalFine) << "\n";
    }

    void extendLoan(string itemId, int extraDays) {
        for (auto& loan : loans) {
            if (loan.getItemId() == itemId) {
                if (loan.extendDuration(extraDays))
                    cout << "-> BERHASIL: Jatuh tempo baru hari ke-" << loan.getDueDay() << ".\n";
                else
                    cout << "-> DITOLAK: Tambahan hari harus lebih dari 0.\n";
                return;
            }
        }
        cout << "-> DITOLAK: Data peminjaman tidak ditemukan.\n";
    }

    void search(string title) {
        cout << "-> search(\"" << title << "\") [cari judul]:\n";
        printItemHeader();
        for (auto item : items) {
            if (item->getTitle().find(title) != string::npos) item->display();
        }
    }
    void search(int year) {
        cout << "-> search(" << year << ") [cari tahun]:\n";
        printItemHeader();
        for (auto item : items) {
            if (item->getYear() == year) item->display();
        }
    }
    void search(string type, bool onlyAvailable) {
        cout << "-> search(\"" << type << "\", " << (onlyAvailable ? "true" : "false") << ") [cari tipe]:\n";
        printItemHeader();
        for (auto item : items) {
            if (item->getType() == type && (!onlyAvailable || !item->getIsBorrowed())) item->display();
        }
    }

    void displayAllItems() const {
        cout << "\n--- DAFTAR KOLEKSI PERPUSTAKAAN ---\n";
        printItemHeader();
        // [OOP: POLIMORFISME]
        // Perulangan ini memanggil ->display() pada tipe data LibraryItem*.
        // Tapi saat dijalankan (Runtime), C++ akan cerdas menampilkan format Buku jika 
        // item tersebut aslinya Buku, atau format DVD jika aslinya DVD (Dynamic Polymorphism).
        for (const auto& item : items) {
            item->display(); 
        }
    }
    void displayFineComparison(int daysLate) const {
        cout << "\n--- PERBANDINGAN DENDA (telat " << daysLate << " hari) ---\n";
        cout << left << setw(10) << "TIPE" << setw(28) << "JUDUL" << setw(13) << "LAMA PINJAM"
             << right << setw(12) << "DENDA" << left << "\n";
        cout << string(63, '-') << "\n";
        for (const auto& item : items) {
            cout << left << setw(10) << item->getType()
                 << setw(28) << item->getTitle()
                 << setw(13) << (to_string(item->getLoanDays()) + " hari")
                 << right << setw(12) << rp(item->calculateFine(daysLate)) << left << "\n";
        }
    }
    void displayAllLoans(int today) const {
        cout << "\n--- PEMINJAMAN AKTIF ---\n";
        if (loans.empty()) { cout << "Belum ada peminjaman aktif.\n"; return; }
        cout << left << setw(8) << "ITEM" << setw(12) << "PEMINJAM" << setw(10) << "PINJAM"
             << setw(14) << "JATUH TEMPO" << "STATUS\n";
        cout << string(55, '-') << "\n";
        for (const auto& loan : loans) {
            int late = today - loan.getDueDay();
            cout << left << setw(8) << loan.getItemId()
                 << setw(12) << loan.getMemberId()
                 << setw(10) << ("hari " + to_string(loan.getBorrowDay()))
                 << setw(14) << ("hari " + to_string(loan.getDueDay()))
                 << (late > 0 ? "TELAT " + to_string(late) + " hari" : "Aman") << "\n";
        }
    }
    void displayAllMembers() const {
        cout << "\n--- DAFTAR ANGGOTA ---\n";
        printMemberHeader();
        for (const auto& member : members) {
            member->display(); // Sama seperti koleksi di atas, ini adalah Dynamic Polymorphism
        }
    }
};

//ADDITIONAL FUNCTION

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(string prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cout << "Harus berupa angka! " << prompt;
        cin.clear();
        clearInputBuffer();
    }
    clearInputBuffer();
    return x;
}

string readLine(string prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// ==========================================
// 5. MAIN FUNCTION (PROGRAM UTAMA)
// ==========================================

int main() {
    Library myLib;
    int today = 1;
    // Menambahkan item dengan class turunannya masing-masing
    myLib.addItem(new Book("B01", "Belajar C++ Dasar",2020, "Budi Santoso"));
    myLib.addItem(new Magazine("M01", "Tech Monthly", 2025, 42));
    myLib.addItem(new DVD("D01", "Tutorial OOP", 2022, "Pak Joko"));
    myLib.addItem(new Journal("J01", "AI Research", 2024, 5));

    // Menambahkan member
    myLib.addMember(new Student("S01", "Andi", "Informatika"));
    myLib.addMember(new Lecturer("L01", "Dr. Ratna", "Ilmu Komputer"));

    cout << "=== SELAMAT DATANG DI SISTEM PERPUSTAKAAN ===\n";
    
    myLib.displayFineComparison(2);
    // Menampilkan dengan memanfaatkan Polimorfisme
    myLib.displayAllItems();
    myLib.displayAllMembers();

    cout << "\n--- PROSES PEMINJAMAN ---\n";
    
    // Meminjam buku menggunakan Abstraksi
    myLib.borrowItem("B01", "S01", 1); 
    
    // Sistem Enkapsulasi akan menolak peminjaman karena buku sedang dipinjam
    myLib.borrowItem("B01", "L01", 1); 
    
    //polymorphism
    myLib.returnItem("B01", 20);
    myLib.borrowItem("D01", "L01", 1);
    myLib.returnItem("D01", 6);

    cout << "\n--- STATUS KOLEKSI SETELAH PEMINJAMAN ---\n";
    myLib.displayAllItems();

    while (true) {
        cout << "\n=======================================\n";
        cout << "  SISTEM MANAJEMEN PERPUSTAKAAN (OOP)\n";
        cout << "  Hari simulasi: ke-" << today << "\n";
        cout << "=======================================\n";
        cout << " 1. Lihat Daftar Koleksi        (Polymorphism)\n";
        cout << " 2. Lihat Daftar Anggota        (Polymorphism)\n";
        cout << " 3. Lihat Peminjaman Aktif\n";
        cout << " 4. Tambah Buku Baru            (Inheritance)\n";
        cout << " 5. Pinjam Item                 (Abstraction & Encapsulation)\n";
        cout << " 6. Kembalikan Item             (Polymorphism: denda)\n";
        cout << " 7. Perpanjang Durasi           (Encapsulation)\n";
        cout << " 8. Cari Koleksi                (Static Polymorphism)\n";
        cout << " 9. Bandingkan Denda Semua Tipe (Dynamic Polymorphism)\n";
        cout << "10. Majukan Hari Simulasi\n";
        cout << " 0. Keluar\n";

        int choice = readInt("Pilih menu (0-10): ");
        if (choice == 0) { cout << "Selesai.\n"; break; }

        try {
            if (choice == 1) {
                cout << "[PENERAPAN POLIMORFISME: display() dipanggil lewat pointer LibraryItem*, hasilnya sesuai wujud asli (Buku/Majalah/DVD/Jurnal).]\n";
                myLib.displayAllItems();
            } else if (choice == 2) {
                cout << "[PENERAPAN POLIMORFISME: getMaxLoans() dan getFineDiscount() berbeda untuk Mahasiswa dan Dosen.]\n";
                myLib.displayAllMembers();
            } else if (choice == 3) {
                myLib.displayAllLoans(today);
            } else if (choice == 4) {
                string id = readLine("ID Buku (cth: B02): ");
                string title = readLine("Judul: ");
                int year = readInt("Tahun terbit: ");
                string author = readLine("Penulis: ");
                myLib.addItem(new Book(id, title, year, author));
                cout << "[PENERAPAN INHERITANCE: Book mewarisi id, judul, tahun, status, borrowItem() dari LibraryItem.]\n";
                cout << "-> BERHASIL: Buku ditambahkan.\n";
            } else if (choice == 5) {
                string itemId = readLine("ID Item: ");
                string memberId = readLine("ID Peminjam: ");
                cout << "[PENERAPAN ABSTRAKSI: pengguna cukup memanggil borrowItem(); pencarian dan pengecekan tersembunyi di dalam Library.]\n";
                cout << "[PENERAPAN ENKAPSULASI: status item hanya bisa diubah lewat LibraryItem::borrowItem(); batas pinjam dijaga Member::canBorrow().]\n";
                myLib.borrowItem(itemId, memberId, today);
            } else if (choice == 6) {
                string itemId = readLine("ID Item: ");
                cout << "[PENERAPAN POLIMORFISME: denda dasar dari tipe item, diskon dari tipe anggota.]\n";
                myLib.returnItem(itemId, today);
            } else if (choice == 7) {
                string itemId = readLine("ID Item: ");
                int extra = readInt("Tambahan hari: ");
                cout << "[PENERAPAN ENKAPSULASI: durasi dilindungi, hanya bisa bertambah lewat extendDuration().]\n";
                myLib.extendLoan(itemId, extra);
            } else if (choice == 8) {
                int mode = readInt("Cari berdasarkan (1 = judul, 2 = tahun, 3 = tipe): ");
                if (mode == 1) {
                    myLib.search(readLine("Kata kunci judul: "));
                } else if (mode == 2) {
                    myLib.search(readInt("Tahun: "));
                } else if (mode == 3) {
                    string type = readLine("Tipe (Buku / Majalah / DVD / Jurnal): ");
                    string a = readLine("Hanya yang tersedia? (y/n): ");
                    myLib.search(type, a == "y");
                } else {
                    cout << "Pilihan tidak valid.\n";
                }
                cout << "[PENERAPAN POLIMORFISME STATIS: satu nama search(), compiler memilih versi dari tipe dan jumlah argumen.]\n";
            } else if (choice == 9) {
                int d = readInt("Jumlah hari terlambat: ");
                myLib.displayFineComparison(d);
            } else if (choice == 10) {
                int d = readInt("Majukan berapa hari? ");
                if (d > 0) { today += d; cout << "-> Sekarang hari ke-" << today << ".\n"; }
                else cout << "-> DITOLAK: Harus lebih dari 0.\n";
            } else {
                cout << "Pilihan tidak valid.\n";
            }
        } catch (const invalid_argument& e) {
            cout << "[PENERAPAN ENKAPSULASI: constructor menolak data yang melanggar aturannya sendiri.]\n";
            cout << "-> DITOLAK: " << e.what() << "\n";
        }
    }

    return 0;
} 