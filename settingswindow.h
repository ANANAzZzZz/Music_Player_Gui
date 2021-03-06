#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class settingswindow;
}

class settingswindow : public QDialog {
  Q_OBJECT

 public:
  explicit settingswindow(QWidget* parent = nullptr);
  ~settingswindow();

 private:
  Ui::settingswindow* ui;

 private slots:
  void set_button_image(QPushButton* button, QString imagePath);

  void on_applyButton_clicked();

 private:
  qint64* theme;

 signals:
  void sendCurrentTheme(qint64 curTheme);

};

#endif // SETTINGSWINDOW_H
