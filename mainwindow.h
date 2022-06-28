#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <iostream>
#include <QDirIterator>
#include <QFileDialog>
#include <QListWidget>
#include <QWindow>
#include <QListView>
#include <QMessageBox>
#include <QDateTime>
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  enum theme {
    SHARINGAN,
    RINNEGAN
  };

 private slots:
  void set_theme();

  void on_playMusicButton_clicked();

  void on_sliderProgress_sliderMoved(int position);

  void on_positionChanged(qint64 position);

  void on_durationChanged(qint64 position);

  void load_tracks(QString path);

  void on_choose_directory_button_clicked();

  void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

  void on_nextTrackButton_clicked();

  void on_previousTrackButton_clicked();

  void on_loopTrackButton_clicked();

  void play_track_with_item(QListWidgetItem* trackItem);

  void restart_track();

  void on_sliderVolume_valueChanged(int position);

  void on_setRandomButton_clicked();

  void set_random_track();

  void set_button_image(QPushButton* button, QString imagePath);

  void set_slider_handle_color(QSlider* slider);

  void on_settingsButton_clicked();

  void change_theme(qint64 theme);

  QString get_path_to_icon(QString iconName);

 private:
  Ui::MainWindow* ui;

  QMediaPlayer* player;
  QAudioOutput* audioOutput;

  qint64 tracksAmount;

  QString path;
  QString maxTrackDuration;
  QString currentTrackDuration;
  QString volumeLevel;
  qint64 currentTheme;

  QListWidgetItem* currentTrack;

  bool isPlaying;
  bool isLooped;
  bool isRandomed;
};
#endif // MAINWINDOW_H
