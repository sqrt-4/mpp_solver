#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ginac/ginac.h>
#include <gui/systemModel.h>
#include <iface/converter.h>
#include <helpers/converter.h>
#include <mpp/engine.h>
#include <iface/plotter.h>
#include <helpers/plotter.h>
#include <QPixmap>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MPP Solver");

    m_system = std::make_unique<gui::System>(gui::System());

    ui->errorsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->errorsLabel->setWordWrap(true);

    setupEquationsTable();

    connect(ui->addEqButton, SIGNAL(clicked()), this, SLOT(addEquation()));
    connect(ui->removeEqButton, SIGNAL(clicked()), this, SLOT(removeEquation()));
    connect(ui->calculateButton, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(ui->equationsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateModelEquation(QTableWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupEquationsTable()
{
    ui->equationsTable->setColumnCount(3);
    ui->equationsTable->setRowCount(0);
    ui->equationsTable->setShowGrid(true);
    QStringList headers;
    headers << "F" << "R(x(a), x(b))" << "p0";
    ui->equationsTable->setHorizontalHeaderLabels(headers);
    ui->equationsTable->resizeRowsToContents();
    ui->equationsTable->setColumnWidth(0, 250);
    ui->equationsTable->setColumnWidth(1, 180);
    ui->equationsTable->setColumnWidth(2, 80);
}

void MainWindow::addEquation()
{
    m_system->addEquation("");
    m_system->addR("");
    m_system->addP0(0.);
    ui->equationsTable->setRowCount(m_system->getEquations().size());
}

void MainWindow::removeEquation()
{
    const size_t curRow = ui->equationsTable->currentRow();
    if (curRow < 0 || curRow > m_system->getEquations().size())
    {
        return;
    }
    m_system->removeEquation(curRow);
    m_system->removeR(curRow);
    m_system->removeP0(curRow);
    ui->equationsTable->removeRow(curRow);
}

void MainWindow::updateResTable(const std::vector<std::vector<double>>& res)
{
    ui->resTable->setColumnCount(res[0].size());
    ui->resTable->setRowCount(res.size());
    for (size_t i = 0; i < res.size(); ++i)
        for (size_t j = 0; j < res[i].size(); ++j)
        {
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(res[i][j]));
            ui->resTable->setItem(i, j, item);
        }
    QStringList headers;
    headers << "t";
    for (size_t j = 1; j < res[0].size(); ++j)
    {
        std::string header = "x" + std::to_string(j);
        headers << header.c_str();
    }
    ui->resTable->setHorizontalHeaderLabels(headers);
}

void MainWindow::calculate() try
{
    ui->errorsLabel->setText("");
    m_system->setTa(ui->ta_textEdit->toPlainText().toDouble());
    m_system->setTb(ui->tb_textEdit->toPlainText().toDouble());
    m_system->setTs(ui->ts_textEdit->toPlainText().toDouble());
    std::unique_ptr<mpp::IConverter> conv(new mpp::Converter);
    std::unique_ptr<mpp::ISystem> system(conv->convertToMppModel(m_system.get()));
    std::unique_ptr<mpp::IEngine> engine(new mpp::Engine);
    const auto res = engine->compute_solution(system.get());
    updateResTable(res);
}
catch(const std::exception& err)
{
    ui->errorsLabel->setText(QString("error occured: ") + err.what());
}

void MainWindow::updateModelEquation(QTableWidgetItem* item)
{
    switch (item->column())
    {
    case 0:
        m_system->updateEquation(item->row(), item->text());
        break;
    case 1:
        m_system->updateR(item->row(), item->text());
        break;
    case 2:
        m_system->updateP0(item->row(), item->text().toDouble());
        break;
    default:
        return;
    }
}

void MainWindow::on_plotButton_clicked()
{
    try
    {
        ui->errorsLabel->setText("");
        std::unique_ptr<IPlotter> pl = std::make_unique<mpp::Plotter>();
        const std::string ax1(ui->axis1TF->toPlainText().toStdString());
        const std::string ax2(ui->axis2TF->toPlainText().toStdString());

        QString imgPath(pl->createPng(ax1, ax2).c_str());
        QPixmap pixmap;
        if (!pixmap.load(imgPath))
        {
            throw std::runtime_error("failed to load image");
        }
        ui->PlotLabel->setPixmap(pixmap);
    }
    catch(const std::exception& err)
    {
        ui->errorsLabel->setText(QString("error occured: ") + err.what());
    }
}

