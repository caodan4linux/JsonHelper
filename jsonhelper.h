/*
** QJson Wrapper
** Provide helper method to operate json document
**
** Json file Example:
** {
**      "firstName": "John",
**      "lastName": "Smith",
**      "age": 25,
**      "address": {
**          "streetAddress": "21 2nd Street",
**          "city": "New York",
**          "state": "NY",
**          "postalCode": "10021"
**      },
**      "phoneNumber": [
**          {
**              "type": "home",
**              "number": "212 555-1234"
**          },
**          {
**              "type": "fax",
**              "number": "646 555-4567"
**          }
**      ],
**      "family": [
**          [
**              "Jeff",
**              "Marika",
**              "Tony"
**          ],
**          [
**              "Steve",
**              "Sonny",
**              "Sally"
**          ],
**          {
**              "father": {
**                  "name": "Mike",
**                  "age": 55
**              },
**              "mother": {
**                  "name": "Jennifer",
**                  "age": 49
**              }
**          }
**      ]
**  }
**
** load json to QjsonDocument doc
** modify or add value
**     setJsonValue(doc, "firstName", QJsonValue("Natalia"));
**     setJsonValue(doc, "age", 22);
**     setJsonValue(doc, "address.state", "None");
**     setJsonValue(doc, "phoneNumber[0].number", "333 543-3210");
**     setJsonValue(doc, "family[0][2]", "Bill");
**     setJsonValue(doc, "family[1][1]", "Winston");
**     setJsonValue(doc, "family[2].father.age", 56);
** remove value
**     setJsonValue(doc, "family[1][2]");
**/

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonDocument>

class JsonHelper
{
public:
    JsonHelper() = default;

    static void setJsonValue(QJsonDocument &doc, const QString &path,
                             const QJsonValue &newValue = QJsonValue());
    static void setJsonValue(QJsonValue &destValue, const QString &path,
                             const QJsonValue &newValue = QJsonValue());

    static const QJsonValue getJsonValue(const QJsonValue &baseValue, const QString &path);

    static void testSetJsonValue();
};

#endif // JSONHELPER_H