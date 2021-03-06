import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import Core 1.0
import SortFilterProxyModel 0.2

Page {
    id: page

    header: TextField {
        id: textField
        text: qsTr("")
        padding: 10
        placeholderText: qsTr("Фильтрация ...")
    }

    SortFilterProxyModel{
        id: productTypesProxyModel
        sourceModel: productTypesModel
        filters: RegExpFilter {
            roleName: "title"
            pattern: textField.text
            caseSensitivity: Qt.CaseInsensitive
        }
        sorters: StringSorter { roleName: "title" }
    }

    GridView {
        id: gridView
        focus: true

        anchors.margins: 20
        anchors.fill: parent

        cellWidth: 200; cellHeight: 200

        model: productTypesProxyModel

        highlight: Rectangle {
            radius: 100
            width: gridView.cellWidth - 20; height: gridView.cellHeight - 20
            color: "lightgrey"; opacity: 0.2
        }

        delegate: Item {
            width: gridView.cellWidth - 20; height: gridView.cellHeight - 20

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
                    source: "image://product_types/"+index
                    cache: false
                }

                Text {
                    id: text2
                    text: model.title
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 12
                }

                Text {
                    id: text3
                    color: "grey"
                    text: model.comment
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Core.headerTitle = model.title
                    parent.GridView.view.currentIndex = index;
                    page.StackView.view.push("qrc:/ProductsPage.qml", { typeId: model.id });
                }
            }
        }
    }
}
