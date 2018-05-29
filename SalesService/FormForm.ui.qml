import QtQuick 2.4
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 400
    height: 400

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Image {
            id: image
            width: 100
            height: 100
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        Text {
            id: text2
            text: qsTr("Text")
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }

        Text {
            id: text3
            color: "#4c4c4c"
            text: qsTr("Text")
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }



    }
}
