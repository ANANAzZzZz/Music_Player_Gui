#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"

/*
// set item's text color
ui->listWidget->item(currentTrackPosition)->setForeground(QColor("#ce1e05"));

// set previous item's text color
ui->listWidget->item(currentTrackPosition - 1)->setForeground(Qt::black);
*/

// purple - #af5fff

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow) {

  isPlaying = false;
  isLooped = false;
  isRandomed = false;

  currentTheme = "Sharingan";

  ui->setupUi(this);

  set_theme();

  player = new QMediaPlayer(this);
  audioOutput = new QAudioOutput(this);

  connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
  connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_playMusicButton_clicked() {
  if (!player->hasAudio()) {
    return;
  }

  if (isPlaying) {
    player->pause();

    set_button_image(ui->playMusicButton, get_path_to_icon("play.png"));

    isPlaying = false;

  } else if (!isPlaying) {
    player->play();

    set_button_image(ui->playMusicButton, get_path_to_icon("pause.png"));

    isPlaying = true;
  }
}

void MainWindow::on_sliderProgress_sliderMoved(int position) {
  player->setPosition(position);
}

void MainWindow::on_sliderVolume_valueChanged(int position) {
  audioOutput->setVolume(position * 0.01);

  volumeLevel = QString::number(position);
  ui->volumeValueLabel->setText(volumeLevel);
}

void MainWindow::on_positionChanged(qint64 position) {
  // move slider on the screen with every second passed
  ui->sliderProgress->setValue(position);

  currentTrackDuration = QDateTime::fromMSecsSinceEpoch(position).toUTC().toString("mm:ss");

  // change the time on timestamp label
  ui->timeStampBeginning->setText(currentTrackDuration);

  if (currentTrackDuration == maxTrackDuration) {

    if (!isLooped && !isRandomed) {
      on_nextTrackButton_clicked();
    }

    if (isLooped) {
      restart_track();
    }

    if (!isLooped && isRandomed) {
      set_random_track();
    }
  }
}

void MainWindow::on_durationChanged(qint64 position) {
  ui->sliderProgress->setMaximum(position);

  maxTrackDuration = QDateTime::fromMSecsSinceEpoch(position).toUTC().toString("mm:ss");

  // set ending timing on right timestamp
  ui->timeStampEnding->setText(maxTrackDuration);
}

void MainWindow::load_tracks(QString path) {
  player->stop();
  ui->listWidget->clear();

  QDirIterator track(path, {"*.mp3", "*.waw", "*.webm"}, QDir::Files);

  while (track.hasNext()) {
    track.next();
    QListWidgetItem* newTrack = new QListWidgetItem;

    // set track's icon in list
    newTrack->setIcon(QIcon(get_path_to_icon("icon.png")));

    // set track's name in list
    newTrack->setText(track.fileName());

    // add track's itemo to the list
    ui->listWidget->addItem(newTrack);
  }

  // set tracks amount
  tracksAmount = ui->listWidget->count();
}

void MainWindow::on_choose_directory_button_clicked() {
  path = QFileDialog::getExistingDirectory(this, "Choose The Path");

  if (path == nullptr) {
    ui->listWidget->clear();

    return;
  }

  load_tracks(path);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem* item) {
  currentTrack = item;
  play_track_with_item(item);
}

void MainWindow::on_nextTrackButton_clicked() {
  if (player->hasAudio()) {
    QListWidgetItem* nextTrack;
    qint64 currentTrackPosition = ui->listWidget->row(currentTrack);
    qint64 tracksAmount = ui->listWidget->count();

    if (isRandomed) {
      set_random_track();
    } else {

      if ((currentTrackPosition + 1) == (tracksAmount)) {
        nextTrack = ui->listWidget->item(0);
        currentTrack = nextTrack;

      } else {
        nextTrack = ui->listWidget->item(currentTrackPosition + 1);
        currentTrack = nextTrack;

      }

      if (nextTrack == nullptr) {
        return;
      }

      play_track_with_item(nextTrack);

    }
  }
}

void MainWindow::on_previousTrackButton_clicked() {
  if (player->hasAudio()) {

    QListWidgetItem* previousTrack;
    qint64 currentTrackPosition = ui->listWidget->row(currentTrack);

    if (currentTrackDuration > "00:04") {
      restart_track();
    } else {

      if ((currentTrackPosition) == (0)) {
        previousTrack = ui->listWidget->item(tracksAmount - 1);
        currentTrack = previousTrack;

      } else {
        previousTrack = ui->listWidget->item(currentTrackPosition - 1);
        currentTrack = previousTrack;
      }

      if (previousTrack != NULL) {
        play_track_with_item(previousTrack);

      } else {
        return;
      }
    }
  }
}

void MainWindow::on_loopTrackButton_clicked() {
  if (!isLooped) {
    set_button_image(ui->loopTrackButton, get_path_to_icon("repeat_on.png"));

    isLooped = true;

  } else {
    set_button_image(ui->loopTrackButton, get_path_to_icon("repeat.png"));

    isLooped = false;
  }
}

void MainWindow::play_track_with_item(QListWidgetItem* trackItem) {
  QString track = path + "/" + trackItem->text();
  qint64 currentTrackPosition = ui->listWidget->row(trackItem);

  player->setAudioOutput(audioOutput);
  player->setSource(QUrl::fromLocalFile(track));
  player->play();

  // set track highlighting
  ui->listWidget->item(currentTrackPosition)->setSelected(true);

  // output track's name on the screen
  ui->trackName->setText(trackItem->text());

  // set pause icon
  set_button_image(ui->playMusicButton, get_path_to_icon("pause.png"));

  isPlaying = true;
}

void MainWindow::restart_track() {
  if (currentTrack != NULL) {
    ui->sliderProgress->setValue(0);
    play_track_with_item(currentTrack);
  }
}

void MainWindow::on_setRandomButton_clicked() {
  if (!isRandomed) {
    set_button_image(ui->setRandomButton, get_path_to_icon("shuffle_on.png"));

    isRandomed = true;

  } else {
    set_button_image(ui->setRandomButton, get_path_to_icon("shuffle.png"));

    isRandomed = false;
  }
}

void MainWindow::set_random_track() {
  qint64 randomTrackNumber;
  qint64 currentTrackNumber;

  if (player->hasAudio()) {
    currentTrackNumber = ui->listWidget->row(currentTrack);

    randomTrackNumber = 0 + (rand() % static_cast<int>(tracksAmount));

    if (randomTrackNumber == currentTrackNumber) {
      return set_random_track();
    }

    currentTrack = ui->listWidget->item(randomTrackNumber);

    if (currentTrack == nullptr) {
      return;
    }

    play_track_with_item(currentTrack);

  }
}

void MainWindow::set_theme() {
  // set window's title
  this->setWindowTitle(currentTheme + " player");

  // set window's icon
  this->setWindowIcon(QIcon(get_path_to_icon("icon.png")));


  // set listWidget's icons size
  ui->listWidget->setIconSize(QSize(30, 30));

  // setting indents between list items
  ui->listWidget->setStyleSheet("QListWidget {padding: 5px;} QListWidget::item { margin: 5px; }");

  // set color and shape for sliders
  set_slider_handle_color(ui->sliderVolume);
  set_slider_handle_color(ui->sliderProgress);

  // set Buttons icons
  set_button_image(ui->previousTrackButton, get_path_to_icon("back.png"));
  set_button_image(ui->nextTrackButton, get_path_to_icon("forward.png"));
  set_button_image(ui->choose_directory_button, get_path_to_icon("directory.png"));
  set_button_image(ui->setRandomButton, get_path_to_icon("shuffle.png"));
  set_button_image(ui->loopTrackButton, get_path_to_icon("repeat.png"));
  set_button_image(ui->settingsButton, get_path_to_icon("settings.png"));

  if (isPlaying) {
    set_button_image(ui->playMusicButton, get_path_to_icon("pause.png"));
  } else {
    set_button_image(ui->playMusicButton, get_path_to_icon("play.png"));
  }

  if (ui->listWidget->count()) {
    for (int i = 0; i < tracksAmount; i++) {
      ui->listWidget->item(i)->setIcon(QIcon(get_path_to_icon("icon.png")));
    }
  }
}

void MainWindow::set_button_image(QPushButton* button, QString imagePath) {
  QString buttonName = button->objectName();

  button->setStyleSheet("#" + buttonName + " " + "{"
                        "background-color: transparent;"
                        "border-image: url(" + imagePath + ");"
                        "background: none;"
                        "border: none;"
                        "background-repeat: none;"
                        "}");
}

void MainWindow::set_slider_handle_color(QSlider* slider) {
  QString currentColor;

  if (currentTheme == "Sharingan") {
    currentColor = "#ce1e05";
  } else if (currentTheme == "Rinnegan") {
    currentColor = "#af5fff";
  }

  slider->setStyleSheet("QSlider::handle:horizontal {"
                        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0" + currentColor + ", stop:1" + currentColor + ");"
                        "border: 1px solid" + currentColor + ";"
                        "width: 18px;"
                        "margin: -2px 0;"
                        "border-radius: 3px;"
                        "}");
}

void MainWindow::on_settingsButton_clicked() {
  settingswindow window;

  window.setModal(true);

  connect(&window, SIGNAL(sendCurrentTheme(QString)), this, SLOT(change_theme(QString)));

  window.exec();
}

void MainWindow::change_theme(QString theme) {
  currentTheme = theme;
  set_theme();
}

QString MainWindow::get_path_to_icon(QString iconName) {
  return ":/icons/" + currentTheme + "/" + iconName;
}
