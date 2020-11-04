#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#define SEQ_LEN 9
#define ENCODED_SEQ_LEN 13

int MainWindow::check_for_incorrect_sequence(QString &Text,int* KeysSequence, bool flag){

    QStringList KeysList;
    KeysList=Text.split(" ");

    /*Проверяем: соответствует ли количество ключей требуемому значению*/
    if(flag){
        if(KeysList.length()!=SEQ_LEN)
            return 1;
    }else{
        if(KeysList.length()!=ENCODED_SEQ_LEN)
            return 1;

        QStringList KeysENCODEDList;
        int counter=0;
        KeysENCODEDList=MainWindow::encodedStr.split(" ");

        for(int i=0;i<KeysList.length();i++){
            if(KeysENCODEDList[i]!=KeysList[i]){
                counter++;
                if(counter==2)
                    return 1;
            }
        }

    }

    /*Проверяем: являются ли ключи числовыми значениями*/
    for(int i=0;i<KeysList.length();i++){
        if(KeysList[i].toInt()!=0 && KeysList[i].toInt()!=1)
            return 1;
    }

    /*Записываем значения ключей в числовой массив*/
    for(int i=0;i<KeysList.length();i++){
        KeysSequence[i]=KeysList[i].toInt();
    }

    return 0;
}

void MainWindow::encode(int* KeysSequence,int* EncodedKeysSequence){

    int index=1;
    int counter=0;
    QTableWidgetItem *newItem;

    /*заполняем строку "b" таблицы "матрицы суммирования"*/
    for(int i=0;i<ENCODED_SEQ_LEN;i++){
        if(i==(index-1)){
            EncodedKeysSequence[i]=0;
            index*=2;
        }else{
            newItem = new QTableWidgetItem(QString::number(KeysSequence[counter]));
            ui->tableSum->setItem(6,i,newItem);
            EncodedKeysSequence[i]=KeysSequence[counter];
            counter++;
        }
    }

    int sum;
    index=1;
    int bit;
    QString text;

    /*Получаем вектора контрольных сумм для столбцов
     * "So" и "St" таблицы "матрицы суммирования"*/
    for(int j=0;j<4;j++){
        sum=0;
        for(int k=0;k<ENCODED_SEQ_LEN;k++){
           newItem=ui->tableSum->item(j+2,k);
           text=newItem->text();
           if(text=="1"){
               if(EncodedKeysSequence[k]==1){
                   sum++;
               }
           }
        }

        if(sum%2==0)
            bit=0;
        else
            bit=1;


        newItem=new QTableWidgetItem(QString::number(0));
        ui->tableSum->setItem(j+2,14,newItem);
        EncodedKeysSequence[index-1]=bit;
        newItem=new QTableWidgetItem(QString::number(bit));
        ui->tableSum->setItem(j+2,13,newItem);
        newItem=new QTableWidgetItem(QString::number(bit));
        ui->tableSum->setItem(7,index-1,newItem);

        index*=2;
    }

    /*заполняем строку "bt" таблицы "матрицы суммирования"*/
    for(int m=0;m<ENCODED_SEQ_LEN;m++){
        newItem=new QTableWidgetItem(QString::number(EncodedKeysSequence[m]));
        ui->tableSum->setItem(8,m,newItem);
    }
}

int MainWindow::find_incorrect_index_in_sequence(int* EncodedKeysSequence){

    int sum;
    int index=1;
    QTableWidgetItem *newItem;
    QString text;
    int bit;

    /*заполняем строку "br" таблицы "матрицы суммирования"*/
    for(int m=0;m<ENCODED_SEQ_LEN;m++){
        newItem=new QTableWidgetItem(QString::number(EncodedKeysSequence[m]));
        ui->tableSum->setItem(9,m,newItem);
    }

    /*Получаем вектора контрольных сумм для столбца
     * "Sr" таблицы "матрицы суммирования"*/
    for(int j=0;j<4;j++){
        sum=0;
        for(int k=0;k<ENCODED_SEQ_LEN;k++){
           newItem=ui->tableSum->item(j+2,k);
           text=newItem->text();
           if(text=="1"){
               if(EncodedKeysSequence[k]==1){
                   sum++;
               }
           }
        }

        if(sum%2==0)
            bit=0;
        else
            bit=1;


        newItem=new QTableWidgetItem(QString::number(bit));
        ui->tableSum->setItem(j+2,15,newItem);
        index*=2;
    }

    int position=0;
    index=1;
    QString txt1;
    QString txt2;

    /*вычисляем позицию ошибочного бита,если он есть*/
    for(int n=0;n<4;n++){
        newItem=ui->tableSum->item(n+2,14);
        txt1=newItem->text();
        newItem=ui->tableSum->item(n+2,15);
        txt2=newItem->text();
        if(txt1!=txt2)
            position+=index;
        index*=2;
    }

    if(position==0)
        return -1;
    else
        return position;
}

void MainWindow::clean_tabel(){

    QTableWidgetItem *newItem;

    for(int i=0;i<4;i++){
        for(int j=13;j<16;j++){
            newItem=new QTableWidgetItem("");
            ui->tableSum->setItem(i+2,j,newItem);
        }
        for(int k=0;k<ENCODED_SEQ_LEN;k++){
            newItem=new QTableWidgetItem("");
            ui->tableSum->setItem(i+6,k,newItem);
        }
    }
}

void MainWindow::on_button_released()
{
    QString Text=ui->inputText->text();
    int KeysSequence[SEQ_LEN];
    int EncodedKeysSequence[ENCODED_SEQ_LEN];

    if(ui->changeEvent->currentText()=="закодировать"){

        if(check_for_incorrect_sequence(Text,KeysSequence, true)){
            ui->statusbar->showMessage("Последовательность символов введена неверно. Смотри примечания.");
            return;
        }

        MainWindow::encode(KeysSequence,EncodedKeysSequence);

        QString encodedSeq="";
        encodedSeq.append(QString::number(EncodedKeysSequence[0]));

        for(int i=1;i<ENCODED_SEQ_LEN;i++){
            encodedSeq.append(" "+QString::number(EncodedKeysSequence[i]));
        }

        ui->outputText->setText(encodedSeq);
        MainWindow::encodedStr=ui->outputText->text();

    }else if(ui->changeEvent->currentText()=="найти ошибку"){

        if(check_for_incorrect_sequence(Text,EncodedKeysSequence, false)){
            ui->statusbar->showMessage("Последовательность символов введена неверно. Смотри примечания.");
            return;
        }

        int position=MainWindow::find_incorrect_index_in_sequence(EncodedKeysSequence);

        if(position==-1)
            ui->outputText->setText("в последовательности нет ошибки");
        else
            ui->outputText->setText(QString::number(position));

    }

    ui->statusbar->showMessage("Операция выполнена успешно.");

}

void MainWindow::on_changeEvent_currentIndexChanged(const QString &arg1)
{
        /*Формируем текстовые поля*/
        if(arg1=="закодировать"){
            MainWindow::encodedStr="";

            ui->laberInputText->setText("Введите последовательность для зашифровки");
            ui->inputText->clear();

            ui->textDiscription->setPlainText("1.Последовательность должна состоять из 0 и 1.\
                                              2.Количество символов в последовательности должно быть равно 9.\
                                              3.Символы последовательности нужно вводить через пробел.");

            ui->button->setText("Зашифровать");

            MainWindow::clean_tabel();

            ui->labelOutputText->setText("Зашифрованная последовательность");
            ui->outputText->clear();

        }else if(arg1=="найти ошибку"){

            ui->laberInputText->setText("Введите полученную последовательность с ошибкой");
            ui->inputText->clear();

            ui->textDiscription->setPlainText("1.Последовательность должна состоять из 0 и 1.\
                                              2.Количество символов в последовательности должно быть равно 13.\
                                              3.Символы последовательности нужно вводить через пробел.\
                                              4.Нельзя добавлять болше одной ошибки в последовательность.\
                                              5.Нельзя изменять биты, соотвтетсвующие степеням двойки.");

            ui->button->setText("найти позицию с ошибкой");

            ui->labelOutputText->setText("Позиция с ошибочным битом в последовательности");
            ui->inputText->setText(MainWindow::encodedStr);
            ui->outputText->clear();
        }
}
