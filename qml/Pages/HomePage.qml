import Core.DropItemParser 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Drag and Drop AppImage(s) to Update!")

    FileDialog {
        id: browseFileDialog

        title: qsTr("Browse AppImage")
        folder: shortcuts.home
        selectFolder: false
        onAccepted: {
            dropParser.clearBuffer();
            for (var i = 0; i < browseFileDialog.fileUrls.length; ++i) {
                dropParser.appendToBuffer(browseFileDialog.fileUrls[i]);
            }
            dropParser.start();
        }
    }

    DropItemParser {
        ///notify("<h1>Queued Item to Updater</h1>");

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
        onFinished: {
            defaultLayout.fetching = false;
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
            if (drop.hasUrls) {
                dropParser.clearBuffer();
                for (var i = 0; i < drop.urls.length; ++i) {
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
        id: defaultLayout

        property bool fetching: false

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

        Button {
            visible: !root.updating && !defaultLayout.fetching && !root.updateLoading
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
            Layout.preferredWidth: (parent.Layout.preferredWidth) * 0.3
            text: qsTr("Browse")
            Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
            highlighted: true
            Material.background: Material.Green
            onClicked: {
                browseFileDialog.open();
            }
        }

        ProgressBar {
            visible: defaultLayout.fetching || root.updateLoading
            indeterminate: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        ScrollView {
            id: mainPageScrollView

            visible: root.updating
            contentWidth: root.width - 10
            Layout.preferredWidth: parent.Layout.preferredWidth - 10
            Layout.preferredHeight: parent.Layout.preferredHeight - 10
            Layout.alignment: Qt.AlignCenter
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            clip: true
            ScrollBar.horizontal.interactive: false

            ColumnLayout {
                // Close ScrollView

                id: mainColumnLayout

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredWidth: root.width - 5
                Layout.preferredHeight: root.height - 80
                spacing: 2

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
                    Layout.preferredWidth: parent.Layout.preferredWidth - 10
                    horizontalAlignment: Qt.AlignHCenter
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

                    visible: !root.actualProgress && !root.loadingProgress && root.updating && root.currentAppImageReleaseNotes.length > 0
                    contentWidth: root.width - 50
                    Layout.preferredWidth: parent.Layout.preferredWidth - 50
                    Layout.preferredHeight: parent.Layout.preferredHeight - 250
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                    clip: true
                    ScrollBar.horizontal.interactive: false

                    TextEdit {
                        width: root.width - 80
                        height: parent.height - 10
                        visible: !root.loadingProgress && root.updating && root.currentAppImageReleaseNotes.length > 0
                        readOnly: true
                        text: root.currentAppImageReleaseNotes
                        wrapMode: Text.WordWrap
                        color: settings_manager.isDarkMode ? "#fff" : "#212121"
                        textFormat: Text.RichText
                        onLinkActivated: Qt.openUrlExternally(link)
                    }

                    background: Rectangle {
			width: releaseScrollView.implicitWidth
                        height: releaseScrollView.implicitHeight
                        color: settings_manager.isDarkMode ? "#212121" : "#fff"
                    }

                }

                RowLayout {
                    Layout.alignment: Qt.AlignCenter
                    visible: !root.loadingProgress && root.showUpdateChoice

                    Button {
                        text: qsTr("Accept Update")
                        Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
                        highlighted: true
                        Material.background: Material.Green
                        onClicked: {
                            coreUpdater.continueCurrentUpdate();
                        }
                    }

                    Button {
                        text: qsTr("Skip Update")
                        Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
                        highlighted: true
                        Material.background: Material.Red
                        onClicked: {
                            coreUpdater.cancelCurrentUpdate();
                        }
                    }

                    Button {
                        text: qsTr("Accept All")
                        Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
                        highlighted: true
                        Material.background: Material.Teal
                        onClicked: {
                            coreUpdater.toggleNoConfirm();
                            coreUpdater.continueCurrentUpdate();
                        }
                    }

                }
		Rectangle {
			visible: root.loadingProgress || root.actualProgress
			width: 100
                        height: root.height * 0.25
                        color: "transparent"
                }

            	ProgressBar {
            		visible: root.loadingProgress
			indeterminate: true
            		Layout.preferredWidth: root.width - 100
			Layout.preferredHeight: 50
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
		}

	        ProgressBar {
            		visible: root.actualProgress
			indeterminate: false
            		Layout.preferredWidth: root.width - 100
			Layout.preferredHeight: 50
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			value: root.progressBarValue
		}

		
                Label {
                    visible: root.actualProgress
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.preferredWidth: parent.Layout.preferredWidth - 10
                    horizontalAlignment: Qt.AlignHCenter
                    font.pixelSize: (function() {
                        var factor = 0.04;
                        var calculatedHPxSize = parent.Layout.preferredHeight * factor;
                        var calculatedWPxSize = parent.Layout.preferredWidth * factor;
                        if (calculatedHPxSize > calculatedWPxSize)
                            return calculatedWPxSize;
                        else
                            return calculatedHPxSize;
                    })()
                    text: root.progressText
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                }

		Flow {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVBottom
                    Layout.preferredWidth: root.width - 100
		    visible: root.actualProgress
		    spacing: 8 
                    Button {
			Layout.alignment: Qt.AlignCenter
                        text: qsTr("Cancel Update")
                        Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
                        highlighted: true
                        Material.background: Material.Red
                        onClicked: {
                            coreUpdater.cancelCurrentUpdate();
                        }
                    }

		    Button {
			Layout.alignment: Qt.AlignCenter
                        text: qsTr("Cancel All")
                        Material.theme: settings_manager.isDarkMode ? Material.Dark : Material.Light
                        highlighted: true
                        Material.background: Material.Teal
                        onClicked: {
                            coreUpdater.cancelAll();
                        }
                    }

        	    Switch {
			Layout.alignment: Qt.AlignCenter
            		checked: coreUpdater.isNoConfirmEnabled
            		text: qsTr("Accept All Updates")
            		onClicked: {
			     coreUpdater.toggleNoConfirm();	
			}
        	    }
                }


	   } // main scroll view
        }

    }

}
