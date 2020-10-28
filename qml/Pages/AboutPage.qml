import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import Core.BuildConstants 1.0


Page {
visible: true
title: qsTr("About")
		BuildConstants { 
			id: bconstants
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
			Button {
				Layout.alignment: Qt.AlignCenter | Qt.AlignVTop
				text: qsTr("Check For Update")
				Material.background: Material.Teal
			}
	


		Image {
			Layout.alignment: Qt.AlignCenter | Qt.AlignVCenter	
				cache: true
				Layout.preferredHeight: parent.Layout.preferredHeight * 0.6
				Layout.preferredWidth: parent.Layout.preferredWidth * 0.6
				fillMode: Image.PreserveAspectFit
				source: "qrc:/ajr_logo.png"
			}

			Label {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
			Layout.preferredWidth: parent.Layout.preferredWidth;
			horizontalAlignment: Qt.AlignHCenter
			font.pixelSize: (function() { 
				var factor = 0.03;
				var calculatedHPxSize = parent.Layout.preferredHeight * factor;
				var calculatedWPxSize = parent.Layout.preferredWidth * factor;
				
				if(calculatedHPxSize > calculatedWPxSize) {
					return calculatedWPxSize;
				}else {
					return calculatedHPxSize;
				}
			})()
			text: 
			      qsTr("AppImage Updater <b>v")+
			      bconstants.getVersion() + 
			      qsTr("</b> (Commit <b>") + 
			      bconstants.getCommit() + 
			      qsTr("</b>), build <b>") +
			      bconstants.getBuildNo() + 
			      qsTr("</b> built on <b>") +
			      bconstants.getBuildTime() + 
			      qsTr("</b>.<br>") +
			      qsTr("This program is licensed under <b>Lesser GNU Public License</b>.<br>") +
			      qsTr("<b>Copyright \u00A9 Antony Jr.</b><br>") +
			      qsTr("Report issues at <a href='https://github.com/antony-jr/AppImageUpdater'>Project Page</a> <br>")+
				      qsTr("<br>Uses QAppImageUpdate <b>v") +
			              bconstants.getQAppImageUpdateVersion() +
				      qsTr("</b> and Torrent Rasterbar <b>v") +
				      bconstants.getTorrentVersion() +
				      qsTr("</b>.<br>") 	
			wrapMode: Text.WordWrap
			textFormat: Text.RichText
			onLinkActivated: Qt.openUrlExternally(link)
			}
	}
}
