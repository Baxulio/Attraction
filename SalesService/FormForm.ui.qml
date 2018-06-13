import QtQuick 2.4
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 400
    height: 400

    RowLayout {
        id: rowLayout2
        x: 13
        y: 22
        width: 367
        height: 100
        spacing: 10

        Text {
            id: bracer_number
            text: qsTr("23")
            font.pixelSize: 70
        }

        ColumnLayout {
            id: columnLayout
            width: 100
            height: 100

            RowLayout {
                id: rowLayout
                width: 120
                height: 25

                Text {
                    id: text3
                    color: "#808080"
                    text: qsTr("Вход #")
                    font.pixelSize: 24
                }

                Text {
                    id: enter_number
                    text: qsTr("Вход #")
                    font.pixelSize: 24
                }
            }

            RowLayout {
                id: rowLayout1
                width: 175
                height: 28

                Text {
                    id: text4
                    color: "#808080"
                    text: qsTr("Время входа: ")
                    font.pixelSize: 24
                }

                Text {
                    id: enter_time
                    text: qsTr("Время входа: ")
                    font.pixelSize: 24
                }

            }


        }


    }

    RowLayout {
        id: rowLayout3
        x: 13
        y: 128
        width: 195
        height: 100

        Image {
            id: tariff_icon
            width: 86
            height: 84
            fillMode: Image.PreserveAspectFit
            source: "icons/gallery/20x20/card.png"
        }

        Text {
            id: text1
            text: qsTr("Text")
            font.pixelSize: 12
        }

        Image {
            id: tariff_icon1
            width: 86
            height: 84
            fillMode: Image.PreserveAspectFit
            source: "icons/gallery/20x20/card.png"
        }

        Text {
            id: text2
            text: qsTr("Text")
            font.pixelSize: 12
        }
    }

    ColumnLayout {
        id: columnLayout1
        x: 82
        y: 281
        width: 100
        height: 100

        Text {
            id: text5
            text: qsTr("Транзакции: ")
            font.pixelSize: 12
        }

        ListView {
            id: listView
            width: 110
            height: 160
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
            }
            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    spacing: 10
                }
            }
        }

        Text {
            id: text6
            text: qsTr("Транзакции: ")
            font.pixelSize: 12
        }
    }
}
