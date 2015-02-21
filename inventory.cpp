#include "inventory.h"
#include "ui_inventory.h"


Inventory::Inventory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventory)
{

    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dbinventory.db");

    if(!db.open()){
        qDebug() << db.lastError();
        qFatal("Failed to Connect to Database");
    }

    qDebug("Connected to Database");

    qry = new QSqlQuery(db);
    qry->prepare("CREATE TABLE IF NOT EXISTS items (id INTEGER UNIQUE PRIMARY KEY, title VARCHAR(20), description VARCHAR(40), price DOUBLE, directory VARCHAR(100), image VARCHAR(100))");
    if(!qry->exec())
        qDebug()<<qry->lastError();
    else
        qDebug("Table Initialised");

    nextId();

}

Inventory::~Inventory()
{
    delete ui;
}

void Inventory::on_pushButton_2_clicked()
{
    QList<QLineEdit*> les = this->findChildren<QLineEdit *>();
    foreach (QLineEdit *le, les) {
        le->clear();
    }
    ui->label->clear();
    nextId();
}

bool Inventory::isEmpty(){
    QList<QLineEdit *> les = this->findChildren<QLineEdit*>();
    foreach (QLineEdit *le, les) {
        if(le->text().isEmpty()){
            return true;
        }
    }
    return false;
}

void Inventory::on_pushButton_clicked()
{
    if(!isEmpty()){

        qry->prepare("INSERT INTO items (id, title , description , price , directory , image ) VALUES(:id,:title,:description,:price,:directory,:image)");
        qry->bindValue(":id",ui->iDLineEdit->text().toInt());
        qry->bindValue(":title",ui->titleLineEdit->text());
        qry->bindValue(":description",ui->descriptionLineEdit->text());
        qry->bindValue(":price",ui->priceLineEdit->text());
        qry->bindValue(":directory",ui->directoryLineEdit->text());
        qry->bindValue(":image",ui->imageLineEdit->text());

        if(!qry->exec())
            qDebug()<<qry->lastError();
        else{
            qDebug("Record Added");

            QMessageBox msg;
            msg.setText("Item Added");
            msg.setInformativeText("New Item Addition Successful");
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        }
    }else{
        QMessageBox msg;
        msg.setText("A Field Cannot be empty");
        msg.setInformativeText("Please Complete the Informations Required");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
}

void Inventory::nextId(){
    qry->prepare("SELECT MAX(id) FROM items");
    if(!qry->exec())
        qDebug()<<qry->lastError();
    else
        qDebug("ID Queried");

    if(!qry->next())
        qDebug()<<qry->lastError();
    else
        {
            qDebug("Query Result Available");
            rec = qry->record();
            QVariant id = rec.value(0).toInt() + 1;
            ui->iDLineEdit->setText(QString().number(id.toInt()));
    }
}

void Inventory::on_pushButton_3_clicked()
{
    close();
}

void Inventory::on_toolButton_clicked()
{
    QUrl dir = QFileDialog::getExistingDirectoryUrl(this,"Select Training Image Directory",QDir::currentPath(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    ui->directoryLineEdit->setText(dir.path());
}

void Inventory::on_toolButton_2_clicked()
{
    QUrl filename = QFileDialog::getOpenFileUrl(this,"Select an Image",QDir::currentPath(),tr("Images (*.jpg)"));
    ui->imageLineEdit->setText(filename.path());

    QPixmap mp(filename.path());
    ui->label->setPixmap(mp);
    ui->label->setScaledContents(true);
}
