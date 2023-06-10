#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <iostream>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QNetworkAccessManager;
enum THEME_COLOR { RED, GREEN, BLUE };
enum THEME_FONT_COLOR { GRAY, WHITE };
enum THEME_FONT_WIGHT { NORMAL, BOLD };

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  int total_rows = 0;
  void update_status_bar();
  //  QNetworkReply *get_char_name(QString objId, int serverID);
  QString timestamp_to_date(qint64 ts);
  QList<QString> PAYMENT_SYSTEM = {"PAYMENTWALL",
                                   "PAYMENTWALL_TEST",
                                   "ENOT",
                                   "FREEKASSA",
                                   "FREEKASSA_NEW",
                                   "G2A",
                                   "G2A_COM",
                                   "PAYOP",
                                   "FREEKASSA_NEW_RUB",
                                   "PAYPAL",
                                   "INTERKASSA",
                                   "PAYGOL",
                                   "ZEN",
                                   "ENOT_V2",
                                   "PRIME_PAYMENTS"};
  QList<QString> PAYMENT_STATUS = {"NEW",      "PENDING",  "AUTHORIZED",
                                   "ACCEPTED", "REJECTED", "CANCELED"};
  QList<QString> SERVER = {"MAIN", "NEW", "LOW", "EASY"};
  int theme = 0;

private slots:
  void on_button_search_clicked();
  void on_payment_table_clicked(const QModelIndex &index);
  void on_input_search_returnPressed();
  void on_theme_radio_blue_clicked();
  void on_theme_radio_green_clicked();
  void on_theme_radio_red_clicked();
  void on_theme_font_weight_bold_clicked();
  void on_theme_font_weight_normal_clicked();
  void on_theme_font_color_white_clicked();
  void on_theme_font_color_gray_clicked();
  void on_button_download_invoice_clicked();
  void on_actionCopy_selection_triggered();
  void on_actionExit_triggered();
  void on_payment_table_activated(const QModelIndex &index);

  void on_payment_table_pressed(const QModelIndex &index);

private:
  Ui::MainWindow *ui;
  float version = 1.21;
  int timer_counter = 0;
  unsigned int theme_color;
  unsigned int theme_font_color;
  unsigned int theme_font_weight;
  unsigned int theme_logo;
  void set_default_header_styles();
  void read_settings();
  void write_settings(unsigned int _theme_color, unsigned int _theme_font_color,
                      unsigned int _theme_font_weight);
  void update_theme(unsigned int _theme_color, unsigned int _theme_font_color,
                    unsigned int _theme_font_weight);
  //  QNetworkAccessManager *netMgr;
  QSqlQueryModel *model;
  QSqlDatabase db;
  QSqlQuery *query;
  QPixmap pix;
};
#endif // MAINWINDOW_H
