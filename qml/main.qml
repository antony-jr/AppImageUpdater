import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

ApplicationWindow {
    id: root

    title: qsTr("AppImage Updater")
    width: 650
    height: 500
    minimumWidth: 400
    minimumHeight: 450
    visible: true
    Material.theme: Material.Light // Use google material design
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }

    Drawer {
        id: drawer

        width: root.width * 0.66
        height: root.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("About")
                width: parent.width
                onClicked: {
                    stackView.push("qrc:/Pages/AboutPage.qml");
                    drawer.close();
                }
            }

        }

    }

    StackView {
        id: stackView

        initialItem: "qrc:/Pages/HomePage.qml"
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton

            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1)
                    stackView.pop();
                else
                    drawer.open();
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }

    }

}
