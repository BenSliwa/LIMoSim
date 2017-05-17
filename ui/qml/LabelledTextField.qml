import QtQuick 2.7
import QtQuick.Controls 2.0

Row
{
    id: row

    property alias key: labelObject.text
    property alias value: textFieldObject.text
    signal textChanged(string _value)

    Label
    {
        id: labelObject
        text: "label"
    }

    spacing: 20

    TextInput
    {
        id: textFieldObject
        text: "textField"
        color: "white"

        onAccepted: row.textChanged(text)
    }



}
