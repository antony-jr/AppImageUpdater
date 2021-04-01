import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    visible: true
    title: qsTr("Settings")

    ColumnLayout {
        id: settingsColLayout

        property real factor: 100

        Layout.preferredWidth: parent.width
        Layout.preferredHeight: parent.height
        anchors.fill: parent
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Layout.alignment: Qt.AlignHLeft | Qt.AlignVTop
        spacing: 0

        Switch {
            Layout.preferredWidth: parent.Layout.preferredWidth - settingsColLayout.factor
            Layout.alignment: Qt.AlignCenter
            checked: settings_manager.isDarkMode
            text: qsTr("Enable Dark Mode")
            onClicked: {
                if (checked)
                    root.Material.theme = Material.Dark;
                else
                    root.Material.theme = Material.Light;
                settings_manager.isDarkMode = checked;
            }
        }

        Switch {
            Layout.preferredWidth: parent.Layout.preferredWidth - settingsColLayout.factor
            Layout.alignment: Qt.AlignCenter
            checked: settings_manager.isAllowSystemTrayNotification
            text: qsTr("Allow System Tray Notification")
            onClicked: {
                settings_manager.isAllowSystemTrayNotification = checked;
            }
        }

        Switch {
            Layout.preferredWidth: parent.Layout.preferredWidth - settingsColLayout.factor
            Layout.alignment: Qt.AlignCenter
            checked: settings_manager.isRunOnStartup
            text: qsTr("Run at Startup")
            onClicked: {
                settings_manager.isRunOnStartup = checked;
            }
        }

        Switch {
            Layout.preferredWidth: parent.Layout.preferredWidth - settingsColLayout.factor
            Layout.alignment: Qt.AlignCenter
            checked: settings_manager.isDecentralizedUpdateEnabled
            text: qsTr("Use Decentralized Update")
            onClicked: {
                if (checked)
                    btWarningPopup.open();

                settings_manager.isDecentralizedUpdateEnabled = checked;
            }
        }

        Switch {
            Layout.preferredWidth: parent.Layout.preferredWidth - settingsColLayout.factor
            Layout.alignment: Qt.AlignCenter
            checked: settings_manager.isProxyEnabled
            text: qsTr("Enable Proxy")
            onClicked: {
                settings_manager.isProxyEnabled = checked;
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: parent.Layout.preferredWidth - (110 + settingsColLayout.factor)

            Label {
                font.pixelSize: (function() {
                    var factor = 0.03;
                    var calculatedHPxSize = parent.Layout.preferredHeight * factor;
                    var calculatedWPxSize = parent.Layout.preferredWidth * factor;
                    if (calculatedHPxSize > calculatedWPxSize)
                        return calculatedWPxSize;
                    else
                        return calculatedHPxSize;
                })()
                text: qsTr("<h4>Proxy Settings</h2>")
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
            }

            RowLayout {
                TextField {
                    placeholderText: qsTr("Proxy Host")
                    text: settings_manager.ProxyHost
                    enabled: settings_manager.isProxyEnabled
                    onEditingFinished: {
                        settings_manager.ProxyHost = text;
                    }
                }

                TextField {
                    placeholderText: qsTr("Proxy Port")
                    text: settings_manager.ProxyPort.toLocaleString()
                    enabled: settings_manager.isProxyEnabled
                    inputMask: "9999"
                    onEditingFinished: {
                        settings_manager.ProxyPort = Number(text);
                    }

		    validator: RegExpValidator{regExp: /^[0-9/]+$/}
                    //validator: IntValidator {
                    //    bottom: 0
                    //    top: 66666
                    //}

                }

            }

            RowLayout {
                Label {
                    text: qsTr("Proxy Type: ")
                    enabled: settings_manager.isProxyEnabled
                }

                ComboBox {
                    model: ["Socks 5", "HTTP(S)"]
                    enabled: settings_manager.isProxyEnabled
                    Component.onCompleted: {
                        var x = find(settings_manager.ProxyType, Qt.MatchExactly);
                        if (x == -1)
                            settings_manager.ProxyType = model[0];
                        else
                            currentIndex = x;
                    }
                    onActivated: {
                        settings_manager.ProxyType = model[index];
                    }
                    onHighlighted: {
                        settings_manager.ProxyType = model[index];
                    }
                }

            }

            RowLayout {
                TextField {
                    placeholderText: qsTr("Username")
                    text: settings_manager.ProxyUser
                    enabled: settings_manager.isProxyEnabled
                    onEditingFinished: {
                        settings_manager.ProxyUser = text;
                    }
                }

                TextField {
                    placeholderText: qsTr("Password")
                    text: settings_manager.ProxyPass
                    echoMode: TextInput.Password
                    enabled: settings_manager.isProxyEnabled
                    onEditingFinished: {
                        settings_manager.ProxyPass = text;
                    }
                }

            }
            //col

        }

    }

}
