import Core.BuildConstants 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Page {
    function getAboutContent() {
        var r = qsTr("AppImage Updater <b>v");
        r += bconstants.getVersion();
        r += qsTr("</b> (Commit <b>");
        r += bconstants.getCommit();
        r += qsTr("</b>), build <b>");
        r += bconstants.getBuildNo();
        r += qsTr("</b> built on <b>");
        r += bconstants.getBuildTime();
        r += qsTr("</b>.<br>");
        r += qsTr("This program is licensed under <b>Lesser GNU Public License</b>.<br>");
        r += qsTr("<b>Copyright \u00A9 Antony Jr.</b><br>");
        r += qsTr("Report issues at <a href='https://github.com/antony-jr/AppImageUpdater'>Project Page</a> <br>");
        r += qsTr("<br>Uses QAppImageUpdate <b>v");
        r += bconstants.getQAppImageUpdateVersion();
        r += qsTr("</b> and Torrent Rasterbar <b>v");
        r += bconstants.getTorrentVersion();
        r += qsTr("</b>.<br>");
        return r;
    }

    visible: true
    title: qsTr("About")

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
            Layout.preferredWidth: parent.Layout.preferredWidth
            horizontalAlignment: Qt.AlignHCenter
            font.pixelSize: (function() {
                var factor = 0.03;
                var calculatedHPxSize = parent.Layout.preferredHeight * factor;
                var calculatedWPxSize = parent.Layout.preferredWidth * factor;
                if (calculatedHPxSize > calculatedWPxSize)
                    return calculatedWPxSize;
                else
                    return calculatedHPxSize;
            })()
            text: getAboutContent()
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            onLinkActivated: Qt.openUrlExternally(link)
        }

    }

}
