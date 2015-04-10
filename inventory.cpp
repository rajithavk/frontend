#include "inventory.h"
#include "ui_inventory.h"


Inventory::Inventory(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventory)
{

    ui->setupUi(this);
    connect(this,SIGNAL(exit()),this,SLOT(close()));
    editingMode=(id==0)?false:true;



    defaultUrl = new QUrl(QDir::currentPath()+"/BOW/images/");  //Default Location to Create Sample Images Directories
    db = QSqlDatabase::addDatabase("QSQLITE");                  //Creating SQLite Database
    db.setDatabaseName("dbinventory.db");

    if(!db.open()){                                             //Trying to Open the DB
        qDebug() << db.lastError();
        qFatal("Failed to Connect to Database");
    }

    qDebug("Connected to Database");

    qry = new QSqlQuery(db);                                    //Prepare the initial Query, if the Table doesn't exist, create it

    qry->prepare("CREATE TABLE IF NOT EXISTS items (id INTEGER UNIQUE PRIMARY KEY, title VARCHAR(20) DEFAULT 'NULL', description VARCHAR(40) DEFAULT 'NULL', price DOUBLE, directory VARCHAR(100) DEFAULT 'NULL', image VARCHAR(100))");
    if(!qry->exec())
        qDebug()<<qry->lastError();
    else
        qDebug("Table Initialised");

    if(id!=0){                                                  //id>0 for Updating a jewellery
        this->setWindowTitle("Update Jewellery Information");
        ui->pushButton->setText("Update");
        ui->pushButton_2->setText("Delete");

        qry->prepare("SELECT * FROM items WHERE id = :id");
        qry->bindValue(":id",id);
        if(!qry->exec())
            qDebug() << qry->lastError();
        else
        {
            qDebug("Jewellery Record retrieved for Editing");
            if(!qry->next())
            {
                qDebug("Jewellery Does not Exist");
                //emit(exit());
            }
            else{
                ui->iDLineEdit->setText(qry->value(0).toString());
                ui->titleLineEdit->setText(qry->value(1).toString());
                ui->descriptionLineEdit->setText(qry->value(2).toString());
                ui->priceLineEdit->setText(qry->value(3).toString());
                ui->directoryLineEdit->setText(qry->value(4).toString());
                ui->imageLineEdit->setText(qry->value(5).toString());

                QPixmap mp(ui->imageLineEdit->text());
                ui->label->setPixmap(mp);
                ui->label->setScaledContents(true);
            }
        }
    }
    else{                                                           // id==0 for New Jwellery
        nextId();                                                   //Get the ID for the new jewellery to be added.
        ui->directoryLineEdit->setText(defaultUrl->path()+ui->iDLineEdit->text());  // Generate the Directory path for the Image Store
        ui->priceLineEdit->setText("0");
    }

}

Inventory::~Inventory()
{
    capture->release();
    delete ui;
    delete qry;
}

void Inventory::on_pushButton_2_clicked()                       // Clearing the Form
{
    if(editingMode){
        QMessageBox msg;
        msg.setWindowTitle("?");
        msg.setText("Do you Really Want to delete this Item?");
        msg.setInformativeText("Item will be Permanently Deleted");
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Abort|QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Abort);
        qry->prepare("DELETE FROM items WHERE id = :id");
        qry->bindValue(":id",ui->iDLineEdit->text());
        int ret = msg.exec();
        switch (ret) {
        case QMessageBox::Abort : break;
        case QMessageBox::Ok    : {
                                    if(!qry->exec())
                                        qDebug("Unable to Delete The Item");
                                    else{
                                        qDebug("Item Deleted Succesfully");
                                        QMessageBox msg1;
                                        msg1.setWindowTitle("Done");
                                        msg1.setText("Item Deleted");
                                        msg1.setIcon(QMessageBox::Information);
                                        msg1.exec();
                                        emit(exit());
                                    }
                                    break;
                                }
        default:
            break;
        }

    }
    else{
        QList<QLineEdit*> les = this->findChildren<QLineEdit *>();
        foreach (QLineEdit *le, les) {
            le->clear();
        }
        ui->label->clear();
        nextId();
        ui->directoryLineEdit->setText(defaultUrl->path()+ui->iDLineEdit->text());  // Generate the Directory path for the Image Store
        ui->priceLineEdit->setText("0");
    }

}

bool Inventory::isEmpty(){                                      // Check if the Form is empty
    QList<QLineEdit *> les = this->findChildren<QLineEdit*>();
    foreach (QLineEdit *le, les) {
        if(le->text().isEmpty()){
            return true;
        }
    }
    return false;
}

void Inventory::on_pushButton_clicked()                         // INSERTing the new RECORD into the TABLE
{
    if(!isEmpty()){

        if(editingMode){
            qry->prepare("UPDATE items SET id = :id, title = :title, description = :description, price = :price, directory = :directory, image = :image");
        }
        else{
            if(!saveImageSet())
                return;
            qry->prepare("INSERT INTO items (id, title , description , price , directory , image ) VALUES(:id,:title,:description,:price,:directory,:image)");
        }
        qry->bindValue(":id",ui->iDLineEdit->text().toInt());
        qry->bindValue(":title",ui->titleLineEdit->text());
        qry->bindValue(":description",ui->descriptionLineEdit->text());
        qry->bindValue(":price",ui->priceLineEdit->text());
        qry->bindValue(":directory",ui->directoryLineEdit->text());
        qry->bindValue(":image",ui->imageLineEdit->text());

        if(!qry->exec()){
            qDebug()<<qry->lastError();
            qry->finish();
        }
        else{
            qDebug("Record Added");
            if(editingMode){
                QMessageBox msg;
                msg.setWindowTitle("Update");
                msg.setText("Item Update");
                msg.setInformativeText("Item Update Successful");
                msg.setIcon(QMessageBox::Information);
                msg.exec();
            }
            else{
                QMessageBox msg;
                msg.setWindowTitle("Add");
                msg.setText("Item Added");
                msg.setInformativeText("New Item Addition successful");
                msg.setIcon(QMessageBox::Information);
                msg.exec();
                QPixmap mp(ui->imageLineEdit->text());
                ui->label->setPixmap(mp);
                ui->label->setScaledContents(true);
            }
        }
        qry->finish();
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

void Inventory::on_pushButton_3_clicked()                   //Close the Window
{
    emit(exit());
}

void Inventory::on_toolButton_clicked()                     // Browsing for Training Images (if Needed)
{
    QUrl dir = QFileDialog::getExistingDirectoryUrl(this,"Select Training Image Directory",QDir::currentPath(),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    ui->directoryLineEdit->setText(dir.path());
}

void Inventory::on_toolButton_2_clicked()                   //Set the Thubnail Image
{

    QUrl filename = QFileDialog::getOpenFileUrl(this,"Select an Image",ui->directoryLineEdit->text(),tr("Images (*.jpg)"));
    ui->imageLineEdit->setText(filename.path());

    QPixmap mp(filename.path());
    ui->label->setPixmap(mp);
    ui->label->setScaledContents(true);
}


void Inventory::on_imageset_recieve(QVector<QPixmap> imgset){       //SLOT to Receive the new image set
    imageSet = imgset;
    if(imageSet.size()>0){
        qDebug() << QString::number(imageSet.size()) + " images recieved";
        ui->imageLineEdit->setText(ui->directoryLineEdit->text()+"/"+ QString::number(imageSet.size()/2) + ".jpg");
    }
}

void Inventory::on_pushButton_4_clicked()                           // Open the Image Capture Window
{
    //newCapture = new ImageCapture(0,capture);
    newCapture = new ImageCapture(0);
    newCapture->show();
    connect(newCapture,SIGNAL(gotImageSet(QVector<QPixmap>)),this,SLOT(on_imageset_recieve(QVector<QPixmap>)));
}

int Inventory::saveImageSet(){                                      // Save the Image Set to the Given Directory
    if(ui->directoryLineEdit->text().length() > 0){
        QUrl dir_path = QUrl::fromUserInput(ui->directoryLineEdit->text());
        if(dir_path.isValid()){
            QDir dir = dir_path.toLocalFile();
            if(!dir.exists()){
                if(dir.mkpath(dir.path()))
                    qDebug() << "Directory Created";
            }else{
                QMessageBox msg;
                msg.setText("Item Added");
                msg.setInformativeText("New Item Addition Successful");
                msg.setIcon(QMessageBox::Critical);
                msg.exec();
                return 1;
            }
            int count = 0;
            foreach (QPixmap i, imageSet) {
                QString path = dir.path() + "/" + QString::number(count) + ".jpg";
                if(!i.save(path,"JPG"))
                   qDebug() << path + " image not created on disk";

                count++;
            }
            qDebug() << "Images Saved to Directory " + dir_path.toString();
            return 1;
        }
    }
    return 0;
}
