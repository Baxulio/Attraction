import QtQuick 2.10
import QtQuick.Controls 2.3

Page {
    id: page

    title: qsTr("Home")

    GridView {
        id: gridView
        cellHeight: 200
        cellWidth: 200
        anchors.margins: 10
        anchors.fill: parent
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        delegate: Item {
            x: 5
            height: 180
            Column {
                Rectangle {
                    width: 170
                    height: 170
                    color: colorCode
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                }
                spacing: 5
            }
        }
        ScrollIndicator.vertical: ScrollIndicator { }

    }

    footer: TextField {
        id: textField
        text: qsTr("")
        padding: 10
        placeholderText: qsTr("Фильтрация ...")
    }

    Connections {
        target: textField
        onTextEdited: print("clicked")
    }
}
