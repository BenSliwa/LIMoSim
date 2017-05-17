import QtQuick 2.0

Rectangle
{
    property var m_segment;

    width: 200
    height: width

    color: "transparent"

    Column
    {
        LabelledTextField
        {
            id: nameField
            key: "Name"
            onTextChanged: m_segment.setName(value)
        }

        LabelledTextField
        {
            id: lanes
            key: "Lanes"
            onTextChanged: m_segment.setLanes(value)
        }

    }

    function setSegment(_segment)
    {
        m_segment = _segment;

        nameField.value = m_segment.getName();
        lanes.value = m_segment.getLanes();
    }

}
