#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTabBar>

QTabBar *tabBar;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  tabBar = new QTabBar(ui->widget);
  tabBar->setObjectName("tabBar");
  tabBar->addTab("Synthesis");
  tabBar->addTab("Template");
  tabBar->addTab("Sketch");
  tabBar->setCurrentIndex(1);
  ui->widget = tabBar;

  QObject::connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(changeScene(int)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::changeScene(int index) {
  switch (index) {
  case 2:
    ui->glwidget->startObjectScene();
    break;
  case 1:
    ui->glwidget->startTemplateScene();
    break;
  case 0:
    ui->glwidget->startSynthesisScene();
    break;
  }
  QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::on_pushButton_2_clicked() { ui->glwidget->m_pipeline->mapSamplesToTemplate(ui->glwidget->objectScene->samples); }

void MainWindow::on_pushButton_3_clicked() { ui->glwidget->m_pipeline->start(&ui->glwidget->m_program); }

void MainWindow::on_pushButton_5_clicked() { ui->glwidget->m_pipeline->registerPoints(); }

void MainWindow::on_toolButton_clicked() {
  if (ui->glwidget->state == STATE_OBJECTS) {
    ui->glwidget->objectScene->setState(STATE_MOVE);
  }
}

void MainWindow::on_toolButton_2_clicked() {
  if (ui->glwidget->state == STATE_OBJECTS) {
    ui->glwidget->objectScene->setState(STATE_SKETCH);
  }
}

void MainWindow::on_toolButton_4_clicked() {
  //    if(ui->glwidget->state == STATE_OBJECTS) {
  ui->glwidget->objectScene->clearUserPoints();
  ui->glwidget->m_pipeline->resetRegistration();
  //    }
}

void MainWindow::on_toolButton_3_clicked() { ui->glwidget->objectScene->setState(STATE_DEFAULT); }
