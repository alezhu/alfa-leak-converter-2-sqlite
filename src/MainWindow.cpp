#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QThreadPool>

MainWindow::MainWindow(model &model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_model{model},
      m_timer{this} {
  ui->setupUi(this);
  ui->pbMain->reset();
  ui->pbMain->setMinimum(0);
  auto fileSize = m_model.fileSize();

  ui->lblTotalBytes->setText(QString("/") + QString::number(fileSize));

  m_shift = 63;
  quint64 one = 1;
  one <<= m_shift;
  while ((fileSize & one) == 0) {
    m_shift--;
    one >>= 1;
  }
  if (m_shift >= 30) {
    m_shift -= 30;
  }
  int pbValue = static_cast<uint32_t>(fileSize >> m_shift) & 0x7FFFFFFF;
  ui->pbMain->setMaximum(pbValue);

  QObject::connect(ui->btnStop, &QPushButton::clicked, &model, &model::stop);
  QObject::connect(ui->btnPause, &QPushButton::clicked, &model,
                   &model::togglePause);
  QObject::connect(&model, &model::stoped, this, &MainWindow::close);
  QObject::connect(&model, &model::paused, this,
                   [&]() { ui->btnPause->setText("Resume"); });
  QObject::connect(&model, &model::resumed, this,
                   [&]() { ui->btnPause->setText("Pause"); });

  // QObject::connect(&model, &model::positionChanged, this,
  // &MainWindow::_updateUI);

  connect(&m_timer, &QTimer::timeout, this, [&]() {
    auto pos = m_model.position();
    ui->lblProcessedBytes->setText(QString::number(pos));
    _updateUI(pos);
  });
  m_timer.start(1000);

  QThreadPool::globalInstance()->start([&]() { m_model.start(); });
}

MainWindow::~MainWindow() {
  m_model.stop();
  delete ui;
}

void MainWindow::_updateUI(quint64 value) {
  auto pbValue = static_cast<uint32_t>(value >> m_shift) & 0x7FFFFFFF;
  ui->pbMain->setValue(pbValue);
  ui->lblUsers->setText(QString::number(m_model.processedUsers()));
}

void MainWindow::closeEvent(QCloseEvent *) { m_model.stop(); }
