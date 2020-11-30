#include "textcipher.h"
#include "ui_textcipher.h"
#include "TranspositionCiphers.h"

TextCipher::TextCipher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TextCipher)
{
    ui->setupUi(this);
    ui->comboBox->addItems(ciphersList);
    ui->ciphersComboBox->addItems(ciphersList);

    connect(ui->SelectInFile, SIGNAL(released()), this, SLOT(OnSelectInFile()));
    connect(ui->SelectOutFile, SIGNAL(released()), this, SLOT(OnSelectOutFile()));
    connect(ui->cipherIt, SIGNAL(released()), this, SLOT(OnEncode()));
    connect(ui->decodebtn, SIGNAL(released()), this, SLOT(OnDecode()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIndexChanged()));
    connect(ui->shortEncode, SIGNAL(released()), this, SLOT(OnEncodeShort()));
    connect(ui->ciphersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnIndexChangedShort()));
    connect(ui->shortDecode, SIGNAL(released()), this, SLOT(OnDecodeShort()));
}

TextCipher::~TextCipher()
{
    delete ui;
}

void TextCipher::OnSelectInFile()
{
    inFile = QFileDialog::getOpenFileName(this, "Select a File", QString(), "Text Files (*.txt)");
    ui->InputFile->setText(inFile);
}

void TextCipher::OnSelectOutFile()
{
    outFile = QFileDialog::getSaveFileName(this, "Save as", QString(), "Text Files (*.txt)");
    ui->OutputFile->setText(outFile);
}

void TextCipher::OnEncode()
{
    inFile = ui->InputFile->text();
    outFile = ui->OutputFile->text();

    QFile input(inFile);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "cannot open the file" + input.errorString());
        return;
    }

    QTextStream inStream(&input);
    QString text = inStream.readAll();

    switch(ui->comboBox->currentIndex())
    {
        case RAIL_FENCE:
        {
            text = railFenceEncode(&text, ui->spinBox->value());
            break;
        }
        case ROUTE:
        {
            text = routeCipherEncode(ui->spinBox->value(), &text, ui->clockwise->isChecked());
            break;
        }
        case COLUMNAR:
        {
            QString keyword = ui->key->text();
            text = columnarCipherEncode(&text, &keyword);
            break;
        }
        case DOUBLE_COLUMNAR:
        {
            QString firstKeyword = ui->key->text();
            QString secondKeyword = ui->secondKey->text();
            text = doubleColumnarEncode(&text, &firstKeyword, &secondKeyword);
            break;
        }
        case MYSZKOWSKI:
        {
            QString keyword2 = ui->key->text();
            text = myszkowskiEncode(&text, &keyword2);
            break;
        }
    }


    QFile output(outFile);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "cannot open the file");
        return;
    }

    QTextStream outStream(&output);
    outStream << text;

    input.close();
    output.close();

}

void TextCipher::OnDecode()
{
    inFile = ui->InputFile->text();
    outFile = ui->OutputFile->text();

    QFile input(inFile);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "cannot open the file" + input.errorString());
        return;
    }

    QTextStream inStream(&input);
    QString text = inStream.readAll();

    switch(ui->comboBox->currentIndex())
    {
        case RAIL_FENCE:
        {
            text = railFenceDecode(&text, ui->spinBox->value());
            break;
        }
        case ROUTE:
        {
            text = routeCipherDecode(ui->spinBox->value(), &text, ui->clockwise->isChecked());
            break;
        }
        case COLUMNAR:
        {
            QString keyword = ui->key->text();
            text = columnarCipherDecode(&text, &keyword);
            break;
        }
        case DOUBLE_COLUMNAR:
        {
            QString firstKeyword = ui->key->text();
            QString secondKeyword = ui->secondKey->text();
            text = doubleColumnarDecode(&text, &firstKeyword, &secondKeyword);
            break;
        }
        case MYSZKOWSKI:
        {

            QString keyword = ui->key->text();
            text = myszkowskiDecode(&text, &keyword);
            break;
        }
    }


    QFile output(outFile);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "warning", "cannot open the file");
        return;
    }

    QTextStream outStream(&output);
    outStream << text;

    input.close();
    output.close();

}

void TextCipher::OnIndexChanged()
{
    switch(ui->comboBox->currentIndex())
    {
    case RAIL_FENCE:
        ui->clockwise->setEnabled(false);
        ui->antiClockwise->setEnabled(false);
        ui->secondKey->setEnabled(false);
        ui->secondKeyLabel->setEnabled(false);
        ui->key->setEnabled(false);
        ui->KeyLabel->setEnabled(false);
        ui->spinBox->setEnabled(true);
        ui->spinLabel->setEnabled(true);
            break;
    case ROUTE:
        ui->clockwise->setEnabled(true);
        ui->antiClockwise->setEnabled(true);
        ui->secondKey->setEnabled(false);
        ui->secondKeyLabel->setEnabled(false);
        ui->key->setEnabled(true);
        ui->KeyLabel->setEnabled(false);
        ui->spinBox->setEnabled(true);
        ui->spinLabel->setEnabled(true);
        break;
    case COLUMNAR:
        ui->clockwise->setEnabled(false);
        ui->antiClockwise->setEnabled(false);
        ui->secondKey->setEnabled(false);
        ui->secondKeyLabel->setEnabled(false);
        ui->key->setEnabled(true);
        ui->KeyLabel->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->spinLabel->setEnabled(false);
        break;
    case DOUBLE_COLUMNAR:
        ui->clockwise->setEnabled(false);
        ui->antiClockwise->setEnabled(false);
        ui->secondKey->setEnabled(true);
        ui->secondKeyLabel->setEnabled(true);
        ui->key->setEnabled(true);
        ui->KeyLabel->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->spinLabel->setEnabled(false);
        break;
    case MYSZKOWSKI:
        ui->clockwise->setEnabled(false);
        ui->antiClockwise->setEnabled(false);
        ui->secondKey->setEnabled(false);
        ui->secondKeyLabel->setEnabled(false);
        ui->key->setEnabled(true);
        ui->KeyLabel->setEnabled(true);
        ui->spinBox->setEnabled(false);
        ui->spinLabel->setEnabled(false);

        break;
    }
}

void TextCipher::OnEncodeShort()
{
    QString text = ui->shortInput->toPlainText();

    switch(ui->ciphersComboBox->currentIndex())
    {
        case RAIL_FENCE:
        {
            text = railFenceEncode(&text, ui->shortKey->value());
            break;
        }
        case ROUTE:
        {
            text = routeCipherEncode(ui->shortKey->value(), &text, ui->shortClockwise->isChecked());
            break;
        }
        case COLUMNAR:
        {
            QString keyword = ui->shortKeyword->text();

            if (keyword.size() > 0)
                text = columnarCipherEncode(&text, &keyword);

            break;
        }
        case DOUBLE_COLUMNAR:
        {
            QString firstKeyword = ui->shortKeyword->text();
            QString secondKeyword = ui->shortSecondKeyword->text();

            if (firstKeyword.size() > 0 && secondKeyword.size() > 0)
                text = doubleColumnarEncode(&text, &firstKeyword, &secondKeyword);
            break;
        }
        case MYSZKOWSKI:
        {
            QString keyword2 = ui->shortKeyword->text();
            if (keyword2.size() > 0)
                text = myszkowskiEncode(&text, &keyword2);

            break;
        }
    }

    ui->shortOutput->setPlainText(text);
}

void TextCipher::OnDecodeShort()
{
    QString text = ui->shortInput->toPlainText();

    switch(ui->ciphersComboBox->currentIndex())
    {
        case RAIL_FENCE:
        {
            text = railFenceDecode(&text, ui->shortKey->value());
            break;
        }
        case ROUTE:
        {
            text = routeCipherDecode(ui->shortKey->value(), &text, ui->shortClockwise->isChecked());
            break;
        }
        case COLUMNAR:
        {
            QString keyword = ui->shortKeyword->text();
            text = columnarCipherDecode(&text, &keyword);
            break;
        }
        case DOUBLE_COLUMNAR:
        {
            QString firstKeyword = ui->shortKeyword->text();
            QString secondKeyword = ui->shortSecondKeyword->text();
            text = doubleColumnarDecode(&text, &firstKeyword, &secondKeyword);
            break;
        }
        case MYSZKOWSKI:
        {
            QString keyword2 = ui->shortKeyword->text();
            text = myszkowskiDecode(&text, &keyword2);
            break;
        }
    }

    ui->shortOutput->setPlainText(text);
}

void TextCipher::OnIndexChangedShort()
{
    switch(ui->ciphersComboBox->currentIndex())
    {
    case RAIL_FENCE:
        ui->shortClockwise->setEnabled(false);
        ui->shortAntiClockwise->setEnabled(false);
        ui->shortSecondKeyword->setEnabled(false);
        ui->shortSecondKeywordLAbel->setEnabled(false);
        ui->shortKeyword->setEnabled(false);
        ui->shortKeywordLabel->setEnabled(false);
        ui->shortKey->setEnabled(true);
        ui->shortKeyLabel->setEnabled(true);
            break;
    case ROUTE:
        ui->shortClockwise->setEnabled(true);
        ui->shortAntiClockwise->setEnabled(true);
        ui->shortSecondKeyword->setEnabled(false);
        ui->shortSecondKeywordLAbel->setEnabled(false);
        ui->shortKeyword->setEnabled(false);
        ui->shortKeywordLabel->setEnabled(false);
        ui->shortKey->setEnabled(true);
        ui->shortKeyLabel->setEnabled(true);
        break;
    case COLUMNAR:
        ui->shortClockwise->setEnabled(false);
        ui->shortAntiClockwise->setEnabled(false);
        ui->shortSecondKeyword->setEnabled(false);
        ui->shortSecondKeywordLAbel->setEnabled(false);
        ui->shortKeyword->setEnabled(true);
        ui->shortKeywordLabel->setEnabled(true);
        ui->shortKey->setEnabled(false);
        ui->shortKeyLabel->setEnabled(false);
        break;
    case DOUBLE_COLUMNAR:
        ui->shortClockwise->setEnabled(false);
        ui->shortAntiClockwise->setEnabled(false);
        ui->shortSecondKeyword->setEnabled(true);
        ui->shortSecondKeywordLAbel->setEnabled(true);
        ui->shortKeyword->setEnabled(true);
        ui->shortKeywordLabel->setEnabled(true);
        ui->shortKey->setEnabled(false);
        ui->shortKeyLabel->setEnabled(false);
        break;
    case MYSZKOWSKI:
        ui->shortClockwise->setEnabled(false);
        ui->shortAntiClockwise->setEnabled(false);
        ui->shortSecondKeyword->setEnabled(false);
        ui->shortSecondKeywordLAbel->setEnabled(false);
        ui->shortKeyword->setEnabled(true);
        ui->shortKeywordLabel->setEnabled(true);
        ui->shortKey->setEnabled(false);
        ui->shortKeyLabel->setEnabled(false);
        break;
    }
}
