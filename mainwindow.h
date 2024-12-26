#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "Database.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SavePatient();
    void byStatus();

private:
    void savePatientsToFile(); // Збереження даних у файл
    void loadPatientsFromFile(); // Завантаження даних із файлу

    QLineEdit *patientName;
    QLineEdit *doctorName;
    QLineEdit *speciality;
    QLineEdit *date;
    QLineEdit *time;
    QLineEdit *status;
    QLineEdit *statusSearch;
    QPushButton *saveButton;
    QPushButton *byStatusButton;
    QTextEdit *patientList;
    Database db;
};
