#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_released();

    void on_changeEvent_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QString encodedStr;

    int check_for_incorrect_sequence(QString &Text,int* KeysSequence,bool flag);

    /**
     * @brief encode шифрует последовтельность битовых значений по методу Хемминга
     * @param KeysSequence исходная битовая последовательность
     * @param EncodedKeysSequence зашифрованная битовая последовательность
     */
    void encode(int* KeysSequence, int* EncodedKeysSequence);

    void clean_tabel();

    /**
     * @brief find_incorrect_index_in_sequence поиск позиции в зашифрованной послеовательности с неверным битом
     * @param EncodedKeysSequence зашифрованная битовая последовательность
     * @return позицию неверного бита или значение -1, если неверных битов было е найдено
     */
    int find_incorrect_index_in_sequence(int* EncodedKeysSequence);
};
#endif // MAINWINDOW_H
