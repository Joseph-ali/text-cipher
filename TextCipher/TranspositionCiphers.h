#ifndef TRANSPOSITIONCIPHERS_H
#define TRANSPOSITIONCIPHERS_H
#include <QString>
#include <vector>

QString railFenceEncode(const QString* message, int key);
QString railFenceDecode(const QString* message, int key);
QString routeCipherEncode(int key, const QString* message, bool clockwise);
QString routeCipherDecode(int key, const QString* message, bool clockwise);
QString columnarCipherEncode(const QString* message, const QString* keyword);
QString columnarCipherDecode(const QString* message, const QString* keyword);
QString doubleColumnarEncode(const QString* message, const QString* firstKeyword, const QString* secondKeyword);
QString doubleColumnarDecode(const QString* message, const QString* firstKeyword, const QString* secondKeyword);
QString myszkowskiEncode(const QString* message, const QString* keyword);
QString myszkowskiDecode(const QString* message, const QString* keyword);
char parseDigit(int ch);
QString parseInt(int n);

#endif // TRANSPOSITIONCIPHERS_H
