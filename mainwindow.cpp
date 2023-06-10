#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  read_settings();

  // set up table header, columns, rows style
  this->set_default_header_styles();

  db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("l2j_ws");
  db.setUserName("root");
  db.setPassword("");

  if (!db.open()) {
    QMessageBox::critical(this, "Error", db.lastError().text());
    return;
  }

  QString query_fetch_payments =
      "SELECT id, obj_Id, e_transaction_id, transaction_id, server_id, "
      "quantity as coins, status, char_name, email "
      "FROM payments "
      "ORDER BY id DESC "
      "LIMIT 1000";

  model = new QSqlQueryModel(this);
  model->setQuery(query_fetch_payments);

  ui->payment_table->setModel(model);

  ui->payment_table->model()->sort(1, Qt::AscendingOrder);

  ui->payment_table->show();

  update_status_bar();
}

void MainWindow::write_settings(unsigned int _theme_color,
                                unsigned int _theme_font_color,
                                unsigned int _theme_font_weight) {

  QSettings settings("Alisa Corporation", "L2J_Payment_System");
  settings.beginGroup("MainWindow");

  settings.setValue("theme_color", _theme_color);
  settings.setValue("theme_font_color", _theme_font_color);
  settings.setValue("theme_font_weight", _theme_font_weight);
  settings.setValue("theme_logo", _theme_color);

  switch (settings.value("theme_color").toUInt()) {
  case RED:
    this->pix = QPixmap("umbrella_corp.png");
    break;
  case GREEN:
    this->pix = QPixmap("gas_mask_green.png");
    break;
  case BLUE:
    this->pix = QPixmap("nuclear_blue.png");
    break;
  }

  ui->logo->setPixmap(pix);

  // set private ints
  this->theme_color = _theme_color;
  this->theme_font_color = _theme_font_color;
  this->theme_font_weight = _theme_font_weight;
  this->theme_logo = _theme_color;

  //  qDebug() << "_theme_color: " << _theme_color;
  //  qDebug() << "_theme_font_color: " << _theme_font_color;
  //  qDebug() << "_theme_font_weight: " << _theme_font_weight;
  //  qDebug() << "theme_logo: " << _theme_color;

  settings.endGroup();
}

void MainWindow::read_settings() {
  QSettings settings("Alisa Corporation", "L2J_Payment_System");
  settings.beginGroup("MainWindow");

  switch (settings.value("theme_color", 0).toUInt()) {
  case RED:
    ui->theme_radio_red->setChecked(true);
    break;
  case GREEN:
    ui->theme_radio_green->setChecked(true);
    break;
  case BLUE:
    ui->theme_radio_blue->setChecked(true);
    break;
  }

  // set logo by color
  // QPixmap pix("umbrella_corp.png");

  switch (settings.value("theme_logo", 0).toUInt()) {
  case RED:
    this->pix = QPixmap("umbrella_corp.png");
    break;
  case GREEN:
    this->pix = QPixmap("gas_mask_green.png");
    break;
  case BLUE:
    this->pix = QPixmap("nuclear_blue.png");
    break;
  }

  ui->logo->setPixmap(pix);

  switch (settings.value("theme_font_color", 0).toUInt()) {
  case GRAY:
    ui->theme_font_color_gray->setChecked(true);
    break;
  case WHITE:
    ui->theme_font_color_white->setChecked(true);
    break;
  }

  switch (settings.value("theme_font_weight", 0).toUInt()) {
  case BOLD:
    ui->theme_font_weight_bold->setChecked(true);
    break;
  case NORMAL:
    ui->theme_font_weight_normal->setChecked(true);
    break;
  }

  // set private ints
  this->theme_color = settings.value("theme_color").toUInt();
  this->theme_font_color = settings.value("theme_font_color").toUInt();
  this->theme_font_weight = settings.value("theme_font_weight").toUInt();
  this->theme_logo = settings.value("theme_logo").toUInt();

  //  qDebug() << "current theme_color id: " << this->theme_color;
  //  qDebug() << "current theme_font_color id: " << this->theme_font_color;
  //  qDebug() << "current theme_font_weight id: " << this->theme_font_weight;
  //  qDebug() << "current theme_logo id: " << this->theme_logo;

  update_theme(this->theme_color, this->theme_font_color,
               this->theme_font_weight);
  settings.endGroup();
}

void MainWindow::update_theme(unsigned int _theme_color,
                              unsigned int _theme_font_color,
                              unsigned int _theme_font_weight) {

  // set private ints
  this->theme_color = _theme_color;
  this->theme_font_color = _theme_font_color;
  this->theme_font_weight = _theme_font_weight;

  QString style, style_font_color, inactive_selected_row_bg_color = "";

  switch (_theme_color) {
  case RED: // RED
    style += "color: #B10000;";
    inactive_selected_row_bg_color += "#B10000";
    break;
  case GREEN: // GREEN
    style += "color: #00FF00;";
    inactive_selected_row_bg_color += "#00FF00";
    break;
  case BLUE:                    // BLUE
    style += "color: #337bb7;"; // #00DDFF;";
    inactive_selected_row_bg_color += "#337bb7";
    break;
  }

  switch (_theme_font_color) {
  case GRAY:
    style_font_color += "color: #666;";
    break;
  case WHITE:
    style_font_color += "color: #fff;";
    break;
  }

  switch (_theme_font_weight) {
  case BOLD:
    style += "font: 900;";
    break;
  case NORMAL:
    style += "font: 400;";
    break;
  }

  // qDebug() << "Style finished: " << style;

  QLabel *labels[] = {ui->label_misc_created,   ui->label_misc_updated,
                      ui->label_misc_delayed,   ui->label_misc_id,
                      ui->label_misc_txid,      ui->label_payment_ex_txid,
                      ui->label_payment_status, ui->label_payment_method,
                      ui->label_payment_system, ui->label_item_id,
                      ui->label_item_quantity,  ui->label_player_email,
                      ui->label_player_name,    ui->label_player_obj_id,
                      ui->label_player_server,  ui->label_dev_company,
                      ui->label_payment_sale_id};

  QPushButton *buttons[] = {ui->button_search, ui->button_download_invoice};

  QLabel *sections[] = {ui->section_label_item, ui->section_label_misc,
                        ui->section_label_payment, ui->section_label_player};

  QLineEdit *inputs[] = {ui->input_item_id,        ui->input_item_quantity,
                         ui->input_misc_created,   ui->input_misc_delayed,
                         ui->input_misc_id,        ui->input_misc_txid,
                         ui->input_misc_updated,   ui->input_payment_ex_txid,
                         ui->input_payment_method, ui->input_payment_status,
                         ui->input_payment_system, ui->input_player_email,
                         ui->input_player_name,    ui->input_player_obj_id,
                         ui->input_player_server,  ui->input_search,
                         ui->input_payment_sale_id};

  //  qDebug() << "typeof: " << typeid(ui->label_misc_created).name();
  //  qDebug() << "sizeof(labels): " << sizeof(labels);

  for (int i = 0; i < ((int)sizeof(labels) / 8); i++) {
    labels[i]->setStyleSheet(style);
  }

  for (int i = 0; i < ((int)sizeof(buttons) / 8); i++) {
    buttons[i]->setStyleSheet(style);
  }

  for (int i = 0; i < ((int)sizeof(sections) / 8); i++) {
    sections[i]->setStyleSheet(style_font_color + "font: 900;");
  }

  for (int i = 0; i < ((int)sizeof(inputs) / 8); i++) {
    inputs[i]->setStyleSheet(style_font_color +
                             "border: 1px solid rgb(60, 60, 60);");
  }

  ui->payment_table->setStyleSheet(
      "QHeaderView::section:horizontal {margin: 0 5px 0 5px; " + style +
      "} QTableView {" + style_font_color +
      "} QTableView::item:selected:!active { background-color: "+inactive_selected_row_bg_color+"; }");

  write_settings(_theme_color, _theme_font_color, _theme_font_weight);
}

MainWindow::~MainWindow() {
  db.close();
  delete ui;
}

QString MainWindow::timestamp_to_date(qint64 ts) {
  return QDateTime::fromSecsSinceEpoch(ts).toString("dd/MM/yyyy hh:mm:ss");
}

void MainWindow::update_status_bar() {
  qDebug() << "update_status_bar";
  QString update_time = timestamp_to_date(QDateTime::currentSecsSinceEpoch());
  QString db_status = db.isOpen() ? "connected" : "disconnected";
  ui->statusBar->showMessage("Last update: " + update_time +
                             " | Status: " + db_status +
                             " | Version: " + QString::number(this->version));
}

void MainWindow::on_payment_table_clicked(const QModelIndex &index) {

  update_theme(this->theme_color, this->theme_font_color,
               this->theme_font_weight);

  // render index, to fetch 0 column of selected row
  QModelIndex index_ =
      ui->payment_table->model()->index(index.row(), 0, QModelIndex());
  // qDebug() << "index_(data): " << index_.data();

  // fetch data from payments by current index
  QString query_fetch_selected_payment =
      "SELECT id, transaction_id, e_transaction_id, obj_Id, email, "
      "payment_system, created_at, updated_at, delayed_at, server_id, "
      "quantity as coins, status, payment_method, item_id, char_name, sale_id"
      " FROM payments WHERE id = " +
      index_.data().toString() + " limit 1";

  // qDebug() << query_fetch_selected_payment;
  QSqlQuery query(query_fetch_selected_payment);
  query.exec();

  if (query.lastError().text() != "") {
    // qDebug() << query.lastError().text();
    return;
  }

  while (query.next()) {

    // qDebug() << "query first" << query.first();

    // convert timestamps -> string -> qint64
    QString q_converter = query.value(6).toString();
    qint64 q_int = q_converter.toULongLong();
    ui->input_misc_created->setText(q_int > 0 ? timestamp_to_date(q_int) : "");

    q_converter = query.value(7).toString();
    q_int = q_converter.toULongLong();
    ui->input_misc_updated->setText(q_int > 0 ? timestamp_to_date(q_int) : "");

    q_converter = query.value(8).toString();
    q_int = q_converter.toULongLong();
    ui->input_misc_delayed->setText(q_int > 0 ? timestamp_to_date(q_int) : "");

    QString id = query.value(0).toString();
    QString transaction_id = query.value(1).toString();
    QString e_transaction_id = query.value(2).toString();
    QString obj_id = query.value(3).toString();
    QString email = query.value(4).toString();
    QString quantity = query.value(10).toString();
    QString payment_method = query.value(12).toString();
    QString item_id = query.value(13).toString();

    ui->input_player_name->setText(query.value(14).toString());
    ui->input_payment_sale_id->setText(query.value(15).toString());
    ui->input_misc_id->setText(id);
    ui->input_misc_txid->setText(transaction_id);
    ui->input_payment_ex_txid->setText(e_transaction_id);
    ui->input_player_obj_id->setText(obj_id);
    ui->input_player_email->setText(email);
    ui->input_item_quantity->setText(quantity);
    ui->input_payment_method->setText(payment_method);
    ui->input_item_id->setText(item_id);

    if (query.value(5).toUInt() > (PAYMENT_SYSTEM.length() - 1)) {
      QMessageBox::critical(this, "Error", query.value(5).toString());
      return;
    }

    if (query.value(9).toUInt() > (SERVER.length() - 1)) {
      QMessageBox::critical(this, "Error", "[SERVER] Out of range exception!");
      return;
    }

    if (query.value(11).toUInt() > (PAYMENT_STATUS.length() - 1)) {
      QMessageBox::critical(this, "Error",
                            "[PAYMENT_STATUS] Out of range exception!");
      return;
    }

    QString server = SERVER.at(query.value(9).toUInt() - 1);
    QString status = PAYMENT_STATUS.at(query.value(11).toUInt());
    QString payment_system = PAYMENT_SYSTEM.at(query.value(5).toUInt());

    ui->input_payment_system->setText(payment_system);
    ui->input_player_server->setText(server);
    ui->input_payment_status->setText(status);
  }
}

void MainWindow::on_button_search_clicked() {
  QString query_fetch_payments;

  if (ui->input_search->text() == "") {
    query_fetch_payments =
        "SELECT id, obj_Id, e_transaction_id, transaction_id, server_id, "
        "quantity as coins, status, char_name, email "
        "FROM payments order by id desc limit 1000";
    update_status_bar();
  } else {
    query_fetch_payments =
        "SELECT id, obj_Id, e_transaction_id, transaction_id, server_id, "
        "quantity as coins, status, char_name, email "
        "FROM payments "
        "WHERE obj_Id LIKE '%" +
        ui->input_search->text() +
        "%' "
        "OR email LIKE '%" +
        ui->input_search->text() +
        "%' "
        "OR transaction_id LIKE '%" +
        ui->input_search->text() +
        "%'  "
        "OR e_transaction_id LIKE '%" +
        ui->input_search->text() +
        "%'  "
        "OR char_name LIKE '%" +
        ui->input_search->text() +
        "%'  "
        "OR sale_id LIKE '%" +
        ui->input_search->text() +
        "%'  "
        "ORDER BY id desc";
  }

  model->setQuery(query_fetch_payments);

  if (model->lastError().text() != "") {
    update_status_bar();
    QMessageBox::critical(this, "Error", model->lastError().text());
    return;
  }

  if (!model->rowCount()) {
    update_status_bar();
    QMessageBox::critical(this, "Error", "Results not found!");
    return;
  }

  ui->payment_table->setModel(model);

  update_status_bar();
}

void MainWindow::set_default_header_styles() {
  ui->payment_table->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  ui->payment_table->horizontalHeader()->setStretchLastSection(true);
  ui->payment_table->horizontalHeader()->setHighlightSections(false);
  ui->payment_table->verticalHeader()->hide();
  ui->payment_table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::on_input_search_returnPressed() { on_button_search_clicked(); }

void MainWindow::on_theme_font_weight_bold_clicked() {
  update_theme(this->theme_color, this->theme_font_color, BOLD);
}
void MainWindow::on_theme_font_weight_normal_clicked() {
  update_theme(this->theme_color, this->theme_font_color, NORMAL);
}
void MainWindow::on_theme_font_color_white_clicked() {
  update_theme(this->theme_color, WHITE, this->theme_font_weight);
}
void MainWindow::on_theme_font_color_gray_clicked() {
  update_theme(this->theme_color, GRAY, this->theme_font_weight);
}
void MainWindow::on_theme_radio_blue_clicked() {
  update_theme(BLUE, this->theme_font_color, this->theme_font_weight);
}
void MainWindow::on_theme_radio_green_clicked() {
  update_theme(GREEN, this->theme_font_color, this->theme_font_weight);
}
void MainWindow::on_theme_radio_red_clicked() {
  update_theme(RED, this->theme_font_color, this->theme_font_weight);
}

void MainWindow::on_button_download_invoice_clicked() {
  if (ui->input_payment_ex_txid->text() == "") {
    QMessageBox::information(this, "Info", "Transaction ID is empty");
    return;
  }

  QDesktopServices::openUrl(
      QUrl("https://api.example.com/api/v1/payment/order/" +
               ui->input_payment_ex_txid->text(),
           QUrl::TolerantMode));
}

void MainWindow::on_actionCopy_selection_triggered() {
  QAbstractItemModel *abmodel = ui->payment_table->model();
  QItemSelectionModel *model = ui->payment_table->selectionModel();
  QModelIndexList list = model->selectedIndexes();

  std::sort(list.begin(), list.end());

  if (list.size() < 1)
    return;

  QString copy_table;
  // QModelIndex last = list.last();
  QModelIndex previous = list.first();

  list.removeFirst();

  for (int i = 0; i < list.size(); i++) {
    QVariant data = abmodel->data(previous);
    QString text = data.toString();

    QModelIndex index = list.at(i);
    copy_table.append(text);

    if (index.row() != previous.row())

    {
      copy_table.append('\n');
    } else {
      copy_table.append('\t');
    }
    previous = index;
  }

  copy_table.append(abmodel->data(list.last()).toString());
  copy_table.append('\n');

  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(copy_table);
}

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_payment_table_activated(const QModelIndex &index) {
  on_payment_table_clicked(index);
}

void MainWindow::on_payment_table_pressed(const QModelIndex &index) {}
