import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle
{
    property alias text: label.text
    signal clicked()

    id: rect
    height: 50
    width: 50
    color: "black"
    border.width: 1
    border.color: "#1a1a1a"

    Label
    {
        id: label
        text: rect.text
        color: "white"
        anchors.centerIn: parent
    }

    MouseArea
    {
        hoverEnabled: true
        anchors.fill: parent
        onHoveredChanged:
        {
            if(containsMouse)
                rect.color = "darkgray";
            else
                rect.color = "black";
        }

        onClicked:
        {
            //rect.color = "green";
            rect.clicked();
        }
    }

    function handleResult(success)
    {
        if(success)
            color = "green";
        else
            color = "red";
    }
}
