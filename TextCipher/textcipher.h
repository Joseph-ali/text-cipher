#ifndef TEXTCIPHER_H
#define TEXTCIPHER_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class TextCipher; }
QT_END_NAMESPACE

class TextCipher : public QMainWindow
{
    Q_OBJECT

private:
    QString inFile = "";
    QString outFile = "";
    QStringList ciphersList = {"Rail Fence", "Route Cipher", "Columnar Cipher", "Double Columnar", "Myszkowski Cipher"};
    enum
    {
      RAIL_FENCE = 0,
      ROUTE = 1,
      COLUMNAR = 2,
      DOUBLE_COLUMNAR = 3,
      MYSZKOWSKI = 4,
    };

public:
    TextCipher(QWidget *parent = nullptr);
    ~TextCipher();

private:
    Ui::TextCipher *ui;

private slots:
    void OnSelectInFile();
    void OnSelectOutFile();
    void OnEncode();
    void OnDecode();
    void OnIndexChanged();
    void OnIndexChangedShort();
    void OnEncodeShort();
    void OnDecodeShort();
};
#endif // TEXTCIPHER_H
