import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Drag and Drop AppImage(s) to Update!")

    DropArea {
        id: appimageDropArea

        anchors.fill: parent
        onEntered: {
	    defaultLayout.visible = false;
            dropLayout.visible = true;
        }
        onExited: {
            dropLayout.visible = false;
            defaultLayout.visible = true;
        }
        onDropped: {
            console.log(drop.text);
            notify("<h1>Queued Item to Updater</h1>");
            dropLayout.visible = false;
	    defaultLayout.visible = true;
    	}
    }

    ColumnLayout {
        id: dropLayout

        Layout.preferredWidth: parent.width
        Layout.preferredHeight: parent.height
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 2
        visible: false

        Image {
            cache: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
            Layout.preferredHeight: (parent.Layout.preferredHeight)
            Layout.preferredWidth: (parent.Layout.preferredWidth)
            fillMode: Image.PreserveAspectFit
            source: "qrc:/drop_image.png"
        }

    }

    ColumnLayout {
        id: defaultLayout

        Layout.preferredWidth: parent.width
        Layout.preferredHeight: parent.height
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 2

        Image {
            cache: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
            Layout.preferredHeight: (parent.Layout.preferredHeight) * 0.85
            Layout.preferredWidth: (parent.Layout.preferredWidth) * 0.85
            fillMode: Image.PreserveAspectFit
            source: "qrc:/dotted_square.png"
        }
    }
}
