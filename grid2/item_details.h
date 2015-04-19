#ifndef ITEM_DETAILS_H
#define ITEM_DETAILS_H

#include <QWidget>

namespace Ui {
class item_details;
}

class item_details : public QWidget
{
    Q_OBJECT

public:
    explicit item_details(QWidget *parent = 0);
    ~item_details();
    void set_label_name ( const QString & text );
    void set_label_image ( const QString & text );
    void set_label_order ( const QString & text );
    void set_label_price ( const QString & text );
    void set_label_code ( const QString & text );
    void set_label_other ( const QString & text );

private:
    Ui::item_details *ui;
};

#endif // ITEM_DETAILS_H
