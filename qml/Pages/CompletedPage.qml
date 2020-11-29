import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Completed Update(s)")

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
		height: contentCol.implicitHeight + 50
		Material.elevation: 4	
		RowLayout {
			Image {
          		Layout.alignment: Qt.AlignHLeft | Qt.AlignVCenter
            		cache: true
            		Layout.preferredHeight: 130
            		Layout.preferredWidth: 130
            		fillMode: Image.PreserveAspectFit
            		 source: "image://AIImage/" + ImageId
			}

			ColumnLayout {
			id: contentCol
			Label {
			  Layout.preferredWidth: root.width - 250
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
			
			RowLayout {
			id: newAbsRow
			visible: Updated
			Rectangle {
			id: absPathLblRec
			Layout.preferredWidth: 30 + absPathLbl.implicitWidth	
			Layout.preferredHeight: absPathRow.implicitHeight
			color: "#00BCD4"
			RowLayout {
			Layout.preferredWidth: 30 + absPathLbl.implicitWidth
			Layout.preferredHeight: absPathLbl.implicitHeight
			id: absPathRow
			Rectangle {
			Layout.preferredWidth: 10;
			Layout.preferredHeight: absPathLbl.implicitHeight
			color: "#00BCD4"
			}
			Label {
			id: absPathLbl
                	Layout.preferredWidth: root.width - 185
			font.pixelSize: (function() {
                    	   var factor = 0.03;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	  text: "New Version"
			  color: "white"
                	  wrapMode: Text.WordWrap
            		}
			}
			}

			Label {
			Layout.preferredWidth: root.width - 300
			font.pixelSize: (function() {
                    	   var factor = 0.023;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	text: NewAbsPath
                	  wrapMode: Text.WordWrap
            		}
			}
			
			RowLayout {
			id: oldAbsRow
			visible: Updated
			Rectangle {
			id: oldAbsPathLblRec
			Layout.preferredWidth: 30 + absPathLbl.implicitWidth	
			Layout.preferredHeight: oldAbsPathRow.implicitHeight
			color: "#607D8B"
			RowLayout {
			Layout.preferredWidth: 30 + absPathLbl.implicitWidth
			Layout.preferredHeight: oldAbsPathLbl.implicitHeight
			id: oldAbsPathRow
			Rectangle {
			Layout.preferredWidth: 10;
			Layout.preferredHeight: oldAbsPathLbl.implicitHeight
			color: "#607D8B"
			}
			Label {
			id: oldAbsPathLbl
                	Layout.preferredWidth: root.width - 185
			font.pixelSize: (function() {
                    	   var factor = 0.03;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	  text: "Old Version"
			  color: "white"
                	  wrapMode: Text.WordWrap
            		}
			}
			}
            
			
            Label {
		Layout.preferredWidth: root.width - 300	
			font.pixelSize: (function() {
                    	   var factor = 0.023;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	text: OldAbsPath
                	  wrapMode: Text.WordWrap
            		}
            }

		RowLayout {
			id: alreadyNewRowTop
			visible: !Updated
			Rectangle {
			id: alreadyNewLblRec
			Layout.preferredWidth: 30 + alreadyNewLbl.implicitWidth	
			Layout.preferredHeight: alreadyNewRow.implicitHeight
			color: "#8BC34A"
			RowLayout {
			Layout.preferredWidth: 30 + absPathLbl.implicitWidth
			Layout.preferredHeight: oldAbsPathLbl.implicitHeight
			id: alreadyNewRow
			Rectangle {
			Layout.preferredWidth: 10;
			Layout.preferredHeight: alreadyNewLbl.implicitHeight
			color: "#8BC34A"
			}
			Label {
			id: alreadyNewLbl
                	Layout.preferredWidth: root.width - 185
			font.pixelSize: (function() {
                    	   var factor = 0.03;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	  text: "ALREADY LATEST VERSION"
			  color: "white"
                	  wrapMode: Text.WordWrap
            		}
			}
			}
		}
            


			Flow {
			Layout.preferredWidth: root.width - 175 
			spacing: 8

			RowLayout {
				width: UsedTorrent ? 200 : 80
				Button {
					text: WaitOpen ? qsTr("Wait") : qsTr("Open")
					enabled: !WaitOpen
					Material.background: Material.Teal
					Material.foreground: "#ffffff"
					onClicked: { 
						mainexecuter.exec(Hash, NewAbsPath);
					}
				}

				Button {
					property bool seeding: false;
					id: thisBtn
					visible: UsedTorrent
					enabled: settings_manager.isDecentralizedUpdateEnabled 
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
					onClicked: { 
						mainexecuter.openDirectory(NewAbsPath);
					}
				}
			}
			}
		 }
		 } // Close delegate property
	     } // Close ListView
    } // Close ScrollView
    } // Column Layout
}
