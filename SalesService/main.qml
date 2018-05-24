import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Attraction Sales Service")

    Settings {
        id: settings
        property string style: "Default"
    }
    //! [orientation]
    readonly property bool inPortrait: window.width < window.height
    //! [orientation]

    header: ToolBar {
        id: overlayHeader
        //        width: parent.width
        //        contentHeight: toolButton.implicitHeight
        Material.foreground: "white"

        RowLayout{
            spacing: 20
            anchors.fill: parent

            ToolButton {
                id: toolButton
                text: stackView.depth > 1 ? "\u25C0" : "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    } else {
                        drawer.opened?drawer.close():drawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.text : "Выберите тип продукта"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                icon.name: "menu"
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        y: overlayHeader.height

        width: inPortrait ? window.width * 0.8 : window.width * 0.25
        height: window.height - overlayHeader.height

        modal: inPortrait
        interactive: inPortrait
        position: inPortrait ? 0 : 1
        visible: !inPortrait

        ListView {
            clip: true
            id: listView
            anchors.fill: parent
            headerPositioning: ListView.OverlayHeader
            header: Pane {
                z:2
                id: header
                width: parent.width
                //                contentHeight: 30
                Text {
                    text: qsTr("Типы продуктов")
                }
                //                ToolButton{
                //                    width: 30
                //                    height: 30
                //                    id: addProductTypeButton
                //                    anchors.right: parent.right
                //                    anchors.top: parent.top
                //                    text: "+"
                //                    //onClicked:
                //                }

                MenuSeparator {
                    parent: header
                    width: parent.width
                    anchors.verticalCenter: parent.bottom
                    visible: !listView.atYBeginning
                }
            }

            //            footer: ItemDelegate {
            //                id: footer
            //                text: qsTr("Footer")
            //                width: parent.width

            //                MenuSeparator {
            //                    parent: footer
            //                    width: parent.width
            //                    anchors.verticalCenter: parent.top
            //                }
            //            }

            model: 5

            delegate: ItemDelegate {
                text: qsTr("Title %1").arg(index + 1)
                width: parent.width
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.ui.qml"
        anchors.fill: parent
        anchors.topMargin: overlayHeader.height
        anchors.leftMargin: !inPortrait && drawer.opened ? drawer.width:undefined
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: availableStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: "About"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "The Qt Quick Controls 2 module delivers the next generation user interface controls based on Qt Quick."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "In comparison to the desktop-oriented Qt Quick Controls 1, Qt Quick Controls 2 "
                      + "are an order of magnitude simpler, lighter and faster, and are primarily targeted "
                      + "towards embedded and mobile platforms."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}

