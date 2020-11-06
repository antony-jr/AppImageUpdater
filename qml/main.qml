import Core.Updater 1.0
import Core.BuildConstants 1.0
import Core.SettingsManager 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import QtQuick.Shapes 1.15

ApplicationWindow {	
    id: root
    property bool updating: false;
    property bool updateLoading: false;
    property bool showUpdateChoice: false;
    property string currentAppImageIconSrc: qsTr("");
    property string currentAppImageName: qsTr("");
    property string currentAppImageReleaseNotes: qsTr("");

    /// Notify routine
    function notify(str) {
        notificationPopup.notifyText = str;
        notificationPopup.open();
    }

    function notificationGlow(glow) {
	    noti_glow.glow = glow;
    }

    /// Count on each page
    function totalCounts() {
	return completedCount.count + queuedCount.count + failedCount.count;
    }

    function setCompletedCount(count) {
	    completedCount.count = count;
    }
    
    function setQueuedCount(count) {
	    queuedCount.count = count;
    }
    
    function setFailedCount(count) {
	    failedCount.count = count;
    }

    flags: Qt.WindowStaysOnTopHint
    title: qsTr("AppImage Updater")
    width: 650
    height: 500
    minimumWidth: 400
    minimumHeight: 500
    visible: true
    Material.theme: Material.Light // Use google material design
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        if (settings_manager.isDarkMode)
            root.Material.theme = Material.Dark;

    }

    /// Popups used.
    Timer {
        id: notificationTimer

        interval: 1000
        running: false
        repeat: false
        onTriggered: {
            notificationTimer.stop();
            notificationPopup.close();
        }
    }

    Popup {
        id: notificationPopup

        property string notifyText: ""

        x: (root.width / 2) - (notificationPopup.width / 2)
        y: (root.height / 2) - (notificationPopup.height / 2)
        width: 250
        height: 70
        modal: true
        focus: true
        onOpened: {
            notificationTimer.start();
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Label {
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 8
                text: notificationPopup.notifyText
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
            }

        }

    }

    Popup {
        id: btWarningPopup

        x: (root.width / 2) - (btWarningPopup.width / 2)
        y: (root.height / 4) - (btWarningPopup.height / 4)
        width: root.width / 1.5
        height: root.height - 250
        modal: true
        focus: true

        ColumnLayout {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Label {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height
                font.pixelSize: (function() {
                    var factor = 0.032;
                    var calculatedHPxSize = btWarningPopup.height * factor;
                    var calculatedWPxSize = btWarningPopup.width * factor;
                    if (calculatedHPxSize > calculatedWPxSize)
                        return calculatedWPxSize;
                    else
                        return calculatedHPxSize;
                })()
                text: qsTr("<b><h1><font color='red'>Privacy Warning</font></h2></b><br>") + (function() {
                    var r = qsTr("");
                    r += qsTr("At the moment the so called <i>'Decentralized Update'</i>");
                    r += qsTr(" is achieved with the <b>BitTorrent protocol</b>. ");
                    r += qsTr(" In any P2P network your <b>IP address will be shared publically</b> with");
                    r += qsTr(" peers and the entire world.");
                    r += qsTr(" This means that anyone can see your IP address downloading some software ");
                    r += qsTr("which is a serious privacy breach in our opinion, So if you really care about ");
                    r += qsTr("privacy please don't use this feature. ");
                    r += qsTr("This also consumes more bandwidth than normal download");
                    r += qsTr(", So for mobile networks and metered networks please don't");
                    r += qsTr(" enable this feature. ");
                    r += qsTr(" This is only suitable for users who update large number of");
                    r += qsTr(" AppImages or a large one.");
                    r += qsTr("This is also very useful in a local network. ");
                    r += qsTr("<br><br>");
                    r += qsTr("<b><i>TL;DR Do not enable this if you don't know what you are doing!");
                    r += qsTr(" This is only for brave hearts! ");
                    r += qsTr("No one is responsible if you do something wrong!</i></b><br><br>");
                    return r;
                })()
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                onLinkActivated: Qt.openUrlExternally(link)
            }

        }

    }

    // Backend Core
    BuildConstants {
        id: bconstants
    }

    SettingsManager {
        id: settings_manager
    }

    Updater {
	id: coreUpdater
	onLoading: {
		root.updating = false;
		root.showUpdateChoice = false;
		root.updateLoading = true;
	}
	onMetaInfo: {
		console.log("Got Meta Data");
		root.currentAppImageIconSrc = "image://AIImage/" + info["ImageId"];
		root.currentAppImageName = info["Name"];
		root.currentAppImageReleaseNotes = info["ReleaseNotes"];
		root.updateLoading = false;
		root.updating = true;
		root.showUpdateChoice = true;
	}

	onFailed: {
		console.log("Failed Update");
	}
	
	onFinished: {
			
	}

	onFinishedAll: {
		root.updateLoading = false;
		root.showUpdateChoice = false;
		root.updating = false;
	}
	onCompletedCountChanged: {
		setCompletedCount(n);
	}

	onFailedCountChanged: {
		setFailedCount(n);	
	}

	onQueuedCountChanged: {		
		setQueuedCount(n);
	}
    }
    // ---

    Drawer {
        id: drawer

        width: root.width * 0.66
        height: root.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Completed Update(s)")
                width: parent.width
		onClicked: {
                    stackView.push("qrc:/Pages/CompletedPage.qml");
                    drawer.close();
	   	}

		RoundButton {
			property int count : 0;
			id: completedCount
			anchors.right: parent.right	
			text: qsTr("1");
			visible: completedCount.count > 0
			flat: true;
			Material.background: Material.Green;
			Material.foreground: "#fff";
		}
            }

            ItemDelegate {
                text: qsTr("Queued Update(s)")
                width: parent.width
                onClicked: {
                    stackView.push("qrc:/Pages/QueuedPage.qml");
                    drawer.close();
	    	}

		RoundButton {
			property int count: 0;
			id: queuedCount
			anchors.right: parent.right	
			text: qsTr("1");
			flat: true;
			visible: queuedCount.count > 0
			Material.background: Material.Blue;
			Material.foreground: "#fff";
		}
        
            }
            
	    ItemDelegate {
                text: qsTr("Failed Update(s)")
                width: parent.width
                onClicked: {
                    stackView.push("qrc:/Pages/FailedPage.qml");
                    drawer.close();
	    	}

		RoundButton {
			property int count: 0;
			id: failedCount
			anchors.right: parent.right	
			text: qsTr("1");
			flat: true
			visible: failedCount.count > 0
			Material.background: Material.Red;
			Material.foreground: "#fff";
		} 
            }

            ItemDelegate {
                text: qsTr("Check For Update")
                width: parent.width
                onClicked: {
                    drawer.close();
                }
            }

            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("qrc:/Pages/SettingsPage.qml");
                    drawer.close();
                }
            }

            ItemDelegate {
                text: qsTr("About")
                width: parent.width
                onClicked: {
                    stackView.push("qrc:/Pages/AboutPage.qml");
                    drawer.close();
                }
            }

            ItemDelegate {
                text: qsTr("Quit")
                width: parent.width
                onClicked: {
                    drawer.close();
                    Qt.quit();
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
	    Material.foreground: "#fff";
	    font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1)
                    stackView.pop();
                else
                    drawer.open();
            }
        }

	Shape {
		property bool glow: false;
		id: noti_glow
		visible: stackView.depth > 1 ? false : totalCounts() == 0 ? glow : true;
    		width: 30
    		height: 30
    		layer.enabled: true
    		layer.samples: 4
    		ShapePath {
			fillColor: "#f44336"
			strokeColor: "#f44336"
        		strokeWidth: 2
        		capStyle: ShapePath.FlatCap

        		PathAngleArc {
            			centerX: 15; centerY: 12 
            			radiusX: 9; radiusY: 9
            			startAngle: 0
            			sweepAngle: 360
			}
		}
	}

        Label {
            text: stackView.currentItem.title
	    anchors.centerIn: parent
	    color: "#fff"
        }

    }

}