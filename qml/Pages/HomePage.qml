import Core.DropItemParser 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Drag and Drop AppImage(s) to Update!")

    DropItemParser {
	id: dropParser
	onLoading: {
	     defaultLayout.fetching = true;
        }
	onFailed: {
	    defaultLayout.fetching = false; 
	}
	onFinished:  {
	    defaultLayout.fetching = false;	
	    notify("<h1>Queued Item to Updater</h1>");
     	}
    }

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
	    dropLayout.visible = false; 
       	    defaultLayout.visible = true; 
	    if(drop.hasUrls) {
		    dropParser.clearBuffer();
		    for(var i = 0; i < drop.urls.length; ++i) {
			    dropParser.appendToBuffer(drop.urls[i]);
		    }
		    dropParser.start();
	    }
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
	property bool updating: false;
	property bool fetching: false;
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
	    visible: !defaultLayout.updating && !defaultLayout.fetching
            cache: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
            Layout.preferredHeight: (parent.Layout.preferredHeight) * 0.85
            Layout.preferredWidth: (parent.Layout.preferredWidth) * 0.85
            fillMode: Image.PreserveAspectFit
            source: "qrc:/dotted_square.png"
    	}

	ProgressBar {
	    visible: !defaultLayout.updating && defaultLayout.fetching
	    indeterminate: true
 	    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
	}
    }
}
