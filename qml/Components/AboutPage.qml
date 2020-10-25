import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import Core.BuildConstants 1.0

GridLayout {
	visible: false
	anchors.fill: parent
	anchors.top: parent.top
	anchors.left: parent.left
	anchors.right: parent.right
	anchors.bottom: parent.bottom
	columns: 1
        rows: 1
	    


	ColumnLayout {
	      	Layout.preferredWidth: parent.width - 100
	    	Layout.preferredHeight: parent.height - 50
	    	Layout.row: 0
	    	Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
	    	Layout.topMargin: 10
		
		BuildConstants { 
			id: bconstants
		}

		RowLayout{
			Layout.preferredWidth: parent.width; 
			Layout.preferredHeight: parent.height;
			Image {
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop		
				cache: true
				Layout.preferredHeight: parent.Layout.preferredHeight * 0.5
				fillMode: Image.PreserveAspectFit
				source: "qrc:/logo.png"
			}
		}

		RowLayout {
			Layout.alignment: Qt.AlignHCenter
			Label {
				Layout.preferredWidth: parent.Layout.preferredWidth;
				horizontalAlignment: Qt.AlignHLeft
				verticalAlignment: Qt.AlignTop
				leftPadding: 10
				wrapMode: Text.WordWrap
				textFormat: Text.RichText
				text: qsTr("<br>") + 
				      qsTr(" QAppImageUpdate &nbsp;&nbsp;: <b>v") +
			              bconstants.getQAppImageUpdateVersion() +
				      qsTr("</b><br>") + 
				      qsTr(" Torrent Rasterbar &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;: <b>v") +
				      bconstants.getTorrentVersion() +
				      qsTr("</b><br>") + 	
				      qsTr(" AppImage Updater &nbsp; : <b>v")  +
				      bconstants.getVersion() +
				      qsTr("</b><br>")
			}
		}

		RowLayout {
			Layout.preferredWidth: parent.width
			Layout.preferredHeight: parent.height
			Label {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignTop	
			Layout.preferredWidth: parent.Layout.preferredWidth;
			horizontalAlignment: Qt.AlignHCenter
			verticalAlignment: Qt.AlignTop
			text: 
			      qsTr("Commit <b>") + 
			      bconstants.getCommit() + 
			      qsTr("</b>, build <b>") +
			      bconstants.getBuildNo() + 
			      qsTr("</b> built on <b>") +
			      bconstants.getBuildTime() + 
			      qsTr("</b>.<br>") +
			      qsTr("Lesser GNU Public License.<br>") +
			      qsTr("Copyright \u00A9 Antony Jr.<br>")
			wrapMode: Text.WordWrap
			textFormat: Text.RichText
			onLinkActivated: Qt.openUrlExternally(link)
			}
		}
	}
	
	function show() {
		visible = true;
	}

	function hide() {
		visible = false;
	}
}
