#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"

settingswindow::settingswindow(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::settingswindow) {
  ui->setupUi(this);


  // set window's title
  this->setWindowTitle("Settings");

  // set window's icon
  this->setWindowIcon(QIcon(":/icons/Sharingan/settings.png"));

  set_button_image(ui->sharinganImageButton, ":/icons/Sharingan/icon.png");
  set_button_image(ui->rinneganImageButton, ":/icons/Rinnegan/icon.png");
}

settingswindow::~settingswindow() {
  delete ui;
}

void settingswindow::set_button_image(QPushButton* button, QString imagePath) {
  QString buttonName = button->objectName();

  button->setStyleSheet(
      "#" + buttonName + " " + "{"
      "background-color: transparent;"
      "border-image: url(" + imagePath + ");"
      "background: none;"
      "border: none;"
      "background-repeat: none;"
      "}"
  );
}

void settingswindow::on_applyButton_clicked() {
  qint64 theme;

  if (ui->sharinganRadioButton->isChecked()) {
    theme = MainWindow::theme::SHARINGAN;

  } else if (ui->rinneganRadioButton->isChecked()) {
    theme = MainWindow::theme::RINNEGAN;
  }

  emit sendCurrentTheme(theme);
}

