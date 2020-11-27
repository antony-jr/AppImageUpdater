import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Completed Update(s)")

    ColumnLayout {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: parent.height
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 2
    }
    Component.onCompleted: {
	    setCompletedCount(0);
    } 

    ColumnLayout {
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: parent.height
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 0


    ScrollView {
	Layout.alignment: Qt.AlignCenter;	
	Layout.preferredWidth: parent.Layout.preferredWidth - 10
	Layout.preferredHeight: parent.Layout.preferredHeight - 60
	
	//width: parent.Layout.preferredWidth - 10
	//height: parent.Layout.preferredHeight - 60
	ScrollBar.vertical.policy: ScrollBar.AlwaysOn

	ListView {
		id: completedList
		width: parent.width
		height: parent.height 

		model: completedUpdatesList

		delegate: Pane {
		width: parent.width - 20
		height: 150
		Material.elevation: 4	
		RowLayout {
			///width: parent.width - 10
			Image {
          		Layout.alignment: Qt.AlignHLeft | Qt.AlignVCenter
            		cache: true
            		Layout.preferredHeight: 130
            		Layout.preferredWidth: 130
            		fillMode: Image.PreserveAspectFit
            		 source: "image://AIImage/" + ImageId
			}

			ColumnLayout {
			Label {
                	  font.pixelSize: (function() {
                    	   var factor = 0.03;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	text: qsTr("<h3>") + Name + qsTr("</h3><br/>")
                	  wrapMode: Text.WordWrap
                	  textFormat: Text.RichText
                	  onLinkActivated: Qt.openUrlExternally(link)
            		}

			Flow {
				width: root.width - 175 
				spacing: 8

			RowLayout {
				width: 200
				Button {
					text: qsTr("Open")
					Material.background: Material.Teal
					Material.foreground: "#ffffff"
				}

				Button {
					property bool seeding: false;
					id: thisBtn
					text: !thisBtn.seeding ? qsTr("Seed AppImage")	: qsTr("Stop Seeding")
					Material.background: !thisBtn.seeding ? Material.Green : Material.Red
					Material.foreground: "#ffffff"
					onClicked: { 
						thisBtn.seeding = !thisBtn.seeding;
					}
				}
			}

				Button {
					text: qsTr("Open Directory")

				}
			}
			}
		 }
		 } // Close delegate property
	     } // Close ListView
    } // Close ScrollView
    } // Column Layout
}
