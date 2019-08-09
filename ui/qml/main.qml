import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import com.LIMOSIM 1.0
import "JS.js" as JS


ApplicationWindow {
    property var m_info;

    visible: true
    width: 1200
    height: 700
    title: qsTr("LIMoSim")
    id: window



    ViewPort
    {
        id: viewPort
        objectName: "viewPort"
        //color: "gray"
        width: 1200 - toolBar.width
        height: 700
        x: toolBar.width

        DropArea // external drag & drop
        {
            anchors.fill: parent
            onDropped: UiManager.handleDrop(drop.text);
        }

        MapUi
        {
            id: map
            objectName: "map"
          //  width: window.width - toolBar.width
           // height: window.height

            width: 2000
            height: 2000

            MouseArea
            {
                hoverEnabled: true
                anchors.fill: parent
                onWheel:
                {
                    var scale = wheel.angleDelta.y / 1200;
                    var s = Settings.scaleFactor + scale;

                    Settings.handleScale(s, mouseX, mouseY);
                }
                acceptedButtons: "NoButton"
            }

            Label
            {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"

                Component.onCompleted:
                {
                    UiEventManager.display.connect(onDisplay);
                }

                function onDisplay(_text)
                {
                    text = _text;
                }
            }


            focus: true
            Keys.onPressed: Editor.handleKey(event.key)
            onFocusChanged: map.focus = true

        }
    }








    Component.onCompleted:
    {
        UiEventManager.segmentSelected.connect(onSegmentSelected);

    }

    function onSegmentSelected(_segment)
    {
        if(m_info)
            JS.deleteObject(m_info);

        var info = JS.createObject("WayInfo", map);
        info.setSegment(_segment);
        m_info = info;

    }

        Rectangle
        {
            id: toolBar
            height: window.height
            width: 50
            color: "#1c1c1c"

            Column
            {
                spacing: 1
                ToolItem
                {
                    id: saveButton
                    text: "Save"

                    FileDialog {
                        id: saveDialog
                        visible: false
                        title: "Save"
                        folder: shortcuts.home

                        selectExisting: false
                        onAccepted:
                        {
                            var path = saveDialog.fileUrls;
                            var saved = UiManager.saveScenario(path);
                            saveButton.handleResult(saved);
                        }
                    }

                    onClicked: {saveDialog.visible = true;}

                }
                ToolItem
                {
                    id: loadButton
                    text: "Load"


                    FileDialog {
                        id: loadDialog
                        visible: false
                        title: "Save"
                        folder: shortcuts.home

                        selectExisting: true
                        onAccepted:
                        {
                            var path = loadDialog.fileUrls;
                            var loaded = UiManager.loadScenario(path);
                            loadButton.handleResult(loaded);
                        }
                    }

                    onClicked: {loadDialog.visible = true;}
                }
                ToolItem
                {
                    id: clearButton
                    text: "Clear"
                    onClicked: {UiManager.clear();}
                }
                ToolItem
                {
                    id: exportButton
                    text: "EPS"
                    onClicked: {UiManager.exportScreenshot();}
                }
            }



        }








    Rectangle
    {
        id: simControlBar
        height: 50
        width: 600
        color: "#1c1c1c"

        anchors.horizontalCenter: parent.horizontalCenter

        Row
        {
            ToolItem
            {
                id: startButton
                text: "Start"
                onClicked:
                {
                    UiManager.start();
                }
            }

            ToolItem
            {
                id: stopButton
                text: "Stop"
                onClicked:
                {
                    UiManager.stop();
                }
            }

            ToolItem
            {
                id: stepButton
                text: "Step"
                onClicked:
                {
                    UiManager.step();
                }
            }
            Column
            {
                width: 100
                Label
                {
                    id: eventCounterLabel
                    color: "white"
                }
                Label
                {
                    id: simTimeLabel
                    color: "white"
                }
                Label
                {
                    id: simDurationLabel
                    color: "white"
                }
            }
            Slider
            {
                from: 1
                to: 100
                value: 10

                onValueChanged: UiManager.setTimeScaleFactor(value)
            }


        }

        Component.onCompleted:
        {
            UiManager.updated.connect(onUpdated);
        }

        function getTimeDisplayString(_time_s)
        {
            var time_h = Math.floor(_time_s/3600);
            var remainder_h = _time_s % 3600;

            var time_m = Math.floor(remainder_h/60);
            var time_s = (remainder_h % 60).toFixed(2);

            var result = "";
            if(time_h>0)
                result += time_h + " h ";
            if(time_m>0)
                result += time_m + " m ";
            if(time_s>0)
                result += time_s + " s ";

            return result;
        }

        function onUpdated(_events, _scheduled, _simTime_s, _simDuration_s)
        {
            eventCounterLabel.text = _events + " / " + _scheduled + " Events";
            simTimeLabel.text = getTimeDisplayString(_simTime_s);
            simDurationLabel.text = getTimeDisplayString(_simDuration_s);

        }
    }

}
