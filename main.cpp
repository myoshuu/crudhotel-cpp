#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif

struct Pemesanan
{
  std::string hari;
  std::string namaPengunjung;
  int banyakPengunjung;
};

std::vector<Pemesanan> pesanan;
std::string filename = "";

std::string generateFileName(const std::string &customName = "")
{
  if (!customName.empty())
    return customName + ".txt";

  time_t t = time(0);
  tm *now = localtime(&t);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "pemesanan_%Y%m%d.txt", now);
  return std::string(buffer);
}

bool isValidDay(const std::string &day)
{
  std::string days[] = {"senin", "selasa", "rabu", "kamis", "jumat", "sabtu", "minggu"};
  for (const auto &d : days)
  {
    if (d == day)
      return true;
  }
  return false;
}

void clearScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void readFile()
{
  pesanan.clear();

  std::ifstream file(filename);
  if (file.is_open())
  {
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      Pemesanan p;
      std::string no;
      iss >> no >> p.hari >> p.namaPengunjung >> p.banyakPengunjung;
      pesanan.push_back(p);
    }
    file.close();
  }
}

void saveToFile()
{
  std::ofstream file(filename);
  if (file.is_open())
  {
    file << std::left << std::setw(10) << "No"
         << std::setw(15) << "Hari"
         << std::setw(20) << "Nama Pengunjung"
         << std::setw(20) << "Banyak Pengunjung" << std::endl;

    for (size_t i = 0; i < pesanan.size(); ++i)
    {
      const auto &p = pesanan[i];
      file << std::left << std::setw(10) << i + 1
           << std::setw(15) << p.hari
           << std::setw(20) << p.namaPengunjung
           << std::setw(20) << p.banyakPengunjung << std::endl;
    }
    file.close();
  }

  std::cout << "Berhasil menyimpan data ke file " << filename << std::endl;
  sleep(2);
  clearScreen();
}

void createData(const Pemesanan &p)
{
  pesanan.push_back(p);
  saveToFile();
}

void readData()
{
  std::cout << std::left << std::setw(10) << "Index"
            << std::setw(15) << "Hari"
            << std::setw(20) << "Nama Pengunjung"
            << std::setw(20) << "Banyak Pengunjung"
            << std::endl;
  std::cout << std::string(65, '-') << std::endl;

  for (size_t i = 0; i < pesanan.size(); ++i)
  {
    const auto &p = pesanan[i];
    std::cout << std::left << std::setw(10) << i
              << std::setw(15) << p.hari
              << std::setw(20) << p.namaPengunjung
              << std::setw(20) << p.banyakPengunjung
              << std::endl;
  }
}

void updateData(size_t index)
{
  if (index < pesanan.size())
  {
    Pemesanan &p = pesanan[index];
    std::string input;

    std::cout << "Perhatian!!" << std::endl
              << "Jika tidak ingin mengubah data, cukup tekan enter" << std::endl;

    std::cin.ignore();

    do
    {
      std::cout << std::endl
                << "Masukkan hari baru ('" << p.hari << "'): ";
      std::getline(std::cin, input);
      if (!input.empty())
      {
        for (auto &c : input)
        {
          c = tolower(c);
        }
        if (!isValidDay(input))
        {
          std::cout << "Data tidak valid" << std::endl;
          sleep(2);
          continue;
        }
        p.hari = input;
      }
    } while (!input.empty() && !isValidDay(input));

    std::cout << "Masukkan Nama Pengunjung baru ('" << p.namaPengunjung << "'): ";
    std::getline(std::cin, input);
    if (!input.empty())
    {
      p.namaPengunjung = input;
    }

    std::cout << "Masukkan Banyak Pengunjung baru ('" << p.banyakPengunjung << "'): ";
    std::getline(std::cin, input);
    if (!input.empty())
    {
      p.banyakPengunjung = std::stoi(input);
    }

    saveToFile();
  }
  else
  {
    std::cout << "Data tidak ditemukan" << std::endl;
    sleep(2);
    clearScreen();
  }
}

void deleteData(size_t index)
{
  if (index < pesanan.size())
  {
    pesanan.erase(pesanan.begin() + index);
    saveToFile();
  }
  else
  {
    std::cout << "Data tidak ditemukan" << std::endl;
    sleep(2);
    clearScreen();
  }
}

int main()
{
  int pilihan;
  Pemesanan p;
  std::string namaPengunjung;
  size_t index;

  filename = generateFileName();
  readFile();

  do
  {
    clearScreen();
    std::cout << "Menu:" << std::endl;
    std::cout << "1. Tambah Pemesanan" << std::endl;
    std::cout << "2. Tampilkan Semua Pemesanan" << std::endl;
    std::cout << "3. Perbarui Pemesanan" << std::endl;
    std::cout << "4. Hapus Pemesanan" << std::endl;
    std::cout << "5. Keluar" << std::endl;
    std::cout << "Pilih: ";
    std::cin >> pilihan;

    clearScreen();

    switch (pilihan)
    {
    case 1:
      if (pesanan.size() > 0)
      {
        char useExistingFile;
        std::string newFileName;

        std::cout << "Apakah anda ingin menggunakan file yang sudah ada? (y/n): ";
        std::cin >> useExistingFile;
        if (useExistingFile == 'n')
        {
          std::cout << "Masukkan nama file: ";
          std::cin >> newFileName;
          filename = generateFileName(newFileName);
          readFile();
        }
        else
        {
          filename = generateFileName();
          readFile();
        }
      }
      do
      {
        std::cout << "Hari (ketik '0' untuk berhenti): ";
        std::cin >> p.hari;
        for (auto &c : p.hari)
          c = tolower(c);

        if (p.hari == "0")
          break;

        if (!isValidDay(p.hari))
        {
          std::cout << "Data tidak valid" << std::endl;
          sleep(2);
          clearScreen();
          continue;
        }

        std::cout << "Nama Pengunjung: ";
        std::cin >> p.namaPengunjung;
        std::cout << "Banyak Pengunjung: ";
        std::cin >> p.banyakPengunjung;

        createData(p);
      } while (true);
      break;
    case 2:
      readData();
      std::cout << std::endl
                << "Tekan enter untuk melanjutkan...";
      std::cin.ignore();
      std::cin.get();
      clearScreen();
      break;
    case 3:
      readData();
      std::cout << std::endl
                << "Index data yang ingin diperbarui: ";
      std::cin >> index;
      if (index < pesanan.size())
        updateData(index);
      else
        std::cout << "Data tidak ditemukan" << std::endl;

      break;
    case 4:
      readData();
      std::cout << std::endl
                << "Index data yang ingin dihapus: ";
      std::cin >> index;
      deleteData(index);
      break;
    case 5:
      std::cout << "Terima kasih!!";
      break;
    }

  } while (pilihan != 5);
}