#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

// Конструктор класу MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), db() // Ініціалізація бази даних
{
    // Ініціалізація полів вводу
    patientName = new QLineEdit(this);
    doctorName = new QLineEdit(this);
    speciality = new QLineEdit(this);
    date = new QLineEdit(this);
    time = new QLineEdit(this);
    status = new QLineEdit(this);
    statusSearch = new QLineEdit(this);

    // Ініціалізація кнопок
    saveButton = new QPushButton("Save and Display Patients", this);
    byStatusButton = new QPushButton("Search by Status", this);

    // Поле для відображення списку пацієнтів
    patientList = new QTextEdit(this);
    patientList->setReadOnly(true); // Заборона редагування

    // Створення основного компонування
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Додавання полів вводу та підписів
    layout->addWidget(new QLabel("Patient Name:"));
    layout->addWidget(patientName);
    layout->addWidget(new QLabel("Doctor Name:"));
    layout->addWidget(doctorName);
    layout->addWidget(new QLabel("Speciality:"));
    layout->addWidget(speciality);
    layout->addWidget(new QLabel("Date:"));
    layout->addWidget(date);
    layout->addWidget(new QLabel("Time:"));
    layout->addWidget(time);
    layout->addWidget(new QLabel("Status:"));
    layout->addWidget(status);

    // Додавання кнопок
    layout->addWidget(saveButton);

    layout->addWidget(new QLabel("Search by Status:"));
    layout->addWidget(statusSearch);
    layout->addWidget(byStatusButton);

    // Додавання поля для списку пацієнтів
    layout->addWidget(new QLabel("Patient Records:"));
    layout->addWidget(patientList);

    setCentralWidget(centralWidget);

    // Підключення сигналів кнопок до відповідних слотів
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::SavePatient);
    connect(byStatusButton, &QPushButton::clicked, this, &MainWindow::byStatus);

    // Завантаження записів з файлу під час запуску програми
    loadPatientsFromFile();
}

// Деструктор класу MainWindow
MainWindow::~MainWindow() {}

// Слот для збереження даних про пацієнта
void MainWindow::SavePatient() {
    // Отримання тексту з полів вводу
    QString patientNameText = patientName->text();
    QString doctorNameText = doctorName->text();
    QString specialityText = speciality->text();
    QString dateText = date->text();
    QString timeText = time->text();
    QString statusText = status->text();

    // Перевірка заповненості всіх полів
    if (patientNameText.isEmpty() || doctorNameText.isEmpty() || specialityText.isEmpty() ||
        dateText.isEmpty() || timeText.isEmpty() || statusText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled!");
        return;
    }

    // Додавання запису до бази даних
    db.addPatient(patientNameText.toStdString(), doctorNameText.toStdString(),
                  specialityText.toStdString(), dateText.toStdString(),
                  timeText.toStdString(), statusText.toStdString());

    // Збереження бази даних у файл
    savePatientsToFile();

    // Очищення полів вводу
    patientName->clear();
    doctorName->clear();
    speciality->clear();
    date->clear();
    time->clear();
    status->clear();

    // Оновлення списку пацієнтів в інтерфейсі
    patientList->clear();
    std::vector<std::string> allPatients = db.getAllPatients();
    for (const std::string &record : allPatients) {
        patientList->append(QString::fromStdString(record));
    }
}

// Слот для пошуку пацієнтів за статусом
void MainWindow::byStatus() {
    QString searchStatus = statusSearch->text();

    // Перевірка заповненості поля пошуку
    if (searchStatus.isEmpty()) {
        QMessageBox::warning(this, "Search Error", "Please enter a status to search!");
        return;
    }

    // Пошук записів у базі даних
    std::vector<std::string> results = db.getPatientsByStatus(searchStatus.toStdString());

    // Відображення результатів у списку
    patientList->clear();
    for (const std::string &record : results) {
        patientList->append(QString::fromStdString(record));
    }
}

// Збереження бази даних у файл
void MainWindow::savePatientsToFile() {
    QFile file("patients.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        std::vector<std::string> allPatients = db.getAllPatients();
        for (const std::string &record : allPatients) {
            out << QString::fromStdString(record) << "\n";
        }
        file.close();
    } else {
        QMessageBox::warning(this, "File Error", "Unable to save patients to file!");
    }
}

// Завантаження бази даних із файлу
void MainWindow::loadPatientsFromFile() {
    QFile file("patients.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                // Імітація додавання запису із файлу
                patientList->append(line);
            }
        }
        file.close();
    }
}
