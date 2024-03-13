#pragma once

#include "model.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(model &model, QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  model &m_model;
  QTimer m_timer;
  int m_shift;

  void _updateUI(quint64 value);

  // QTC_TEMP
  // QWidget interface
  protected:
  virtual void closeEvent(QCloseEvent* event) override;
};
