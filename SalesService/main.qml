import QtQuick 2.10
import QtQuick.Controls 2.3
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 500
    height: 500
    title: qsTr("Attraction Sales Service")

    Settings {
        id: settings
        property string style: "Default"
        property string host: "localhost"
        property string user: "root"
        property string password: ""
        property int port: 3306
    }

    header: CommonToolBar{}

    Popup {
        id: popup
        parent: Overlay.overlay

        modal: true
        focus: true
        contentItem: Image {
            id: reconnectIcon
            source: "icons/gallery/20x20@4/menu.png"
            fillMode: Image.PreserveAspectFit
        }
//        Component.onCompleted: popup.open()
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page {

        }

        Page{
            StackView {
                id: stackView
                initialItem: ProductTypesPage {}
                anchors.fill: parent
            }

        }

        Page {
        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            icon.name : "card"
            text: qsTr("ИНФОРМАЦИЯ О БРАСЛЕТЕ")
        }
        TabButton {
            icon.name : "store"
            text: qsTr("ПОКУПКА")
        }
        TabButton {
            icon.name : "cart"
            text: qsTr("КОРЗИНА")
        }
    }

}

