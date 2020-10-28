import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Page {
visible: true
title: qsTr("Drag and Drop AppImage(s) to Update!")	
ColumnLayout {
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
		Layout.preferredHeight: (parent.Layout.preferredHeight) * 0.8
		Layout.preferredWidth: (parent.Layout.preferredWidth) * 0.85
		fillMode: Image.PreserveAspectFit
		source: "qrc:/dotted_square.png"
	
	}
		RowLayout {
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVTop
			Button {
				text: qsTr("(0) Completed")
				Material.background: Material.Green
				enabled: false
			}
	
			Button {
				text: qsTr("(0) Failed")
				Material.background: Material.Red
				enabled: false
			}

			Button {
				text: qsTr("(0) Queued")
				Material.background: Material.Blue
				enabled: false
			}
		}
}
}
