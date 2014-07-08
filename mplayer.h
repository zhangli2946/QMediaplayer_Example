#ifndef MPLAYER_H
#define MPLAYER_H

#include <QMainWindow>
#include <QMultimedia>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QFileDialog>
#include<QMessageBox>
 #include<QDebug>
#include<QLabel>
#include<QTime>
#include<QTimer>

namespace Ui {
class mplayer;
}

class mplayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit mplayer(QWidget *parent = 0);
    ~mplayer();

private:
    QMediaPlayer * my_player;
    QStringList *file_list;
    Ui::mplayer *ui;
    QLabel * cur_name;
    QLabel * cur_time;
    QLabel * cur_state;
    int play_index;
    void play();

private slots:
    void slider_drag(int position);
    void on_vol_slider_valueChanged(int value);
    void upstate();
    void on_btn_prev_clicked();
    void on_btn_play_clicked();
    void on_btn_next_clicked();
    void on_btn_stop_clicked();
    void on_btn_add_clicked();
    void upstatusbar(qint64 position);
    void upslider(qint64 position);
};

#endif // MPLAYER_H
