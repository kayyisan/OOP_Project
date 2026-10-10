#include <iostream>
#include <string>
#include <vector>

using namespace std;

// barang perpustakaan 

class LibraryItem {
    /*ENKAPSULASI
    cegah variabel dapat diubah dari main
    */
    protected:
        string id;
        string title;
        bool isBorrowed;
    
    public:
        LibraryItem(string id, string title) : id(id), title(title), isBorrowed(false) {}
        virtual ~LibraryItem() {}

        // enkapsulasi getter setter
        string getId() const {
            return id;
        }
        string getTittle() const {
            return title;
        }
        bool getIsBorrowed() const{
            return isBorrowed;
        }
        void setBorrowed(bool status) {
            isBorrowed = status;
        } 

        //polimorfisme (agar bisa digunakan all its child class)
        virtual void display() const {
            cout << "ID: " << id << " | Judul: " << title
                 << " | Status: " << (isBorrowed ? "Dipinjam" : "Tersedia");
        }
};

//inheritance (mewarisi semua sifat library)
class Book : public LibraryItem {
    private:
        string author;
    public:
        Book(string id, string title, string author) : LibraryItem(id, title), author(author) {}
        //polimorfisme (override induk + output nama)
        void display () const override{
            LibraryItem::display();
            cout << " | Penulis: " << author << " [Buku]\n";
        }
};

// inheritance turunan dari library
class Magazine : public LibraryItem{
    private:
        int issueNumber;
    public:
    Magazine(string id, string title, int issue) : LibraryItem(id, title), issueNumber(issue) {}
    void display() const override{
        LibraryItem :: display();
        cout << " | Edisi : " << issueNumber << " [Majalah]\n";
    }
};

// inheritance turunan dari library
class DVD : public LibraryItem{
    private :
        string director;
    public:
        DVD(string id, string title, string director) : LibraryItem(id, title), director(director) {}
        void display() const override{
            LibraryItem :: display();
            cout << " | Sutradara: " << director << " [DVD]\n";
        }
        
};

//inheritance turunan dari library
class Journal : public LibraryItem{
    private:
        int volume;
    public:
        Journal(string id, string title, int volume) : LibraryItem(id, title), volume(volume){}
        void display() const override(){
            LibraryItem::display();
            cout << " Volume: " << volume << " [Jurnal]\n";
        }
};

//ANGGOTA PERPUSTAKAAN
class Member {
    protected:
        string memberId;
        string name;
    public:
        Member(string id, string name) : memberId(id), name(name){}
        virtual ~Member(){}
        string getId() const {
            retun memberId;
        }
        string getName() const{
            return name;
        }
        virtual void display() const{
            cout << "ID Anggota: " << memberId << " | Nama: " << name;
        }
};

class Student :: public Member{
    private:
        string major;
    public:
        void display() const override{
            Member::display();
            cout << " | Jurusan: " << major << " [Mahasiswa]\n";
        }
};

class Lecturer : public Member {
    private:
        string department;
    public:
        Lecturer(int string id, string title, string department) : Member(id, title), department(department){}
        void display() const override{
            Member::display();
            cout << " | Department: " << department << " [Dosen]\n";
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
    string loanDate;

public:
    Loan(string item, string member, string date) : itemId(item), memberId(member), loanDate(date) {}
    void display() const {
        cout << "Item ID: " << itemId << " dipinjam oleh Anggota ID: " << memberId 
             << " pada tanggal " << loanDate << "\n";
    }
};

// ==========================================
// 4. KELAS LIBRARY (MANAJEMEN UTAMA)
// ==========================================

class Library {
private:
    // [OOP: POLIMORFISME] 
    // Menyimpan BERBAGAI JENIS objek (Book, DVD, Journal) ke dalam SATU TIPE wadah 
    // menggunakan pointer ke kelas Induk (LibraryItem*). 
    vector<LibraryItem*> items;
    vector<Member*> members;
    vector<Loan> loans;

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
    void borrowItem(string itemId, string memberId, string date) {
        LibraryItem* foundItem = nullptr;
        
        // Proses pencarian disembunyikan dari pengguna
        for (auto item : items) {
            if (item->getId() == itemId) {
                foundItem = item;
                break;
            }
        }

        if (foundItem != nullptr) {
            // [OOP: ENKAPSULASI]
            // Validasi state (status) dikendalikan secara mandiri oleh sistem.
            // Buku tidak bisa dipinjam paksa jika sedang berstatus dipinjam.
            if (!foundItem->getIsBorrowed()) {
                foundItem->setBorrowed(true);
                loans.push_back(Loan(itemId, memberId, date));
                cout << "-> Berhasil! Item " << foundItem->getTitle() << " telah dipinjam.\n";
            } else {
                cout << "-> Maaf, item ini sedang dipinjam orang lain.\n";
            }
        } else {
            cout << "-> Item tidak ditemukan.\n";
        }
    }

    void displayAllItems() const {
        cout << "\n--- DAFTAR KOLEKSI PERPUSTAKAAN ---\n";
        // [OOP: POLIMORFISME]
        // Perulangan ini memanggil ->display() pada tipe data LibraryItem*.
        // Tapi saat dijalankan (Runtime), C++ akan cerdas menampilkan format Buku jika 
        // item tersebut aslinya Buku, atau format DVD jika aslinya DVD (Dynamic Polymorphism).
        for (const auto& item : items) {
            item->display(); 
        }
    }

    void displayAllMembers() const {
        cout << "\n--- DAFTAR ANGGOTA ---\n";
        for (const auto& member : members) {
            member->display(); // Sama seperti koleksi di atas, ini adalah Dynamic Polymorphism
        }
    }
};

// ==========================================
// 5. MAIN FUNCTION (PROGRAM UTAMA)
// ==========================================

int main() {
    Library myLib;

    // Menambahkan item dengan class turunannya masing-masing
    myLib.addItem(new Book("B01", "Belajar C++ Dasar", "Budi Santoso"));
    myLib.addItem(new Magazine("M01", "Tech Monthly", 42));
    myLib.addItem(new DVD("D01", "Tutorial OOP", "Pak Joko"));
    myLib.addItem(new Journal("J01", "AI Research", 5));

    // Menambahkan member
    myLib.addMember(new Student("S01", "Andi", "Informatika"));
    myLib.addMember(new Lecturer("L01", "Dr. Ratna", "Ilmu Komputer"));

    cout << "=== SELAMAT DATANG DI SISTEM PERPUSTAKAAN ===\n";
    
    // Menampilkan dengan memanfaatkan Polimorfisme
    myLib.displayAllItems();
    myLib.displayAllMembers();

    cout << "\n--- PROSES PEMINJAMAN ---\n";
    
    // Meminjam buku menggunakan Abstraksi
    myLib.borrowItem("B01", "S01", "10-Okt-2026"); 
    
    // Sistem Enkapsulasi akan menolak peminjaman karena buku sedang dipinjam
    myLib.borrowItem("B01", "L01", "11-Okt-2026"); 

    cout << "\n--- STATUS KOLEKSI SETELAH PEMINJAMAN ---\n";
    myLib.displayAllItems();

    return 0;
} 