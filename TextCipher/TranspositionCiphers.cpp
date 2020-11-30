#include "TranspositionCiphers.h"
#include <iostream>

using namespace std;

QString myszkowskiDecode(const QString* message, const QString* keyword)
{
    vector<int> numbersSequence(keyword->size(), 0);

    //determining the rows and the columns for the grid
    int rows = (message->size() % keyword->size() == 0) ? message->size() / keyword->size() : message->size() / keyword->size() + 1;
    int columns = keyword->size();
    vector<vector<QString>> grid(++rows, vector<QString>(columns, "%"));
    QString plaintext = "";

    //assigning the sequence of numbers
    for (int i = 0; i < keyword->size(); ++i)
    {
        int n = 1;

        for (int j = 0; j < keyword->size(); ++j)
        {
            if ((*keyword)[i] > (*keyword)[j])
                ++n;
        }

        numbersSequence[i] = n;
    }

    //finding the heighest number in the numbersSequence
    int heighestNum = 0;

    for (size_t i = 0; i < numbersSequence.size(); ++i)
    {
        if (numbersSequence[i] > heighestNum)
            heighestNum = numbersSequence[i];
    }


    //finding the same numbers in the numbersSequence
    bool sameNumbers = false;
    vector<int> sameNumbersVec;

    for (size_t i = 1; i < numbersSequence.size(); ++i)
        for (size_t j = i; j < numbersSequence.size(); ++j)
            if (numbersSequence[i - 1] == numbersSequence[j])
            {
                sameNumbersVec.push_back(numbersSequence[i - 1]);
            }


    //reading the letters form the grid column by column in order
    for (int i = 1, flage = 0; i <= heighestNum; ++i)
    {
        sameNumbers = false;
        for (int j = 0, k = 0; j < numbersSequence.size(); ++j)
        {
            if (!sameNumbers && sameNumbersVec.size() > 0)
                if (i == sameNumbersVec[k])
                {
                    for (int y = 1; y < rows; ++y)
                    {
                        for (int x = i; x < numbersSequence.size(); ++x)
                        {
                            if (i == numbersSequence[x])
                                grid[y][x] = (*message)[flage++];
                        }
                    }

                    if (k < sameNumbersVec.size())
                    {
                        ++k;
                        if (k == sameNumbersVec.size())
                            sameNumbers = true;
                    }
                    continue;
                }

            if (i == numbersSequence[j])
            {
                bool terminator = false;

                for (int x = 0; x < sameNumbersVec.size(); ++x)
                    if (sameNumbersVec[x] == i)
                        terminator = true;

                if (terminator)
                    break;

                cout << message->size();

                for (int x = 1; x < rows; ++x)
                {
                    grid[x][j] = (*message)[flage++];
                }
                break;
            }
        }
    }

    //reading the text form the grid
    for (int i = 1, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            if (flage < message->size() && grid[i][j][0] != '%')
                plaintext += grid[i][j];
    }

    return plaintext;
}

QString myszkowskiEncode(const QString* message, const QString* keyword)
{

    vector<int> numbersSequence(keyword->size(), 0);

    //determining the rows and the columns for the grid
    int rows = (message->size() % keyword->size() == 0) ? message->size() / keyword->size() : message->size() / keyword->size() + 1;
    int columns = keyword->size();
    vector<vector<QString>> grid(++rows, vector<QString>(columns, "%"));
    QString ciphertext = "";

    //writing the message to the grid and skiping the first row
    for (int i = 1, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            if (flage < message->size())
                grid[i][j] = (*message)[flage++];
    }

    //assigning the sequence of numbers
    for (int i = 0; i < keyword->size(); ++i)
    {
        int n = 1;

        for (int j = 0; j < keyword->size(); ++j)
        {
            if ((*keyword)[i] > (*keyword)[j])
                ++n;
        }

        numbersSequence[i] = n;
    }

    //converting the integers of the sequence of numbers to a string to put it int the grid
    for (int i = 0; i < columns; ++i)
        grid[0][i] = parseInt(numbersSequence[i]);

    //finding the heighest number in the numbersSequence
    int heighestNum = 0;

    for (int i = 0; i < numbersSequence.size(); ++i)
    {
        if (numbersSequence[i] > heighestNum)
            heighestNum = numbersSequence[i];
    }


    //finding the same numbers in the numbersSequence
    bool sameNumbers = false;
    vector<int> sameNumbersVec;

    for (int i = 1; i < numbersSequence.size(); ++i)
        for (int j = i; j < numbersSequence.size(); ++j)
            if (numbersSequence[i - 1] == numbersSequence[j])
            {
                sameNumbersVec.push_back(numbersSequence[i - 1]);
            }


    //reading the letters form the grid column by column in order
    for (int i = 1; i <= heighestNum; ++i)
    {
        sameNumbers = false;
        for (int j = 0, k = 0; j < numbersSequence.size(); ++j)
        {
            if (!sameNumbers && sameNumbersVec.size() > 0)
                if (i == sameNumbersVec[k])
                {
                    for (int y = 1; y < rows; ++y)
                    {
                        for (int x = i; x < numbersSequence.size(); ++x)
                        {
                            if (i == numbersSequence[x])
                                ciphertext += grid[y][x];
                        }
                    }

                    if (k < sameNumbersVec.size())
                    {
                        ++k;
                        if (k == sameNumbersVec.size())
                            sameNumbers = true;
                    }
                    continue;
                }

            if (i == numbersSequence[j])
            {
                bool terminator = false;

                for (int x = 0; x < sameNumbersVec.size(); ++x)
                    if (sameNumbersVec[x] == i)
                        terminator = true;

                if (terminator)
                    break;

                for (int x = 1; x < rows; ++x)
                {
                    ciphertext += grid[x][j];
                }
                break;
            }
        }
    }

    return ciphertext;
}

QString doubleColumnarEncode(const QString* message, const QString* firstKeyword, const QString* secondKeyword)
{
    QString ciphertext = columnarCipherEncode(&(*message), &(*firstKeyword));
    ciphertext = columnarCipherEncode(&ciphertext, &(*secondKeyword));
    return ciphertext;
}

QString doubleColumnarDecode(const QString* message, const QString* firstKeyword, const QString* secondKeyword)
{
    QString plaintext = columnarCipherDecode(&(*message), &(*firstKeyword));
    plaintext = columnarCipherDecode(&plaintext, &(*secondKeyword));
    return plaintext;
}

//Parses a digit argument and return the digit in a char type
char parseDigit(int ch)
{
    switch (ch)
    {
    case 0:
        return '0';
        break;
    case 1:
        return '1';
        break;
    case 2:
        return '2';
        break;
    case 3:
        return '3';
        break;
    case 4:
        return '4';
        break;
    case 5:
        return '5';
        break;
    case 6:
        return '6';
        break;
    case 7:
        return '7';
        break;
    case 8:
        return '8';
        break;
    case 9:
        return '9';
        break;
    default:
        return 0;
        break;
    }
}

//Parses an integer and return the integer in a string
QString parseInt(int n)
{
    QString result = "";

    while (n >= 1)
    {
        result += parseDigit(n % 10);
        n /= 10;
    }

    QString reversed = "";
    for (int i = result.size() - 1; i >= 0; --i)
    {
        reversed += result[i];
    }
    return reversed;
}


/*
* parameters message, keyword
* keyword is the secret keyword it is usually a word or a sequence of letters. It will be converted to a sequence of numbers
* the numbers are defined by an alphabetical order of the letters in the keyword
*/
QString columnarCipherEncode(const QString* message, const QString* keyword)
{

    vector<int> numbersSequence(keyword->size(), 0);

    //determining the rows and the columns for the grid
    int rows = (message->size() % keyword->size() == 0) ? message->size() / keyword->size() : message->size() / keyword->size() + 1;
    int columns = keyword->size();
    vector<vector<QString>> grid(++rows, vector<QString>(columns, "0"));
    QString ciphertext = "";

    //writing the message to the grid and skiping the first row
    for (int i = 1, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            if (flage < message->size())
                grid[i][j] = (*message)[flage++];
    }

    //assigning the sequence of numbers
    for (int i = 0; i < keyword->size(); ++i)
    {
        int n = 1;

        for (int j = 0; j < keyword->size(); ++j)
        {
            if ((*keyword)[i] > (*keyword)[j])
                ++n;
        }

        for (int j = 0; j < i; ++j)
        {
            if ((*keyword)[i] == (*keyword)[j])
                ++n;
        }

        numbersSequence[i] = n;
    }

    //converting the integers of the sequence of numbers to a string to put it int the grid
    for (int i = 0; i < columns; ++i)
        grid[0][i] = parseInt(numbersSequence[i]);

    //reading the letters form the grid column by column in order
    for (int i = 1; i <= numbersSequence.size(); ++i)
    {
        for (int j = 0; j < numbersSequence.size(); ++j)
        {
            if (i == numbersSequence[j])
            {
                for (int k = 1; k < rows; ++k)
                {
                    ciphertext += grid[k][j];
                }
                break;
            }
        }
    }

    return ciphertext;
}


QString columnarCipherDecode(const QString* message, const QString* keyword)
{
    vector<int> numbersSequence(keyword->size(), 0);

    //determining the rows and the columns for the grid
    int rows = (message->size() % keyword->size() == 0) ? message->size() / keyword->size() : message->size() / keyword->size() + 1;
    int columns = keyword->size();
    vector<vector<QString>> grid(++rows, vector<QString>(columns, "0"));
    QString plaintext = "";

    //determinig the sequence of numbers
    for (int i = 0; i < keyword->size(); ++i)
    {
        int n = 1;

        for (int j = 0; j < keyword->size(); ++j)
        {
            if ((*keyword)[i] > (*keyword)[j])
                ++n;
        }

        for (int j = 0; j < i; ++j)
        {
            if ((*keyword)[i] == (*keyword)[j])
                ++n;
        }

        numbersSequence[i] = n;
    }

    //converting the integers of numbersSequence and putting it in the grid
    for (int i = 0; i < columns; ++i)
        grid[0][i] = parseInt(numbersSequence[i]);

    //writing the message to the grid row by row and skiping the first row
    for (int i = 1, index = 0; i <= numbersSequence.size(); ++i)
    {
        for (int j = 0; j < numbersSequence.size(); ++j)
        {
            if (i == numbersSequence[j])
            {
                for (int k = 1; k < rows; ++k)
                {
                    grid[k][j] = (*message)[index++];
                }
                break;
            }
        }
    }

    //reading the message
    for (int i = 1, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            if (flage < message->size() && (*message)[0] != '0')
            {
                plaintext += grid[i][j];
                ++flage;
            }
    }

    return plaintext;
}


/*
* parameters message, key
* key will determine the number of rows
*/
QString railFenceEncode(const QString* message, int key)
{
    vector<vector<QChar>> railFence(key, vector<QChar>(message->size(), '\0'));
    QString cipher = "";

    bool increment = true;

    //writing the message in a zigzag pattern to the grid
    for (int i = 0, j = 0; j < message->size(); j++)
    {
        if (i == key - 1)
            increment = false;
        else if (i == 0)
            increment = true;

        railFence[i][j] = (*message)[j];

        if (increment)
            ++i;
        else
            --i;
    }

    // reading the letters row by row and skip any whitespace
    for (int i = 0; i < key; i++)
        for (int j = 0; j < message->size(); j++)
        {
            if (railFence[i][j] == '\0')
                continue;

            cipher += railFence[i][j];
        }

    return cipher;
}

QString railFenceDecode(const QString* message, int key)
{
    vector<vector<QChar>> railFence(key, vector<QChar>(message->size(), ' '));
    QString cipher = "";

    bool increment = true;

    //writing a 0 char to the cells in zigzag pattern
    for (int i = 0, j = 0; j < message->size(); j++)
    {
        if (i == key - 1)
            increment = false;
        else if (i == 0)
            increment = true;

        railFence[i][j] = '0';

        if (increment)
            ++i;
        else
            --i;
    }

    //replacing the 0 char by the message letters
    for (int i = 0, temp = 0; i < key; ++i)
        for (int j = 0; j < message->size(); ++j)
            if (railFence[i][j] == '0')
                railFence[i][j] = (*message)[temp++];

    //reading the letters from the grid in a zigzag pattern
    for (int i = 0, j = 0; j < message->size(); j++)
    {
        if (i == key - 1)
            increment = false;
        else if (i == 0)
            increment = true;

        cipher += railFence[i][j];

        if (increment)
            ++i;
        else
            --i;
    }

    return cipher;
}

/*
* parameters key, message, clockwise
* key will determine the number of columns and clockwise will determine the order of reading the letters to the grid
*/
QString routeCipherEncode(int key, const QString* message, bool clockwise)
{

    int rows = (message->size() % key == 0) ? message->size() / key : message->size() / key + 1;;
    int columns = key;

    vector<vector<QChar>> grid(rows, vector<QChar>(columns, ' '));

    // writing the letters to the grid
    for (int i = 0, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++, ++flage)
        {
            if (message->size() > flage)
                grid[i][j] = (*message)[flage];
            else
                grid[i][j] = '0';
        }
    }

    /*
    * declaring a bounds for the grid to determine the path of reading the letters from the grid
    * and declaring the increment and decrement variables to move in the grid. when crossing the bounds we decrement the index and we update the bound
    * and assign a false value to the increment variable and true for the other increment or decrement
    */
    QString cipherText = "";
    int leftBound = -1;
    int rightBound = key;
    int upperBound = -1;
    int lowerBound = rows;
    bool rIncrement = (clockwise) ? true : false;
    bool cIncrement = false;
    bool rDecrement = false;
    bool cDecrement = (clockwise) ? false : true;
    int terminator = rows * columns;
    bool skip = false;
    for (int r = 0, c = key - 1; cipherText.size() < terminator;)
    {
        if (!skip)
        {
            cipherText += grid[r][c];
        }

        if (rIncrement && r < lowerBound)
        {
            ++r;
            skip = false;
            if (r == lowerBound)
            {
                --r;
                if (clockwise)
                {
                    --rightBound;
                    rIncrement = false;
                    cDecrement = true;
                }
                else
                {
                    ++leftBound;
                    rIncrement = false;
                    cIncrement = true;
                }

                skip = true;
            }
            continue;
        }

        else if (cDecrement && c > leftBound)
        {
            --c;
            skip = false;

            if (c == leftBound)
            {
                ++c;
                if (clockwise)
                {
                    --lowerBound;
                    cDecrement = false;
                    rDecrement = true;
                }
                else
                {
                    ++upperBound;
                    cDecrement = false;
                    rIncrement = true;
                }
                skip = true;
            }
            continue;
        }

        else if (rDecrement && r > upperBound)
        {
            --r;
            skip = false;

            if (r == upperBound)
            {
                ++r;
                if (clockwise)
                {
                    ++leftBound;
                    rDecrement = false;
                    cIncrement = true;
                }
                else
                {
                    --rightBound;
                    rDecrement = false;
                    cDecrement = true;
                }
                skip = true;
            }
            continue;
        }

        else if (cIncrement && c < rightBound)
        {
            ++c;
            skip = false;

            if (c == rightBound)
            {
                --c;
                if (clockwise)
                {
                    ++upperBound;
                    cIncrement = false;
                    rIncrement = true;
                }
                else
                {
                    --lowerBound;
                    cIncrement = false;
                    rDecrement = true;
                }
                skip = true;
            }
        }
    }


    return cipherText;
}

QString routeCipherDecode(int key, const QString* message, bool clockwise)
{
    int rows = (message->size() % key == 0) ? message->size() / key : message->size() / key + 1;
    int columns = key;
    vector<vector<QChar>> grid(rows, vector<QChar>(columns, ' '));



    QString cipherText = "";
    int leftBound = -1;
    int rightBound = key;
    int upperBound = -1;
    int lowerBound = rows;
    bool rIncrement = (clockwise) ? true : false;
    bool cIncrement = false;
    bool rDecrement = false;
    bool cDecrement = (clockwise) ? false : true;
    bool skip = false;

    for (int r = 0, c = key - 1, flage = 0; flage < message->size();)
    {
        if (!skip)
        {
            grid[r][c] = (*message)[flage++];
        }

        if (rIncrement && r < lowerBound)
        {
            ++r;
            skip = false;
            if (r == lowerBound)
            {
                --r;
                if (clockwise)
                {
                    --rightBound;
                    rIncrement = false;
                    cDecrement = true;
                }
                else
                {
                    ++leftBound;
                    rIncrement = false;
                    cIncrement = true;
                }

                skip = true;
            }
            continue;
        }

        else if (cDecrement && c > leftBound)
        {
            --c;
            skip = false;

            if (c == leftBound)
            {
                ++c;
                if (clockwise)
                {
                    --lowerBound;
                    cDecrement = false;
                    rDecrement = true;
                }
                else
                {
                    ++upperBound;
                    cDecrement = false;
                    rIncrement = true;
                }
                skip = true;
            }
            continue;
        }

        else if (rDecrement && r > upperBound)
        {
            --r;
            skip = false;

            if (r == upperBound)
            {
                ++r;
                if (clockwise)
                {
                    ++leftBound;
                    rDecrement = false;
                    cIncrement = true;
                }
                else
                {
                    --rightBound;
                    rDecrement = false;
                    cDecrement = true;
                }
                skip = true;
            }
            continue;
        }

        else if (cIncrement && c < rightBound)
        {
            ++c;
            skip = false;

            if (c == rightBound)
            {
                --c;
                if (clockwise)
                {
                    ++upperBound;
                    cIncrement = false;
                    rIncrement = true;
                }
                else
                {
                    --lowerBound;
                    cIncrement = false;
                    rDecrement = true;
                }
                skip = true;
            }
        }
    }


    for (int i = 0, flage = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++, ++flage)
        {
            if (message->size() > flage)
                cipherText += grid[i][j];
        }
    }
    return cipherText;
}
