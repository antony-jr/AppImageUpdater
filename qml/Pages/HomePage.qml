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
	onEnqueue: {
	    coreUpdater.queue(absolutePath, appName, icon);
	}
	onFinished:  {
	    defaultLayout.fetching = false;	
	    ///notify("<h1>Queued Item to Updater</h1>");
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
	    visible: !root.updating && !defaultLayout.fetching && !root.updateLoading
            cache: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
            Layout.preferredHeight: (parent.Layout.preferredHeight) * 0.85
            Layout.preferredWidth: (parent.Layout.preferredWidth) * 0.85
            fillMode: Image.PreserveAspectFit
            source: "qrc:/dotted_square.png"
    	}

	ProgressBar {
	    visible: defaultLayout.fetching || root.updateLoading
	    indeterminate: true
 	    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

	Image {
	    visible: root.updating
	    cache: true
   	    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 125
	    Layout.preferredWidth: 125
	    fillMode: Image.PreserveAspectFit
	    source: root.currentAppImageIconSrc
        }
	
	Label {
		visible: root.updating
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.pixelSize: (function() {
                    var factor = 0.04;
                    var calculatedHPxSize = parent.Layout.preferredHeight * factor;
                    var calculatedWPxSize = parent.Layout.preferredWidth * factor;
                    if (calculatedHPxSize > calculatedWPxSize)
                        return calculatedWPxSize;
                    else
                        return calculatedHPxSize;
		})()
		text: "<h1>" + root.currentAppImageName + "</h1>"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
	}

	ScrollView {
		id: releaseScrollView
		visible: root.updating && root.currentAppImageReleaseNotes.length > 0
		contentWidth: root.width - 50
		Layout.preferredWidth: parent.Layout.preferredWidth - 50 
		Layout.preferredHeight: parent.Layout.preferredHeight - 250
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
		ScrollBar.vertical.policy: ScrollBar.AlwaysOn
		clip: true

		 ScrollBar.horizontal.interactive: false;

		TextEdit {
		 width: root.width - 80	 
		 height: parent.height - 10
		 visible: root.updating && root.currentAppImageReleaseNotes.length > 0	
		readOnly: true;
		text: root.currentAppImageReleaseNotes;
		wrapMode: Text.WordWrap
		color: settings_manager.isDarkMode ? "#fff" : "#212121";
		textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
      	      }

	      background: Rectangle {
		      width: releaseScrollView.implicitWidth;
		      height: releaseScrollView.implicitHeight;
		      color: settings_manager.isDarkMode ? "#212121" : "#fff"; 
	      }
      
      } // Close ScrollView

      RowLayout {
	      Layout.alignment: Qt.AlignCenter
	      visible: root.showUpdateChoice
	      Button {
		      text: qsTr("Accept Update");
		       Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light;
		      highlighted: true
		      Material.background: Material.Green;
		      onClicked: {
			      coreUpdater.continueCurrentUpdate();
		      }
	      }

	      Button {
		      text: qsTr("Skip Update");
		       Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light;
		      highlighted: true
		      Material.background: Material.Red;
		      onClicked: {
			      coreUpdater.cancelCurrentUpdate();
		      }
	      }

	      Button {
		      text: qsTr("Accept All");
		      Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light;
		      highlighted: true
		      Material.background: Material.Teal;
		      onClicked: {
			      coreUpdater.toggleNoConfirm();
			      coreUpdater.continueCurrentUpdate();
		      }
	      }
      }
    }
}
