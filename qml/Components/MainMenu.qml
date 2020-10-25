import QtQuick 2.12
import QtQuick.Controls 2.12

MenuBar {
	property ApplicationWindow mainWindow; 
        Menu {
	    title: qsTr("File")
	    MenuItem {
		text: qsTr("Dashboard")
		onTriggered: {
			mainWindow.showDashboard();
		}
            }
	    MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
	    }
        }
	Menu {
	    title: qsTr("Help")
	    MenuItem {
		text: qsTr("Check For Update")
		onTriggered: {
		}
	    }
	    MenuItem {
		text: qsTr("About")
		onTriggered: {
			mainWindow.showAboutPage();	
		}
	    }
        }
}

