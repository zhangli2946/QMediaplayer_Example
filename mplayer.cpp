/*
 * Author : zhangli
 * Time : 7/7/14
 * Version : 1.0
 * Description : In this version we still have many BUGS to fix
 * 1. Can't get file description from metadata or some where else
 * 2. Play the next by default not configureable
 * 3. Haven't configured the form style
 * 4. No lrc display
 */
#include "mplayer.h"
#include "ui_mplayer.h"
#include<QDebug>
#define DEBUG    // this DEBUG symble to let internal data out from debug console ;-)

mplayer::mplayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mplayer)
{
    ui->setupUi(this);
    // basic mumber of the problem
    file_list = new QStringList();
    play_index=0;
    my_player = new QMediaPlayer(); // When in times of QT4 We have PHONO to play songs for me

    // status label and init it
    cur_name = new QLabel();
    cur_state = new QLabel();
    cur_time = new QLabel();
    cur_time->setText("00:00/00:00");\
    cur_state->setText(tr("Stopped"));
    //add them into statusBar with  setting position automatically
    ui->statusBar->addPermanentWidget(cur_name,1);
    ui->statusBar->addPermanentWidget(cur_time,1);
    ui->statusBar->addPermanentWidget(cur_state,1);

    //connect signals and slots
    connect(my_player,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(upstate()));
    connect(my_player,SIGNAL(positionChanged(qint64)),this,SLOT(upstatusbar(qint64)));
    connect(my_player,SIGNAL(positionChanged(qint64)),this,SLOT(upslider(qint64)));
    connect(ui->pos_slider,SIGNAL(valueChanged(int)),this,SLOT(slider_drag(int)));
}

mplayer::~mplayer()
{
    delete ui;
    // have questions  about delete a class and should i delet below by hand?
    delete cur_name;
    delete cur_time;
    delete cur_state;
    delete []file_list;
    delete my_player;
}

void mplayer::on_vol_slider_valueChanged(int value)
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_vol_slider_valueChanged(int)
     * Permanent : value
     * Description : change the volumn
     * Sender : vol_slider
     * Signal : valueChanged(int)
     */

     my_player->setVolume(value); // When system volumn < value voice we heard  decrease. Shit!
}

void mplayer::on_btn_add_clicked()
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_btn_add_clicked()
     * Permanent : Null
     * Description : open a default dialog for recieving filenames
     * Sender : btn_add
     * Signal : clicked()
     */

     file_list->append(QFileDialog::getOpenFileNames(this,tr("Open File"),QDir::homePath(), tr("MP3 files (*.mp3);;All files (*.*)")));
     if(my_player->state() != QMediaPlayer::PlayingState) //This line  can prevent  songs stop playing when add files to file_list
        on_btn_play_clicked();
#ifdef DEBUG
      qDebug () << file_list->at(0); // output the first song to make sure add function work properly
#endif
}

void mplayer::upstate()
{
    /*
     * Function Type: (slot) [user]
     * Slot Name : upstate()
     * Permanent : Null
     * Description : update the control button icon and statusBars' label
     * Sender : my_player
     * Signal : stateChanged(QMediaPlayer::State)
     */

    switch (my_player->state())
    {
    case QMediaPlayer::PlayingState:
        ui->btn_play->setIcon(QIcon(":/new/prefix1/icons/4.png"));
        //cur_name->setText((QString)(my_player->metaData("SubTitle").toString()));
        cur_state->setText(tr("Playing"));
        break;
    case QMediaPlayer::PausedState:
        ui->btn_play->setIcon(QIcon(":/new/prefix1/icons/6.png"));
        cur_state->setText(tr("Paused"));
        break;
    case QMediaPlayer::StoppedState:
        ui->btn_play->setIcon(QIcon(":/new/prefix1/icons/6.png"));
        cur_time->setText("00:00/00:00");
        cur_state->setText(tr("Stopped"));
        on_btn_next_clicked();                                  //BUGS: auto play next song
        break;
    default:
        break;
    }
}

void mplayer::play()
{
    /*
     * Function Type: (normal)
     * Function Name : play()
     * Permanent : Null
     * Description : play music
     */

    if(my_player->state() == QMediaPlayer::PausedState)
        my_player->play();
    //If paused resume it instead of play again  this motherd comeout of debug Idont know why
     else
    {
    if(file_list->isEmpty())
    {
        QMessageBox::warning(this, "Warrning!","No media to play");
    }
    else
    {
        my_player->setMedia(QUrl::fromLocalFile(file_list->at(play_index)));
        if(!my_player->isAvailable())
        {
            QMessageBox::warning(this, "Warrning!","Wrong media format to play");
            file_list->removeAt(play_index); // un display able remove
        }
        else
        {
//         QString name = file_list->at(play_index).split;
//            cur_name->setText(name);  // BUGS : can not get the name of the file
            my_player->play();
        }
    }
    }
}

void mplayer::on_btn_prev_clicked()
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_btn_prev_clicked()
     * Permanent : Null
     * Description : play prev song
     * Sender : btn_prev
     * Signal : clicked()
     */

    if(play_index==0)
    {
        play_index =file_list->size()-1; // handle out of range
    }
    play_index--;
    play();
}

void mplayer::on_btn_play_clicked()
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_btn_play_clicked()
     * Permanent : Null
     * Description : control  play rules
     * Sender : btn_play
     * Signal : clicked()
     */

    switch (my_player->state()) // Act accordingly
    {
    case QMediaPlayer::PlayingState:
        my_player->pause();
        break;
    case QMediaPlayer::PausedState:
        play();
        break;
    case QMediaPlayer::StoppedState:
        play();
        break;
    default:
        break;
    }
}

void mplayer::on_btn_next_clicked()
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_btn_next_clicked()
     * Permanent : Null
     * Description : play prev song
     * Sender : btn_next
     * Signal : clicked()
     */

    if(file_list->size()-1==play_index)
    {
        play_index =-1; //handle out of range
    }
    play_index++;
    play();
}

void mplayer::on_btn_stop_clicked()
{
    /*
     * Function Type: (slot) [auto]
     * Slot Name : on_btn_stop_clicked()
     * Permanent : Null
     * Description : stop playing  song
     * Sender : btn_stop
     * Signal : clicked()
     */

        if(my_player->state() != QMediaPlayer::StoppedState)
        {
            my_player->stop();  //;-)
        }
}

void mplayer::upstatusbar(qint64 position)
{
    /*
     * Function Type: (slot) [user]
     * Slot Name : upstatusbar(qint64)
     * Permanent : position
     * Description : update statusbar status
     * Sender : my_player
     * Signal : positionChanged(qint64)
     */
    // Shit! Shit! Shit! I've never coded
    QTime total(0, my_player->duration ()/ 60000, qRound((my_player->duration ()% 60000) / 1000.0));
    cur_time->setText(total.toString(tr("mm:ss")));
    QTime now(0, position / 60000, qRound((position % 60000) / 1000.0));
    cur_time->setText(now.toString(tr("mm:ss/"))+total.toString(tr("mm:ss")));
}

void mplayer::upslider(qint64 position)
{
    /*
     * Function Type: (slot) [user]
     * Slot Name : upslider(qint64)
     * Permanent : position
     * Description : update slider status
     * Sender : my_player
     * Signal : positionChanged(qint64)
     */

    ui->pos_slider->setValue(position*100/my_player->duration()); // sick for many days
#ifdef DEBUG
        qDebug () << "mplayer" << my_player->position()
                  <<"sliderpos"<<ui->pos_slider->value()
                    << position;
#endif
 }

void mplayer::slider_drag(int position)
{
    /*
     * Function Type: (slot) [user]
     * Slot Name : slider_drag(int)
     * Permanent : position
     * Description : update play position using drag
     * Sender : pos_slider
     * Signal :  valueChanged(int)
     */

    if(ui->pos_slider->isSliderDown())
    {
        // the most puzzle part disconnect the connections
        //and reconnect the connectios
        disconnect(my_player,SIGNAL(positionChanged(qint64)),0,0);
        my_player->setPosition(position*my_player->duration()/100);
        connect(my_player,SIGNAL(positionChanged(qint64)),this,SLOT(upslider(qint64)));
        connect(my_player,SIGNAL(positionChanged(qint64)),this,SLOT(upstatusbar(qint64)));
    }
#ifdef DEBUG
        qDebug () << "mplayer" << my_player->duration()
                  <<"sliderpos"<<ui->pos_slider->value()
                    << position;
#endif
}

