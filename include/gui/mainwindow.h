#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iface/system.h>
#include <memory>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addEquation();
    void removeEquation();
    void calculate();
    void updateModelEquation(QTableWidgetItem* item);
    //void //();
    //void removeR();
    void on_plotButton_clicked();

private:
    void setupEquationsTable();
    void updateResTable(const std::vector<std::vector<double>>& res);

private:
    Ui::MainWindow* ui;
    std::unique_ptr<gui::ISystem> m_system;
};
#endif // MAINWINDOW_H
