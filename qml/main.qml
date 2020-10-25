import QtQuick 2.12
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import "./Components" as Components

ApplicationWindow {
    function hideAll() {
	aboutPage.hide();
    }

    function showDashboard() {
	hideAll();
    }

    function showAboutPage() {
	hideAll();
	aboutPage.show();
    }

    id: root
    title: qsTr("AppImage Updater")
    width: 650
    height: 500
    minimumWidth: 400
    minimumHeight: 450
    visible: true
    Material.theme: Material.Light // Use google material design
    
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);

	/// Do init stuff.
    }
    
    
    /* Components.MainMenu is a MenuBar QML Object.
     * This is a Customized Object which takes a ApplicationWindow.
     * This expects all the properties on changing pages to be 
     * present. */
    menuBar: Components.MainMenu {
        id: mainMenu
        mainWindow: root
    }
    
    /* All the Pages. */
    Components.AboutPage {
	id: aboutPage
    } 

    // -- End All Pages -- 
}
