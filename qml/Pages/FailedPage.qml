import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Failed Update(s)")

    Component.onCompleted: {
	    setFailedCount(0);
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
                id: failedList
                width: parent.width
                height: parent.height 

                model: failedUpdatesList

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
                           var factor = 0.023;
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
                        }
 			RowLayout {
			id: pathRow
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
			font.pixelSize: (function() {
                    	   var factor = 0.03;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	  text: "Path"
			  color: "white"
                	  wrapMode: Text.WordWrap
            		}
			}
			}

			Label {
                	Layout.preferredWidth: root.width - 250
			font.pixelSize: (function() {
                    	   var factor = 0.023;
                    	   var calculatedHPxSize = root.height * factor;
                    	   var calculatedWPxSize = root.width * factor;
                    	   if (calculatedHPxSize > calculatedWPxSize)
                           	return calculatedWPxSize;
                    	   else
                        	return calculatedHPxSize;
                	  })()
                	text: AbsolutePath
                	  wrapMode: Text.WordWrap
            		}
			}
			Label {
                Layout.preferredWidth: root.width - 250
                          font.pixelSize: (function() {
                           var factor = 0.023;
                           var calculatedHPxSize = root.height * factor;
                           var calculatedWPxSize = root.width * factor;
                           if (calculatedHPxSize > calculatedWPxSize)
                                return calculatedWPxSize;
                           else
                                return calculatedHPxSize;
                          })()
                          text: ErrorMsg
			  wrapMode: Text.WordWrap
                          textFormat: Text.RichText
                        }

                    Button {
                            text: qsTr("Retry")
                            Material.background: Material.Teal
                            Material.foreground: "#ffffff"
                        }

                        }
                 }
                 } // Close delegate property
             } // Close ListView
    } // Close ScrollView
    } // Column Layout
}
