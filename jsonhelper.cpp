#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "jsonhelper.h"

/*
 * set the Json value recursive
 * Example:
**     setJsonValue(doc, "firstName", QJsonValue("Natalia"));
**     setJsonValue(doc, "age", 22);
**     setJsonValue(doc, "address.state", "None");
**     setJsonValue(doc, "phoneNumber[0].number", "333 543-3210");
**     setJsonValue(doc, "family[0][2]", "Bill");
**     setJsonValue(doc, "family[1][1]", "Winston");
**     setJsonValue(doc, "family[2].father.age", 56);
**/
void JsonHelper::setJsonValue(QJsonValue &destValue, const QString &path,
                                    const QJsonValue &newValue)
{
    /* The first dot position */
    const int indexOfDot = path.indexOf('.');
    /* dotPropertyName.dotSubPath */
    QString dotPropertyName;
    QString dotSubPath;
    if (indexOfDot > 0) {
        dotPropertyName = path.left(indexOfDot);
        dotSubPath = indexOfDot > 0 ? path.mid(indexOfDot + 1) : QString();
    }

    /* The first 'open and close' square bracket position  */
    const int indexOfSquareBracketOpen = path.indexOf('[');
    const int indexOfSquareBracketClose = path.indexOf(']');
    if ((indexOfSquareBracketOpen >= 0 && indexOfSquareBracketClose < 0)
            || (indexOfSquareBracketOpen < 0 && indexOfSquareBracketClose >= 0)) {
        qDebug() << "Invalid path format: [ and ] mismatch";
        return;
    }

    QString squareBracketPropertyName;
    QString squareBracketSubPath;
    int arrayIndex = 0;
    if (indexOfSquareBracketOpen >= 0) {
        /* squareBracketPropertyName[arrayIndex]squareBracketSubPath */
        arrayIndex = path.midRef(indexOfSquareBracketOpen + 1,
                                 indexOfSquareBracketClose - indexOfSquareBracketOpen - 1).toInt();
        squareBracketPropertyName = path.left(indexOfSquareBracketOpen);
        /* ']' right is '.' */
        if (path.mid(indexOfSquareBracketClose + 1)[0] == '.')
            squareBracketSubPath = path.mid(indexOfSquareBracketClose + 2);
        else
            squareBracketSubPath = path.mid(indexOfSquareBracketClose + 1);
    }

    /* determine what is first in path. dot or bracket */
    QString usedPropertyName;
    QString usedSubPath;
    if (!destValue.isArray() && (indexOfSquareBracketOpen >= 0)
        && ((indexOfDot > 0 && indexOfSquareBracketOpen < indexOfDot) || (indexOfDot < 0))) {
        /* a[n][m].b
         *     have '[]', but desValue is not array,
         *     propertyName is a, subPath is [n][m].b
         * a[n][m]
         *     have '[]', but desValue is not array,
         *     propertyName is a, subPath is [n][m]
         */
        usedPropertyName = squareBracketPropertyName;
        usedSubPath = path.mid(indexOfSquareBracketOpen);
    } else {
        if (indexOfDot < 0 && indexOfSquareBracketOpen < 0) {
            usedPropertyName = path;
            usedSubPath = QString();
        } else if (indexOfDot < 0 && indexOfSquareBracketOpen >= 0) {
            usedPropertyName = squareBracketPropertyName;
            usedSubPath = squareBracketSubPath;
        } else if (indexOfDot >=0 && indexOfSquareBracketOpen < 0) {
            usedPropertyName = dotPropertyName;
            usedSubPath = dotSubPath;
        } else {
            /* indexOfDot >=0 && indexOfSquareBracketOpen >= 0 */
            if (indexOfDot < indexOfSquareBracketOpen) {
                usedPropertyName = dotPropertyName;
                usedSubPath = dotSubPath;
            } else {
                usedPropertyName = squareBracketPropertyName;
                usedSubPath = squareBracketSubPath;
            }
        }
    }

    QJsonValue subValue;
    if (destValue.isArray())
        subValue = destValue.toArray()[arrayIndex];
    else if (destValue.isObject())
        subValue = destValue.toObject()[usedPropertyName];
    else if (destValue.isNull()) {
        /* destValue is null, should create a object */
        destValue = QJsonObject();
    } else
        qDebug() << "oh, what should i do now with the following value?! " << destValue;

    /* Recursize end condition */
    if(usedSubPath.isEmpty())
        subValue = newValue;
    else
        setJsonValue(subValue, usedSubPath, newValue);

    if (destValue.isArray()) {
        QJsonArray arr = destValue.toArray();
        if (subValue.isNull())
            arr.removeAt(arrayIndex);
        else
            arr[arrayIndex] = subValue;
        destValue = arr;
    } else if (destValue.isObject()) {
        QJsonObject obj = destValue.toObject();
        if (subValue.isNull())
            obj.remove(usedPropertyName);
        else
            obj[usedPropertyName] = subValue;
        destValue = obj;
    } else
        destValue = newValue;

}

void JsonHelper::setJsonValue(QJsonDocument& doc, const QString& path,
                                    const QJsonValue& newValue)
{
    QJsonValue val;
    if (doc.isArray())
        val = doc.array();
    else
        val = doc.object();

    setJsonValue(val, path, newValue);

    if (val.isArray())
        doc = QJsonDocument(val.toArray());
    else
        doc = QJsonDocument(val.toObject());
}

QJsonValue const JsonHelper::getJsonValue(const QJsonValue &baseValue, const QString &path)
{
    Q_UNUSED(baseValue)
    Q_UNUSED(path)

    /* TODO */
    return QJsonValue();
}

void JsonHelper::testSetJsonValue()
{
    std::string sampleJson = R"({
                          "firstName": "John",
                          "lastName": "Smith",
                          "age": 25,
                          "address": {
                              "streetAddress": "21 2nd Street",
                              "city": "New York",
                              "state": "NY",
                              "postalCode": "10021"
                          },
                          "phoneNumber": [
                              {
                                "type": "home",
                                "number": "212 555-1234"
                              },
                              {
                                "type": "fax",
                                "number": "646 555-4567"
                              }
                          ],
                          "family": [
                              [
                                  "Jeff",
                                  "Marika",
                                  "Tony"
                              ],
                              [
                                  "Steve",
                                  "Sonny",
                                  "Sally"
                              ],
                              {
                                  "father": {
                                      "name": "Mike",
                                      "age": 55,
                                      "sports": [ "tennis", "footbal", "hiking"]
                                  },
                                  "mother": {
                                      "name": "Jennifer",
                                      "age": 49
                                  }
                              },
                              [
                                    ["Tom", "Jerry", "Robin"],
                                    ["Hanks", "Rose", "Nami"]
                              ]
                          ]
                      })";
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(sampleJson), &parseErr);
    if (parseErr.error != QJsonParseError::NoError) {
        qDebug() << "Fail to load Json, error: " << parseErr.error <<  parseErr.errorString();
    }

    /* modify or add value */
    setJsonValue(doc, "firstName", QJsonValue("Natalia"));
    setJsonValue(doc, "age", 22);
    setJsonValue(doc, "address.state", "None");
    setJsonValue(doc, "phoneNumber[0].number", "333 543-3210");
    setJsonValue(doc, "family[0][2]", "Bill");
    setJsonValue(doc, "family[1][1]", "Winston");
    setJsonValue(doc, "family[2].father.age", 56);
    setJsonValue(doc, "family[2].father.sports[2]", "walking");
    setJsonValue(doc, "family[3][1][2]", "Tux");

    /* Add new Value */
    setJsonValue(doc, "family[2].father.job", "Engineer");
    QJsonArray arr;
    arr.append("Dancing");
    arr.append("Table Tennis");
    setJsonValue(doc, "family[2].mother.sports", arr);

    /* remove value */
    setJsonValue(doc, "family[1][2]");

    /* save to file */
    QByteArray jsonData = doc.toJson();
    QFile jsonFile("testSetJson.json");
    if (!jsonFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Fail to open " << jsonFile.fileName();
        return;
    }

    QTextStream in(&jsonFile);
    in << jsonData;
    jsonFile.close();
}

